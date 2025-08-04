/*---------------------------------------------------------*\
| AOCMouseController.cpp                                    |
|                                                           |
|   Driver for AOC mouse                                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                20 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "AOCMouseController.h"
#include "StringUtils.h"

AOCMouseController::AOCMouseController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

AOCMouseController::~AOCMouseController()
{
    hid_close(dev);
}

std::string AOCMouseController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string AOCMouseController::GetDeviceName()
{
    return(name);
}

std::string AOCMouseController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void AOCMouseController::SendDirect
    (
    RGBColor*       color_data
    )
{
    SendPacket(AOC_MOUSE_MODE_STATIC_SINGLE_COLOR,
               AOC_MOUSE_BRIGHTNESS_HIGH,
               AOC_MOUSE_SPEED_MEDIUM,
               AOC_MOUSE_DIRECTION_CLOCKWISE,
               color_data);
}

void AOCMouseController::SendPacket
    (
    unsigned char   mode,
    unsigned char   brightness,
    unsigned char   speed,
    unsigned char   direction,
    RGBColor*       color_data
    )
{
    unsigned char buf[60];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up packet                                         |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x20;
    buf[0x01]   = 0x03;
    buf[0x02]   = 0x01;
    buf[0x03]   = mode;
    buf[0x04]   = speed;
    buf[0x05]   = brightness;
    buf[0x06]   = direction;
    buf[0x07]   = 0x01;
    buf[0x08]   = 0x02;
    buf[0x09]   = 0xFF;
    buf[0x0D]   = 0x01;
    buf[0x0E]   = 0x03;
    buf[0x0F]   = 0xFF;
    buf[0x10]   = 0x7F;
    buf[0x13]   = 0x01;
    buf[0x14]   = 0x04;
    buf[0x17]   = 0xFF;
    buf[0x19]   = 0x01;
    buf[0x1A]   = 0x05;
    buf[0x1C]   = 0xFF;
    buf[0x1F]   = 0x01;
    buf[0x20]   = 0x06;
    buf[0x21]   = 0xFF;
    buf[0x23]   = 0xFF;
    buf[0x25]   = 0x01;
    buf[0x26]   = 0x07;
    buf[0x27]   = 0xFF;
    buf[0x28]   = 0xFF;
    buf[0x2C]   = 0x0A;
    buf[0x2D]   = 0x0A;
    buf[0x30]   = 0x14;
    buf[0x31]   = 0x08;
    buf[0x3A]   = 0x32;
    buf[0x3B]   = 0x32;

    /*-----------------------------------------------------*\
    | Copy in color                                         |
    \*-----------------------------------------------------*/
    buf[0x33]   = RGBGetRValue(color_data[0]);
    buf[0x34]   = RGBGetGValue(color_data[0]);
    buf[0x35]   = RGBGetBValue(color_data[0]);

    buf[0x36]   = RGBGetRValue(color_data[1]);
    buf[0x37]   = RGBGetGValue(color_data[1]);
    buf[0x38]   = RGBGetBValue(color_data[1]);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 60);
}
