/*---------------------------------------------------------*\
| HYTEKeyboardController.h                                  |
|                                                           |
|   Driver for HYTE keyboard                                |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      30 Oct 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

enum
{
    HYTE_KEYBOARD_ZONE_KEYBOARD,
    HYTE_KEYBOARD_ZONE_SURROUND
};

class HYTEKeyboardController
{
public:
    HYTEKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~HYTEKeyboardController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();

    void LEDStreaming(unsigned int zone, RGBColor* colors);

private:
    hid_device* dev;
    std::string location;
    std::string name;

    void LEDStreaming_Keyboard(RGBColor* colors);
    void LEDStreaming_Surround(RGBColor* colors);
};
