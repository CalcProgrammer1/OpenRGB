/*---------------------------------------------------------*\
| SteelSeriesQCKControllerMat.cpp                           |
|                                                           |
|   Driver for SteelSeries Mouse                            |
|                                                           |
|   Edbgon                                      22 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "SteelSeriesQCKMatController.h"
#include "StringUtils.h"

SteelSeriesQCKMatController::SteelSeriesQCKMatController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

SteelSeriesQCKMatController::~SteelSeriesQCKMatController()
{
    hid_close(dev);
}

std::string SteelSeriesQCKMatController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string SteelSeriesQCKMatController::GetDeviceName()
{
    return(name);
}

std::string SteelSeriesQCKMatController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void SteelSeriesQCKMatController::SetColors(std::vector<RGBColor> colors)
{
    unsigned char buf[525];
    unsigned char cbuf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));
    memset(cbuf, 0x00, sizeof(cbuf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;
    buf[0x01]   = 0x0E;
    buf[0x03]   = 0x02;

    buf[0x08]   = 0xFF;
    buf[0x09]   = 0x32;
    buf[0x0A]   = 0xC8;
    buf[0x0E]   = 0x01;

    buf[0x14]   = 0xFF;
    buf[0x15]   = 0x32;
    buf[0x16]   = 0xC8;
    buf[0x19]   = 0x01;

    buf[0x1A]   = 0x01;
    buf[0x1C]   = 0x01;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    buf[0x05] = RGBGetRValue(colors[0]);
    buf[0x06] = RGBGetGValue(colors[0]);
    buf[0x07] = RGBGetBValue(colors[0]);

    buf[0x11] = RGBGetRValue(colors[1]);
    buf[0x12] = RGBGetGValue(colors[1]);
    buf[0x13] = RGBGetBValue(colors[1]);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 525);

    cbuf[0x01] = 0x0D;
    hid_write(dev, cbuf, 65);
}
