/*---------------------------------------------------------*\
| DarkProjectKeyboardController.h                           |
|                                                           |
|   Driver for Dark Project keyboard                        |
|                                                           |
|   Chris M (DrNo)                              08 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define NA                                      0xFFFFFFFF
#define HID_MAX_STR                             255

#define DARKPROJECTKEYBOARD_PACKET_SIZE         256
#define DARKPROKECTKEYBOARD_TKL_KEYCOUNT        87

enum
{
    DARKPROJECTKEYBOARD_MODE_DIRECT     = 0x01,   //Direct Led Control - Independently set LEDs in zone
};

enum
{
    DARKPROJECTKEYBOARD_REPORT_BYTE     = 1,
    DARKPROJECTKEYBOARD_COMMAND_BYTE    = 2,
    DARKPROJECTKEYBOARD_RED_BLUE_BYTE   = 5,
    DARKPROJECTKEYBOARD_GREEN_BYTE      = 107
};

class DarkProjectKeyboardController
{
public:
    DarkProjectKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~DarkProjectKeyboardController();

    std::string     GetLocation();
    std::string     GetName();
    std::string     GetSerial();

    void            SetLedsDirect(std::vector<RGBColor> colors);
private:
    hid_device*     dev;
    std::string     location;
    std::string     name;
};
