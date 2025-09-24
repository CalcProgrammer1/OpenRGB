/*---------------------------------------------------------*\
| ThrustmasterSolController.cpp                             |
|                                                           |
|   Driver for Thrustmaster Sol series joysticks            |
|                                                           |
|   Ken Sanislo                                 02 Apr 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <thread>
#include <chrono>
#include "ThrustmasterSolController.h"
#include "StringUtils.h"

ThrustmasterSolController::ThrustmasterSolController(libusb_device_handle* dev_handle, const char* path, unsigned short pid, std::string dev_name)
{
    dev             = dev_handle;
    name            = dev_name;
    this->pid       = pid;

    location = "USB: ";
    location += path;

    /*---------------------------------------------------------*\
    | Get serial number string descriptor                       |
    \*---------------------------------------------------------*/
    libusb_device_descriptor desc;
    libusb_get_device_descriptor(libusb_get_device(dev_handle), &desc);

    if(desc.iSerialNumber != 0)
    {
        unsigned char serial_str[256];
        int ret = libusb_get_string_descriptor_ascii(dev_handle,
                                                     desc.iSerialNumber,
                                                     serial_str,
                                                     sizeof(serial_str));
        if(ret > 0)
        {
            serial = std::string(reinterpret_cast<char*>(serial_str), ret);
        }
    }
}

ThrustmasterSolController::~ThrustmasterSolController()
{
    if(dev != nullptr)
    {
        libusb_release_interface(dev, THRUSTMASTER_SOL_INTERFACE);
        libusb_close(dev);
    }
}

std::string ThrustmasterSolController::GetDeviceLocation()
{
    return(location);
}

std::string ThrustmasterSolController::GetDeviceName()
{
    return(name);
}

std::string ThrustmasterSolController::GetSerialString()
{
    return(serial);
}

unsigned short ThrustmasterSolController::GetPID()
{
    return(pid);
}

void ThrustmasterSolController::SendPacket(unsigned char* packet, unsigned int size)
{
    if(dev == nullptr)
    {
        return;
    }

    int actual_length = 0;
    libusb_interrupt_transfer(dev,
                              THRUSTMASTER_SOL_ENDPOINT_OUT,
                              packet,
                              size,
                              &actual_length,
                              1000);
}

void ThrustmasterSolController::BuildAndSendPackets(unsigned int* zones,
                                                    RGBColor* colors,
                                                    unsigned int count,
                                                    bool persistent)
{
    unsigned char persist_flag = persistent ? THRUSTMASTER_SOL_PERSISTENT
                                           : THRUSTMASTER_SOL_VOLATILE;

    /*---------------------------------------------------------*\
    | Separate grip zones from base zones. Zone 0x00 exists on  |
    | both the base (logo LED) and the grip (thumbstick LED).   |
    | The GRIP_FLAG bit selects the grip report type.           |
    \*---------------------------------------------------------*/
    unsigned char   base_zones[THRUSTMASTER_SOL_R_ZONE_COUNT];
    RGBColor        base_colors[THRUSTMASTER_SOL_R_ZONE_COUNT];
    unsigned int    base_count = 0;

    for(unsigned int i = 0; i < count; i++)
    {
        if(zones[i] & THRUSTMASTER_SOL_GRIP_FLAG)
        {
            /*-------------------------------------------------*\
            | Send grip packet for thumbstick immediately       |
            \*-------------------------------------------------*/
            unsigned char zone_id = zones[i] & 0xFF;

            unsigned char packet[THRUSTMASTER_SOL_PACKET_SIZE];
            memset(packet, 0x00, THRUSTMASTER_SOL_PACKET_SIZE);

            packet[0] = THRUSTMASTER_SOL_REPORT_GRIP_LO;
            packet[1] = THRUSTMASTER_SOL_REPORT_GRIP_HI;
            packet[2] = persist_flag | 0x01;
            packet[3] = THRUSTMASTER_SOL_MARKER;
            packet[4] = zone_id;
            packet[5] = RGBGetRValue(colors[i]);
            packet[6] = RGBGetGValue(colors[i]);
            packet[7] = RGBGetBValue(colors[i]);

            SendPacket(packet, THRUSTMASTER_SOL_PACKET_SIZE);
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
        else
        {
            base_zones[base_count]  = zones[i] & 0xFF;
            base_colors[base_count] = colors[i];
            base_count++;
        }
    }

    /*---------------------------------------------------------*\
    | Send base zone packets, batching up to 15 entries each    |
    \*---------------------------------------------------------*/
    for(unsigned int offset = 0; offset < base_count; offset += THRUSTMASTER_SOL_MAX_ENTRIES)
    {
        unsigned int entries = base_count - offset;

        if(entries > THRUSTMASTER_SOL_MAX_ENTRIES)
        {
            entries = THRUSTMASTER_SOL_MAX_ENTRIES;
        }

        unsigned char packet[THRUSTMASTER_SOL_PACKET_SIZE];
        memset(packet, 0x00, THRUSTMASTER_SOL_PACKET_SIZE);

        packet[0] = THRUSTMASTER_SOL_REPORT_BASE_LO;
        packet[1] = THRUSTMASTER_SOL_REPORT_BASE_HI;
        packet[2] = persist_flag | (unsigned char)entries;
        packet[3] = THRUSTMASTER_SOL_MARKER;

        for(unsigned int j = 0; j < entries; j++)
        {
            unsigned int idx = offset + j;
            packet[4 + j * 4]     = base_zones[idx];
            packet[4 + j * 4 + 1] = RGBGetRValue(base_colors[idx]);
            packet[4 + j * 4 + 2] = RGBGetGValue(base_colors[idx]);
            packet[4 + j * 4 + 3] = RGBGetBValue(base_colors[idx]);
        }

        SendPacket(packet, THRUSTMASTER_SOL_PACKET_SIZE);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

void ThrustmasterSolController::SetLEDColor(unsigned int zone, RGBColor color)
{
    BuildAndSendPackets(&zone, &color, 1, false);
}

void ThrustmasterSolController::SetLEDColors(unsigned int* zones,
                                              RGBColor* colors,
                                              unsigned int count)
{
    BuildAndSendPackets(zones, colors, count, false);
}

void ThrustmasterSolController::SaveColors(unsigned int* zones,
                                            RGBColor* colors,
                                            unsigned int count)
{
    BuildAndSendPackets(zones, colors, count, true);
}

void ThrustmasterSolController::ReadColors(std::vector<unsigned int>& zones,
                                           std::vector<RGBColor>& colors)
{
    if(dev == nullptr)
    {
        return;
    }

    zones.clear();
    colors.clear();

    /*---------------------------------------------------------*\
    | Read base zones (report 0x0002). May require multiple     |
    | queries if more than 14 zones are returned per response.  |
    | Zone IDs are returned without the grip flag, so they      |
    | match the base LED values (including zone 0x00 = logo).   |
    \*---------------------------------------------------------*/
    unsigned char start = 0;

    for(int page = 0; page < 4; page++)
    {
        unsigned char query[THRUSTMASTER_SOL_PACKET_SIZE];
        memset(query, 0x00, THRUSTMASTER_SOL_PACKET_SIZE);

        query[0] = THRUSTMASTER_SOL_READ_BASE_LO;
        query[1] = THRUSTMASTER_SOL_READ_BASE_HI;
        query[2] = start;

        SendPacket(query, THRUSTMASTER_SOL_PACKET_SIZE);

        unsigned char resp[THRUSTMASTER_SOL_PACKET_SIZE];
        int actual = 0;
        int ret = libusb_interrupt_transfer(dev,
                                            THRUSTMASTER_SOL_ENDPOINT_IN,
                                            resp,
                                            THRUSTMASTER_SOL_PACKET_SIZE,
                                            &actual,
                                            1000);

        if(ret != LIBUSB_SUCCESS || actual < 4)
        {
            break;
        }

        unsigned int n = resp[2] & 0x0F;

        if(n == 0)
        {
            break;
        }

        unsigned char last = start;

        for(unsigned int i = 0; i < n; i++)
        {
            unsigned int off = 4 + i * 4;

            if(off + 3 >= (unsigned int)actual)
            {
                break;
            }

            unsigned char zid = resp[off];
            unsigned char r   = resp[off + 1];
            unsigned char g   = resp[off + 2];
            unsigned char b   = resp[off + 3];

            zones.push_back(zid);
            colors.push_back(ToRGBColor(r, g, b));
            last = zid;
        }

        start = last + 1;

        if(n < 14 || start > 0x20)
        {
            break;
        }
    }

    /*---------------------------------------------------------*\
    | Read grip zones (report 0x8002). Grip zone IDs are        |
    | returned with the GRIP_FLAG set so they match the         |
    | thumbstick LED value. They are added as new entries,      |
    | not overwriting base entries (zone 0x00 exists on both).  |
    \*---------------------------------------------------------*/
    unsigned char grip_query[THRUSTMASTER_SOL_PACKET_SIZE];
    memset(grip_query, 0x00, THRUSTMASTER_SOL_PACKET_SIZE);

    grip_query[0] = THRUSTMASTER_SOL_READ_GRIP_LO;
    grip_query[1] = THRUSTMASTER_SOL_READ_GRIP_HI;

    SendPacket(grip_query, THRUSTMASTER_SOL_PACKET_SIZE);

    unsigned char grip_resp[THRUSTMASTER_SOL_PACKET_SIZE];
    int grip_actual = 0;
    int grip_ret = libusb_interrupt_transfer(dev,
                                             THRUSTMASTER_SOL_ENDPOINT_IN,
                                             grip_resp,
                                             THRUSTMASTER_SOL_PACKET_SIZE,
                                             &grip_actual,
                                             1000);

    if(grip_ret == LIBUSB_SUCCESS && grip_actual >= 8)
    {
        unsigned int grip_n = grip_resp[2] & 0x0F;

        for(unsigned int i = 0; i < grip_n; i++)
        {
            unsigned int off = 4 + i * 4;

            if(off + 3 >= (unsigned int)grip_actual)
            {
                break;
            }

            unsigned char zid = grip_resp[off];
            unsigned char r   = grip_resp[off + 1];
            unsigned char g   = grip_resp[off + 2];
            unsigned char b   = grip_resp[off + 3];

            zones.push_back(THRUSTMASTER_SOL_GRIP_FLAG | zid);
            colors.push_back(ToRGBColor(r, g, b));
        }
    }
}
