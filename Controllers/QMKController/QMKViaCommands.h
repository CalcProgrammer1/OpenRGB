/*---------------------------------------------------------*\
| QMKViaCommands.h                                          |
|                                                           |
|   List of QMK VIA command values                          |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com)      21 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

enum
{
    QMK_VIA_CMD_GET_PROTOCOL_VERSION                = 0x01,
    QMK_VIA_CMD_GET_KEYBOARD_VALUE                  = 0x02,
    QMK_VIA_CMD_SET_KEYBOARD_VALUE                  = 0x03,
    QMK_VIA_CMD_VIA_DYNAMIC_KEYMAP_GET_KEYCODE      = 0x04,
    QMK_VIA_CMD_VIA_DYNAMIC_KEYMAP_SET_KEYCODE      = 0x05,
    QMK_VIA_CMD_VIA_DYNAMIC_KEYMAP_RESET            = 0x06,
    QMK_VIA_CMD_CUSTOM_SET_VALUE                    = 0x07,
    QMK_VIA_CMD_CUSTOM_GET_VALUE                    = 0x08,
    QMK_VIA_CMD_CUSTOM_SAVE                         = 0x09,
    QMK_VIA_CMD_EEPROM_RESET                        = 0x0A,
};

enum
{
    QMK_VIA_CUSTOM_CHANNEL                          = 0,
    QMK_VIA_BACKLIGHT_CHANNEL                       = 1,
    QMK_VIA_RGBLIGHT_CHANNEL                        = 2,
    QMK_VIA_RGB_MATRIX_CHANNEL                      = 3,
    QMK_VIA_AUDIO_CHANNEL                           = 4,
    QMK_VIA_LED_MATRIX_CHANNEL                      = 5,
};

enum
{
    QMK_VIA_BACKLIGHT_BRIGHTNESS                    = 1,
    QMK_VIA_BACKLIGHT_EFFECT                        = 2,
};

enum
{
    QMK_VIA_RGBLIGHT_BRIGHTNESS                     = 1,
    QMK_VIA_RGBLIGHT_EFFECT                         = 2,
    QMK_VIA_RGBLIGHT_EFFECT_SPEED                   = 3,
    QMK_VIA_RGBLIGHT_COLOR                          = 4,
};

enum
{
    QMK_VIA_RGB_MATRIX_BRIGHTNESS                   = 1,
    QMK_VIA_RGB_MATRIX_EFFECT                       = 2,
    QMK_VIA_RGB_MATRIX_EFFECT_SPEED                 = 3,
    QMK_VIA_RGB_MATRIX_COLOR                        = 4,
};

enum
{
    QMK_VIA_LED_MATRIX_BRIGHTNESS                   = 1,
    QMK_VIA_LED_MATRIX_EFFECT                       = 2,
    QMK_VIA_LED_MATRIX_EFFECT_SPEED                 = 3,
};

enum
{
    QMK_VIA_AUDIO_ENABLE                            = 1,
    QMK_VIA_AUDIO_CLICKY_ENABLE                     = 2,
};
