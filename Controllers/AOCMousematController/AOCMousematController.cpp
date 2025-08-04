/*---------------------------------------------------------*\
| AOCMousematController.cpp                                 |
|                                                           |
|   Driver for AOC mousemat                                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "AOCMousematController.h"
#include "StringUtils.h"

AOCMousematController::AOCMousematController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

AOCMousematController::~AOCMousematController()
{
    hid_close(dev);
}

std::string AOCMousematController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string AOCMousematController::GetDeviceName()
{
    return(name);
}

std::string AOCMousematController::GetSerialString()
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

void AOCMousematController::SendDirect
    (
    RGBColor*       color_data
    )
{
    SendPacket(AOC_MOUSEMAT_MODE_STATIC_SINGLE_COLOR,
               AOC_MOUSEMAT_BRIGHTNESS_HIGH,
               AOC_MOUSEMAT_SPEED_MEDIUM,
               AOC_MOUSEMAT_DIRECTION_CLOCKWISE,
               color_data);
}

void AOCMousematController::SendPacket
    (
    unsigned char   mode,
    unsigned char   brightness,
    unsigned char   speed,
    unsigned char   direction,
    RGBColor*       color_data
    )
{
    unsigned char buf[32];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up packet                                         |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x20;
    buf[0x01]   = brightness;
    buf[0x02]   = speed;
    buf[0x03]   = direction;
    buf[0x04]   = 0x01;
    buf[0x05]   = mode;
    buf[0x09]   = 0xFF;
    buf[0x0C]   = 0xFF;
    buf[0x0D]   = 0x3F;
    buf[0x0F]   = 0xFF;
    buf[0x10]   = 0xFF;
    buf[0x13]   = 0xFF;
    buf[0x17]   = 0xFF;
    buf[0x19]   = 0xFF;
    buf[0x1A]   = 0xFF;
    buf[0x1B]   = 0xFF;
    buf[0x1D]   = 0xFF;
    buf[0x1E]   = 0x32;
    buf[0x1F]   = 0x32;

    /*-----------------------------------------------------*\
    | Copy in color                                         |
    \*-----------------------------------------------------*/
    buf[0x06]   = RGBGetRValue(color_data[0]);
    buf[0x07]   = RGBGetGValue(color_data[0]);
    buf[0x08]   = RGBGetBValue(color_data[0]);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 32);
}
