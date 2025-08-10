/*---------------------------------------------------------*\
| GigabyteAorusPCCaseController.cpp                         |
|                                                           |
|   Driver for Gigabyte Aorus case                          |
|                                                           |
|   Denis Nazarov (nenderus)                    10 Feb 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "GigabyteAorusPCCaseController.h"
#include "StringUtils.h"

GigabyteAorusPCCaseController::GigabyteAorusPCCaseController(hid_device *dev_handle, const char *path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

GigabyteAorusPCCaseController::~GigabyteAorusPCCaseController()
{
    hid_close(dev);
}

std::string GigabyteAorusPCCaseController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string GigabyteAorusPCCaseController::GetNameString()
{
    return(name);
}

std::string GigabyteAorusPCCaseController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void GigabyteAorusPCCaseController::SendColor(uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t usb_buf[9] = { 0x00, 0x01, 0xC8, red, green, blue, 0x08, 0x01, 0x00 };
    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void GigabyteAorusPCCaseController::SendMode(uint8_t mode, uint8_t speed, uint8_t brightness)
{
    uint8_t usb_buf[9] = { 0x00, 0x01, 0xC9, mode, brightness, speed, 0x01, 0x08, 0x00 };
    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void GigabyteAorusPCCaseController::SendOk()
{
    uint8_t usb_buf[9] = { 0x00, 0x01, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void GigabyteAorusPCCaseController::SetMode(uint8_t mode, aorus_pc_case_mode_config zone_config)
{
    switch (mode)
    {
        case AORUS_PC_CASE_MODE_CUSTOM:
        {
            SendColor(RGBGetRValue(zone_config.color), RGBGetGValue(zone_config.color), RGBGetBValue(zone_config.color));
            SendMode(mode, AORUS_PC_CASE_SPEED_NORMAL, zone_config.brightness);
        }
            break;

        case AORUS_PC_CASE_MODE_OFF:
        {
            SendColor(0x00, 0x00, 0x00);
            SendMode(AORUS_PC_CASE_MODE_CUSTOM, AORUS_PC_CASE_SPEED_SLOWEST, AORUS_PC_CASE_BRIGHTNESS_MAX + 0x01);
        }
            break;

        case AORUS_PC_CASE_MODE_BREATHING:
        {
            SendColor(RGBGetRValue(zone_config.color), RGBGetGValue(zone_config.color), RGBGetBValue(zone_config.color));
            SendMode(mode, zone_config.speed, AORUS_PC_CASE_BRIGHTNESS_MAX);
        }
            break;

        case AORUS_PC_CASE_MODE_SPECTRUM_CYCLE:
        {
            SendColor(0xFF, 0x00, 0x00);
            SendMode(mode, zone_config.speed, AORUS_PC_CASE_BRIGHTNESS_MAX);
        }
            break;

        case AORUS_PC_CASE_MODE_FLASHING:
        case AORUS_PC_CASE_MODE_DOUBLE_FLASHING:
        {
            SendColor(RGBGetRValue(zone_config.color), RGBGetGValue(zone_config.color), RGBGetBValue(zone_config.color));
            SendMode(mode, zone_config.speed, zone_config.brightness * 0x0A);
        }
            break;
    }

    SendOk();
}
