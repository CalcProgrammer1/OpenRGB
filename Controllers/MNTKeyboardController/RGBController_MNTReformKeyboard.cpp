/*---------------------------------------------------------*\
| RGBController_MNTReformKeyboard.cpp                       |
|                                                           |
|   Driver for the MNT Reform keyboards                     |
|                                                           |
|   Christian Heller <c.heller@plomlompom.de>  7 Aug 2024   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_MNTReformKeyboard.h"

/**----------------------------------------------*\
  @name       MNT Reform Keyboard
  @category   Keyboard
  @type       USB
  @detectors  DetectMNTKeyboardControllers
\*----------------------------------------------**/

static unsigned int matrix_keys_MNTReform[KBD_ROWS][KBD_COLS_REFORM] =
    {
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
        {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27},
        {28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41},
        {42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, NA},
        {56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69},
        {70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, NA, NA, NA}};

static const char *led_names_MNTReform[] =
    {
        // row 0
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
        KEY_EN_POWER,
        // row 1
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
        // row 2
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
        // row 3
        KEY_EN_LEFT_CONTROL,
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
        KEY_EN_UNUSED,
        // row 4
        KEY_EN_LEFT_SHIFT,
        KEY_EN_DELETE,
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
        KEY_EN_UP_ARROW,
        KEY_EN_RIGHT_SHIFT,
        // row 5
        KEY_EN_LEFT_FUNCTION,
        KEY_EN_LEFT_WINDOWS,
        KEY_EN_LEFT_ALT,
        KEY_EN_SPACE,
        KEY_EN_SPACE,
        KEY_EN_SPACE,
        KEY_EN_RIGHT_ALT,
        KEY_EN_RIGHT_CONTROL,
        KEY_EN_LEFT_ARROW,
        KEY_EN_DOWN_ARROW,
        KEY_EN_RIGHT_ARROW,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED};

RGBController_MNTReformKeyboard::RGBController_MNTReformKeyboard(MNTReformKeyboardController *controller_ptr)
{
    led_names   = led_names_MNTReform;
    matrix_keys = matrix_keys_MNTReform[0];
    controller  = controller_ptr;
    name        = "MNT Reform Keyboard";
    description = "MNT Reform Keyboard";
    CommonInit();
}
