/*---------------------------------------------------------*\
| LenovoDevices4Zone.h                                      |
|                                                           |
|   Device list for Lenovo 4-Zone devices                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "RGBControllerKeyNames.h"
#include "RGBController.h"
#include "LenovoDevices.h"

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define IDEAPAD_315ACH6                         0xC963
#define LEGION_5_2023_PID                       0xC985
#define LEGION_5_2023_IDEAPAD_PID               0xC984
#define LEGION_5_2022_PID                       0xC975
#define LEGION_5_2022_IDEAPAD_PID               0xC973
#define LEGION_5_2021_PID                       0xC965
#define LEGION_5_2021_IDEAPAD_PID               0xC963
#define LEGION_5_2020_PID                       0xC955

enum LENOVO_4_ZONE_EFFECT
{
    LENOVO_4_ZONE_EFFECT_STATIC = 1,
    LENOVO_4_ZONE_EFFECT_BREATH = 3,
    LENOVO_4_ZONE_EFFECT_WAVE   = 4,
    LENOVO_4_ZONE_EFFECT_SMOOTH = 6,
};

enum LENOVO_4_ZONE_BRIGHTNESS
{
    LENOVO_4_ZONE_BRIGHTNESS_LOW  = 1,
    LENOVO_4_ZONE_BRIGHTNESS_HIGH = 2,
};

enum LENOVO_4_ZONE_SPEED
{
    LENOVO_4_ZONE_SPEED_SLOWEST = 1,
    LENOVO_4_ZONE_SPEED_SLOW    = 2,
    LENOVO_4_ZONE_SPEED_FAST    = 3,
    LENOVO_4_ZONE_SPEED_FASTEST = 4,
};

/// struct a USB packet for set the keyboard LEDs
class KeyboardState
{
public:
    uint8_t header[2] = {0xCC, 0x16};
    uint8_t effect = LENOVO_4_ZONE_EFFECT_STATIC;
    uint8_t speed = LENOVO_4_ZONE_SPEED_SLOWEST;
    uint8_t brightness = LENOVO_4_ZONE_BRIGHTNESS_LOW;
    uint8_t zone0_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone1_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone2_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone3_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t wave_ltr = 0;
    uint8_t wave_rtl = 0;

    void Reset()
    {
        header[0] = 0xCC, header[1] = 0x16;
        effect = LENOVO_4_ZONE_EFFECT_STATIC;
        speed = LENOVO_4_ZONE_SPEED_SLOWEST;
        brightness = LENOVO_4_ZONE_BRIGHTNESS_LOW;
        zone0_rgb[0] = 0xFF, zone0_rgb[1] = 0xFF, zone0_rgb[2] = 0xFF;
        zone1_rgb[0] = 0xFF, zone1_rgb[1] = 0xFF, zone1_rgb[2] = 0xFF;
        zone2_rgb[0] = 0xFF, zone2_rgb[1] = 0xFF, zone2_rgb[2] = 0xFF;
        zone3_rgb[0] = 0xFF, zone3_rgb[1] = 0xFF, zone3_rgb[2] = 0xFF;
        wave_ltr = 0;
        wave_rtl = 0;
    }

    void SetColors(std::vector<RGBColor> group_colors)
    {
        zone0_rgb[0] = RGBGetRValue(group_colors[0]);
        zone0_rgb[1] = RGBGetGValue(group_colors[0]);
        zone0_rgb[2] = RGBGetBValue(group_colors[0]);
        zone1_rgb[0] = RGBGetRValue(group_colors[1]);
        zone1_rgb[1] = RGBGetGValue(group_colors[1]);
        zone1_rgb[2] = RGBGetBValue(group_colors[1]);
        zone2_rgb[0] = RGBGetRValue(group_colors[2]);
        zone2_rgb[1] = RGBGetGValue(group_colors[2]);
        zone2_rgb[2] = RGBGetBValue(group_colors[2]);
        zone3_rgb[0] = RGBGetRValue(group_colors[3]);
        zone3_rgb[1] = RGBGetGValue(group_colors[3]);
        zone3_rgb[2] = RGBGetBValue(group_colors[3]);
    }
};

/*-------------------------*\
| 4-Zone keyboard           |
\*-------------------------*/

static const lenovo_led lenovo_4_zone_leds[]
{
    {0x00, "Left side"},
    {0x01, "Left center"},
    {0x02, "Right center"},
    {0x03, "Right side"},
};
