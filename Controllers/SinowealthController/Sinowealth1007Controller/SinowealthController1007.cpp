/*---------------------------------------------------------*\
| SinowealthController1007.cpp                              |
|                                                           |
|   Driver for Sinowealth mice with PID 1007                |
|                                                           |
|   Moon_darker (Vaker)                         02 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogManager.h"
#include "SinowealthController1007.h"
#include "StringUtils.h"

SinowealthController1007::SinowealthController1007(hid_device* dev, char *_path, std::string dev_name)
{
    this->dev               = dev;
    this->location          = _path;
    this->name              = dev_name;

    this->led_count         = 7;
    this->current_mode      = ZET_FURY_PRO_MODE_CUSTOM + ZET_FURY_PRO_SPEED_DEF;
    this->current_direction = ZET_FURY_PRO_DIR_RIGHT;

    memset(device_colors, 0x00, sizeof(device_colors));
}

SinowealthController1007::~SinowealthController1007()
{
    hid_close(dev);
}

std::string SinowealthController1007::GetLocation()
{
    return("HID: " + location);
}

std::string SinowealthController1007::GetName()
{
    return(name);
}

unsigned int SinowealthController1007::GetLEDCount()
{
    return(led_count);
}

std::string SinowealthController1007::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void SinowealthController1007::SetLEDColors(const std::vector<RGBColor>& colors)
{
    memset(device_colors, 0x00, sizeof(device_colors));

    unsigned int color_counter = 0;
    for (RGBColor color: colors)
    {
        unsigned int pkt_pointer = (color_counter * 3); // 3 bytes per color

        device_colors[pkt_pointer]     = RGBGetRValue(color);
        device_colors[pkt_pointer + 1] = RGBGetGValue(color);
        device_colors[pkt_pointer + 2] = RGBGetBValue(color);

        if (++color_counter == 7) break;
    }

    SendPacket();
}

void SinowealthController1007::SetMode(
    unsigned char                mode,
    unsigned char                spd_or_lum,
    unsigned char                direction,
    const std::vector<RGBColor>& colors,
    unsigned char                random,
    bool                         has_per_led_colors)
{
    current_mode = mode + (spd_or_lum ? spd_or_lum : ZET_FURY_PRO_SPEED_DEF);
    current_direction = random ? random : direction;

    if (!has_per_led_colors)
    {
        memset(device_colors, 0x00, sizeof(device_colors));
        SetLEDColors(colors);
    }
}

void SinowealthController1007::SendPacket()
{
    if (GetProfile() < 0) return;

    unsigned char usb_buf[ZET_FURY_PRO_STATE_BUFFER_LENGTH];
    memcpy(usb_buf,      device_configuration, sizeof(usb_buf));
    memcpy(usb_buf + 23, device_colors,        sizeof(device_colors)); // colors are bytes 23-43 in RGB format counting from 0

    usb_buf[21] = current_mode;
    usb_buf[22] = current_direction;

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

int SinowealthController1007::GetProfile()
{
    int bytesReceived;

    memset(device_configuration, 0x00, ZET_FURY_PRO_STATE_BUFFER_LENGTH);
    device_configuration[0] = 0x04;

    bytesReceived = hid_get_feature_report(dev, device_configuration, ZET_FURY_PRO_STATE_BUFFER_LENGTH);
    if (bytesReceived < 0)
    {
        LOG_ERROR("[ZET Fury Pro] Error reading device configuration!");
    }

    return bytesReceived;
}
