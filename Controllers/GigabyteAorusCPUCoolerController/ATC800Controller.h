/*---------------------------------------------------------*\
| ATC800Controller.h                                        |
|                                                           |
|   Driver for Aorus ATC800 cooler                          |
|                                                           |
|   Felipe Cavalcanti                           13 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

struct aorus_atc800_mode_config
{
    RGBColor    colors[8];
    uint8_t     numberOfColors;
    uint8_t     speed;
    uint8_t     brightness;
};

enum
{
    AORUS_ATC800_MODE_OFF               = 0x00,
    AORUS_ATC800_MODE_CUSTOM            = 0x01,
    AORUS_ATC800_MODE_BREATHING         = 0x02,
    AORUS_ATC800_MODE_SPECTRUM_CYCLE    = 0x03,
    AORUS_ATC800_MODE_FLASHING          = 0x04,
    AORUS_ATC800_MODE_DOUBLE_FLASHING   = 0x05,
    AORUS_ATC800_MODE_GRADIENT          = 0x06,
    AORUS_ATC800_MODE_COLOR_SHIFT       = 0x07,
    AORUS_ATC800_MODE_RAINBOW_WAVE      = 0x08,
    AORUS_ATC800_MODE_RADIATE           = 0x09,
    AORUS_ATC800_MODE_RAINBOW_LOOP      = 0x0A,
    AORUS_ATC800_MODE_TRICOLOR          = 0x0B,
};

enum
{
    AORUS_ATC800_SPEED_SLOWEST          = 0x00, /* Slowest speed                */
    AORUS_ATC800_SPEED_NORMAL           = 0x02, /* Normal speed                 */
    AORUS_ATC800_SPEED_FASTEST          = 0x05, /* Fastest speed                */
};

enum
{
    AORUS_ATC800_BRIGHTNESS_MIN         = 0x00,
    AORUS_ATC800_BRIGHTNESS_MAX         = 0x05
};

enum
{
    AORUS_ATC800_FANS_ZONE              = 0,
    AORUS_ATC800_TOP_ZONE               = 1
};

class ATC800Controller
{
public:
    ATC800Controller(hid_device* dev_handle, const char* path, std::string name);
    ~ATC800Controller();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void        DisableTempRPMIndicator();
    void        SendMode(uint8_t mode, uint8_t brightness, uint8_t speed, uint8_t mystery_flag, uint8_t zone);
    void        SendOneColor(uint8_t color_flag, uint8_t red, uint8_t green, uint8_t blue);
    void        SendMultiColor(uint8_t flag, uint8_t mode, uint8_t red1, uint8_t green1, uint8_t blue1, uint8_t red2, uint8_t green2, uint8_t blue2);
    void        SendOk();
    void        SendCoolerMode(uint8_t zone, uint8_t mode, aorus_atc800_mode_config zone_config);

private:
    hid_device* dev;
    std::string location;
    std::string name;
};
