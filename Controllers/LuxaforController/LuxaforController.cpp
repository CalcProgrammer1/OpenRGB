/*---------------------------------------------------------*\
| LuxaforController.cpp                                     |
|                                                           |
|   Driver for Luxafor devices                              |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      05 Sep 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LuxaforController.h"
#include "StringUtils.h"

LuxaforController::LuxaforController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

LuxaforController::~LuxaforController()
{
    hid_close(dev);
}

std::string LuxaforController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LuxaforController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void LuxaforController::SendPacket(unsigned char mode, unsigned char led, unsigned char red, unsigned char grn, unsigned char blu)
{
    unsigned char usb_buf[9];

    memset(usb_buf, 0, sizeof(usb_buf));

    usb_buf[0] = 0x00;
    usb_buf[1] = mode;
    usb_buf[2] = led;
    usb_buf[3] = red;
    usb_buf[4] = grn;
    usb_buf[5] = blu;
    usb_buf[6] = 100;
    usb_buf[7] = 0;
    usb_buf[8] = 255;

    hid_write(dev, usb_buf, sizeof(usb_buf));
}
