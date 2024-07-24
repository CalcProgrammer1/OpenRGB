/*---------------------------------------------------------*\
| RGBController_LightSaltKeyboard.cpp                       |
|                                                           |
|   RGBController for LightSalt Keyboard                    |
|                                                           |
|   James Buren (braewoods)                     23 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_LightSaltKeyboard.h"

// 0xFFFFFFFF indicates an unused entry in matrix
#define NA 0xFFFFFFFF

enum
{
    LED_SETS    =  4,
    LED_ROWS    =  9,
    LED_COLUMNS = 10,
    LED_COUNT   = 83,
    MAP_HEIGHT  =  6,
    MAP_WIDTH   = 15,
};

static char const * const led_names[LED_COUNT] =
{
    /* Row 1 */
    KEY_EN_ESCAPE,
    KEY_EN_F1,
    KEY_EN_F2,
    KEY_EN_F3,
    KEY_EN_F4,
    KEY_EN_F5,
    KEY_EN_F6,
    KEY_EN_F7,
    KEY_EN_F8,
    KEY_EN_F9,
    KEY_EN_F10,
    KEY_EN_F11,
    KEY_EN_F12,

    /* Row 2 */
    KEY_EN_BACK_TICK,
    KEY_EN_1,
    KEY_EN_2,
    KEY_EN_3,
    KEY_EN_4,
    KEY_EN_5,
    KEY_EN_6,
    KEY_EN_7,
    KEY_EN_8,
    KEY_EN_9,
    KEY_EN_0,
    KEY_EN_MINUS,
    KEY_EN_EQUALS,
    KEY_EN_BACKSPACE,
    KEY_EN_DELETE,

    /* Row 3 */
    KEY_EN_TAB,
    KEY_EN_Q,
    KEY_EN_W,
    KEY_EN_E,
    KEY_EN_R,
    KEY_EN_T,
    KEY_EN_Y,
    KEY_EN_U,
    KEY_EN_I,
    KEY_EN_O,
    KEY_EN_P,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_PRINT_SCREEN,

    /* Row 4 */
    KEY_EN_CAPS_LOCK,
    KEY_EN_A,
    KEY_EN_S,
    KEY_EN_D,
    KEY_EN_F,
    KEY_EN_G,
    KEY_EN_H,
    KEY_EN_J,
    KEY_EN_K,
    KEY_EN_L,
    KEY_EN_SEMICOLON,
    KEY_EN_QUOTE,
    KEY_EN_ANSI_ENTER,

    /* Row 5 */
    KEY_EN_LEFT_SHIFT,
    KEY_EN_Z,
    KEY_EN_X,
    KEY_EN_C,
    KEY_EN_V,
    KEY_EN_B,
    KEY_EN_N,
    KEY_EN_M,
    KEY_EN_COMMA,
    KEY_EN_PERIOD,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_UP_ARROW,

    /* Row 6 */
    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    KEY_EN_SPACE,
    KEY_EN_SPACE,
    KEY_EN_SPACE,
    KEY_EN_SPACE,
    KEY_EN_SPACE,
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW
};

static const int led_indices[LED_COUNT] =
{
    /* Row 1 */
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,

    /* Row 2 */
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,

    /* Row 3 */
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,

    /* Row 4 */
    45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,

    /* Row 5 */
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 73,

    /* Row 6 */
    75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89
};

static const int led_deflections[LED_COUNT] =
{
    /* Row 1 */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    /* Row 2 */
    0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0x0, 0x0,

    /* Row 3 */
    0x0, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0xA, 0xA, 0xA, 0x0,

    /* Row 4 */
    0x0, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0xA, 0xA, 0x0,

    /* Row 5 */
    0x0, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0xA, 0xA, 0xA, 0x0, 0x0,

    /* Row 6 */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

static const unsigned int matrix_map[MAP_HEIGHT][MAP_WIDTH] =
{
    /* Row 1 */
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, NA, NA },

    /* Row 2 */
    { 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 },

    /* Row 3 */
    { 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42 },

    /* Row 4 */
    { 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, NA, NA },

    /* Row 5 */
    { 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, NA, 68, NA },

    /* Row 6 */
    { 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, NA, 80, 81, 82 }
};

RGBController_LightSaltKeyboard::RGBController_LightSaltKeyboard(LightSaltController* controller_ptr)
{
    table.device.name     = "Keyboard";
    table.device.type     = DEVICE_TYPE_KEYBOARD;
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
