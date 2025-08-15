/*---------------------------------------------------------*\
| WushiL50USBController.h                                   |
|                                                           |
|   Driver for Wushi L50                                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "hidapi_wrapper.h"

#ifndef HID_MAX_STR
#define HID_MAX_STR                 255
#endif

#define WUSHI_L50_HID_PACKET_SIZE   65
#define WUSHI_L50_NUM_LEDS          4

enum WUSHI_L50_EFFECT
{
    WUSHI_L50_EFFECT_STATIC         = 1,    /* Static mode              */
    WUSHI_L50_EFFECT_BREATH         = 3,    /* Breathing mode           */
    WUSHI_L50_EFFECT_WAVE           = 4,    /* Wave mode                */
    WUSHI_L50_EFFECT_SMOOTH         = 6,    /* Smooth mode              */
    WUSHI_L50_EFFECT_RACE           = 8,    /* Race mode                */
    WUSHI_L50_EFFECT_STACK          = 10,   /* Stack mode               */
};

enum WUSHI_L50_BRIGHTNESS
{
    WUSHI_L50_BRIGHTNESS_LOW        = 1,    /* Low brightness           */
    WUSHI_L50_BRIGHTNESS_HIGH       = 2,    /* High brightness          */
};

enum WUSHI_L50_SPEED
{
    WUSHI_L50_SPEED_SLOWEST         = 1,    /* Slowest speed            */
    WUSHI_L50_SPEED_SLOW            = 2,    /* Slow speed               */
    WUSHI_L50_SPEED_FAST            = 3,    /* Fast speed               */
    WUSHI_L50_SPEED_FASTEST         = 4,    /* Fastest speed            */
};

enum WUSHI_L50_Direction
{
    WUSHI_L50_Direction_LEFT        = 1,    /* Left direction           */
    WUSHI_L50_Direction_RIGHT       = 2,    /* Right direction          */
};

class WushiL50State
{
public:
    uint8_t effect                  = WUSHI_L50_EFFECT_STATIC;
    uint8_t speed                   = WUSHI_L50_SPEED_SLOWEST;
    uint8_t brightness              = WUSHI_L50_BRIGHTNESS_LOW;
    uint8_t zone0_rgb[3]            = {0xFF, 0xFF, 0xFF};
    uint8_t zone1_rgb[3]            = {0xFF, 0xFF, 0xFF};
    uint8_t zone2_rgb[3]            = {0xFF, 0xFF, 0xFF};
    uint8_t zone3_rgb[3]            = {0xFF, 0xFF, 0xFF};
    uint8_t wave_ltr                = 0;
    uint8_t wave_rtl                = 0;

    void Reset()
    {
        effect                      = WUSHI_L50_EFFECT_STATIC;
        speed                       = WUSHI_L50_SPEED_SLOWEST;
        brightness                  = WUSHI_L50_BRIGHTNESS_LOW;
        wave_ltr                    = 0;
        wave_rtl                    = 0;
    }

    void SetColors(std::vector<RGBColor> group_colors)
    {
        zone0_rgb[0]                = RGBGetRValue(group_colors[0]);
        zone0_rgb[1]                = RGBGetGValue(group_colors[0]);
        zone0_rgb[2]                = RGBGetBValue(group_colors[0]);
        zone1_rgb[0]                = RGBGetRValue(group_colors[1]);
        zone1_rgb[1]                = RGBGetGValue(group_colors[1]);
        zone1_rgb[2]                = RGBGetBValue(group_colors[1]);
        zone2_rgb[0]                = RGBGetRValue(group_colors[2]);
        zone2_rgb[1]                = RGBGetGValue(group_colors[2]);
        zone2_rgb[2]                = RGBGetBValue(group_colors[2]);
        zone3_rgb[0]                = RGBGetRValue(group_colors[3]);
        zone3_rgb[1]                = RGBGetGValue(group_colors[3]);
        zone3_rgb[2]                = RGBGetBValue(group_colors[3]);

        wave_rtl                    = 0;
    }
};

class WushiL50USBController
{
public:
    WushiL50USBController(hidapi_wrapper hid_wrapper, hid_device* dev_handle, const char* path, std::string dev_name);
    ~WushiL50USBController();

    std::string getName();
    std::string getLocation();
    std::string GetSerialString();

    void        setMode(WushiL50State * in_mode);

private:
    hidapi_wrapper  wrapper;
    hid_device *    dev;
    std::string     location;
    std::string     name;
};
