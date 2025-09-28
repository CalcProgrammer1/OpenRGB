/*---------------------------------------------------------*\
| IntelArcA770LEController.cpp                              |
|                                                           |
|   Driver for Intel Arc A770 LE                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                01 Nov 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "IntelArcA770LEController.h"
#include "StringUtils.h"

IntelArcA770LEController::IntelArcA770LEController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

IntelArcA770LEController::~IntelArcA770LEController()
{
    hid_close(dev);
}

std::string IntelArcA770LEController::GetLocationString()
{
    return("HID: " + location);
}

std::string IntelArcA770LEController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string IntelArcA770LEController::GetFirmwareVersionString()
{
    std::string ret_string = "";

    unsigned char usb_buf[] =
    {
        0x00,
        0x12, 0x20, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    unsigned char   fw_buf[16] = {0x00};

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);

    for(int char_idx = 0; char_idx < 16; char_idx+=2)
    {
        if(usb_buf[char_idx + 0x08] != 0)
        {
            fw_buf[char_idx / 2] = usb_buf[char_idx + 0x08];
        }
        else
        {
            break;
        }
    }

    ret_string.append((char *)fw_buf);

    return(ret_string);
}

void IntelArcA770LEController::SendEnableCommand()
{
    unsigned char usb_buf[] =
    {
        0x00,
        0x41, 0x03, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
}

void IntelArcA770LEController::SendApplyCommand()
{
    unsigned char usb_buf[] =
    {
        0x00,
        0x51, 0x28, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
}

void IntelArcA770LEController::SendDirectPacket
    (
    unsigned char   size,
    unsigned char * led_ids,
    RGBColor *      colors
    )
{
    unsigned char usb_buf[] =
    {
        0x00,
        0xC0, 0x01, size, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    for(unsigned int led_idx = 0; led_idx < size; led_idx++)
    {
        unsigned int index  = led_idx * 4;

        /*-----------------------------------------*\
        | Special handling for Logo LED (0x96)      |
        | Use the maximum channel value as this is  |
        | a white LED using the red channel         |
        \*-----------------------------------------*/
        if(led_ids[led_idx] == 0x96)
        {
            usb_buf[index + 5]  = led_ids[led_idx];

            usb_buf[index + 6]  = std::max(RGBGetRValue(colors[led_idx]), std::max(RGBGetGValue(colors[led_idx]), RGBGetBValue(colors[led_idx])));
            usb_buf[index + 7]  = 0;
            usb_buf[index + 8]  = 0;
        }
        else
        {
            usb_buf[index + 5]  = led_ids[led_idx];
            usb_buf[index + 6]  = RGBGetRValue(colors[led_idx]);
            usb_buf[index + 7]  = RGBGetGValue(colors[led_idx]);
            usb_buf[index + 8]  = RGBGetBValue(colors[led_idx]);
        }
    }

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
}
