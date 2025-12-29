/*---------------------------------------------------------*\
| AresonController.cpp                                      |
|                                                           |
|   Driver for Areson mice                                  |
|                                                           |
|   Morgan Guimard (morg)                       29 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string.h>
#include "StringUtils.h"
#include "AresonController.h"

AresonController::AresonController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
}

AresonController::~AresonController()
{
    hid_close(dev);
}

std::string AresonController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AresonController::GetNameString()
{
    return(name);
}

std::string AresonController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

unsigned char AresonController::GetSpeedValue(unsigned char speed, unsigned char mode_value)
{
    unsigned char speed_values_lookup_high[ARESON_SPEED_MAX] =
    {
        0xFF,  0xE6, 0xD2, 0xBe, 0xAA, 0x96, 0x82, 0x6E, 0x46, 0x28
    };

        unsigned char speed_values_lookup_low[ARESON_SPEED_MAX] =
    {
        0x2D,  0x28, 0x23, 0x1E, 0x19, 0x13, 0x0F, 0x0A, 0x05, 0x03
    };

    switch (mode_value)
    {
        case BREATHING_MODE_VALUE:
        case SPECRTUM_CYCLE_MODE_VALUE:
        case SINGLE_COLOR_WAVE_MODE_VALUE:
        case BREATHING_COLORFUL_MODE_VALUE:
            return speed_values_lookup_high[speed -1];

        case RAINBOW_WAVE_MODE_VALUE:
            return speed_values_lookup_low[speed -1];

        default:
            return 0;
    }
}

void AresonController::SetMode(RGBColor color, unsigned char brightness, unsigned char speed, unsigned char mode_value)
{
    /*---------------------------------------------------------*\
    | Init the packet buffer                                    |
    \*---------------------------------------------------------*/
    unsigned char usb_buf[ARESON_PACKET_SIZE];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*---------------------------------------------------------*\œ
    | Constant data                                             |
    \*---------------------------------------------------------*/
    usb_buf[0x00] = ARESON_REPORT_ID;
    usb_buf[0x01] = 0x07;
    usb_buf[0x04] = 0xA0;
    usb_buf[0x05] = 0x07;

    /*---------------------------------------------------------*\
    | Set the mode                                              |
    \*---------------------------------------------------------*/
    usb_buf[0x06] = mode_value;

    /*---------------------------------------------------------*\
    | Set the color                                             |
    \*---------------------------------------------------------*/
    usb_buf[0x07] = RGBGetRValue(color);
    usb_buf[0x08] = RGBGetGValue(color);
    usb_buf[0x09] = RGBGetBValue(color);

    /*---------------------------------------------------------*\
    | Set speed if needed                                       |
    \*---------------------------------------------------------*/
    usb_buf[0x0A] = GetSpeedValue(speed, mode_value);

    /*---------------------------------------------------------*\
    | Set brightness if needed                                  |
    \*---------------------------------------------------------*/
    if(mode_value != OFF_MODE_VALUE)
    {
        unsigned char brightness_values[ARESON_BRIGHTNESS_MAX] =
        {
            0x19,  0x32, 0x4B, 0x64, 0x7D, 0x96, 0xAF, 0xC8, 0xE1, 0xFF
        };

        usb_buf[0x0B] = brightness_values[brightness - 1];
    }

    /*---------------------------------------------------------*\
    | Custom CRC - thanks to Vaker for this <3                  |
    \*---------------------------------------------------------*/
    usb_buf[0x0C] = 0x55 - usb_buf[0x06] - usb_buf[0x07] - usb_buf[0x08] - usb_buf[0x09] - usb_buf[0x0A] - usb_buf[0x0B];

    /*---------------------------------------------------------*\
    | Constant data                                             |
    \*---------------------------------------------------------*/
    usb_buf[0x10] = ARESON_PACKET_END;

    /*---------------------------------------------------------*\
    | Send the report                                           |
    \*---------------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, ARESON_PACKET_SIZE);
}
