/*---------------------------------------------------------*\
| NvidiaESAController.cpp                                   |
|                                                           |
|   Driver for NVIDIA ESA                                   |
|                                                           |
|   Morgan Guimard (morg)                       18 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "NvidiaESAController.h"
#include "StringUtils.h"

NvidiaESAController::NvidiaESAController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev         = dev_handle;
    location    = info.path;
    name        = dev_name;
}

NvidiaESAController::~NvidiaESAController()
{
    hid_close(dev);
}

std::string NvidiaESAController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string NvidiaESAController::GetNameString()
{
    return(name);
}

std::string NvidiaESAController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void NvidiaESAController::SetZoneColor(unsigned int zone_idx, RGBColor color)
{
    unsigned char red = (unsigned char)(0x0F - 0x0F * RGBGetRValue(color) / 255.0f);
    unsigned char grn = (unsigned char)(0x0F - 0x0F * RGBGetGValue(color) / 255.0f);
    unsigned char blu = (unsigned char)(0x0F - 0x0F * RGBGetBValue(color) / 255.0f);

    unsigned char usb_buf[4];

    usb_buf[0x00] = 0x42 + zone_idx;

    usb_buf[0x01] = red;
    usb_buf[0x02] = grn;
    usb_buf[0x03] = blu;

    hid_write(dev, usb_buf, 4);
}
