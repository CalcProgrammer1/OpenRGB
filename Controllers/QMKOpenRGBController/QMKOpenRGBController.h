/*---------------------------------------------------------*\
| QMKOpenRGBController.h                                    |
|                                                           |
|   Driver for OpenRGB QMK Keyboard Protocol                |
|                                                           |
|   Kasper                                      10 Oct 2020 |
|   Jath03                                      28 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstring>
#include <map>
#include <hidapi.h>
#include "ResourceManager.h"
#include "RGBController.h"
#include "hsv.h"

#define QMK_OPENRGB_PACKET_SIZE         65
#define QMK_OPENRGB_HID_READ_TIMEOUT    50

enum CommandsId
{
    QMK_OPENRGB_GET_PROTOCOL_VERSION = 1,
    QMK_OPENRGB_GET_QMK_VERSION,
    QMK_OPENRGB_GET_DEVICE_INFO,
    QMK_OPENRGB_GET_MODE_INFO,
    QMK_OPENRGB_GET_LED_INFO,
    QMK_OPENRGB_GET_IS_MODE_ENABLED,
    QMK_OPENRGB_GET_ENABLED_MODES = QMK_OPENRGB_GET_IS_MODE_ENABLED,

    QMK_OPENRGB_SET_MODE,
    QMK_OPENRGB_DIRECT_MODE_SET_SINGLE_LED,
    QMK_OPENRGB_DIRECT_MODE_SET_LEDS,
};

enum Modes
{
    QMK_OPENRGB_MODE_OPENRGB_DIRECT = 1,
    QMK_OPENRGB_MODE_SOLID_COLOR,
    QMK_OPENRGB_MODE_ALPHA_MOD,
    QMK_OPENRGB_MODE_GRADIENT_UP_DOWN,
    QMK_OPENRGB_MODE_GRADIENT_LEFT_RIGHT,
    QMK_OPENRGB_MODE_BREATHING,
    QMK_OPENRGB_MODE_BAND_SAT,
    QMK_OPENRGB_MODE_BAND_VAL,
    QMK_OPENRGB_MODE_BAND_PINWHEEL_SAT,
    QMK_OPENRGB_MODE_BAND_PINWHEEL_VAL,
    QMK_OPENRGB_MODE_BAND_SPIRAL_SAT,
    QMK_OPENRGB_MODE_BAND_SPIRAL_VAL,
    QMK_OPENRGB_MODE_CYCLE_ALL,
    QMK_OPENRGB_MODE_CYCLE_LEFT_RIGHT,
    QMK_OPENRGB_MODE_CYCLE_UP_DOWN,
    QMK_OPENRGB_MODE_CYCLE_OUT_IN,
    QMK_OPENRGB_MODE_CYCLE_OUT_IN_DUAL,
    QMK_OPENRGB_MODE_RAINBOW_MOVING_CHEVRON,
    QMK_OPENRGB_MODE_CYCLE_PINWHEEL,
    QMK_OPENRGB_MODE_CYCLE_SPIRAL,
    QMK_OPENRGB_MODE_DUAL_BEACON,
    QMK_OPENRGB_MODE_RAINBOW_BEACON,
    QMK_OPENRGB_MODE_RAINBOW_PINWHEELS,
    QMK_OPENRGB_MODE_RAINDROPS,
    QMK_OPENRGB_MODE_JELLYBEAN_RAINDROPS,
    QMK_OPENRGB_MODE_HUE_BREATHING,
    QMK_OPENRGB_MODE_HUE_PENDULUM,
    QMK_OPENRGB_MODE_HUE_WAVE,
    QMK_OPENRGB_MODE_TYPING_HEATMAP,
    QMK_OPENRGB_MODE_DIGITAL_RAIN,
    QMK_OPENRGB_MODE_SOLID_REACTIVE_SIMPLE,
    QMK_OPENRGB_MODE_SOLID_REACTIVE,
    QMK_OPENRGB_MODE_SOLID_REACTIVE_WIDE,
    QMK_OPENRGB_MODE_SOLID_REACTIVE_MULTIWIDE,
    QMK_OPENRGB_MODE_SOLID_REACTIVE_CROSS,
    QMK_OPENRGB_MODE_SOLID_REACTIVE_MULTICROSS,
    QMK_OPENRGB_MODE_SOLID_REACTIVE_NEXUS,
    QMK_OPENRGB_MODE_SOLID_REACTIVE_MULTINEXUS,
    QMK_OPENRGB_MODE_SPLASH,
    QMK_OPENRGB_MODE_MULTISPLASH,
    QMK_OPENRGB_MODE_SOLID_SPLASH,
    QMK_OPENRGB_MODE_SOLID_MULTISPLASH,
    QMK_OPENRGB_MODE_PIXEL_RAIN,
    QMK_OPENRGB_MODE_PIXEL_FLOW,
    QMK_OPENRGB_MODE_PIXEL_FRACTAL,
};

enum SpeedCommands
{
    QMK_OPENRGB_SPEED_SLOWEST   = 0x00, /* Slowest speed                    */
    QMK_OPENRGB_SPEED_NORMAL    = 0x7F, /* Normal speed                     */
    QMK_OPENRGB_SPEED_FASTEST   = 0xFF, /* Fastest speed                    */
};

enum
{
    QMK_OPENRGB_FAILURE         = 25,   /* Failure status code              */
    QMK_OPENRGB_SUCCESS         = 50,   /* Success status code              */
    QMK_OPENRGB_END_OF_MESSAGE  = 100,  /* End of Message status code       */
};

enum
{
    QMK_OPENRGB_TOTAL_NUMBER_OF_LEDS_BYTE                   = 1,
    QMK_OPENRGB_TOTAL_NUMBER_OF_LEDS_WITH_EMPTY_SPACE_BYTE  = 2
};

enum
{
    QMK_OPENRGB_MODE_BYTE       = 1,
    QMK_OPENRGB_SPEED_BYTE      = 2,
    QMK_OPENRGB_HUE_BYTE        = 3,
    QMK_OPENRGB_SATURATION_BYTE = 4,
    QMK_OPENRGB_VALUE_BYTE      = 5,
};

enum
{
    QMK_OPENRGB_POINT_X_BYTE    = 1,
    QMK_OPENRGB_POINT_Y_BYTE    = 2,
    QMK_OPENRGB_FLAG_BYTE       = 3,
    QMK_OPENRGB_R_COLOR_BYTE    = 4,
    QMK_OPENRGB_G_COLOR_BYTE    = 5,
    QMK_OPENRGB_B_COLOR_BYTE    = 6,
    QMK_OPENRGB_KEYCODE_BYTE    = 7
};

typedef struct
{
    uint8_t x;
    uint8_t y;
} point_t;
