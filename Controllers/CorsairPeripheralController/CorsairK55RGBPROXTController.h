/*---------------------------------------------------------*\
| CorsairK55RGBPROXTController.h                            |
|                                                           |
|   Driver for Corsair K55 RGB PRO XT keyboard              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

class CorsairK55RGBPROXTController
{
public:
    CorsairK55RGBPROXTController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~CorsairK55RGBPROXTController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void SetLEDs(std::vector<RGBColor> colors);
    void SetHardwareMode
         (
             int mode_value,
             unsigned int color_mode,
             std::vector<RGBColor> colors,
             unsigned int speed,
             unsigned int direction
         );
    void SwitchMode(bool software);

    enum
    {
        CORSAIR_MODE_DIRECT_VALUE           = 0xFFFF,
        CORSAIR_HW_MODE_STATIC_VALUE        = 0x207E,
        CORSAIR_HW_MODE_COLOR_PULSE_VALUE   = 0xAD4F,
        CORSAIR_HW_MODE_COLOR_SHIFT_VALUE   = 0xA5FA,
        CORSAIR_HW_MODE_COLOR_WAVE_VALUE    = 0x7BFF,
        CORSAIR_HW_MODE_RAINBOW_WAVE_VALUE  = 0xB94C,
        CORSAIR_HW_MODE_RAIN_VALUE          = 0xA07E,
        CORSAIR_HW_MODE_SPIRAL_VALUE        = 0xAB87,
        CORSAIR_HW_MODE_WATER_COLOR_VALUE   = 0x0022,
        CORSAIR_HW_MODE_TYPE_KEY_VALUE      = 0xB1F9,
        CORSAIR_HW_MODE_TYPE_RIPPLE_VALUE   = 0x09A2,
        CORSAIR_HW_MODE_VISOR_VALUE         = 0x90c0
    };

    enum
    {
        CORSAIR_HW_MODE_COLOR_NONE          = 0x00,
        CORSAIR_HW_MODE_COLOR_PREDEF        = 0x01,
        CORSAIR_HW_MODE_COLOR_RANDOM        = 0x02,
        CORSAIR_HW_MODE_COLOR_UNKNOWN       = 0x03
    };

    enum
    {
        CORSAIR_HW_MODE_SPEED_NONE          = 0x00,
        CORSAIR_HW_MODE_SPEED_MIN           = 0x03,
        CORSAIR_HW_MODE_SPEED_MED           = 0x04,
        CORSAIR_HW_MODE_SPEED_MAX           = 0x05
    };

    enum
    {
        CORSAIR_HW_MODE_DIR_NONE                = 0x00,
        CORSAIR_HW_MODE_DIR_DOWN                = 0x01,
        CORSAIR_HW_MODE_DIR_UP                  = 0x02,
        CORSAIR_HW_MODE_DIR_RIGHT               = 0x04,
        CORSAIR_HW_MODE_DIR_LEFT                = 0x05,
        CORSAIR_HW_MODE_DIR_CLOCK_WISE          = 0x06,
        CORSAIE_HW_MODE_DIR_COUNTER_CLOCK_WISE  = 0x07
    };

private:
    hid_device* dev;

    std::string location;
    std::string name;

    void LightingControl();
};
