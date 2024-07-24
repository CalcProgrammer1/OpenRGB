/*---------------------------------------------------------*\
| RGBController_LightSaltKeypad.cpp                         |
|                                                           |
|   RGBController for LightSalt Keypad                      |
|                                                           |
|   James Buren (braewoods)                     23 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_LightSaltKeypad.h"

// 0xFFFFFFFF indicates an unused entry in matrix
#define NA 0xFFFFFFFF

enum
{
    LED_SETS    =  1,
    LED_ROWS    =  5,
    LED_COLUMNS = 10,
    LED_COUNT   = 18,
    MAP_HEIGHT  =  5,
    MAP_WIDTH   =  4
};

static char const * const led_names[LED_COUNT] =
{
    /* Row 1 */
    KEY_EN_TAB,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_BACKSPACE,

    /* Row 2 */
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_MINUS,

    /* Row 3 */
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_6,
    KEY_EN_PLUS,

    /* Row 4 */
    KEY_EN_NUMPAD_1,
    KEY_EN_NUMPAD_2,
    KEY_EN_NUMPAD_3,

    /* Row 5 */
    KEY_EN_NUMPAD_0,
    KEY_EN_NUMPAD_PERIOD,
    KEY_EN_NUMPAD_ENTER
};

static const int led_indices[LED_COUNT] =
{
    /* Row 1 */
    0, 1, 2, 3,

    /* Row 2 */
    4, 5, 6, 7,

    /* Row 3 */
    8, 9, 10, 11,

    /* Row 4 */
    12, 13, 14,

    /* Row 5 */
    16, 17, 18
};

static const int led_deflections[LED_COUNT] =
{
    /* Row 1 */
    0x0, 0x0, 0x0, 0x0,

    /* Row 2 */
    0x0, 0x0, 0x0, 0x0,

    /* Row 3 */
    0x0, 0x0, 0x0, 0x0,

    /* Row 4 */
    0x0, 0x0, 0x0,

    /* Row 5 */
    0x0, 0x0, 0x0
};

static const unsigned int matrix_map[MAP_HEIGHT][MAP_WIDTH] =
{
    /* Row 1 */
    { 0,  1 , 2,  3},

    /* Row 2 */
    { 4,  5,  6,  7},

    /* Row 3 */
    { 8,  9, 10, 11},

    /* Row 4 */
    {12, 13, 14, NA},

    /* Row 5 */
    {15, NA, 16, 17}
};

RGBController_LightSaltKeypad::RGBController_LightSaltKeypad(LightSaltController* controller_ptr)
{
    table.device.name     = "Keypad";
    table.device.type     = DEVICE_TYPE_KEYPAD;
    table.led.sets        = LED_SETS;
    table.led.rows        = LED_ROWS;
    table.led.columns     = LED_COLUMNS;
    table.led.count       = LED_COUNT;
    table.led.names       = led_names;
    table.led.indices     = led_indices;
    table.led.deflections = led_deflections;
    table.map.height      = MAP_HEIGHT;
    table.map.width       = MAP_WIDTH;
    table.map.matrix      = (unsigned int *)matrix_map;

    SetupData(controller_ptr);
    SetupModes();
    SetupZones();
}
