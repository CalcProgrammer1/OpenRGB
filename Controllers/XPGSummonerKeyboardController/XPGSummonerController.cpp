/*---------------------------------------------------------*\
| XPGSummonerController.cpp                                 |
|                                                           |
|   Driver for XPG Summoner keyboard                        |
|                                                           |
|   Erick Granados (eriosgamer)                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "XPGSummonerController.h"
#include "StringUtils.h"

XPGSummonerController::XPGSummonerController(hid_device *dev_handle, const char *path, const unsigned short pid, std::string dev_name)
{
    dev      = dev_handle;
    location = path;
    name     = dev_name;
    usb_pid  = pid;

    SendInitialize();
}

XPGSummonerController::~XPGSummonerController()
{
    hid_close(dev);
}

void XPGSummonerController::SendInitialize()
{
    unsigned char init_buf[265] =
    {
        0x07, 0xEA, 0x00, 0x00,
    };
    memset(init_buf + 4, 0x00, 261);
    hid_write(dev, init_buf, 265);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

std::string XPGSummonerController::GetLocationString()
{
    return("HID: " + location);
}

std::string XPGSummonerController::GetNameString()
{
    return(name);
}

std::string XPGSummonerController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

unsigned short XPGSummonerController::GetUSBPID()
{
    return(usb_pid);
}

void XPGSummonerController::SendColors(unsigned char *color_data, unsigned int color_data_size)
{
    const int leds_count     = 126;
    const int bytes_per_led  = 4;
    const int total_bytes    = leds_count * bytes_per_led;
    const int block_size     = 256;
    int zones                = 2;

    for(int zone = 0; zone < zones; zone++)
    {
        int offset      = zone * block_size;
        int remaining   = total_bytes - offset;
        color_data_size = (remaining > block_size) ? block_size : remaining;

        SendColorDataPacket(zone, &color_data[offset], color_data_size);
    }
}

unsigned int XPGSummonerController::SendColorDataPacket(
    unsigned char packet_id,
    unsigned char *color_data,
    unsigned int color_size)
{
    unsigned char packet[265] = {0};
    packet[0] = 0x07;
    packet[1] = 0xA3;
    packet[2] = 0x08;
    packet[3] = 0x00;
    packet[4] = packet_id;
    packet[5] = 0x00;

    unsigned int copy_size = (color_size > 256) ? 256 : color_size;
    memcpy(&packet[6], color_data, copy_size);

    hid_write(dev, packet, 265);

    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    return copy_size;
}

void XPGSummonerController::SendTerminateColorPacket()
{
    /*-------------------------------*\
    | Set up Terminate Color packet   |
    | This packet is used to stop     |
    | any active color effects on the |
    | keyboard.                       |
    \*-------------------------------*/
    unsigned char terminate_buf[265] =
    {
        0x07, 0xEA, 0x00, 0x00,
    };
    memset(terminate_buf + 4, 0x00, 261);
    hid_write(dev, terminate_buf, 265);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}
