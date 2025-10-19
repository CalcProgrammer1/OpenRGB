/*---------------------------------------------------------*\
| AsusMonitorController.cpp                                 |
|                                                           |
|   Driver for Asus monitors                                |
|                                                           |
|   Morgan Guimard (morg)                       19 oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string.h>
#include "AsusMonitorController.h"
#include "StringUtils.h"

AsusMonitorController::AsusMonitorController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev         = dev_handle;
    location    = info.path;
    name        = dev_name;
}

AsusMonitorController::~AsusMonitorController()
{
    hid_close(dev);
}

std::string AsusMonitorController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AsusMonitorController::GetNameString()
{
    return(name);
}

std::string AsusMonitorController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

unsigned int AsusMonitorController::GetNumberOfLEDs()
{
    uint8_t usb_buf[ASUS_MONITOR_REPORT_SIZE];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0xEC;
    usb_buf[0x01] = 0xB0;

    hid_write(dev, usb_buf, sizeof(usb_buf));

    memset(usb_buf, 0x00, sizeof(usb_buf));

    int bytes = hid_read(dev, usb_buf, sizeof(usb_buf));

    return bytes > 0 ? usb_buf[32] : 0;
}

void AsusMonitorController::SendInit()
{
    uint8_t usb_buf[ASUS_MONITOR_REPORT_SIZE];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0xEC;
    usb_buf[0x01] = 0x35;
    usb_buf[0x05] = 0xFF;
    usb_buf[0x08] = 0x01;

    hid_write(dev, usb_buf, sizeof(usb_buf));
}

void AsusMonitorController::SetDirect(std::vector<RGBColor> colors)
{
    uint8_t usb_buf[ASUS_MONITOR_REPORT_SIZE];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0xEC;
    usb_buf[0x01] = 0x40;
    usb_buf[0x02] = 0x84;
    usb_buf[0x04] = colors.size();

    for(size_t i = 0; i < colors.size(); i++)
    {
        usb_buf[0x05 + (3 * i)]     = RGBGetRValue(colors[i]);
        usb_buf[0x05 + (3 * i + 1)] = RGBGetGValue(colors[i]);
        usb_buf[0x05 + (3 * i + 2)] = RGBGetBValue(colors[i]);
    }

    hid_write(dev, usb_buf, sizeof(usb_buf));
}
