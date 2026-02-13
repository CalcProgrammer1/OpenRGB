/*---------------------------------------------------------*\
| SteelSeriesApexController.cpp                             |
|                                                           |
|   Driver for SteelSeries Apex Keyboards                   |
|                                                           |
|   New driver based on SignalRGB Plugins                   |
|   https://gitlab.com/signalrgb/signal-plugins/            |
|                                                           |
|   Eric Samuelson (edbgon)                     05 Jul 2020 |
|   Filipe S. (filipesn)                         5 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <cstdio>
#include "SteelSeriesApexController.h"
#include "LogManager.h"

using namespace std::chrono_literals;

#define FIRMWARE_REQ_LEN 645

static unsigned int keys[] = {0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
                              0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, //20
                              0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21,
                              0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, //40
                              0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x32, 0x33, 0x34, 0x35, 0x36,
                              0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, //60
                              0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A,
                              0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x64, 0xE0, //80
                              0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xF0, 0x31, 0x87,
                              0x88, 0x89, 0x8A, 0x8B, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, //100
                              0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62,
                              0x63 };

SteelSeriesApexController::SteelSeriesApexController(hid_device* dev_handle, steelseries_type type, const char* path, std::string dev_name) : SteelSeriesApexBaseController(dev_handle, path, dev_name)
{
    proto_type = type;
    use_new_protocol = false;

    SendInitialization();
}

SteelSeriesApexController::~SteelSeriesApexController()
{
    hid_close(dev);
}

void SteelSeriesApexController::SetMode
    (
    unsigned char mode,
    std::vector<RGBColor> /*colors*/
    )
{
    unsigned char mode_colors[9];

    active_mode = mode;

    memset(mode_colors, 0x00, sizeof(mode_colors));
}

void SteelSeriesApexController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    unsigned char buf[643];
    int num_keys = 0;

    unsigned char packet_id = APEX_PACKET_ID_DIRECT;

    num_keys = sizeof(keys) / sizeof(*keys);

    if(use_new_protocol)
    {
        struct hid_device_info* info = hid_get_device_info(dev);
        if(info && (info->product_id == 0x162C || info->product_id == 0x162D)) // Aparently Gen 3 wireless models reuse this protocol, make sure to place their PID here and further below when developing.
        {
             packet_id = APEX_2023_PACKET_ID_DIRECT_WIRELESS;
        }
        else
        {
             packet_id = APEX_2023_PACKET_ID_DIRECT;
        }
    }

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0;
    buf[0x01]   = packet_id;
    buf[0x02]   = (use_new_protocol) ? (unsigned char)colors.size() : num_keys;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for(int i = 0; i < num_keys; i++)
    {
        buf[(i*4)+3] = keys[i];
        buf[(i*4)+4] = RGBGetRValue(colors[i]);
        buf[(i*4)+5] = RGBGetGValue(colors[i]);
        buf[(i*4)+6] = RGBGetBValue(colors[i]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 643);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void SteelSeriesApexController::SelectProfile
    (
    unsigned char   profile
    )
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer, set up packet and send               |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));
    buf[0x00]   = 0;
    buf[0x01]   = 0x89;
    buf[0x02]   = profile;
    hid_send_feature_report(dev, buf, 65);
}

void SteelSeriesApexController::SendInitialization()
{
    unsigned char buf[FIRMWARE_REQ_LEN];
    unsigned char read_buf[65];
    int res = 0;
    char version_str[32] = "Unknown";

    struct hid_device_info* info = hid_get_device_info(dev);
    unsigned short pid = (info) ? info->product_id : 0;

    // Firmware check
    if(pid == 0x1628)
    {
        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(buf, 0x00, sizeof(buf));
        buf[0x00]   = 0x00;
        buf[0x01]   = 0x90;

        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        hid_write(dev, buf, 65);

        /*-----------------------------------------------------*\
        | Read Response                                         |
        \*-----------------------------------------------------*/
        memset(read_buf, 0x00, sizeof(read_buf));
        res = hid_read_timeout(dev, read_buf, sizeof(read_buf), 200);

        /*-----------------------------------------------------*\
        | Firmware Check                                        |
        \*-----------------------------------------------------*/
        if(res > 2 && read_buf[0] == 0x90)
        {
            int major = 0, minor = 0, patch = 0;
            char* fw_ptr = (char*)&read_buf[2];

            snprintf(version_str, sizeof(version_str), "%s", fw_ptr);

            int count = sscanf(version_str, "%d.%d.%d", &major, &minor, &patch);

            if(count == 3)
            {
                // Currently set to 1.19.7 or newer.
                if(major > 1)
                {
                    use_new_protocol = true;
                }
                else if(major == 1)
                {
                    if(minor > 19)
                    {
                        use_new_protocol = true;
                    }
                    else if(minor == 19 && patch >= 7)
                    {
                        use_new_protocol = true;
                    }
                }
            }
        }
    }
    // // Aparently Gen 3 models reuse this protocol, make sure to place their PID here and further above for wireless when developing.
    else if(pid == 0x162C || pid == 0x162D)
    {
        use_new_protocol = true;
    }

    /*-----------------------------------------------------*\
    | Send Initialization packet on new protocol.           |
    \*-----------------------------------------------------*/
    if(use_new_protocol)
    {
        memset(buf, 0x00, sizeof(buf));
        buf[0x00]   = 0x00;
        buf[0x01]   = APEX_2023_PACKET_ID_INIT;
        hid_send_feature_report(dev, buf, APEX_2023_PACKET_LENGTH);

        LOG_DEBUG("[%s] Using Apex 2023 protocol. FW: %s", name.c_str(), version_str);
    }
    else
    {
        LOG_DEBUG("[%s] Using Apex Legacy protocol. FW: %s", name.c_str(), version_str);
    }
}

std::string SteelSeriesApexController::GetSerial()
{
    if(use_new_protocol)
    {
        return "64865";
    }

    return SteelSeriesApexBaseController::GetSerial();
}
