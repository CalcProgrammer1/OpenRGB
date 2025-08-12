/*---------------------------------------------------------*\
| LenovoMotherboardController.cpp                           |
|                                                           |
|   Driver for Lenovo motherboard                           |
|                                                           |
|   Morgan Guimard (morg)                       26 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "LenovoMotherboardController.h"
#include "StringUtils.h"

LenovoMotherboardController::LenovoMotherboardController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
}

LenovoMotherboardController::~LenovoMotherboardController()
{
    hid_close(dev);
}

std::string LenovoMotherboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LenovoMotherboardController::GetNameString()
{
    return(name);
}

std::string LenovoMotherboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void LenovoMotherboardController::SetMode(uint8_t zone, uint8_t mode, uint8_t brightness, uint8_t speed, RGBColor color)
{
    uint8_t r = RGBGetRValue(color);
    uint8_t g = RGBGetGValue(color);
    uint8_t b = RGBGetBValue(color);

    uint8_t buffer[LENOVO_MB_PACKET_LENGTH];
    memset(buffer, 0x00, LENOVO_MB_PACKET_LENGTH);

    buffer[0]   = LENOVO_MB_REPORT_ID;

    buffer[1]   = zone;
    buffer[2]   = mode;
    buffer[3]   = speed;
    buffer[4]   = brightness;

    buffer[5]   = r;
    buffer[6]   = g;
    buffer[7]   = b;

    hid_send_feature_report(dev, buffer, LENOVO_MB_PACKET_LENGTH);

    memset(buffer, 0x00, LENOVO_MB_PACKET_LENGTH);

    buffer[0]   = LENOVO_MB_REPORT_ID;
    buffer[1]   = 0x28;
    buffer[2]   = 0x06;

    buffer[33]  = zone;
    buffer[34]  = mode;
    buffer[35]  = speed;
    buffer[36]  = brightness;

    buffer[37]  = r;
    buffer[38]  = g;
    buffer[39]  = b;

    hid_send_feature_report(dev, buffer, LENOVO_MB_PACKET_LENGTH);
}
