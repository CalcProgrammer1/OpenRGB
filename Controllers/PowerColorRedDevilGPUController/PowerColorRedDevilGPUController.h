/*---------------------------------------------------------*\
| PowerColorRedDevilGPUController.h                         |
|                                                           |
|   Driver for PowerColor Red Devil GPU                     |
|                                                           |
|   Jana Rettig (SapphicKitten)                 14 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

#pragma once

#define RED_DEVIL_GPU_READ_OFFSET 0x80
#define RED_DEVIL_GPU_LED_MAX_COUNT 12

typedef unsigned char red_devil_dev_id;

struct red_devil_mode_config
{
    unsigned char mode;
    unsigned char brightness;
    unsigned char speed;
};

enum
{
    RED_DEVIL_GPU_REG_MODE            = 0x01,
    RED_DEVIL_GPU_REG_LED_1           = 0x02,
    RED_DEVIL_GPU_REG_LED_2           = 0x03,
    RED_DEVIL_GPU_REG_LED_3           = 0x04,
    RED_DEVIL_GPU_REG_LED_4           = 0x05,
    RED_DEVIL_GPU_REG_LED_5           = 0x06,
    RED_DEVIL_GPU_REG_LED_6           = 0x07,
    RED_DEVIL_GPU_REG_LED_7           = 0x08,
    RED_DEVIL_GPU_REG_LED_8           = 0x09,
    RED_DEVIL_GPU_REG_LED_9           = 0x0A,
    RED_DEVIL_GPU_REG_LED_10          = 0x0B,
    RED_DEVIL_GPU_REG_LED_11          = 0x0C,
    RED_DEVIL_GPU_REG_LED_12          = 0x0D,     // Unused for now, acts like any other led reg
    RED_DEVIL_GPU_REG_LED_ALL         = 0x0E,
    RED_DEVIL_GPU_REG_MODE_COLOR      = 0x0F,
    RED_DEVIL_GPU_REG_UNKNOWN_1       = 0x10,     // Never seen writes to this, reads 0x01 0x05 0x00. Maybe Version?
    RED_DEVIL_GPU_REG_UNKNOWN_2       = 0x11,     // DevilZone writes to this sometimes. No observable change
    RED_DEVIL_GPU_REG_MB_SYNC         = 0x12      // Unused on NAVI 1X cards. Disables controller and allows LEDs to be controlled by external source via ARGB header
};

enum
{
    RED_DEVIL_GPU_MODE_OFF            = 0x00,
    RED_DEVIL_GPU_MODE_STATIC         = 0x01,
    RED_DEVIL_GPU_MODE_BREATHING      = 0x02,
    RED_DEVIL_GPU_MODE_NEON           = 0x03,
    RED_DEVIL_GPU_MODE_BLINK          = 0x04,
    RED_DEVIL_GPU_MODE_DOUBLE_BLINK   = 0x05,
    RED_DEVIL_GPU_MODE_COLOR_SHIFT    = 0x06,
    RED_DEVIL_GPU_MODE_METEOR         = 0x07,
    RED_DEVIL_GPU_MODE_RIPPLE         = 0x08,
    RED_DEVIL_GPU_MODE_SEVEN_COLORS   = 0x09,
    RED_DEVIL_GPU_MODE_MB_SYNC        = 0xFF
};

enum
{
    RED_DEVIL_GPU_BRIGHTNESS_MIN      = 0x00,
    RED_DEVIL_GPU_BRIGHTNESS_MAX      = 0xFF,
};

enum
{
    RED_DEVIL_GPU_SPEED_SLOWEST       = 0x64,
    RED_DEVIL_GPU_SPEED_DEFAULT       = 0x32,
    RED_DEVIL_GPU_SPEED_FASTEST       = 0x00
};

class RedDevilGPUController
{
public:
    RedDevilGPUController(i2c_smbus_interface* bus, red_devil_dev_id dev, std::string dev_name);
    ~RedDevilGPUController();

    std::string             GetDeviceLocation();
    std::string             GetDeviceName();

    void                    SetLEDColor(int led, RGBColor color);
    RGBColor                GetLEDColor(int led);

    void                    SetLEDColorAll(RGBColor color);

    void                    SetModeColor(RGBColor color);
    RGBColor                GetModeColor();

    void                    SetMode(red_devil_mode_config config);
    red_devil_mode_config   GetMode();

    int                     RegisterRead(unsigned char reg, unsigned char *data);
    int                     RegisterWrite(unsigned char reg, unsigned char *data);

    bool                    has_sync_mode = false;

private:
    i2c_smbus_interface*    bus;
    red_devil_dev_id        dev;
    std::string             name;
};
