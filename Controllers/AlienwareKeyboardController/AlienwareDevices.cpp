/*---------------------------------------------------------*\
| AlienwareDevices.cpp                                      |
|                                                           |
|   Device table for Alienware keyboards                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "AlienwareDevices.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int alienware_aw410k_matrix_map[6][24] =
{ {  0,   NA,   1,   2,   3,   4,  NA,   5,   6,   7,   8,   9,  10,  11,  12, NA,  13,  14,  15,  NA,  NA,  16,  17,  18 },
  {  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  NA, NA,  33,  34,  35,  NA,  36,  37,  38,  39 },
  {  40,  NA,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53, NA,  54,  55,  56,  NA,  57,  58,  59,  60 },
  {  61,  NA,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  NA,  73, NA,  NA,  NA,  NA,  NA,  74,  75,  76,  NA },
  {  77,  NA,  NA,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  NA, NA,  NA,  89,  NA,  NA,  90,  91,  92,  93 },
  {  94,  NA,  95,  96,  NA,  NA,  NA,  97,  NA,  NA,  NA,  98,  99, 100, 101, NA, 102, 103, 104,  NA, 105,  NA, 106,  NA }};

static unsigned int alienware_aw510k_matrix_map[7][24] =
{ {  0,   NA,   1,   2,   3,   4,  NA,   5,   6,   7,   8,   9,  10,  11,  12,  NA,  13,  14,  15,  NA,  16,  NA,  NA,  NA },
  {  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  NA,  NA,  31,  32,  33,  NA,  34,  35,  36,  37 },
  {  38,  NA,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  NA,  52,  53,  54,  NA,  55,  56,  57,  74 },
  {  58,  NA,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  NA,  70,  NA,  NA,  NA,  NA,  NA,  71,  72,  73,  NA },
  {  75,  NA,  NA,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  NA,  NA,  NA,  87,  NA,  NA,  88,  89,  90, 104 },
  {  91,  NA,  92,  93,  NA,  94,  NA,  NA,  NA,  NA,  NA,  95,  96,  97,  98,  NA,  99, 100, 101,  NA, 102,  NA, 103,  NA },
  {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  105, NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA }};

static const alienware_led alienware_aw410k_leds[] =
{
    /* Key Label                Zone,                                      Index  */
    { KEY_EN_ESCAPE,            ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xB0    },
    { KEY_EN_F1,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x98    },
    { KEY_EN_F2,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x90    },
    { KEY_EN_F3,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x88    },
    { KEY_EN_F4,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x80    },
    { KEY_EN_F5,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x70    },
    { KEY_EN_F6,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x68    },
    { KEY_EN_F7,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x60    },
    { KEY_EN_F8,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x58    },
    { KEY_EN_F9,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x50    },
    { KEY_EN_F10,               ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x48    },
    { KEY_EN_F11,               ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x40    },
    { KEY_EN_F12,               ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x38    },
    { KEY_EN_PRINT_SCREEN,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x30    },
    { KEY_EN_SCROLL_LOCK,       ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x28    },
    { KEY_EN_PAUSE_BREAK,       ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x20    },
    { KEY_EN_MEDIA_MUTE,        ALIENWARE_ZONE_DIRECT_MEDIA,        0x18    },
    { KEY_EN_MEDIA_VOLUME_DOWN, ALIENWARE_ZONE_DIRECT_MEDIA,        0x10    },
    { KEY_EN_MEDIA_VOLUME_UP,   ALIENWARE_ZONE_DIRECT_MEDIA,        0x08    },
    { KEY_EN_BACK_TICK,         ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xB1    },
    { KEY_EN_1,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xA1    },
    { KEY_EN_2,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x99    },
    { KEY_EN_3,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x91    },
    { KEY_EN_4,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x89    },
    { KEY_EN_5,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x81    },
    { KEY_EN_6,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x79    },
    { KEY_EN_7,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x71    },
    { KEY_EN_8,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x69    },
    { KEY_EN_9,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x61    },
    { KEY_EN_0,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x59    },
    { KEY_EN_MINUS,             ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x51    },
    { KEY_EN_EQUALS,            ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x49    },
    { KEY_EN_BACKSPACE,         ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x39    },
    { KEY_EN_INSERT,            ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x31    },
    { KEY_EN_HOME,              ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x29    },
    { KEY_EN_PAGE_UP,           ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x21    },
    { KEY_EN_NUMPAD_LOCK,       ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x19    },
    { KEY_EN_NUMPAD_DIVIDE,     ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x11    },
    { KEY_EN_NUMPAD_TIMES,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x09    },
    { KEY_EN_NUMPAD_MINUS,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x01    },
    { KEY_EN_TAB,               ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xB2    },
    { KEY_EN_Q,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xA2    },
    { KEY_EN_W,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x9A    },
    { KEY_EN_E,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x92    },
    { KEY_EN_R,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x8A    },
    { KEY_EN_T,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x82    },
    { KEY_EN_Y,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x7A    },
    { KEY_EN_U,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x72    },
    { KEY_EN_I,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x6A    },
    { KEY_EN_O,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x62    },
    { KEY_EN_P,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x5A    },
    { KEY_EN_LEFT_BRACKET,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x52    },
    { KEY_EN_RIGHT_BRACKET,     ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x4A    },
    { KEY_EN_ANSI_BACK_SLASH,   ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x42    },
    { KEY_EN_DELETE,            ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x32    },
    { KEY_EN_END,               ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x2A    },
    { KEY_EN_PAGE_DOWN,         ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x22    },
    { KEY_EN_NUMPAD_7,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x1A    },
    { KEY_EN_NUMPAD_8,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x12    },
    { KEY_EN_NUMPAD_9,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x0A    },
    { KEY_EN_NUMPAD_PLUS,       ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x03    },
    { KEY_EN_CAPS_LOCK,         ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xB3    },
    { KEY_EN_A,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xA3    },
    { KEY_EN_S,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x9B    },
    { KEY_EN_D,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x93    },
    { KEY_EN_F,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x8B    },
    { KEY_EN_G,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x83    },
    { KEY_EN_H,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x7B    },
    { KEY_EN_J,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x73    },
    { KEY_EN_K,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x6B    },
    { KEY_EN_L,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x63    },
    { KEY_EN_SEMICOLON,         ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x5B    },
    { KEY_EN_QUOTE,             ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x53    },
    { KEY_EN_ANSI_ENTER,        ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x43    },
    { KEY_EN_NUMPAD_4,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x1B    },
    { KEY_EN_NUMPAD_5,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x13    },
    { KEY_EN_NUMPAD_6,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x0B    },
    { KEY_EN_LEFT_SHIFT,        ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xB4    },
    { KEY_EN_Z,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xA4    },
    { KEY_EN_X,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x9C    },
    { KEY_EN_C,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x94    },
    { KEY_EN_V,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x8C    },
    { KEY_EN_B,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x84    },
    { KEY_EN_N,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x7C    },
    { KEY_EN_M,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x74    },
    { KEY_EN_COMMA,             ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x6C    },
    { KEY_EN_PERIOD,            ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x64    },
    { KEY_EN_FORWARD_SLASH,     ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x5C    },
    { KEY_EN_RIGHT_SHIFT,       ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x4C    },
    { KEY_EN_UP_ARROW,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x2C    },
    { KEY_EN_NUMPAD_1,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x1C    },
    { KEY_EN_NUMPAD_2,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x14    },
    { KEY_EN_NUMPAD_3,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x0C    },
    { KEY_EN_NUMPAD_ENTER,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x05    },
    { KEY_EN_LEFT_CONTROL,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xB5    },
    { KEY_EN_LEFT_WINDOWS,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xAD    },
    { KEY_EN_LEFT_ALT,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xA5    },
    { KEY_EN_SPACE,             ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x85    },
    { KEY_EN_RIGHT_ALT,         ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x65    },
    { KEY_EN_RIGHT_FUNCTION,    ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x5D    },
    { KEY_EN_MENU,              ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x55    },
    { KEY_EN_RIGHT_CONTROL,     ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x4D    },
    { KEY_EN_LEFT_ARROW,        ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x35    },
    { KEY_EN_DOWN_ARROW,        ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x2D    },
    { KEY_EN_RIGHT_ARROW,       ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x25    },
    { KEY_EN_NUMPAD_0,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x1D    },
    { KEY_EN_NUMPAD_PERIOD,     ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x0D    }
};
static const alienware_led alienware_aw510k_leds[] =
{
    /* Key Label                Zone,                                      Index  */
    { KEY_EN_ESCAPE,            ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xB0    },
    { KEY_EN_F1,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x98    },
    { KEY_EN_F2,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x90    },
    { KEY_EN_F3,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x88    },
    { KEY_EN_F4,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x80    },
    { KEY_EN_F5,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x70    },
    { KEY_EN_F6,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x68    },
    { KEY_EN_F7,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x60    },
    { KEY_EN_F8,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x58    },
    { KEY_EN_F9,                ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x50    },
    { KEY_EN_F10,               ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x48    },
    { KEY_EN_F11,               ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x40    },
    { KEY_EN_F12,               ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x38    },
    { KEY_EN_PRINT_SCREEN,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x30    },
    { KEY_EN_SCROLL_LOCK,       ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x28    },
    { KEY_EN_PAUSE_BREAK,       ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x20    },
    { KEY_EN_MEDIA_MUTE,        ALIENWARE_ZONE_DIRECT_MEDIA,        0x18    },
    { KEY_EN_BACK_TICK,         ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xB1    },
    { KEY_EN_1,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xA1    },
    { KEY_EN_2,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x99    },
    { KEY_EN_3,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x91    },
    { KEY_EN_4,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x89    },
    { KEY_EN_5,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x81    },
    { KEY_EN_6,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x79    },
    { KEY_EN_7,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x71    },
    { KEY_EN_8,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x69    },
    { KEY_EN_9,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x61    },
    { KEY_EN_0,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x59    },
    { KEY_EN_MINUS,             ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x51    },
    { KEY_EN_EQUALS,            ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x49    },
    { KEY_EN_BACKSPACE,         ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x39    },
    { KEY_EN_INSERT,            ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x31    },
    { KEY_EN_HOME,              ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x29    },
    { KEY_EN_PAGE_UP,           ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x21    },
    { KEY_EN_NUMPAD_LOCK,       ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x19    },
    { KEY_EN_NUMPAD_DIVIDE,     ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x11    },
    { KEY_EN_NUMPAD_TIMES,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x09    },
    { KEY_EN_NUMPAD_MINUS,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x01    },
    { KEY_EN_TAB,               ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xB2    },
    { KEY_EN_Q,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xA2    },
    { KEY_EN_W,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x9A    },
    { KEY_EN_E,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x92    },
    { KEY_EN_R,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x8A    },
    { KEY_EN_T,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x82    },
    { KEY_EN_Y,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x7A    },
    { KEY_EN_U,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x72    },
    { KEY_EN_I,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x6A    },
    { KEY_EN_O,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x62    },
    { KEY_EN_P,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x5A    },
    { KEY_EN_LEFT_BRACKET,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x52    },
    { KEY_EN_RIGHT_BRACKET,     ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x4A    },
    { KEY_EN_ANSI_BACK_SLASH,   ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x42    },//ANSI only
    { KEY_EN_DELETE,            ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x32    },
    { KEY_EN_END,               ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x2A    },
    { KEY_EN_PAGE_DOWN,         ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x22    },
    { KEY_EN_NUMPAD_7,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x1A    },
    { KEY_EN_NUMPAD_8,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x12    },
    { KEY_EN_NUMPAD_9,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x0A    },
    { KEY_EN_CAPS_LOCK,         ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xB3    },
    { KEY_EN_A,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xA3    },
    { KEY_EN_S,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x9B    },
    { KEY_EN_D,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x93    },
    { KEY_EN_F,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x8B    },
    { KEY_EN_G,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x83    },
    { KEY_EN_H,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x7B    },
    { KEY_EN_J,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x73    },
    { KEY_EN_K,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x6B    },
    { KEY_EN_L,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x63    },
    { KEY_EN_SEMICOLON,         ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x5B    },
    { KEY_EN_QUOTE,             ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x53    },
    { KEY_EN_ANSI_ENTER,        ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x43    },
    { KEY_EN_NUMPAD_4,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x1B    },
    { KEY_EN_NUMPAD_5,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x13    },
    { KEY_EN_NUMPAD_6,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x0B    },
    { KEY_EN_NUMPAD_PLUS,       ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x03    },
    { KEY_EN_LEFT_SHIFT,        ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xB4    },
    { KEY_EN_Z,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xA4    },
    { KEY_EN_X,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x9C    },
    { KEY_EN_C,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x94    },
    { KEY_EN_V,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x8C    },
    { KEY_EN_B,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x84    },
    { KEY_EN_N,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x7C    },
    { KEY_EN_M,                 ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x74    },
    { KEY_EN_COMMA,             ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x6C    },
    { KEY_EN_PERIOD,            ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x64    },
    { KEY_EN_FORWARD_SLASH,     ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x5C    },
    { KEY_EN_RIGHT_SHIFT,       ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x4C    },
    { KEY_EN_UP_ARROW,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x2C    },
    { KEY_EN_NUMPAD_1,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x1C    },
    { KEY_EN_NUMPAD_2,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x14    },
    { KEY_EN_NUMPAD_3,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x0C    },
    { KEY_EN_LEFT_CONTROL,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xB5    },
    { KEY_EN_LEFT_WINDOWS,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xAD    },
    { KEY_EN_LEFT_ALT,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0xA5    },
    { KEY_EN_SPACE,             ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x85    },
    { KEY_EN_RIGHT_ALT,         ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x65    },
    { KEY_EN_RIGHT_FUNCTION,    ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x5D    },
    { KEY_EN_MENU,              ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x55    },
    { KEY_EN_RIGHT_CONTROL,     ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x4D    },
    { KEY_EN_LEFT_ARROW,        ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x35    },
    { KEY_EN_DOWN_ARROW,        ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x2D    },
    { KEY_EN_RIGHT_ARROW,       ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x25    },
    { KEY_EN_NUMPAD_0,          ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x1D    },
    { KEY_EN_NUMPAD_PERIOD,     ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x0D    },
    { KEY_EN_NUMPAD_ENTER,      ALIENWARE_ZONE_DIRECT_KEYBOARD,     0x05    },
    { "Logo",                   ALIENWARE_ZONE_DIRECT_LOGO,         0x07    }
};

/*---------------------------------------------------------*\
| Device table                                              |
\*---------------------------------------------------------*/

static const alienware_device alienware_aw410k_device =
{
    "Alienware AW410K",
    ALIENWARE_AW410K_PID,
    6,
    24,
    alienware_aw410k_matrix_map,
    107,
    "AW410K",
    alienware_aw410k_leds,
    107,
    false
};

static const alienware_device alienware_aw510k_device =
{
    "Alienware AW510K",
    ALIENWARE_AW510K_PID,
    7,
    24,
    alienware_aw510k_matrix_map,
    106,
    "AW510K",
    alienware_aw510k_leds,
    106,
    true
};

const unsigned int ALIENWARE_NUM_DEVICES = 2;
const alienware_device* alienware_device_list[] =
{
    &alienware_aw410k_device,
    &alienware_aw510k_device,
};

const alienware_device* GetAlienwareDevice(unsigned short pid)
{
    for(unsigned int i = 0; i < ALIENWARE_NUM_DEVICES; i++)
    {
        if(alienware_device_list[i]->pid == pid)
        {
            return alienware_device_list[i];
        }
    }
    return nullptr;
}
