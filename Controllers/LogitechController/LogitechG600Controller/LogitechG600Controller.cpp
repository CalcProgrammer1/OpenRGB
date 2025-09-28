/*---------------------------------------------------------*\
| LogitechG600Controller.cpp                                |
|                                                           |
|   Driver for Logitech G600 Gaming Mouse                   |
|                                                           |
|   Austin B     (austinleroy)                11 Sep 2025   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogitechG600Controller.h"
#include "StringUtils.h"

LogitechG600Controller::LogitechG600Controller(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev       = dev_handle;
    location  = path;
    name      = dev_name;
}

LogitechG600Controller::~LogitechG600Controller()
{
    if(dev != nullptr)
    {
        hid_close(dev);
    }
}

std::string LogitechG600Controller::GetNameString()
{
    return(name);
}

std::string LogitechG600Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void LogitechG600Controller::SetMode
    (
    unsigned char       mode,
    unsigned short      speed,
    RGBColor            color
    )
{
    unsigned char usb_buf[8];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0xF1;
    usb_buf[0x01] = RGBGetRValue(color);
    usb_buf[0x02] = RGBGetGValue(color);
    usb_buf[0x03] = RGBGetBValue(color);
    usb_buf[0x04] = mode;
    usb_buf[0x05] = speed & 0xFF;

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}
