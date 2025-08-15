/*---------------------------------------------------------*\
| SapphireNitroGlowV3Controller.h                           |
|                                                           |
|   Driver for Sapphire Nitro Glow V3                       |
|                                                           |
|   K900                                        03 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char	sapphire_dev_id;

enum
{
    SAPPHIRE_NITRO_GLOW_V3_REG_MODE                             = 0x10,
    SAPPHIRE_NITRO_GLOW_V3_REG_EXTERNAL_CONTROL                 = 0x0F,
    SAPPHIRE_NITRO_GLOW_V3_REG_BRIGHTNESS                       = 0x3E,
    SAPPHIRE_NITRO_GLOW_V3_REG_RUNWAY_ANIMATION_SPEED           = 0x11,
    SAPPHIRE_NITRO_GLOW_V3_REG_RUNWAY_ANIMATION_REPEAT_COUNT    = 0x12,
    SAPPHIRE_NITRO_GLOW_V3_REG_COLOR_CYCLE_ANIMATION_SPEED      = 0x13,
    SAPPHIRE_NITRO_GLOW_V3_REG_RAINBOW_ANIMATION_SPEED          = 0x15,
    SAPPHIRE_NITRO_GLOW_V3_REG_SERIAL_ANIMATION_SPEED           = 0x16,
    SAPPHIRE_NITRO_GLOW_V3_REG_MUSIC_VOLUME                     = 0x29,
    SAPPHIRE_NITRO_GLOW_V3_REG_RED                              = 0x1A,
    SAPPHIRE_NITRO_GLOW_V3_REG_GREEN                            = 0x1B,
    SAPPHIRE_NITRO_GLOW_V3_REG_BLUE                             = 0x1C,
};

enum
{
    SAPPHIRE_NITRO_GLOW_V3_MODE_RAINBOW             = 0x00,
    SAPPHIRE_NITRO_GLOW_V3_MODE_RUNWAY              = 0x01,
    SAPPHIRE_NITRO_GLOW_V3_MODE_COLOR_CYCLE         = 0x02,
    SAPPHIRE_NITRO_GLOW_V3_MODE_SERIAL              = 0x03,
    SAPPHIRE_NITRO_GLOW_V3_MODE_SAPPHIRE_BLUE       = 0x04,
    SAPPHIRE_NITRO_GLOW_V3_MODE_AUDIO_VISUALIZATION = 0x05,
    SAPPHIRE_NITRO_GLOW_V3_MODE_CUSTOM              = 0x06,
    SAPPHIRE_NITRO_GLOW_V3_MODE_OFF                 = 0x07,
    SAPPHIRE_NITRO_GLOW_V3_MODE_EXTERNAL_CONTROL    = 0xFF,
};

class SapphireNitroGlowV3Controller
{
public:
    SapphireNitroGlowV3Controller(i2c_smbus_interface* bus, sapphire_dev_id dev, std::string dev_name);
    ~SapphireNitroGlowV3Controller();

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();

    unsigned char   GetRed();
    unsigned char   GetGreen();
    unsigned char   GetBlue();

    void            SetColor(unsigned char red, unsigned char green, unsigned char blue);

    unsigned char   GetMode();
    void            SetMode(unsigned char mode);

    bool            GetExternalControl();
    void            SetExternalControl(bool enabled);

    unsigned char   GetBrightness();
    void            SetBrightness(unsigned char brightness);

    unsigned char   GetRainbowAnimationSpeed();
    void            SetRainbowAnimationSpeed(unsigned char speed);

    unsigned char   GetRunwayAnimationSpeed();
    void            SetRunwayAnimationSpeed(unsigned char speed);

    unsigned char   GetRunwayAnimationRepeatCount();
    void            SetRunwayAnimationRepeatCount(unsigned char count);

    unsigned char   GetColorCycleAnimationSpeed();
    void            SetColorCycleAnimationSpeed(unsigned char speed);

    unsigned char   GetSerialAnimationSpeed();
    void            SetSerialAnimationSpeed(unsigned char speed);

private:
    i2c_smbus_interface*    bus;
    sapphire_dev_id         dev;
    std::string             name;
};
