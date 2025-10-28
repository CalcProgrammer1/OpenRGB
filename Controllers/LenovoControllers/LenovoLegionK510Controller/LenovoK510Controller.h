/*---------------------------------------------------------*\
| LenovoK510Controller.h                                    |
|                                                           |
|   Driver for Lenovo Legion K510 keyboard                  |
|                                                           |
|   Bnyro                                       27 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define K510_DATA_SIZE  64

#define K510_BRIGHTNESS_DEFAULT 2
#define K510_BRIGHTNESS_MIN 0
#define K510_BRIGHTNESS_MAX 2

// the lower the speed value, the faster the animation
#define K510_SPEED_DEFAULT 2
#define K510_SPEED_MIN 4
#define K510_SPEED_MAX 0

enum
{
    K510_MODE_CORRUGATED  = 0x01,
    K510_MODE_CLOUD       = 0x02,
    K510_MODE_SERPENTINE  = 0x03,
    K510_MODE_SPECTRUM    = 0x04,
    K510_MODE_BREATH      = 0x05,
    K510_MODE_NORMAL      = 0x06,
    K510_MODE_REACTION    = 0x07,
    K510_MODE_RIPPLES     = 0x08,
    K510_MODE_TRAVERSE    = 0x09,
    K510_MODE_STARS       = 0x0A,
    K510_MODE_FLOWERS     = 0x0B,
    K510_MODE_ROLL        = 0x0C,
    K510_MODE_WAVE        = 0x0D,
    K510_MODE_CARTOON     = 0x0E,
    K510_MODE_RAIN        = 0x0F,
    K510_MODE_SCAN        = 0x10,
    K510_MODE_SURMOUNT    = 0x11,
    K510_MODE_SPEED       = 0x12,
};

class LenovoK510Controller
{
public:
    LenovoK510Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~LenovoK510Controller();

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();

    void SetMode(unsigned int color_mode, RGBColor color, unsigned char mode_value, unsigned int brightness, unsigned int speed, unsigned int direction);
    mode GetCurrentState();
;

private:
    hid_device*     device;
    std::string     location;
    std::string     name;
};
