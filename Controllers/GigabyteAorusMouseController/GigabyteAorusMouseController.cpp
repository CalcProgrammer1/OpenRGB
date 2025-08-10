/*---------------------------------------------------------*\
| GigabyteAorusMouseController.cpp                          |
|                                                           |
|   Driver for Gigabyte Aorus mouse                         |
|                                                           |
|   Morgan Guimard (morg)                       29 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "GigabyteAorusMouseController.h"
#include "StringUtils.h"

GigabyteAorusMouseController::GigabyteAorusMouseController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
    version             = "";
}

GigabyteAorusMouseController::~GigabyteAorusMouseController()
{
    hid_close(dev);
}

std::string GigabyteAorusMouseController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string GigabyteAorusMouseController::GetFirmwareVersion()
{
    return(version);
}

std::string GigabyteAorusMouseController::GetNameString()
{
    return(name);
}

std::string GigabyteAorusMouseController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void GigabyteAorusMouseController::SetMode(RGBColor color, uint8_t mode_value, uint8_t brightness, uint8_t speed)
{
    uint8_t usb_buf[GIGABYTE_AORUS_MOUSE_REPORT_SIZE];

    usb_buf[0] = GIGABYTE_AORUS_MOUSE_HARDWARE_CMD;
    usb_buf[1] = mode_value;
    usb_buf[2] = brightness;
    usb_buf[3] = RGBGetRValue(color);
    usb_buf[4] = RGBGetGValue(color);
    usb_buf[5] = RGBGetBValue(color);
    usb_buf[6] = speed;
    usb_buf[7] = 0x00;

    hid_send_feature_report(dev, usb_buf, GIGABYTE_AORUS_MOUSE_REPORT_SIZE);
}

void GigabyteAorusMouseController::SendDirect(RGBColor color)
{
    uint8_t usb_buf[8];

    memset(usb_buf, 0x00, GIGABYTE_AORUS_MOUSE_REPORT_SIZE);

    usb_buf[0] = GIGABYTE_AORUS_MOUSE_DIRECT_CMD;
    usb_buf[2] = RGBGetRValue(color);
    usb_buf[3] = RGBGetGValue(color);
    usb_buf[4] = RGBGetBValue(color);

    hid_send_feature_report(dev, usb_buf, GIGABYTE_AORUS_MOUSE_REPORT_SIZE);
}
