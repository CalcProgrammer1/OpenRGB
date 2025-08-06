/*---------------------------------------------------------*\
| DreamCheekyController.cpp                                 |
|                                                           |
|   Driver for Dream Cheeky devices                         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      06 Sep 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "DreamCheekyController.h"
#include "StringUtils.h"

DreamCheekyController::DreamCheekyController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    /*-----------------------------------------------------*\
    | The Dream Cheeky Webmail Notifier requires four       |
    | initialization packets before sending colors.         |
    \*-----------------------------------------------------*/
    const unsigned char init_0[9] = { 0x00, 0x1F, 0x02, 0x00, 0x5F, 0x00, 0x00, 0x1F, 0x03 };
    const unsigned char init_1[9] = { 0x00, 0x00, 0x02, 0x00, 0x5F, 0x00, 0x00, 0x1F, 0x04 };
    const unsigned char init_2[9] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x05 };
    const unsigned char init_3[9] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

    hid_write(dev, init_0, sizeof(init_0));
    hid_write(dev, init_1, sizeof(init_1));
    hid_write(dev, init_2, sizeof(init_2));
    hid_write(dev, init_3, sizeof(init_3));
}

DreamCheekyController::~DreamCheekyController()
{
    hid_close(dev);
}

std::string DreamCheekyController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string DreamCheekyController::GetNameString()
{
    return(name);
}

std::string DreamCheekyController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void DreamCheekyController::SetColor(unsigned char red, unsigned char grn, unsigned char blu)
{
    unsigned char usb_buf[9];

    memset(usb_buf, 0, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | The Dream Cheeky Webmail Notifier color values range  |
    | from 0-64, so we scale the 0-255 input range down by  |
    | right shifting by 2 to get the range 0-63. Add 1 if   |
    | the value is exactly 255 because otherwise the maximum|
    | value of 64 would never be reached.                   |
    \*-----------------------------------------------------*/
    usb_buf[0] = 0x00;
    usb_buf[1] = (red >> 2) + (red == 255);
    usb_buf[2] = (grn >> 2) + (grn == 255);
    usb_buf[3] = (blu >> 2) + (blu == 255);
    usb_buf[4] = 0x00;
    usb_buf[5] = 0x00;
    usb_buf[6] = 0x00;
    usb_buf[7] = 0x1F;
    usb_buf[8] = 0x05;

    hid_write(dev, usb_buf, sizeof(usb_buf));
}
