/*---------------------------------------------------------*\
| AOCKeyboardController.cpp                                 |
|                                                           |
|   Driver for AOC keyboard                                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                10 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <cstring>
#include <thread>
#include "AOCKeyboardController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

AOCKeyboardController::AOCKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

AOCKeyboardController::~AOCKeyboardController()
{
    hid_close(dev);
}

std::string AOCKeyboardController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string AOCKeyboardController::GetDeviceName()
{
    return(name);
}

std::string AOCKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void AOCKeyboardController::SetLightingConfig
    (
    unsigned char   mode,
    unsigned char   random,
    unsigned char   brightness,
    unsigned char   speed,
    unsigned char   direction,
    RGBColor*       color_data
    )
{
    SendStartPacket();
    std::this_thread::sleep_for(5ms);

    SendLightingConfigPacket(mode, random, brightness, speed, direction, color_data);
    std::this_thread::sleep_for(5ms);

    SendEndPacket();
    std::this_thread::sleep_for(10ms);
}

void AOCKeyboardController::SetCustom
    (
    RGBColor*       color_data
    )
{
    SendStartPacket();
    std::this_thread::sleep_for(5ms);

    SendCustomPacket(color_data);
    std::this_thread::sleep_for(5ms);

    SendEndPacket();
    std::this_thread::sleep_for(5ms);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void AOCKeyboardController::SendStartPacket()
{
    unsigned char buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up start packet                                   |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x09;
    buf[0x01]   = 0x21;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, buf, sizeof(buf));
}

void AOCKeyboardController::SendEndPacket()
{
    unsigned char buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up end packet                                     |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x09;
    buf[0x01]   = 0x22;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, buf, sizeof(buf));
}

void AOCKeyboardController::SendCustomPacket
    (
    RGBColor*       color_data
    )
{
    unsigned char buf[361];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up custom lighting packet                         |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x20;

    /*-----------------------------------------------------*\
    | Copy in color data                                    |
    \*-----------------------------------------------------*/
    for(unsigned int color_idx = 0; color_idx < 120; color_idx++)
    {
        buf[color_idx + 1]      = RGBGetRValue(color_data[color_idx]);
        buf[color_idx + 121]    = RGBGetGValue(color_data[color_idx]);
        buf[color_idx + 241]    = RGBGetBValue(color_data[color_idx]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, sizeof(buf));
}

void AOCKeyboardController::SendLightingConfigPacket
    (
    unsigned char   mode,
    unsigned char   random,
    unsigned char   brightness,
    unsigned char   speed,
    unsigned char   direction,
    RGBColor*       color_data
    )
{
    unsigned char buf[117];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up lighting configuration packet                  |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x14;
    buf[0x01]   = 0x01;

    buf[0x06]   = mode;

    buf[0x07 + (9 * mode) + 0] = RGBGetRValue(color_data[0]);
    buf[0x07 + (9 * mode) + 1] = RGBGetGValue(color_data[0]);
    buf[0x07 + (9 * mode) + 2] = RGBGetBValue(color_data[0]);
    buf[0x07 + (9 * mode) + 3] = random;
    buf[0x07 + (9 * mode) + 4] = direction;
    buf[0x07 + (9 * mode) + 5] = speed;
    buf[0x07 + (9 * mode) + 6] = brightness;

    unsigned short checksum = 0x4A9E;

    for(unsigned int buf_idx = 0; buf_idx < 115; buf_idx++)
    {
        checksum += buf[buf_idx];
    }

    buf[115]    = checksum & 0xFF;
    buf[116]    = checksum >> 8;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 117);
}
