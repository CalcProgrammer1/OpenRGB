/*---------------------------------------------------------*\
| RoccatVulcanKeyboardLayouts.h                             |
|                                                           |
|   Layouts for Roccat Vulcan keyboard                      |
|                                                           |
|   Mola19                                      29 Sep 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <map>
#include <vector>
#include "RGBControllerKeyNames.h"
#include "RGBController.h"

enum
{
    ROCCAT_VULCAN_LAYOUT_US    = 0,
    ROCCAT_VULCAN_LAYOUT_DE    = 1,
    ROCCAT_VULCAN_LAYOUT_UK    = 2,
    ROCCAT_VULCAN_LAYOUT_FR    = 3,
};

#define NA  0xFFFFFFFF

struct led_value
{
    const char*         name;
    unsigned char       id;
};

struct layout_info
{
    unsigned int*               matrix_map;
    int                         size;
    int                         rows;
    int                         cols;
    std::vector<led_value>      led_names;
};

static unsigned int ROCCAT_VULCAN_120_AIMO_LAYOUT_KEYS_104[6][24] =
{
    {   0,  NA,   8,  14,  19,  24,  NA,  34,  39,  44,  49,  55,  61,  66,  70,  NA,  74,  78,  83,  NA,  NA,  NA,  NA,  NA },
    {   1,   6,   9,  15,  20,  25,  29,  35,  40,  45,  50,  56,  62,  67,  NA,  NA,  75,  79,  84,  NA,  87,  92,  96, 101 },
    {   2,  NA,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  71,  NA,  76,  80,  85,  NA,  88,  93,  97, 102 },
    {   3,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  NA,  72,  NA,  NA,  NA,  NA,  NA,  89,  94,  98,  NA },
    {   4,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  NA,  69,  NA,  NA,  NA,  81,  NA,  NA,  90,  95,  99, 103 },
    {   5,   7,  13,  NA,  NA,  NA,  33,  NA,  NA,  NA,  54,  60,  65,  NA,  73,  NA,  77,  82,  86,  NA,  91,  NA, 100,  NA }
};

static unsigned int ROCCAT_VULCAN_120_AIMO_LAYOUT_KEYS_105[6][24] =
{
    {   0,  NA,   9,  15,  20,  25,  NA,  35,  40,  45,  50,  56,  62,  67,  72,  NA,  75,  79,  84,  NA,  NA,  NA,  NA,  NA },
    {   1,   6,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  NA,  NA,  76,  80,  85,  NA,  88,  93,  97, 102 },
    {   2,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  69,  NA,  NA,  77,  81,  86,  NA,  89,  94,  98, 103 },
    {   3,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  65,  70,  73,  NA,  NA,  NA,  NA,  NA,  90,  95,  99,  NA },
    {   4,   7,  13,  19,  24,  29,  33,  39,  44,  49,  54,  60,  NA,  71,  NA,  NA,  NA,  82,  NA,  NA,  91,  96, 100, 104 },
    {   5,   8,  14,  NA,  NA,  NA,  34,  NA,  NA,  NA,  55,  61,  66,  NA,  74,  NA,  78,  83,  87,  NA,  92,  NA, 101,  NA }
};

static unsigned int ROCCAT_VULCAN_II_LAYOUT_KEYS_105[6][24] =
{
    {   0,  NA,   9,  15,  20,  25,  NA,  35,  40,  45,  50,  56,  62,  67,  72,  NA,  75,  79,  84,  NA, 105, 106, 107,  NA },
    {   1,   6,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  NA,  NA,  76,  80,  85,  NA,  88,  93,  97, 102 },
    {   2,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  69,  NA,  NA,  77,  81,  86,  NA,  89,  94,  98, 103 },
    {   3,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  65,  70,  73,  NA,  NA,  NA,  NA,  NA,  90,  95,  99,  NA },
    {   4,   7,  13,  19,  24,  29,  33,  39,  44,  49,  54,  60,  NA,  71,  NA,  NA,  NA,  82,  NA,  NA,  91,  96, 100, 104 },
    {   5,   8,  14,  NA,  NA,  NA,  34,  NA,  NA,  NA,  55,  61,  66,  NA,  74,  NA,  78,  83,  87,  NA,  92,  NA, 101,  NA }
};

static unsigned int ROCCAT_VULCAN_TKL_LAYOUT_KEYS_104[6][19] =
{
    {   0,  NA,   8,  14,  19,  24,  NA,  34,  39,  44,  49,  55,  61,  66,  70,  NA,  74,  NA,  NA },
    {   1,   6,   9,  15,  20,  25,  29,  35,  40,  45,  50,  56,  62,  67,  NA,  NA,  75,  78,  82 },
    {   2,  NA,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  71,  NA,  76,  79,  83 },
    {   3,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  NA,  72,  NA,  NA,  NA,  NA },
    {   4,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  NA,  69,  NA,  NA,  NA,  80,  NA },
    {   5,   7,  13,  NA,  NA,  NA,  33,  NA,  NA,  NA,  54,  60,  65,  NA,  73,  NA,  77,  81,  84 }
};

static unsigned int ROCCAT_VULCAN_TKL_LAYOUT_KEYS_105[6][19] =
{
    {   0,  NA,   9,  15,  20,  25,  NA,  35,  40,  45,  50,  56,  62,  67,  72,  NA,  75,  NA,  NA },
    {   1,   6,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  NA,  NA,  76,  79,  83 },
    {   2,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  69,  NA,  NA,  77,  80,  84 },
    {   3,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  65,  70,  73,  NA,  NA,  NA,  NA },
    {   4,   7,  13,  19,  24,  29,  33,  39,  44,  49,  54,  60,  NA,  71,  NA,  NA,  NA,  81,  NA },
    {   5,   8,  14,  NA,  NA,  NA,  34,  NA,  NA,  NA,  55,  61,  66,  NA,  74,  NA,  78,  82,  85 }
};

static unsigned int ROCCAT_MAGMA_LAYOUT_KEYS[1][5] =
{
    {  0,  1,  2,  3,  4 },
};

static std::map<int, layout_info> RoccatVulcan120AimoLayouts =
{
    {
        ROCCAT_VULCAN_LAYOUT_UK,
        {
            *ROCCAT_VULCAN_120_AIMO_LAYOUT_KEYS_105,
            105,
            6,
            24,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x06 },
                { KEY_EN_ISO_BACK_SLASH,      0x09 },
                { KEY_EN_LEFT_WINDOWS,        0x0A },

                { KEY_EN_F1,                  0x0B },
                { KEY_EN_2,                   0x0C },
                { KEY_EN_Q,                   0x07 },
                { KEY_EN_A,                   0x08 },
                { KEY_EN_Z,                   0x0F },
                { KEY_EN_LEFT_ALT,            0x10 },

                { KEY_EN_F2,                  0x11 },
                { KEY_EN_3,                   0x12 },
                { KEY_EN_W,                   0x0D },
                { KEY_EN_S,                   0x0E },
                { KEY_EN_X,                   0x15 },

                { KEY_EN_F3,                  0x17 },
                { KEY_EN_4,                   0x18 },
                { KEY_EN_E,                   0x13 },
                { KEY_EN_D,                   0x14 },
                { KEY_EN_C,                   0x1B },

                { KEY_EN_F4,                  0x1C },
                { KEY_EN_5,                   0x1D },
                { KEY_EN_R,                   0x19 },
                { KEY_EN_F,                   0x1A },
                { KEY_EN_V,                   0x20 },

                { KEY_EN_6,                   0x21 },
                { KEY_EN_T,                   0x1E },
                { KEY_EN_G,                   0x1F },
                { KEY_EN_B,                   0x24 },
                { KEY_EN_SPACE,               0x25 },

                { KEY_EN_F5,                  0x30 },
                { KEY_EN_7,                   0x31 },
                { KEY_EN_Y,                   0x22 },
                { KEY_EN_H,                   0x23 },
                { KEY_EN_N,                   0x34 },

                { KEY_EN_F6,                  0x35 },
                { KEY_EN_8,                   0x36 },
                { KEY_EN_U,                   0x32 },
                { KEY_EN_J,                   0x33 },
                { KEY_EN_M,                   0x39 },

                { KEY_EN_F7,                  0x3B },
                { KEY_EN_9,                   0x3C },
                { KEY_EN_I,                   0x37 },
                { KEY_EN_K,                   0x38 },
                { KEY_EN_COMMA,               0x3F },

                { KEY_EN_F8,                  0x41 },
                { KEY_EN_0,                   0x42 },
                { KEY_EN_O,                   0x3D },
                { KEY_EN_L,                   0x3E },
                { KEY_EN_PERIOD,              0x45 },
                { KEY_EN_RIGHT_ALT,           0x46 },

                { KEY_EN_F9,                  0x4E },
                { KEY_EN_MINUS,               0x48 },
                { KEY_EN_P,                   0x43 },
                { KEY_EN_SEMICOLON,           0x44 },
                { KEY_EN_FORWARD_SLASH,       0x4B },
                { KEY_EN_RIGHT_FUNCTION,      0x4C },

                { KEY_EN_F10,                 0x54 },
                { KEY_EN_EQUALS,              0x4F },
                { KEY_EN_LEFT_BRACKET,        0x49 },
                { KEY_EN_QUOTE,               0x4A },
                { KEY_EN_MENU,                0x53 },

                { KEY_EN_F11,                 0x55 },
                { KEY_EN_BACKSPACE,           0x57 },
                { KEY_EN_RIGHT_BRACKET,       0x50 },
                { KEY_EN_POUND,               0x60 },
                { KEY_EN_RIGHT_SHIFT,         0x52 },

                { KEY_EN_F12,                 0x56 },
                { KEY_EN_ISO_ENTER,           0x58 },
                { KEY_EN_RIGHT_CONTROL,       0x59 },

                { KEY_EN_PRINT_SCREEN,        0x63 },
                { KEY_EN_INSERT,              0x64 },
                { KEY_EN_DELETE,              0x65 },
                { KEY_EN_LEFT_ARROW,          0x66 },

                { KEY_EN_SCROLL_LOCK,         0x67 },
                { KEY_EN_HOME,                0x68 },
                { KEY_EN_END,                 0x69 },
                { KEY_EN_UP_ARROW,            0x6A },
                { KEY_EN_DOWN_ARROW,          0x6B },

                { KEY_EN_PAUSE_BREAK,         0x6C },
                { KEY_EN_PAGE_UP,             0x6D },
                { KEY_EN_PAGE_DOWN,           0x6E },
                { KEY_EN_RIGHT_ARROW,         0x6F },

                { KEY_EN_NUMPAD_LOCK,         0x71 },
                { KEY_EN_NUMPAD_7,            0x72 },
                { KEY_EN_NUMPAD_4,            0x73 },
                { KEY_EN_NUMPAD_1,            0x74 },
                { KEY_EN_NUMPAD_0,            0x75 },

                { KEY_EN_NUMPAD_DIVIDE,       0x77 },
                { KEY_EN_NUMPAD_8,            0x78 },
                { KEY_EN_NUMPAD_5,            0x79 },
                { KEY_EN_NUMPAD_2,            0x7A },

                { KEY_EN_NUMPAD_TIMES,        0x7C },
                { KEY_EN_NUMPAD_9,            0x7D },
                { KEY_EN_NUMPAD_6,            0x7E },
                { KEY_EN_NUMPAD_3,            0x7F },

                { KEY_EN_NUMPAD_PERIOD,       0x80 },
                { KEY_EN_NUMPAD_MINUS,        0x81 },
                { KEY_EN_NUMPAD_PLUS,         0x82 },
                { KEY_EN_NUMPAD_ENTER,        0x83 }
            }
        }
    },
    {
        ROCCAT_VULCAN_LAYOUT_US,
        {
            *ROCCAT_VULCAN_120_AIMO_LAYOUT_KEYS_104,
            104,
            6,
            24,
            {
                { KEY_EN_ESCAPE,              0x00 },
                { KEY_EN_BACK_TICK,           0x01 },
                { KEY_EN_TAB,                 0x02 },
                { KEY_EN_CAPS_LOCK,           0x03 },
                { KEY_EN_LEFT_SHIFT,          0x04 },
                { KEY_EN_LEFT_CONTROL,        0x05 },

                { KEY_EN_1,                   0x06 },
                { KEY_EN_LEFT_WINDOWS,        0x0A },

                { KEY_EN_F1,                  0x0B },
                { KEY_EN_2,                   0x0C },
                { KEY_EN_Q,                   0x07 },
                { KEY_EN_A,                   0x08 },
                { KEY_EN_Z,                   0x0F },
                { KEY_EN_LEFT_ALT,            0x10 },

                { KEY_EN_F2,                  0x11 },
                { KEY_EN_3,                   0x12 },
                { KEY_EN_W,                   0x0D },
                { KEY_EN_S,                   0x0E },
                { KEY_EN_X,                   0x15 },

                { KEY_EN_F3,                  0x17 },
                { KEY_EN_4,                   0x18 },
                { KEY_EN_E,                   0x13 },
                { KEY_EN_D,                   0x14 },
                { KEY_EN_C,                   0x1B },

                { KEY_EN_F4,                  0x1C },
                { KEY_EN_5,                   0x1D },
                { KEY_EN_R,                   0x19 },
                { KEY_EN_F,                   0x1A },
                { KEY_EN_V,                   0x20 },

                { KEY_EN_6,                   0x21 },
                { KEY_EN_T,                   0x1E },
                { KEY_EN_G,                   0x1F },
                { KEY_EN_B,                   0x24 },
                { KEY_EN_SPACE,               0x25 },

                { KEY_EN_F5,                  0x30 },
                { KEY_EN_7,                   0x31 },
                { KEY_EN_Y,                   0x22 },
                { KEY_EN_H,                   0x23 },
                { KEY_EN_N,                   0x34 },

                { KEY_EN_F6,                  0x35 },
                { KEY_EN_8,                   0x36 },
                { KEY_EN_U,                   0x32 },
                { KEY_EN_J,                   0x33 },
                { KEY_EN_M,                   0x39 },

                { KEY_EN_F7,                  0x3B },
                { KEY_EN_9,                   0x3C },
                { KEY_EN_I,                   0x37 },
                { KEY_EN_K,                   0x38 },
                { KEY_EN_COMMA,               0x3F },

                { KEY_EN_F8,                  0x41 },
                { KEY_EN_0,                   0x42 },
                { KEY_EN_O,                   0x3D },
                { KEY_EN_L,                   0x3E },
                { KEY_EN_PERIOD,              0x45 },
                { KEY_EN_RIGHT_ALT,           0x46 },

                { KEY_EN_F9,                  0x4E },
                { KEY_EN_MINUS,               0x48 },
                { KEY_EN_P,                   0x43 },
                { KEY_EN_SEMICOLON,           0x44 },
                { KEY_EN_FORWARD_SLASH,       0x4B },
                { KEY_EN_RIGHT_FUNCTION,      0x4C },

                { KEY_EN_F10,                 0x54 },
                { KEY_EN_EQUALS,              0x4F },
                { KEY_EN_LEFT_BRACKET,        0x49 },
                { KEY_EN_QUOTE,               0x4A },
                { KEY_EN_MENU,                0x53 },

                { KEY_EN_F11,                 0x55 },
                { KEY_EN_BACKSPACE,           0x57 },
                { KEY_EN_RIGHT_BRACKET,       0x50 },
                { KEY_EN_RIGHT_SHIFT,         0x52 },

                { KEY_EN_F12,                 0x56 },
                { KEY_EN_ANSI_BACK_SLASH,     0x51 },
                { KEY_EN_ANSI_ENTER,          0x58 },
                { KEY_EN_RIGHT_CONTROL,       0x59 },

                { KEY_EN_PRINT_SCREEN,        0x63 },
                { KEY_EN_INSERT,              0x64 },
                { KEY_EN_DELETE,              0x65 },
                { KEY_EN_LEFT_ARROW,          0x66 },

                { KEY_EN_SCROLL_LOCK,         0x67 },
                { KEY_EN_HOME,                0x68 },
                { KEY_EN_END,                 0x69 },
                { KEY_EN_UP_ARROW,            0x6A },
                { KEY_EN_DOWN_ARROW,          0x6B },

                { KEY_EN_PAUSE_BREAK,         0x6C },
                { KEY_EN_PAGE_UP,             0x6D },
                { KEY_EN_PAGE_DOWN,           0x6E },
                { KEY_EN_RIGHT_ARROW,         0x6F },

                { KEY_EN_NUMPAD_LOCK,         0x71 },
                { KEY_EN_NUMPAD_7,            0x72 },
                { KEY_EN_NUMPAD_4,            0x73 },
                { KEY_EN_NUMPAD_1,            0x74 },
                { KEY_EN_NUMPAD_0,            0x75 },

                { KEY_EN_NUMPAD_DIVIDE,       0x77 },
                { KEY_EN_NUMPAD_8,            0x78 },
                { KEY_EN_NUMPAD_5,            0x79 },
                { KEY_EN_NUMPAD_2,            0x7A },

                { KEY_EN_NUMPAD_TIMES,        0x7C },
                { KEY_EN_NUMPAD_9,            0x7D },
                { KEY_EN_NUMPAD_6,            0x7E },
                { KEY_EN_NUMPAD_3,            0x7F },

                { KEY_EN_NUMPAD_PERIOD,       0x80 },
                { KEY_EN_NUMPAD_MINUS,        0x81 },
                { KEY_EN_NUMPAD_PLUS,         0x82 },
                { KEY_EN_NUMPAD_ENTER,        0x83 }
            }
        }
    },
};

static std::map<int, layout_info> RoccatPyroLayouts =
{
    {
        ROCCAT_VULCAN_LAYOUT_UK,
        {
            *ROCCAT_VULCAN_120_AIMO_LAYOUT_KEYS_105,
            105,
            6,
            24,
            {
                { KEY_EN_ESCAPE,              0x02 },
                { KEY_EN_BACK_TICK,           0x03 },
                { KEY_EN_TAB,                 0x04 },
                { KEY_EN_CAPS_LOCK,           0x05 },
                { KEY_EN_LEFT_SHIFT,          0x00 },
                { KEY_EN_LEFT_CONTROL,        0x01 },

                { KEY_EN_1,                   0x08 },
                { KEY_EN_ISO_BACK_SLASH,      0x06 },
                { KEY_EN_LEFT_WINDOWS,        0x07 },

                { KEY_EN_F1,                  0x0D },
                { KEY_EN_2,                   0x0E },
                { KEY_EN_Q,                   0x09 },
                { KEY_EN_A,                   0x0A },
                { KEY_EN_Z,                   0x0B },
                { KEY_EN_LEFT_ALT,            0x0C },

                { KEY_EN_F2,                  0x14 },
                { KEY_EN_3,                   0x15 },
                { KEY_EN_W,                   0x0F },
                { KEY_EN_S,                   0x10 },
                { KEY_EN_X,                   0x11 },

                { KEY_EN_F3,                  0x19 },
                { KEY_EN_4,                   0x1A },
                { KEY_EN_E,                   0x16 },
                { KEY_EN_D,                   0x17 },
                { KEY_EN_C,                   0x18 },

                { KEY_EN_F4,                  0x1E },
                { KEY_EN_5,                   0x1F },
                { KEY_EN_R,                   0x1B },
                { KEY_EN_F,                   0x1C },
                { KEY_EN_V,                   0x1D },

                { KEY_EN_6,                   0x24 },
                { KEY_EN_T,                   0x20 },
                { KEY_EN_G,                   0x21 },
                { KEY_EN_B,                   0x22 },
                { KEY_EN_SPACE,               0x23 },

                { KEY_EN_F5,                  0x28 },
                { KEY_EN_7,                   0x29 },
                { KEY_EN_Y,                   0x25 },
                { KEY_EN_H,                   0x26 },
                { KEY_EN_N,                   0x27 },

                { KEY_EN_F6,                  0x2F },
                { KEY_EN_8,                   0x30 },
                { KEY_EN_U,                   0x2A },
                { KEY_EN_J,                   0x2B },
                { KEY_EN_M,                   0x2C },

                { KEY_EN_F7,                  0x35 },
                { KEY_EN_9,                   0x36 },
                { KEY_EN_I,                   0x31 },
                { KEY_EN_K,                   0x32 },
                { KEY_EN_COMMA,               0x33 },

                { KEY_EN_F8,                  0x3B },
                { KEY_EN_0,                   0x3C },
                { KEY_EN_O,                   0x37 },
                { KEY_EN_L,                   0x38 },
                { KEY_EN_PERIOD,              0x39 },
                { KEY_EN_RIGHT_ALT,           0x3A },

                { KEY_EN_F9,                  0x41 },
                { KEY_EN_MINUS,               0x42 },
                { KEY_EN_P,                   0x3D },
                { KEY_EN_SEMICOLON,           0x3E },
                { KEY_EN_FORWARD_SLASH,       0x3F },
                { KEY_EN_RIGHT_FUNCTION,      0x40 },

                { KEY_EN_F10,                 0x47 },
                { KEY_EN_EQUALS,              0x48 },
                { KEY_EN_LEFT_BRACKET,        0x43 },
                { KEY_EN_QUOTE,               0x44 },
                { KEY_EN_MENU,                0x46 },

                { KEY_EN_F11,                 0x4D },
                { KEY_EN_BACKSPACE,           0x50 },
                { KEY_EN_RIGHT_BRACKET,       0x49 },
                { KEY_EN_POUND,               0x4A },
                { KEY_EN_RIGHT_SHIFT,         0x4B },

                { KEY_EN_F12,                 0x4F },
                { KEY_EN_ISO_ENTER,           0x52 },
                { KEY_EN_RIGHT_CONTROL,       0x4C },

                { KEY_EN_PRINT_SCREEN,        0x53 },
                { KEY_EN_INSERT,              0x54 },
                { KEY_EN_DELETE,              0x55 },
                { KEY_EN_LEFT_ARROW,          0x56 },

                { KEY_EN_SCROLL_LOCK,         0x57 },
                { KEY_EN_HOME,                0x58 },
                { KEY_EN_END,                 0x59 },
                { KEY_EN_UP_ARROW,            0x5A },
                { KEY_EN_DOWN_ARROW,          0x5B },

                { KEY_EN_PAUSE_BREAK,         0x5C },
                { KEY_EN_PAGE_UP,             0x5D },
                { KEY_EN_PAGE_DOWN,           0x5E },
                { KEY_EN_RIGHT_ARROW,         0x5F },

                { KEY_EN_NUMPAD_LOCK,         0x61 },
                { KEY_EN_NUMPAD_7,            0x62 },
                { KEY_EN_NUMPAD_4,            0x63 },
                { KEY_EN_NUMPAD_1,            0x64 },
                { KEY_EN_NUMPAD_0,            0x65 },

                { KEY_EN_NUMPAD_DIVIDE,       0x67 },
                { KEY_EN_NUMPAD_8,            0x68 },
                { KEY_EN_NUMPAD_5,            0x69 },
                { KEY_EN_NUMPAD_2,            0x6A },

                { KEY_EN_NUMPAD_TIMES,        0x6C },
                { KEY_EN_NUMPAD_9,            0x6D },
                { KEY_EN_NUMPAD_6,            0x6E },
                { KEY_EN_NUMPAD_3,            0x6F },
                { KEY_EN_NUMPAD_PERIOD,       0x70 },

                { KEY_EN_NUMPAD_MINUS,        0x72 },
                { KEY_EN_NUMPAD_PLUS,         0x73 },
                { KEY_EN_NUMPAD_ENTER,        0x75 }
            }
        }
    },
    {
        ROCCAT_VULCAN_LAYOUT_US,
        {
            *ROCCAT_VULCAN_120_AIMO_LAYOUT_KEYS_104,
            104,
            6,
            24,
            {
                { KEY_EN_ESCAPE,              0x02 },
                { KEY_EN_BACK_TICK,           0x03 },
                { KEY_EN_TAB,                 0x04 },
                { KEY_EN_CAPS_LOCK,           0x05 },
                { KEY_EN_LEFT_SHIFT,          0x00 },
                { KEY_EN_LEFT_CONTROL,        0x01 },

                { KEY_EN_1,                   0x08 },
                { KEY_EN_LEFT_WINDOWS,        0x07 },

                { KEY_EN_F1,                  0x0D },
                { KEY_EN_2,                   0x0E },
                { KEY_EN_Q,                   0x09 },
                { KEY_EN_A,                   0x0A },
                { KEY_EN_Z,                   0x0B },
                { KEY_EN_LEFT_ALT,            0x0C },

                { KEY_EN_F2,                  0x14 },
                { KEY_EN_3,                   0x15 },
                { KEY_EN_W,                   0x0F },
                { KEY_EN_S,                   0x10 },
                { KEY_EN_X,                   0x11 },

                { KEY_EN_F3,                  0x19 },
                { KEY_EN_4,                   0x1A },
                { KEY_EN_E,                   0x16 },
                { KEY_EN_D,                   0x17 },
                { KEY_EN_C,                   0x18 },

                { KEY_EN_F4,                  0x1E },
                { KEY_EN_5,                   0x1F },
                { KEY_EN_R,                   0x1B },
                { KEY_EN_F,                   0x1C },
                { KEY_EN_V,                   0x1D },

                { KEY_EN_6,                   0x24 },
                { KEY_EN_T,                   0x20 },
                { KEY_EN_G,                   0x21 },
                { KEY_EN_B,                   0x22 },
                { KEY_EN_SPACE,               0x23 },

                { KEY_EN_F5,                  0x28 },
                { KEY_EN_7,                   0x29 },
                { KEY_EN_Y,                   0x25 },
                { KEY_EN_H,                   0x26 },
                { KEY_EN_N,                   0x27 },

                { KEY_EN_F6,                  0x2F },
                { KEY_EN_8,                   0x30 },
                { KEY_EN_U,                   0x2A },
                { KEY_EN_J,                   0x2B },
                { KEY_EN_M,                   0x2C },

                { KEY_EN_F7,                  0x35 },
                { KEY_EN_9,                   0x36 },
                { KEY_EN_I,                   0x31 },
                { KEY_EN_K,                   0x32 },
                { KEY_EN_COMMA,               0x33 },

                { KEY_EN_F8,                  0x3B },
                { KEY_EN_0,                   0x3C },
                { KEY_EN_O,                   0x37 },
                { KEY_EN_L,                   0x38 },
                { KEY_EN_PERIOD,              0x39 },
                { KEY_EN_RIGHT_ALT,           0x3A },

                { KEY_EN_F9,                  0x41 },
                { KEY_EN_MINUS,               0x42 },
                { KEY_EN_P,                   0x3D },
                { KEY_EN_SEMICOLON,           0x3E },
                { KEY_EN_FORWARD_SLASH,       0x3F },
                { KEY_EN_RIGHT_FUNCTION,      0x40 },

                { KEY_EN_F10,                 0x47 },
                { KEY_EN_EQUALS,              0x48 },
                { KEY_EN_LEFT_BRACKET,        0x43 },
                { KEY_EN_QUOTE,               0x44 },
                { KEY_EN_MENU,                0x46 },

                { KEY_EN_F11,                 0x4D },
                { KEY_EN_BACKSPACE,           0x50 },
                { KEY_EN_RIGHT_BRACKET,       0x49 },
                { KEY_EN_RIGHT_SHIFT,         0x4B },

                { KEY_EN_F12,                 0x4F },
                { KEY_EN_ANSI_BACK_SLASH,     0x51 },
                { KEY_EN_ANSI_ENTER,          0x52 },
                { KEY_EN_RIGHT_CONTROL,       0x4C },

                { KEY_EN_PRINT_SCREEN,        0x53 },
                { KEY_EN_INSERT,              0x54 },
                { KEY_EN_DELETE,              0x55 },
                { KEY_EN_LEFT_ARROW,          0x56 },

                { KEY_EN_SCROLL_LOCK,         0x57 },
                { KEY_EN_HOME,                0x58 },
                { KEY_EN_END,                 0x59 },
                { KEY_EN_UP_ARROW,            0x5A },
                { KEY_EN_DOWN_ARROW,          0x5B },

                { KEY_EN_PAUSE_BREAK,         0x5C },
                { KEY_EN_PAGE_UP,             0x5D },
                { KEY_EN_PAGE_DOWN,           0x5E },
                { KEY_EN_RIGHT_ARROW,         0x5F },

                { KEY_EN_NUMPAD_LOCK,         0x61 },
                { KEY_EN_NUMPAD_7,            0x62 },
                { KEY_EN_NUMPAD_4,            0x63 },
                { KEY_EN_NUMPAD_1,            0x64 },
                { KEY_EN_NUMPAD_0,            0x65 },

                { KEY_EN_NUMPAD_DIVIDE,       0x67 },
                { KEY_EN_NUMPAD_8,            0x68 },
                { KEY_EN_NUMPAD_5,            0x69 },
                { KEY_EN_NUMPAD_2,            0x6A },

                { KEY_EN_NUMPAD_TIMES,        0x6C },
                { KEY_EN_NUMPAD_9,            0x6D },
                { KEY_EN_NUMPAD_6,            0x6E },
                { KEY_EN_NUMPAD_3,            0x6F },
                { KEY_EN_NUMPAD_PERIOD,       0x70 },

                { KEY_EN_NUMPAD_MINUS,        0x72 },
                { KEY_EN_NUMPAD_PLUS,         0x73 },
                { KEY_EN_NUMPAD_ENTER,        0x75 }
            }
        }
    },
};

static std::map<int, layout_info> RoccatVulcanIILayouts =
{
    {
        ROCCAT_VULCAN_LAYOUT_UK,
        {
            *ROCCAT_VULCAN_II_LAYOUT_KEYS_105,
            108,
            6,
            24,
            {
                { KEY_EN_ESCAPE,              0x02 },
                { KEY_EN_BACK_TICK,           0x03 },
                { KEY_EN_TAB,                 0x04 },
                { KEY_EN_CAPS_LOCK,           0x05 },
                { KEY_EN_LEFT_SHIFT,          0x00 },
                { KEY_EN_LEFT_CONTROL,        0x01 },

                { KEY_EN_1,                   0x08 },
                { KEY_EN_ISO_BACK_SLASH,      0x06 },
                { KEY_EN_LEFT_WINDOWS,        0x07 },

                { KEY_EN_F1,                  0x0D },
                { KEY_EN_2,                   0x0E },
                { KEY_EN_Q,                   0x09 },
                { KEY_EN_A,                   0x0A },
                { KEY_EN_Z,                   0x0B },
                { KEY_EN_LEFT_ALT,            0x0C },

                { KEY_EN_F2,                  0x14 },
                { KEY_EN_3,                   0x15 },
                { KEY_EN_W,                   0x0F },
                { KEY_EN_S,                   0x10 },
                { KEY_EN_X,                   0x11 },

                { KEY_EN_F3,                  0x19 },
                { KEY_EN_4,                   0x1A },
                { KEY_EN_E,                   0x16 },
                { KEY_EN_D,                   0x17 },
                { KEY_EN_C,                   0x18 },

                { KEY_EN_F4,                  0x1E },
                { KEY_EN_5,                   0x1F },
                { KEY_EN_R,                   0x1B },
                { KEY_EN_F,                   0x1C },
                { KEY_EN_V,                   0x1D },

                { KEY_EN_6,                   0x24 },
                { KEY_EN_T,                   0x20 },
                { KEY_EN_G,                   0x21 },
                { KEY_EN_B,                   0x22 },
                { KEY_EN_SPACE,               0x23 },

                { KEY_EN_F5,                  0x28 },
                { KEY_EN_7,                   0x29 },
                { KEY_EN_Y,                   0x25 },
                { KEY_EN_H,                   0x26 },
                { KEY_EN_N,                   0x27 },

                { KEY_EN_F6,                  0x2F },
                { KEY_EN_8,                   0x30 },
                { KEY_EN_U,                   0x2A },
                { KEY_EN_J,                   0x2B },
                { KEY_EN_M,                   0x2C },

                { KEY_EN_F7,                  0x35 },
                { KEY_EN_9,                   0x36 },
                { KEY_EN_I,                   0x31 },
                { KEY_EN_K,                   0x32 },
                { KEY_EN_COMMA,               0x33 },

                { KEY_EN_F8,                  0x3B },
                { KEY_EN_0,                   0x3C },
                { KEY_EN_O,                   0x37 },
                { KEY_EN_L,                   0x38 },
                { KEY_EN_PERIOD,              0x39 },
                { KEY_EN_RIGHT_ALT,           0x3A },

                { KEY_EN_F9,                  0x41 },
                { KEY_EN_MINUS,               0x42 },
                { KEY_EN_P,                   0x3D },
                { KEY_EN_SEMICOLON,           0x3E },
                { KEY_EN_FORWARD_SLASH,       0x3F },
                { KEY_EN_RIGHT_FUNCTION,      0x40 },

                { KEY_EN_F10,                 0x47 },
                { KEY_EN_EQUALS,              0x48 },
                { KEY_EN_LEFT_BRACKET,        0x43 },
                { KEY_EN_QUOTE,               0x44 },
                { KEY_EN_MENU,                0x46 },

                { KEY_EN_F11,                 0x4D },
                { KEY_EN_BACKSPACE,           0x50 },
                { KEY_EN_RIGHT_BRACKET,       0x49 },
                { KEY_EN_POUND,               0x4A },
                { KEY_EN_RIGHT_SHIFT,         0x4B },

                { KEY_EN_F12,                 0x4F },
                { KEY_EN_ISO_ENTER,           0x52 },
                { KEY_EN_RIGHT_CONTROL,       0x4C },

                { KEY_EN_PRINT_SCREEN,        0x53 },
                { KEY_EN_INSERT,              0x54 },
                { KEY_EN_DELETE,              0x55 },
                { KEY_EN_LEFT_ARROW,          0x56 },

                { KEY_EN_SCROLL_LOCK,         0x57 },
                { KEY_EN_HOME,                0x58 },
                { KEY_EN_END,                 0x59 },
                { KEY_EN_UP_ARROW,            0x5A },
                { KEY_EN_DOWN_ARROW,          0x5B },

                { KEY_EN_PAUSE_BREAK,         0x5C },
                { KEY_EN_PAGE_UP,             0x5D },
                { KEY_EN_PAGE_DOWN,           0x5E },
                { KEY_EN_RIGHT_ARROW,         0x5F },

                { KEY_EN_NUMPAD_LOCK,         0x61 },
                { KEY_EN_NUMPAD_7,            0x62 },
                { KEY_EN_NUMPAD_4,            0x63 },
                { KEY_EN_NUMPAD_1,            0x64 },
                { KEY_EN_NUMPAD_0,            0x65 },

                { KEY_EN_NUMPAD_DIVIDE,       0x67 },
                { KEY_EN_NUMPAD_8,            0x68 },
                { KEY_EN_NUMPAD_5,            0x69 },
                { KEY_EN_NUMPAD_2,            0x6A },

                { KEY_EN_NUMPAD_TIMES,        0x6C },
                { KEY_EN_NUMPAD_9,            0x6D },
                { KEY_EN_NUMPAD_6,            0x6E },
                { KEY_EN_NUMPAD_3,            0x6F },
                { KEY_EN_NUMPAD_PERIOD,       0x70 },

                { KEY_EN_NUMPAD_MINUS,        0x72 },
                { KEY_EN_NUMPAD_PLUS,         0x73 },
                { KEY_EN_NUMPAD_ENTER,        0x75 },
                { KEY_EN_MEDIA_PREVIOUS,      0x76 },
                { KEY_EN_MEDIA_PLAY_PAUSE,    0x78 },

                { KEY_EN_MEDIA_NEXT,          0x79 }
            }
        }
    },
};

static std::map<int, layout_info> RoccatVulcanTKLLayouts =
{
    {
        ROCCAT_VULCAN_LAYOUT_UK,
        {
            *ROCCAT_VULCAN_TKL_LAYOUT_KEYS_105,
            86,
            6,
            19,
            {
                { KEY_EN_ESCAPE,              0x02 },
                { KEY_EN_BACK_TICK,           0x03 },
                { KEY_EN_TAB,                 0x04 },
                { KEY_EN_CAPS_LOCK,           0x05 },
                { KEY_EN_LEFT_SHIFT,          0x00 },
                { KEY_EN_LEFT_CONTROL,        0x01 },

                { KEY_EN_1,                   0x08 },
                { KEY_EN_ISO_BACK_SLASH,      0x06 },
                { KEY_EN_LEFT_WINDOWS,        0x07 },

                { KEY_EN_F1,                  0x0D },
                { KEY_EN_2,                   0x0E },
                { KEY_EN_Q,                   0x09 },
                { KEY_EN_A,                   0x0A },
                { KEY_EN_Z,                   0x0B },
                { KEY_EN_LEFT_ALT,            0x0C },

                { KEY_EN_F2,                  0x14 },
                { KEY_EN_3,                   0x15 },
                { KEY_EN_W,                   0x0F },
                { KEY_EN_S,                   0x10 },
                { KEY_EN_X,                   0x11 },

                { KEY_EN_F3,                  0x19 },
                { KEY_EN_4,                   0x1A },
                { KEY_EN_E,                   0x16 },
                { KEY_EN_D,                   0x17 },
                { KEY_EN_C,                   0x18 },

                { KEY_EN_F4,                  0x1E },
                { KEY_EN_5,                   0x1F },
                { KEY_EN_R,                   0x1B },
                { KEY_EN_F,                   0x1C },
                { KEY_EN_V,                   0x1D },

                { KEY_EN_6,                   0x24 },
                { KEY_EN_T,                   0x20 },
                { KEY_EN_G,                   0x21 },
                { KEY_EN_B,                   0x22 },
                { KEY_EN_SPACE,               0x23 },

                { KEY_EN_F5,                  0x28 },
                { KEY_EN_7,                   0x29 },
                { KEY_EN_Y,                   0x25 },
                { KEY_EN_H,                   0x26 },
                { KEY_EN_N,                   0x27 },

                { KEY_EN_F6,                  0x2F },
                { KEY_EN_8,                   0x30 },
                { KEY_EN_U,                   0x2A },
                { KEY_EN_J,                   0x2B },
                { KEY_EN_M,                   0x2C },

                { KEY_EN_F7,                  0x35 },
                { KEY_EN_9,                   0x36 },
                { KEY_EN_I,                   0x31 },
                { KEY_EN_K,                   0x32 },
                { KEY_EN_COMMA,               0x33 },

                { KEY_EN_F8,                  0x3B },
                { KEY_EN_0,                   0x3C },
                { KEY_EN_O,                   0x37 },
                { KEY_EN_L,                   0x38 },
                { KEY_EN_PERIOD,              0x39 },
                { KEY_EN_RIGHT_ALT,           0x3A },

                { KEY_EN_F9,                  0x41 },
                { KEY_EN_MINUS,               0x42 },
                { KEY_EN_P,                   0x3D },
                { KEY_EN_SEMICOLON,           0x3E },
                { KEY_EN_FORWARD_SLASH,       0x3F },
                { KEY_EN_RIGHT_FUNCTION,      0x40 },

                { KEY_EN_F10,                 0x47 },
                { KEY_EN_EQUALS,              0x48 },
                { KEY_EN_LEFT_BRACKET,        0x43 },
                { KEY_EN_QUOTE,               0x44 },
                { KEY_EN_MENU,                0x46 },

                { KEY_EN_F11,                 0x4D },
                { KEY_EN_BACKSPACE,           0x50 },
                { KEY_EN_RIGHT_BRACKET,       0x49 },
                { KEY_EN_POUND,               0x4A },
                { KEY_EN_RIGHT_SHIFT,         0x4B },

                { KEY_EN_F12,                 0x4F },
                { KEY_EN_ISO_ENTER,           0x52 },
                { KEY_EN_RIGHT_CONTROL,       0x4C },

                { KEY_EN_MEDIA_MUTE,          0x5C },
                { KEY_EN_INSERT,              0x54 },
                { KEY_EN_DELETE,              0x55 },
                { KEY_EN_LEFT_ARROW,          0x56 },

                { KEY_EN_HOME,                0x58 },
                { KEY_EN_END,                 0x59 },
                { KEY_EN_UP_ARROW,            0x5A },
                { KEY_EN_DOWN_ARROW,          0x5B },

                { KEY_EN_PAGE_UP,             0x5D },
                { KEY_EN_PAGE_DOWN,           0x5E },
                { KEY_EN_RIGHT_ARROW,         0x5F },
            }
        }
    },
    {
        ROCCAT_VULCAN_LAYOUT_US,
        {
            *ROCCAT_VULCAN_TKL_LAYOUT_KEYS_104,
            85,
            6,
            19,
            {
                { KEY_EN_ESCAPE,              0x02 },
                { KEY_EN_BACK_TICK,           0x03 },
                { KEY_EN_TAB,                 0x04 },
                { KEY_EN_CAPS_LOCK,           0x05 },
                { KEY_EN_LEFT_SHIFT,          0x00 },
                { KEY_EN_LEFT_CONTROL,        0x01 },

                { KEY_EN_1,                   0x08 },
                { KEY_EN_LEFT_WINDOWS,        0x07 },

                { KEY_EN_F1,                  0x0D },
                { KEY_EN_2,                   0x0E },
                { KEY_EN_Q,                   0x09 },
                { KEY_EN_A,                   0x0A },
                { KEY_EN_Z,                   0x0B },
                { KEY_EN_LEFT_ALT,            0x0C },

                { KEY_EN_F2,                  0x14 },
                { KEY_EN_3,                   0x15 },
                { KEY_EN_W,                   0x0F },
                { KEY_EN_S,                   0x10 },
                { KEY_EN_X,                   0x11 },

                { KEY_EN_F3,                  0x19 },
                { KEY_EN_4,                   0x1A },
                { KEY_EN_E,                   0x16 },
                { KEY_EN_D,                   0x17 },
                { KEY_EN_C,                   0x18 },

                { KEY_EN_F4,                  0x1E },
                { KEY_EN_5,                   0x1F },
                { KEY_EN_R,                   0x1B },
                { KEY_EN_F,                   0x1C },
                { KEY_EN_V,                   0x1D },

                { KEY_EN_6,                   0x24 },
                { KEY_EN_T,                   0x20 },
                { KEY_EN_G,                   0x21 },
                { KEY_EN_B,                   0x22 },
                { KEY_EN_SPACE,               0x23 },

                { KEY_EN_F5,                  0x28 },
                { KEY_EN_7,                   0x29 },
                { KEY_EN_Y,                   0x25 },
                { KEY_EN_H,                   0x26 },
                { KEY_EN_N,                   0x27 },

                { KEY_EN_F6,                  0x2F },
                { KEY_EN_8,                   0x30 },
                { KEY_EN_U,                   0x2A },
                { KEY_EN_J,                   0x2B },
                { KEY_EN_M,                   0x2C },

                { KEY_EN_F7,                  0x35 },
                { KEY_EN_9,                   0x36 },
                { KEY_EN_I,                   0x31 },
                { KEY_EN_K,                   0x32 },
                { KEY_EN_COMMA,               0x33 },

                { KEY_EN_F8,                  0x3B },
                { KEY_EN_0,                   0x3C },
                { KEY_EN_O,                   0x37 },
                { KEY_EN_L,                   0x38 },
                { KEY_EN_PERIOD,              0x39 },
                { KEY_EN_RIGHT_ALT,           0x3A },

                { KEY_EN_F9,                  0x41 },
                { KEY_EN_MINUS,               0x42 },
                { KEY_EN_P,                   0x3D },
                { KEY_EN_SEMICOLON,           0x3E },
                { KEY_EN_FORWARD_SLASH,       0x3F },
                { KEY_EN_RIGHT_FUNCTION,      0x40 },

                { KEY_EN_F10,                 0x47 },
                { KEY_EN_EQUALS,              0x48 },
                { KEY_EN_LEFT_BRACKET,        0x43 },
                { KEY_EN_QUOTE,               0x44 },
                { KEY_EN_MENU,                0x46 },

                { KEY_EN_F11,                 0x4D },
                { KEY_EN_BACKSPACE,           0x50 },
                { KEY_EN_RIGHT_BRACKET,       0x49 },
                { KEY_EN_RIGHT_SHIFT,         0x4B },

                { KEY_EN_F12,                 0x4F },
                { KEY_EN_ANSI_BACK_SLASH,     0x51 }, // this one is guessed, not tested with ansi layout
                { KEY_EN_ANSI_ENTER,          0x52 },
                { KEY_EN_RIGHT_CONTROL,       0x4C },

                { KEY_EN_MEDIA_MUTE,          0x5C },
                { KEY_EN_INSERT,              0x54 },
                { KEY_EN_DELETE,              0x55 },
                { KEY_EN_LEFT_ARROW,          0x56 },

                { KEY_EN_HOME,                0x58 },
                { KEY_EN_END,                 0x59 },
                { KEY_EN_UP_ARROW,            0x5A },
                { KEY_EN_DOWN_ARROW,          0x5B },

                { KEY_EN_PAGE_UP,             0x5D },
                { KEY_EN_PAGE_DOWN,           0x5E },
                { KEY_EN_RIGHT_ARROW,         0x5F },
            }
        }
    },
};

static std::map<int, layout_info> RoccatMagmaLayouts =
{
    {
        ROCCAT_VULCAN_LAYOUT_US,
        {
            *ROCCAT_MAGMA_LAYOUT_KEYS,
            5,
            1,
            5,
            {
                { "Keyboard LED 1",             0x00 },
                { "Keyboard LED 2",             0x01 },
                { "Keyboard LED 3",             0x02 },
                { "Keyboard LED 4",             0x03 },
                { "Keyboard LED 5",             0x04 },
            }
        }
    },
};
