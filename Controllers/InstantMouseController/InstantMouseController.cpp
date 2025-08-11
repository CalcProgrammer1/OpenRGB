/*---------------------------------------------------------*\
| InstantMouseController.cpp                                |
|                                                           |
|   Driver for Instant mouse                                |
|                                                           |
|   Morgan Guimard (morg)                       19 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "InstantMouseController.h"
#include "StringUtils.h"

InstantMouseController::InstantMouseController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    pid                 = info.product_id;
    name                = dev_name;
}

InstantMouseController::~InstantMouseController()
{
    hid_close(dev);
}

std::string InstantMouseController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string InstantMouseController::GetNameString()
{
    return(name);
}

std::string InstantMouseController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

uint16_t InstantMouseController::GetPID()
{
    return pid;
}

void InstantMouseController::SendColor(RGBColor color)
{
    /*---------------------------------------------------------*\
    | Packet details                                            |
    | 07 14 XG RB 00 00 00 00                                   |
    | where X is the DPI slot                                   |
    \*---------------------------------------------------------*/

    uint8_t red = 0xF - RGBGetRValue(color) / 16;
    uint8_t grn = 0xF - RGBGetGValue(color) / 16;
    uint8_t blu = 0xF - RGBGetBValue(color) / 16;

    uint8_t pkt[INSTANT_MOUSE_REPORT_SIZE];
    memset(pkt, 0 , INSTANT_MOUSE_REPORT_SIZE);

    pkt[0] = INSTANT_MOUSE_REPORT_ID;
    pkt[1] = INSTANT_MOUSE_SET_COLOR;
    pkt[3] = (red << 4) | (blu & 0xF);

    for(unsigned int i = 0; i <= 0xA; i+= 2)
    {
        pkt[2] = (i << 4) | (grn & 0xF);
        hid_send_feature_report(dev, pkt, INSTANT_MOUSE_REPORT_SIZE);
    }
}

void InstantMouseController::SetMode(uint8_t mode_value, uint8_t speed, uint8_t brightness, uint8_t direction)
{
    /*---------------------------------------------------------*\
    | Packet details:                                           |
    | 07 13 FF MS DN -B M- --                                   |
    |                                                           |
    | 07 = report id                                            |
    | 13 = set mode function                                    |
    | FF = constant                                             |
    | - = still undiscovered/useless                            |
    | M = mode                                                  |
    | S = speed 0 -> 5  (6 and above makes the effect static)   |
    | N = number of leds                                        |
    | D = direction (0/1)                                       |
    | B = brightness (0 full, 7 off)                            |
    \*---------------------------------------------------------*/

    uint8_t led_mask = 0xB;

    uint8_t pkt[INSTANT_MOUSE_REPORT_SIZE];

    pkt[0] = INSTANT_MOUSE_REPORT_ID;
    pkt[1] = INSTANT_MOUSE_SET_MODE;
    pkt[2] = 0xFF;

    pkt[3] = (mode_value << 4) | (speed & 0xF);
    pkt[4] = (direction << 4) | led_mask;
    pkt[5] = 0xF - (brightness & 0xF);

    pkt[6] = mode_value & 0xF0;
    pkt[7] = 0x00;

    hid_send_feature_report(dev, pkt, INSTANT_MOUSE_REPORT_SIZE);
}
