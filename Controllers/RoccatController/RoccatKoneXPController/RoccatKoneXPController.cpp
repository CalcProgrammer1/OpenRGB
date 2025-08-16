/*---------------------------------------------------------*\
| RoccatKoneXPController.cpp                                |
|                                                           |
|   Driver for Roccat Kone XP                               |
|                                                           |
|   Mola19                                      12 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <cstring>
#include <thread>
#include "LogManager.h"
#include "RoccatKoneXPController.h"
#include "StringUtils.h"

RoccatKoneXPController::RoccatKoneXPController(hid_device* dev_handle, char *path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

RoccatKoneXPController::~RoccatKoneXPController()
{
    hid_close(dev);
}

std::string RoccatKoneXPController::GetLocation()
{
    return("HID: " + location);
}

std::string RoccatKoneXPController::GetName()
{
    return(name);
}

std::string RoccatKoneXPController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));

}

std::string RoccatKoneXPController::GetVersion()
{
    uint8_t buf[9] = { 0x09 };
    int return_length = hid_get_feature_report(dev, buf, 9);

    if(return_length == -1)
    {
        LOG_DEBUG("[Roccat Kone XP]: Could not fetch version. HIDAPI Error: %ls", hid_error(dev));
        return std::string("Unknown");
    }

    char version[5];
    snprintf(version, 5, "%d.%02d", buf[2] / 100, buf[2] % 100);

    return std::string(version);
}

uint8_t RoccatKoneXPController::GetActiveProfile()
{
    uint8_t buf[4] = { 0x05 };
    int return_length = hid_get_feature_report(dev, buf, 4);

    if(return_length == -1)
    {
        LOG_DEBUG("[Roccat Kone XP]: Could not fetch active profile. HIDAPI Error: %ls", hid_error(dev));
        return 0;
    }

    return buf[2];
}

#include <iostream>
roccat_kone_xp_mode_struct RoccatKoneXPController::GetMode()
{
    uint8_t buf[0xAE] = { 0x06 };
    int return_length = hid_get_feature_report(dev, buf, 0xAE);

    if(return_length == -1)
    {
        LOG_DEBUG("[Roccat Kone XP]: Could not fetch mode. HIDAPI Error: %ls", hid_error(dev));
        roccat_kone_xp_mode_struct default_mode;
        return default_mode;
    }

    roccat_kone_xp_mode_struct active_mode;

    active_mode.profile     = buf[2];
    active_mode.byte_3      = buf[3];
    active_mode.byte_4      = buf[4];
    active_mode.dpi_flag    = buf[5];
    active_mode.byte_6      = buf[6];

    for(uint8_t i = 0; i < 10; i++)
    {
        active_mode.dpi[i] = (buf[8 + i * 2] << 8) + buf[7 + i * 2];
    }

    active_mode.angle_snapping  = (bool) buf[27];
    active_mode.byte_28         = buf[28];
    active_mode.polling_rate    = buf[29];
    active_mode.mode            = buf[30];
    active_mode.speed           = buf[31];
    active_mode.brightness      = buf[32];
    active_mode.time_until_idle = buf[33];
    active_mode.idle_mode       = buf[34];
    active_mode.byte_35         = buf[35];

    for(uint8_t i = 0; i < 20; i++)
    {
        active_mode.colors[i].brightness = buf[37 + i * 6];
        active_mode.colors[i].color = ToRGBColor(
            buf[38 + i * 6],
            buf[39 + i * 6],
            buf[40 + i * 6]
        );
    }

    active_mode.byte_156                    = buf[156];
    active_mode.byte_157                    = buf[157];
    active_mode.profile_color_brightness    = buf[158];
    active_mode.profile_color_red           = buf[159];
    active_mode.profile_color_green         = buf[160];
    active_mode.profile_color_blue          = buf[161];
    active_mode.byte_162                    = buf[162];
    active_mode.theme                       = buf[163];
    active_mode.auto_dpi_flag               = buf[164];

    active_mode.end_bytes[0] = buf[165];
    active_mode.end_bytes[1] = buf[166];
    active_mode.end_bytes[2] = buf[167];
    active_mode.end_bytes[3] = buf[168];
    active_mode.end_bytes[4] = buf[169];
    active_mode.end_bytes[5] = buf[170];
    active_mode.end_bytes[6] = buf[171];

    return active_mode;
}

void RoccatKoneXPController::EnableDirect(bool on_off_switch)
{
    unsigned char usb_buf[6];

    usb_buf[0x00] = 0x0E;
    usb_buf[0x01] = 0x06;
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = on_off_switch;
    usb_buf[0x04] = 0x00;
    usb_buf[0x05] = 0xFF;

    int return_length = hid_send_feature_report(dev, usb_buf, 6);

    if(return_length == -1)
    {
        LOG_DEBUG("[Roccat Kone XP]: Could not send mode. HIDAPI Error: %ls", hid_error(dev));
    }
}

void RoccatKoneXPController::SetReadProfile(uint8_t profile)
{
    unsigned char usb_buf[4];

    usb_buf[0x00] = 0x04;
    usb_buf[0x01] = profile;
    usb_buf[0x02] = 0x80;
    usb_buf[0x03] = 0xFF;

    int return_length = hid_send_feature_report(dev, usb_buf, 4);

    if(return_length == -1)
    {
        LOG_DEBUG("[Roccat Kone XP]: Could not set profile to read. HIDAPI Error: %ls", hid_error(dev));
    }
}

void RoccatKoneXPController::SetMode(roccat_kone_xp_mode_struct mode)
{
    uint8_t buf[0xAE];
    memset(buf, 0x00, 0xAE);

    buf[0x00] = 0x06;
    buf[0x01] = 0xAE;

    buf[0x02] = mode.profile;
    buf[0x03] = mode.byte_3;
    buf[0x04] = mode.byte_4;
    buf[0x05] = mode.dpi_flag;
    buf[0x06] = mode.byte_6;

    for(uint8_t i = 0; i < 10; i++)
    {
        buf[0x07 + i * 2] = mode.dpi[i] & 0xFF;
        buf[0x08 + i * 2] = mode.dpi[i] >> 8;
    }

    buf[0x1B] = mode.angle_snapping;
    buf[0x1C] = mode.byte_28;
    buf[0x1D] = mode.polling_rate;
    buf[0x1E] = mode.mode;
    buf[0x1F] = mode.speed;
    buf[0x20] = mode.brightness;
    buf[0x21] = mode.time_until_idle;
    buf[0x22] = mode.idle_mode;
    buf[0x23] = mode.byte_35;

    for(uint8_t i = 0; i < 20; i++)
    {
        buf[0x24 + i * 6] = mode.colors[i].byte_0;
        buf[0x25 + i * 6] = 0xFF;
        buf[0x26 + i * 6] = RGBGetRValue(mode.colors[i].color);
        buf[0x27 + i * 6] = RGBGetGValue(mode.colors[i].color);
        buf[0x28 + i * 6] = RGBGetBValue(mode.colors[i].color);
        buf[0x29 + i * 6] = mode.colors[i].byte_5;
    }

    buf[0x9C] = mode.byte_156;
    buf[0x9D] = mode.byte_157;
    buf[0x9E] = mode.profile_color_brightness;
    buf[0x9F] = mode.profile_color_red;
    buf[0xA0] = mode.profile_color_green;
    buf[0xA1] = mode.profile_color_blue;
    buf[0xA2] = mode.byte_162;
    buf[0xA3] = mode.theme & ~0x80; // this stores the swarm intern selected theme (biggest bit is a flag for custom theme)
    buf[0xA4] = mode.auto_dpi_flag;

    for(uint8_t i = 0; i < 7; i++)
    {
        buf[0xA5 + i] = mode.end_bytes[i];
    }

    unsigned short total = 0;
    for(int i = 0; i < 0xAE - 2; i++) total += buf[i];

    buf[0xAE - 2] = total & 0xFF;
    buf[0xAE - 1] = total >> 8;


    int return_length = hid_send_feature_report(dev, buf, 0xAE);

    if(return_length == -1)
    {
        LOG_DEBUG("[Roccat Kone XP]: Could not send mode. HIDAPI Error: %ls", hid_error(dev));
    }
}

void RoccatKoneXPController::SendDirect(std::vector<RGBColor> colors)
{
    uint8_t buf[0x7A];
    memset(buf, 0x00, 0x7A);

    buf[0x00] = 0x0D;
    buf[0x01] = 0x7A;

    for(uint8_t i = 0; i < colors.size() && i < 20; i++)
    {
        /*-----------------------------------------------------------*\
        | This device uses rgbrgb which means that e.g. the red value |
        | is calculated by multiplying both given red values.         |
        | Maybe it is some sort of brightness?                        |
        | For OpenRGBs purpose these aren't usefull,                  |
        | so the second part is always 0xff.                          |
        \*-----------------------------------------------------------*/
        buf[0x02 + i * 6] = RGBGetRValue(colors[i]);
        buf[0x03 + i * 6] = RGBGetGValue(colors[i]);
        buf[0x04 + i * 6] = RGBGetBValue(colors[i]);
        buf[0x05 + i * 6] = 0xFF;
        buf[0x06 + i * 6] = 0xFF;
        buf[0x07 + i * 6] = 0xFF;
    }

    int return_length = hid_send_feature_report(dev, buf, 0x7A);

    if(return_length == -1)
    {
        LOG_DEBUG("[Roccat Kone XP]: Could not send direct. HIDAPI Error: %ls", hid_error(dev));
    }
}

void RoccatKoneXPController::WaitUntilReady()
{
    uint8_t buf[4];
    memset(buf, 0x00, 4);

    buf[0] = 0x04;

    for(unsigned char i = 0; buf[1] != 1 && i < 100; i++)
    {
        if(i != 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        hid_get_feature_report(dev, buf, 4);
    }
}
