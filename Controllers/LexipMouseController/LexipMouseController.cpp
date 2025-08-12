/*---------------------------------------------------------*\
| LexipMouseController.cpp                                  |
|                                                           |
|   Driver for Lexip mouse                                  |
|                                                           |
|   Morgan Guimard (morg)                       21 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "LexipMouseController.h"
#include "StringUtils.h"

LexipMouseController::LexipMouseController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
}

LexipMouseController::~LexipMouseController()
{
    hid_close(dev);
}

std::string LexipMouseController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LexipMouseController::GetNameString()
{
    return(name);
}

std::string LexipMouseController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void LexipMouseController::SetDirect(RGBColor color)
{
    /*-----------------------------------------*\
    |  Send a change color packet               |
    |                                           |
    |  URB INTERRUPT OUT, pad a leading zero    |
    |                                           |
    |  00 24 01 RR GG BB 00 64 80 00 .... 00    |
    \*-----------------------------------------*/
    unsigned char usb_buf[PACKET_DATA_LENGTH];

    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[0x01] = 0x24;                   // constant data
    usb_buf[0x02] = 0x01;                   // constant data

    usb_buf[0x03] = RGBGetRValue(color);    // red channel
    usb_buf[0x04] = RGBGetGValue(color);    // green channel
    usb_buf[0x05] = RGBGetBValue(color);    // blue channel

    usb_buf[0x07] = 0x64;                   // constant data
    usb_buf[0x08] = 0x80;                   // constant data

    hid_write(dev, usb_buf, PACKET_DATA_LENGTH);
}
