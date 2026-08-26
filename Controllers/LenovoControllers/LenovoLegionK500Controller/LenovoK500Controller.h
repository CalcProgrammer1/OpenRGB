/*---------------------------------------------------------*\
| LenovoK500Controller.h                                    |
|                                                           |
|   Driver for Lenovo Legion K500 keyboard                  |
|                                                           |
|   MX1D                                        10 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define K500_DATA_SIZE  64

#define K500_BRIGHTNESS_DEFAULT 2
#define K500_BRIGHTNESS_MIN 0
#define K500_BRIGHTNESS_MAX 2

#define K500_SPEED_DEFAULT 3
#define K500_SPEED_MIN 0
#define K500_SPEED_MAX 3

enum
{
    K500_MODE_STATIC            = 0x01,
    K500_MODE_SPIRAL            = 0x02,
    K500_MODE_LASER             = 0x03,
    K500_MODE_DRIFTING          = 0x04,
    K500_MODE_FADING_TRACE      = 0x05,
    K500_MODE_CUSTOMIZE         = 0x06,
    K500_MODE_BREATHING         = 0x07,
    K500_MODE_BREATHING_RAINBOW = 0x08,
    K500_MODE_STARLIGHT         = 0x09,
    K500_MODE_STARLIGHT_DUAL    = 0x0A,
    K500_MODE_SURFING           = 0x0B,
};

class LenovoK500Controller
{
public:
    LenovoK500Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~LenovoK500Controller();

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();

    void SetMode(unsigned char mode_value, std::vector<RGBColor> colors, unsigned int brightness, unsigned int speed);

private:
    hid_device*             device;
    std::string             location;
    std::string             name;
};
