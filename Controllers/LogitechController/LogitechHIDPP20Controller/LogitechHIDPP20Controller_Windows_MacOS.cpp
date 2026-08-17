/*---------------------------------------------------------*\
| LogitechHIDPP20Controller_Windows_MacOS.cpp               |
|                                                           |
|   Path-migration and device-name lookup for the unified   |
|   Logitech HID++ 2.0 controller on Windows and macOS.     |
|                                                           |
|   Uses hidapi's hid_enumerate + hid_device_info fields    |
|   (serial_number, product_string, product_id, usage_page) |
|   on platforms with no /sys/class/hidraw equivalent.      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <chrono>
#include <cstring>
#include <string>
#include <vector>
#include "LogitechHIDPP20Controller.h"
#include "LogManager.h"
#include "StringUtils.h"

#define LOG_TAG log_tag.c_str()

std::string LogitechHIDPP20Controller::GetCenturionSubDeviceName(const std::string& path)
{
    /*-----------------------------------------------------*\
    | On Windows, hidapi's hid_device_info                  |
    | carries a product_string field (wchar_t*).            |
    | Enumerate all Logitech devices and find               |
    | the one whose path matches `path`.                    |
    |                                                       |
    | Caveat: Windows hidapi typically returns the          |
    | parent product string on every (interface,            |
    | usage_page, usage) entry that maps to the same        |
    | USB device, so Centurion sub-devices may share a      |
    | name with the parent dongle. That's a less            |
    | specific name than Linux's HID_NAME, but still        |
    | better than "Logitech Centurion Device".              |
    \*-----------------------------------------------------*/
    std::string friendly;

    hid_device_info* devs = hid_enumerate(0x046D, 0x0000);

    for(hid_device_info* d = devs; d != nullptr; d = d->next)
    {
        if(d->path == nullptr)
        {
            continue;
        }

        if(std::string(d->path) != path)
        {
            continue;
        }

        friendly = StringUtils::wchar_to_string(d->product_string);
        break;
    }

    hid_free_enumeration(devs);
    return friendly;
}
