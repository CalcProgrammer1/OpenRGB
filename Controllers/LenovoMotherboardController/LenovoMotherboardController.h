/*---------------------------------------------------------*\
| LenovoMotherboardController.h                             |
|                                                           |
|   Driver for Lenovo motherboard                           |
|                                                           |
|   Morgan Guimard (morg)                       26 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define LENOVO_MB_PACKET_LENGTH     64
#define LENOVO_MB_REPORT_ID         0xCC
#define LENOVO_MB_NUMBER_OF_LEDS    2
#define LENOVO_MB_ZONE_1_VALUE      0x12
#define LENOVO_MB_ZONE_2_VALUE      0x11

enum
{
    LENOVO_MB_STATIC_MODE       = 0x01,
    LENOVO_MB_SPARKLE_MODE      = 0x02,
    LENOVO_MB_BREATHING_MODE    = 0x03,
    LENOVO_MB_WAVE_MODE         = 0x04,
    LENOVO_MB_SPECTER_MODE      = 0x06,
    LENOVO_MB_RAINBOW_WAVE_MODE = 0x09,
    LENOVO_MB_RANDOM_MODE       = 0x0A
};

enum
{
    LENOVO_MB_BRIGHTNESS_MIN    = 1,
    LENOVO_MB_BRIGHTNESS_MAX    = 4,
    LENOVO_MB_SPEED_MIN         = 1,
    LENOVO_MB_SPEED_MAX         = 4,
};

class LenovoMotherboardController
{
public:
    LenovoMotherboardController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~LenovoMotherboardController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void SetMode(uint8_t zone, uint8_t mode, uint8_t brightness, uint8_t speed, RGBColor color);

private:
    hid_device* dev;
    std::string location;
    std::string name;
};
