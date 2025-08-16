/*---------------------------------------------------------*\
| SinowealthController1007.h                                |
|                                                           |
|   Driver for Sinowealth mice with PID 1007                |
|                                                           |
|   Moon_darker (Vaker)                         25 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include <hidapi.h>
#include "RGBController.h"

#define ZET_FURY_PRO_STATE_BUFFER_LENGTH 59
#define ZET_FURY_PRO_COLOR_BUFFER_LENGTH 21

#define ZET_FURY_PRO_BRIGHTNESS_MIN 1
#define ZET_FURY_PRO_BRIGHTNESS_MAX 9
#define ZET_FURY_PRO_BRIGHTNESS_DEF 9

#define ZET_FURY_PRO_SPEED_MIN 1
#define ZET_FURY_PRO_SPEED_MAX 3
#define ZET_FURY_PRO_SPEED_DEF 2

enum
{
    ZET_FURY_PRO_MODE_OFF           = 0x00,
    ZET_FURY_PRO_MODE_RAINBOW       = 0x10,
    ZET_FURY_PRO_MODE_STATIC        = 0x20,
    ZET_FURY_PRO_MODE_BREATHING     = 0x30,
    ZET_FURY_PRO_MODE_PENDULUM      = 0x40,
    ZET_FURY_PRO_MODE_SPECTRUM      = 0x50,
    ZET_FURY_PRO_MODE_CUSTOM        = 0x60,
    ZET_FURY_PRO_MODE_TWO_COLORS    = 0x70,
    ZET_FURY_PRO_MODE_REACTIVE      = 0x80,
    ZET_FURY_PRO_MODE_FLICKER       = 0x90,
    ZET_FURY_PRO_MODE_RAIN          = 0xA0,
    ZET_FURY_PRO_MODE_SNAKE         = 0xB0,
};

enum
{
    ZET_FURY_PRO_SUBMODE_SET_COLOR  = 0x07,
    ZET_FURY_PRO_SUBMODE_RANDOM     = 0x80,
};

enum
{
    ZET_FURY_PRO_DIR_LEFT           = 0x80,
    ZET_FURY_PRO_DIR_RIGHT          = 0x00,
};

class SinowealthController1007
{
public:
    SinowealthController1007(hid_device* dev, char *_path, std::string dev_name);
    ~SinowealthController1007();

    unsigned int    GetLEDCount();
    std::string     GetLocation();
    std::string     GetName();
    std::string     GetSerialString();

    void            SetLEDColors(const std::vector<RGBColor>& colors);
    void            SetMode(unsigned char mode, unsigned char spd_or_lum, unsigned char direction, const std::vector<RGBColor>& colors, unsigned char random, bool has_per_led_colors);
    int             GetProfile();
    void            SendPacket();
private:
    hid_device*     dev;

    unsigned int    led_count;

    unsigned char   current_mode;
    unsigned char   current_direction;
    unsigned char   device_configuration[ZET_FURY_PRO_STATE_BUFFER_LENGTH];
    unsigned char   device_colors[ZET_FURY_PRO_COLOR_BUFFER_LENGTH];

    std::string     location;
    std::string     name;
};
