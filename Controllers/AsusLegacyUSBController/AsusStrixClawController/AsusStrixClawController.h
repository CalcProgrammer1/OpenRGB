/*---------------------------------------------------------*\
| AsusStrixClawController.h                                 |
|                                                           |
|   Driver for ASUS Strix Claw mouse                        |
|                                                           |
|   Mola19                                      06 Aug 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define HID_MAX_STR 255

class StrixClawController
{
public:
    StrixClawController(hid_device* dev_handle, const char* path, std::string dev_name);
    virtual ~StrixClawController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();
    std::string GetSerialString();
    std::string GetVersion();

    void SetScrollWheelLED(bool OnOff);
    void SetLogoLED(uint8_t brightness);

private:
    hid_device*                 dev;
    std::string                 location;
    std::string                 name;
};
