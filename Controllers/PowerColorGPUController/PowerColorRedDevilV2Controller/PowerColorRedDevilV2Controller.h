/*---------------------------------------------------------*\
| PowerColorRedDevilV2Controller.h                          |
|                                                           |
|   Driver for PowerColor Red Devil GPU                     |
|                                                           |
|   Nexrem                                      15 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

typedef unsigned char   red_devil_v2_dev_id;

struct red_devil_v2_mode
{
    unsigned char mode;
    unsigned char brightness;
    unsigned char speed;
};

#define RED_DEVIL_V2_NUM_LEDS              24

enum
{
    RED_DEVIL_V2_WRITE_REG_MODE          = 0x01,
    RED_DEVIL_V2_WRITE_REG_SYNC          = 0x04,
    RED_DEVIL_V2_WRITE_REG_RGBX          = 0x10,
    RED_DEVIL_V2_WRITE_REG_RGB1          = 0x30,
    RED_DEVIL_V2_WRITE_REG_RGB2          = 0x31
};

enum
{
    RED_DEVIL_V2_READ_REG_MODE           = 0x81,
    RED_DEVIL_V2_READ_REG_MAGIC          = 0x82,
    RED_DEVIL_V2_READ_REG_SYNC           = 0x84,
    RED_DEVIL_V2_READ_REG_RGBX           = 0x90
};

enum
{
    RED_DEVIL_V2_MODE_OFF                = 0x00,
    RED_DEVIL_V2_MODE_STATIC             = 0x01,
    RED_DEVIL_V2_MODE_BREATHING          = 0x02,
    RED_DEVIL_V2_MODE_SECRET_RAINBOW     = 0x03,
    RED_DEVIL_V2_MODE_RADIANCE           = 0x04,
    RED_DEVIL_V2_MODE_DIFFUSE            = 0x05,
    RED_DEVIL_V2_MODE_COLOR_SHIFT        = 0x06,
    RED_DEVIL_V2_MODE_METEOR             = 0x07,
    RED_DEVIL_V2_MODE_RIPPLE             = 0x08,
    RED_DEVIL_V2_MODE_RAINBOW            = 0x09,
    RED_DEVIL_V2_MODE_SYNC               = 0xFF
};

enum
{
    RED_DEVIL_V2_BRIGHTNESS_MIN          = 0x00,
    RED_DEVIL_V2_BRIGHTNESS_MAX          = 0xFF
};

enum
{
    RED_DEVIL_V2_SPEED_MIN               = 0xFF,
    RED_DEVIL_V2_SPEED_DEFAULT           = 0x32,
    RED_DEVIL_V2_SPEED_MAX               = 0x00
};


class PowerColorRedDevilV2Controller
{
public:
    PowerColorRedDevilV2Controller(i2c_smbus_interface* bus, red_devil_v2_dev_id dev, std::string dev_name);
    ~PowerColorRedDevilV2Controller();

    std::string                     GetDeviceLocation();
    std::string                     GetDeviceName();

    bool                            GetSync();
    void                            SetSync(bool sync);

    red_devil_v2_mode               GetMode();
    void                            SetMode(red_devil_v2_mode mode);

    RGBColor                        GetLedColor(int led);
    void                            SetLedColor(int led, RGBColor color);
    void                            SetLedColorAll(RGBColor color);

private:
    i2c_smbus_interface*            bus;
    red_devil_v2_dev_id             dev;
    std::string                     name;

    int                             RegisterRead(unsigned char reg, unsigned char *data);
    int                             RegisterWrite(unsigned char reg, unsigned char *data);
};
