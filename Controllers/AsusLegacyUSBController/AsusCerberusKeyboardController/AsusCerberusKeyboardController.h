/*---------------------------------------------------------*\
| AsusCerberusKeyboardController.h                          |
|                                                           |
|   Driver for ASUS Cerberus keyboard                       |
|                                                           |
|   Mola19                                      03 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

enum
{
    CERBERUS_KEYBOARD_MODE_STATIC       = 0,
    CERBERUS_KEYBOARD_MODE_BREATHING    = 1,
    CERBERUS_KEYBOARD_MODE_REACTIVE     = 2,
    CERBERUS_KEYBOARD_MODE_EXPLOSION    = 3,
    CERBERUS_KEYBOARD_MODE_COLOR_CYCLE  = 4,
    CERBERUS_KEYBOARD_MODE_WAVE         = 6,
    CERBERUS_KEYBOARD_MODE_CUSTOM       = 7,
};

class AsusCerberusKeyboardController
{
public:
    AsusCerberusKeyboardController(hid_device* dev_handle, const char* path, unsigned short rev_version, std::string dev_name);
    ~AsusCerberusKeyboardController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();
    std::string GetSerialString();
    std::string GetVersion();

    void SetProfile(uint8_t profile);
    void SetPerLEDColor(uint8_t key, uint8_t red, uint8_t green, uint8_t blue);
    void SendPerLEDColorEnd();
    void SetPerLEDMode(uint8_t mode);
    void SetMode(uint8_t mode, uint8_t red, uint8_t green, uint8_t blue, uint8_t direction, uint8_t brightness);


private:
    hid_device*                 dev;
    std::string                 location;
    std::string                 name;
    unsigned short              version;
};
