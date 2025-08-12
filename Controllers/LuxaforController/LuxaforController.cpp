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

LuxaforController::LuxaforController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

LuxaforController::~LuxaforController()
{
    hid_close(dev);
}

std::string LuxaforController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LuxaforController::GetNameString()
{
    return(name);
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

void LuxaforController::SendPacket(unsigned char mode, unsigned char led, unsigned char red, unsigned char grn, unsigned char blu, unsigned char type)
{
    unsigned char usb_buf[9];

    memset(usb_buf, 0, sizeof(usb_buf));

    switch(mode)
    {
        /*-------------------------------------------------*\
        | For Direct, Fade, and Strobe, the packet format:  |
        |   0: Report ID (Always 0)                         |
        |   1: Mode (1: Direct, 2: Fade, 3: Strobe)         |
        |   2: LED Index                                    |
        |   3: Red                                          |
        |   4: Green                                        |
        |   5: Blue                                         |
        |   6: Changing Time (Fade) / Speed (Strobe)        |
        |   7: Unused                                       |
        |   8: Repeat (Strobe)                              |
        \*-------------------------------------------------*/
        case LUXAFOR_MODE_DIRECT:
        case LUXAFOR_MODE_FADE:
        case LUXAFOR_MODE_STROBE:
            usb_buf[0] = 0x00;
            usb_buf[1] = mode;
            usb_buf[2] = led;
            usb_buf[3] = red;
            usb_buf[4] = grn;
            usb_buf[5] = blu;
            usb_buf[6] = 100;
            usb_buf[7] = 0;
            usb_buf[8] = (mode == LUXAFOR_MODE_STROBE) ? 255 : 0;
            break;

        /*-------------------------------------------------*\
        | For Wave, the packet format:                      |
        |   0: Report ID (Always 0)                         |
        |   1: Mode (4: Wave)                               |
        |   2: Wave Type (1-5)                              |
        |   3: Red                                          |
        |   4: Green                                        |
        |   5: Blue                                         |
        |   6: Unused                                       |
        |   7: Repeat                                       |
        |   8: Speed                                        |
        \*-------------------------------------------------*/
        case LUXAFOR_MODE_WAVE:
            usb_buf[0] = 0x00;
            usb_buf[1] = mode;
            usb_buf[2] = type;
            usb_buf[3] = red;
            usb_buf[4] = grn;
            usb_buf[5] = blu;
            usb_buf[6] = 0;
            usb_buf[7] = 255;
            usb_buf[8] = 100;
            break;

        /*-------------------------------------------------*\
        | For Pattern, the packet format:                   |
        |   0: Report ID (Always 0)                         |
        |   1: Mode (6: Pattern)                            |
        |   2: Pattern Number (1-8)                         |
        |   3: Repeat                                       |
        |   4: Unused                                       |
        |   5: Unused                                       |
        |   6: Unused                                       |
        |   7: Unused                                       |
        |   8: Unused                                       |
        \*-------------------------------------------------*/
        case LUXAFOR_MODE_PATTERN:
            usb_buf[0] = 0x00;
            usb_buf[1] = mode;
            usb_buf[2] = type;
            usb_buf[3] = 255;
            usb_buf[4] = 0;
            usb_buf[5] = 0;
            usb_buf[6] = 0;
            usb_buf[7] = 0;
            usb_buf[8] = 0;
            break;
    }

    hid_write(dev, usb_buf, sizeof(usb_buf));
}
