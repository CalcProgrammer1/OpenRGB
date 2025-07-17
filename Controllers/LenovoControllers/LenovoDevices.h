/*---------------------------------------------------------*\
| LenovoDevices.h                                           |
|                                                           |
|   Device list for Lenovo USB devices                      |
|                                                           |
|   Cooper Hall (geobot19)                      17 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "RGBControllerKeyNames.h"
#include "RGBController.h"

#define NA 0xFFFFFFFF

/*-------------------------------------------------------------------*\
| Note: additions here must be adeed to RGBController_LenovoUSB.cpp in|
| the switch statements which are on lines 28 and 60 at time of       |
| writing                                                             |
\*-------------------------------------------------------------------*/

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define LEGION_Y740                             0xC935
#define LEGION_Y750                             0xC956
#define LEGION_Y750S                            0xC957
#define LEGION_Y760                             0xC968
#define LEGION_Y760S                            0xC967
#define LEGION_S7GEN7                           0xC977
#define LEGION_7GEN7                            0xC978
#define LEGION_7GEN8                            0xC988
#define LEGION_7GEN9                            0xC997
#define LEGION_7GEN9_H                          0xC998

enum LENOVO_KEYBOARD
{
    ISO,
    ANSI,
    JAPAN
};

enum LENOVO_SIZE
{
    SEVENTEEN,
    FIFTEEN,
    UNKNOWN
};

struct lenovo_led
{
    uint8_t led_num;
    std::string name;
};

struct lenovo_zone
{
    std::string name;
    zone_type type;
    unsigned char id;
    unsigned int height;
    unsigned int width;
    const unsigned int* matrix_map;
    const lenovo_led* leds;
    unsigned int start; //index to start reading the list of leds
    unsigned int end; //end index
};

/*---------*\
| LED MAPS  |
\*---------*/

static const unsigned int legion_Y760_ansi_leds_map[] =
    {    0,  NA,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
        20,  NA,  21,  22,  23,  24,  25,  26,  27,  28,  29,  NA,  30,  31,  32,  NA,  52,  33,  34,  35,  36,
        37,  NA,  38,  39,  40,  41,  42,  43,  44,  45,  46,  NA,  47,  48,  49,  50,  51,  53,  NA,  NA,  NA,
        54,  NA,  55,  56,  57,  NA,  58,  59,  60,  61,  62,  63,  64,  65,  66,  NA,  67,  68,  69,  70,  NA,
        72,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  90,  91,  NA,  92,  93,  94,  95,  71,
        73,  74,  75,  NA,  76,  77,  78,  79,  80,  81,  82,  83,  84,  NA,  86,  88,  89, 111, 113, 114,  96,
        NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  85,  NA,  87,  NA,  NA, 112,  NA, 115,  NA,
        97,  98,  NA,  99, 100, 101,  NA, 102, 103, 104, 105, 106, 107, 108,  NA, 109, 110, 119, 121, 123,  NA,
        NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 116, 117, 118,  NA,  NA,  NA, 120, 122, 124, 125,
       126,  NA, 127, 128, 129, 130,  NA,  NA,  NA,  NA, 131, 132, 133,  NA, 135,  NA,  NA, 140,  NA, 142, 144,
        NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 136,  NA,  NA,  NA, 141, 143,  NA,
        NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 134, 137,  NA, 139,  NA,  NA,  NA,  NA,
        NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 138,  NA,  NA,  NA,  NA,  NA,  NA };

static const unsigned int legion_Y760_iso_leds_map[] =
    {   0,  NA,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
       20,  NA,  21,  22,  23,  24,  25,  26,  27,  28,  29,  NA,  30,  31,  32,  NA,  NA,  33,  34,  35,  36,
       37,  NA,  38,  39,  40,  41,  42,  43,  44,  45,  46,  NA,  47,  48,  49,  NA,  50,  51,  NA,  NA,  NA,
       52,  NA,  53,  54,  55,  NA,  56,  57,  58,  59,  60,  61,  62,  63,  64,  NA,  65,  66,  67,  68,  NA,
       70,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  88,  89,  NA,  NA,  90,  91,  92,  69,
       71,  72,  73,  NA,  74,  75,  76,  77,  78,  79,  80,  81,  82,  NA,  84,  86,  NA, 109, 111, 112,  93,
       NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  83,  NA,  85,  87,  NA, 110,  NA, 113,  NA,
       94,  NA,  95,  97,  98,  99,  NA, 100, 101, 102, 103, 104, 105, 106,  NA, 107, 108, 117, 119, 121,  NA,
       NA,  NA,  96,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 114, 115, 116,  NA,  NA,  NA, 118, 120, 122, 123,
       124, NA, 125, 126, 127, 128,  NA,  NA,  NA,  NA, 129, 130, 131,  NA, 133,  NA,  NA, 138,  NA, 140, 142,
       NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 134,  NA,  NA,  NA, 139, 141,  NA,
       NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 132, 135,  NA, 137,  NA,  NA,  NA,  NA,
       NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 136,  NA,  NA,  NA,  NA,  NA,  NA };

static const unsigned int legion_Y760_jp_leds_map[] =
    {   0,  NA,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
       20,  NA,  21,  22,  23,  24,  25,  26,  27,  28,  29,  NA,  30,  31,  32,  33,  53,  34,  35,  36,  37,
       38,  NA,  39,  40,  41,  42,  43,  44,  45,  46,  47,  NA,  48,  49,  50,  51,  52,  54,  NA,  NA,  NA,
       55,  NA,  56,  57,  58,  NA,  59,  60,  61,  62,  63,  64,  65,  66,  68,  NA,  70,  71,  72,  73,  NA,
       75,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  67,  69,  NA,  93,  94,  95,  96,  74,
       76,  77,  78,  NA,  79,  80,  81,  82,  83,  84,  85,  86,  87,  NA,  89,  91,  NA, 116, 118, 119,  97,
       NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  88,  NA,  90,  92,  NA, 117,  NA, 120,  NA,
       98,  99,  NA, 100, 101, 102,  NA, 103, 104, 105, 106, 107, 109, 111, 113, 114, 115, 121, 123, 125,  NA,
       NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 108, 110, 112,  NA,  NA,  NA, 122, 124, 126, 127,
      128,  NA, 129, 130, 131, 132, 133,  NA,  NA, 134, 135, 136, 137,  NA, 139,  NA,  NA, 144,  NA, 146, 148,
       NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 140,  NA,  NA,  NA, 145, 147,  NA,
       NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 138, 141,  NA, 143,  NA,  NA,  NA,  NA,
       NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 142,  NA,  NA,  NA,  NA,  NA,  NA };

static const unsigned int legion_Y740_17_ansi_leds_map[] =
    {   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  NA,  14,  15,  16,  17,
       18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
       37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  NA,  51,  52,  53,  54,
       55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,
       74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  NA,  89,  NA,  88,  NA,
       90,  91,  92,  93,  94,  NA,  NA,  95,  NA,  96,  97,  98,  99, 100,  NA,  NA, 101, 102, 103 };

static const unsigned int legion_Y740_17_iso_leds_map[] =
    {   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  NA,  14,  15,  16,  17,
       18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
       37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  NA,  51,  52,  53,  54,
       55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  NA,  NA,  69,  70,  71,  72,
       73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  NA,  88,  NA,  87,  NA,
       89,  90,  91,  92,  93,  NA,  NA,  94,  NA,  95,  96,  97,  98,  99,  NA,  NA, 100, 101, 102 };

static const unsigned int legion_Y740_15_ansi_leds_map[] =
    {  84,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  NA,
       85,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,
       86,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  NA,
       87,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,
       88,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  NA,
       89,  72,  73,  74,  75,  76,  NA,  NA,  NA,  77,  78,  79,  NA,  80,  NA,  NA,
       NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  81,  82,  83,  NA };

static const unsigned int legion_Y740_15_iso_leds_map[] =
    {  83,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  NA,
       84,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,
       85,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  NA,
       86,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  NA,
       87,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  NA,
       88,  71,  72,  73,  74,  75,  NA,  NA,  NA,  76,  77,  78,  NA,  79,  NA,  NA,
       NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  80,  81,  82,  NA };

/*---------------------*\
| zone 1, keyboard ANSI |
\*---------------------*/

const lenovo_led legion_Y760_ansi_leds[]
{
    //row 1
    {0x01, KEY_EN_ESCAPE},//0
    {0x02, KEY_EN_F1},//1
    {0x03, KEY_EN_F2},//2
    {0x04, KEY_EN_F3},//3
    {0x05, KEY_EN_F4},//4
    {0x06, KEY_EN_F5},//5
    {0x07, KEY_EN_F6},//6
    {0x08, KEY_EN_F7},//7
    {0x09, KEY_EN_F8},//8
    {0x0A, KEY_EN_F9},//9
    {0x0B, KEY_EN_F10},//10
    {0x0C, KEY_EN_F11},//11
    {0x0D, KEY_EN_F12},//12
    {0x0E, KEY_EN_INSERT},//13
    {0x0F, KEY_EN_PRINT_SCREEN},//14
    {0x10, KEY_EN_DELETE},//15
    {0x11, KEY_EN_HOME},//16
    {0x12, KEY_EN_END},//17
    {0x13, KEY_EN_PAGE_UP},//18
    {0x14, KEY_EN_PAGE_DOWN},//19

    //row 2
    {0x16, "Key: ~"},//20
    {0x17, "Key: !"},//21
    {0x18, "Key: @"},//22
    {0x19, KEY_EN_POUND},//23
    {0x1A, "Key: $"},//24
    {0x1B, "Key: %"},//25
    {0x1C, "Key: ^"},//26
    {0x1D, "Key: &"},//27
    {0x1E, "Key: *"},//28
    {0x1F, "Key: ("},//29
    {0x20, "Key: )"},//30
    {0x21, "Key: _"},//31
    {0x22, "Key: +"},//32
    {0x26, KEY_EN_NUMPAD_LOCK},//33
    {0x27, KEY_EN_NUMPAD_DIVIDE},//34
    {0x28, KEY_EN_NUMPAD_TIMES},//35
    {0x29, KEY_EN_NUMPAD_MINUS},//36

    //row 3
    {0x2B, KEY_EN_BACK_TICK},//37
    {0x2C, KEY_EN_1},//38
    {0x2D, KEY_EN_2},//39
    {0x2E, KEY_EN_3},//40
    {0x2F, KEY_EN_4},//41
    {0x30, KEY_EN_5},//42
    {0x31, KEY_EN_6},//43
    {0x32, KEY_EN_7},//44
    {0x33, KEY_EN_8},//45
    {0x34, KEY_EN_9},//46
    {0x35, KEY_EN_0},//47
    {0x36, KEY_EN_MINUS},//48
    {0x37, KEY_EN_EQUALS},//49
    {0x38, KEY_EN_BACKSPACE},//50
    {0x39, KEY_EN_BACKSPACE},//51
    {0x3A, KEY_EN_BACKSPACE},//52
    {0x3B, KEY_EN_NUMPAD_LOCK},//53

    //row 4
    {0x40, KEY_EN_TAB},//54
    {0x42, KEY_EN_Q},//55
    {0x43, KEY_EN_W},//56
    {0x44, KEY_EN_E},//57
    {0x45, KEY_EN_R},//58
    {0x46, KEY_EN_T},//59
    {0x47, KEY_EN_Y},//60
    {0x48, KEY_EN_U},//61
    {0x49, KEY_EN_I},//62
    {0x4A, KEY_EN_O},//63
    {0x4B, KEY_EN_P},//64
    {0x4C, "Key: {"},//65
    {0x4D, "Key: }"},//66
    {0x4E, "Key: |"},//67
    {0x4F, KEY_EN_NUMPAD_7},//68
    {0x50, KEY_EN_NUMPAD_8},//69
    {0x51, KEY_EN_NUMPAD_9},//70
    {0x67, KEY_EN_NUMPAD_PLUS},//71

    //row 5
    {0x55, KEY_EN_CAPS_LOCK},//72
    {0x56, KEY_EN_CAPS_LOCK},//73
    {0x57, KEY_EN_CAPS_LOCK},//74
    {0x6D, KEY_EN_A},//75
    {0x6E, KEY_EN_S},//76
    {0x58, KEY_EN_D},//77
    {0x59, KEY_EN_F},//78
    {0x5A, KEY_EN_G},//79
    {0x71, KEY_EN_H},//80
    {0x72, KEY_EN_J},//81
    {0x5B, KEY_EN_K},//82
    {0x5C, KEY_EN_L},//83
    {0x5D, "Key: :"},//84
    {0x5E, KEY_EN_SEMICOLON},//85
    {0x5F, "Key: \""},//86
    {0x60, KEY_EN_QUOTE},//87
    {0x77, KEY_EN_ANSI_ENTER},//88
    {0x78, KEY_EN_ANSI_ENTER},//89
    {0x61, KEY_EN_LEFT_BRACKET},//90
    {0x62, KEY_EN_RIGHT_BRACKET},//91
    {0x63, KEY_EN_ANSI_BACK_SLASH},//92
    {0x64, "Key: Number Pad Home"},//93
    {0x65, "Key: Number Pad Up Arrow"},//94
    {0x66, "Key: Number Pad Page Up"},//95
    {0x68, KEY_EN_NUMPAD_PLUS},//96

    //row 6
    {0x6A, KEY_EN_LEFT_SHIFT},//97
    {0x6B, KEY_EN_LEFT_SHIFT},//98
    {0x82, KEY_EN_Z},//99
    {0x83, KEY_EN_X},//100
    {0x6F, KEY_EN_C},//101
    {0x70, KEY_EN_V},//102
    {0x87, KEY_EN_B},//103
    {0x88, KEY_EN_N},//104
    {0x73, KEY_EN_M},//105
    {0x74, "Key: <"},//106
    {0x75, "Key: >"},//107
    {0x76, "Key: ?"},//108
    {0x8D, KEY_EN_RIGHT_SHIFT},//109
    {0xA2, KEY_EN_RIGHT_SHIFT},//110
    {0x79, KEY_EN_NUMPAD_4},//111
    {0x7A, "Key: Number Pad Right Arrow"},//112
    {0x7B, KEY_EN_NUMPAD_5},//113
    {0x7C, KEY_EN_NUMPAD_6},//114
    {0x7D, "Key: Number Pad Left Arrow"},//115

    //row 7
    {0x89, KEY_EN_COMMA},//116
    {0x8A, KEY_EN_PERIOD},//117
    {0x8B, KEY_EN_FORWARD_SLASH},//118
    {0x8E, KEY_EN_NUMPAD_1},//119
    {0x8F, "Key: Number Pad End"},//120
    {0x90, KEY_EN_NUMPAD_2},//121
    {0x91, "Key: Number Pad Down Arrow"},//122
    {0x92, KEY_EN_NUMPAD_3},//123
    {0x93, "Key: Number Pad Page Down"},//124
    {0xA7, KEY_EN_NUMPAD_ENTER},//125

    //row 8
    {0x7F, KEY_EN_LEFT_CONTROL},//126
    {0x80, KEY_EN_LEFT_FUNCTION},//127
    {0x96, KEY_EN_LEFT_WINDOWS},//128
    {0x97, KEY_EN_LEFT_ALT},//129
    {0x98, KEY_EN_SPACE},//130
    {0x99, KEY_EN_SPACE},//131
    {0x9A, KEY_EN_RIGHT_ALT},//132
    {0x9B, KEY_EN_RIGHT_CONTROL},//133
    {0x9C, KEY_EN_LEFT_ARROW},//134
    {0x9D, KEY_EN_UP_ARROW},//135
    {0x9E, "Key: Brightness +"},//136
    {0x9F, KEY_EN_DOWN_ARROW},//137
    {0xA0, "Key: Brightness -"},//138
    {0xA1, KEY_EN_RIGHT_ARROW},//139
    {0xA3, KEY_EN_NUMPAD_0},//140
    {0xA4, "Key: Number Pad Insert"},//141
    {0xA5, KEY_EN_NUMPAD_PERIOD},//142
    {0xA6, "Key: Number Pad Delete"},//143
    {0xA8, KEY_EN_NUMPAD_ENTER},//144
};

/*--------------------*\
| zone 1, keyboard ISO |
\*--------------------*/

const lenovo_led legion_Y760_iso_leds[]
{
    //row 1
    {0x01, KEY_EN_ESCAPE},//0
    {0x02, KEY_EN_F1},//1
    {0x03, KEY_EN_F2},//2
    {0x04, KEY_EN_F3},//3
    {0x05, KEY_EN_F4},//4
    {0x06, KEY_EN_F5},//5
    {0x07, KEY_EN_F6},//6
    {0x08, KEY_EN_F7},//7
    {0x09, KEY_EN_F8},//8
    {0x0A, KEY_EN_F9},//9
    {0x0B, KEY_EN_F10},//10
    {0x0C, KEY_EN_F11},//11
    {0x0D, KEY_EN_F12},//12
    {0x0E, KEY_EN_INSERT},//13
    {0x0F, KEY_EN_PRINT_SCREEN},//14
    {0x10, KEY_EN_DELETE},//15
    {0x11, KEY_EN_HOME},//16
    {0x12, KEY_EN_END},//17
    {0x13, KEY_EN_PAGE_UP},//18
    {0x14, KEY_EN_PAGE_DOWN},//19

    //row 2
    {0x16, "Key: ¬"},//20
    {0x17, "Key: !"},//21
    {0x18, "Key: \""},//22
    {0x19, "Key: £"},//23
    {0x1A, "Key: $"},//24
    {0x1B, "Key: %"},//25
    {0x1C, "Key: ^"},//26
    {0x1D, "Key: &"},//27
    {0x1E, "Key: *"},//28
    {0x1F, "Key: ("},//29
    {0x20, "Key: )"},//30
    {0x21, "Key: _"},//31
    {0x22, "Key: +"},//32
    {0x26, KEY_EN_NUMPAD_LOCK},//33
    {0x27, KEY_EN_NUMPAD_DIVIDE},//34
    {0x28, KEY_EN_NUMPAD_TIMES},//35
    {0x29, KEY_EN_NUMPAD_MINUS},//36

    //row 3
    {0x2B, KEY_EN_BACK_TICK},//37
    {0x2C, KEY_EN_1},//38
    {0x2D, KEY_EN_2},//39
    {0x2E, KEY_EN_3},//40
    {0x2F, KEY_EN_4},//41
    {0x30, KEY_EN_5},//42
    {0x31, KEY_EN_6},//43
    {0x32, KEY_EN_7},//44
    {0x33, KEY_EN_8},//45
    {0x34, KEY_EN_9},//46
    {0x35, KEY_EN_0},//47
    {0x36, KEY_EN_MINUS},//48
    {0x37, KEY_EN_EQUALS},//49
    {0x3A, KEY_EN_BACKSPACE},//50
    {0x3B, KEY_EN_NUMPAD_LOCK},//51

    //row 4
    {0x40, KEY_EN_TAB},//52
    {0x42, KEY_EN_Q},//53
    {0x43, KEY_EN_W},//54
    {0x44, KEY_EN_E},//55
    {0x45, KEY_EN_R},//56
    {0x46, KEY_EN_T},//57
    {0x47, KEY_EN_Y},//58
    {0x48, KEY_EN_U},//59
    {0x49, KEY_EN_I},//60
    {0x4A, KEY_EN_O},//61
    {0x4B, KEY_EN_P},//62
    {0x4C, "Key: {"},//63
    {0x4D, "Key: }"},//64
    {0x4E, KEY_EN_ISO_ENTER},//65
    {0x4F, KEY_EN_NUMPAD_7},//66
    {0x50, KEY_EN_NUMPAD_8},//67
    {0x51, KEY_EN_NUMPAD_9},//68
    {0x67, KEY_EN_NUMPAD_PLUS},//69

    //row 5
    {0x55, KEY_EN_CAPS_LOCK},//70
    {0x56, KEY_EN_CAPS_LOCK},//71
    {0x57, KEY_EN_CAPS_LOCK},//72
    {0x6D, KEY_EN_A},//73
    {0x6E, KEY_EN_S},//74
    {0x58, KEY_EN_D},//75
    {0x59, KEY_EN_F},//76
    {0x5A, KEY_EN_G},//77
    {0x71, KEY_EN_H},//78
    {0x72, KEY_EN_J},//79
    {0x5B, KEY_EN_K},//80
    {0x5C, KEY_EN_L},//81
    {0x5D, "Key: :"},//82
    {0x5E, KEY_EN_SEMICOLON},//83
    {0x5F, "Key: @"},//84
    {0x60, KEY_EN_QUOTE},//85
    {0x77, "Key: ~"},//86
    {0x78, KEY_EN_POUND},//87
    {0x61, KEY_EN_LEFT_BRACKET},//88
    {0x62, KEY_EN_RIGHT_BRACKET},//89
    {0x64, "Key: Number Pad Home"},//90
    {0x65, "Key: Number Pad Up Arrow"},//91
    {0x66, "Key: Number Pad Page Up"},//92
    {0x68, KEY_EN_NUMPAD_PLUS},//93

    //row 6
    {0x6A, KEY_EN_LEFT_SHIFT},//94
    {0x6C, "Key: |"},//95
    {0x81, KEY_EN_ISO_BACK_SLASH},//96
    {0x82, KEY_EN_Z},//97
    {0x83, KEY_EN_X},//98
    {0x6F, KEY_EN_C},//99
    {0x70, KEY_EN_V},//100
    {0x87, KEY_EN_B},//101
    {0x88, KEY_EN_N},//102
    {0x73, KEY_EN_M},//103
    {0x74, "Key: <"},//104
    {0x75, "Key: >"},//105
    {0x76, "Key: ?"},//106
    {0x8D, KEY_EN_RIGHT_SHIFT},//107
    {0xA2, KEY_EN_RIGHT_SHIFT},//108
    {0x79, KEY_EN_NUMPAD_4},//109
    {0x7A, "Key: Number Pad Right Arrow"},//110
    {0x7B, KEY_EN_NUMPAD_5},//111
    {0x7C, KEY_EN_NUMPAD_6},//112
    {0x7D, "Key: Number Pad Left Arrow"},//113

    //row 7
    {0x89, KEY_EN_COMMA},//114
    {0x8A, KEY_EN_PERIOD},//115
    {0x8B, KEY_EN_FORWARD_SLASH},//116
    {0x8E, KEY_EN_NUMPAD_1},//117
    {0x8F, "Key: Number Pad End"},//118
    {0x90, KEY_EN_NUMPAD_2},//119
    {0x91, "Key: Number Pad Down Arrow"},//120
    {0x92, KEY_EN_NUMPAD_3},//121
    {0x93, "Key: Number Pad Page Down"},//122
    {0xA7, KEY_EN_NUMPAD_ENTER},//123

    //row 8
    {0x7F, KEY_EN_LEFT_CONTROL},//124
    {0x80, KEY_EN_LEFT_FUNCTION},//125
    {0x96, KEY_EN_LEFT_WINDOWS},//126
    {0x97, KEY_EN_LEFT_ALT},//127
    {0x98, KEY_EN_SPACE},//128
    {0x99, KEY_EN_SPACE},//129
    {0x9A, KEY_EN_RIGHT_ALT},//130
    {0x9B, KEY_EN_RIGHT_CONTROL},//131
    {0x9C, KEY_EN_LEFT_ARROW},//132
    {0x9D, KEY_EN_UP_ARROW},//133
    {0x9E, "Key: Brightness +"},//134
    {0x9F, KEY_EN_DOWN_ARROW},//135
    {0xA0, "Key: Brightness -"},//136
    {0xA1, KEY_EN_RIGHT_ARROW},//137
    {0xA3, KEY_EN_NUMPAD_0},//138
    {0xA4, "Key: Number Pad Insert"},//139
    {0xA5, KEY_EN_NUMPAD_PERIOD},//140
    {0xA6, "Key: Number Pad Delete"},//141
    {0xA8, KEY_EN_NUMPAD_ENTER},//142
};

/*----------------------*\
| zone 1, keyboard Japan |
\*----------------------*/

const lenovo_led legion_Y760_jp_leds[]
{
    //row 1
    {0x01, KEY_EN_ESCAPE}, //0
    {0x02, KEY_EN_F1}, //1
    {0x03, KEY_EN_F2}, //2
    {0x04, KEY_EN_F3}, //3
    {0x05, KEY_EN_F4}, //4
    {0x06, KEY_EN_F5}, //5
    {0x07, KEY_EN_F6}, //6
    {0x08, KEY_EN_F7}, //7
    {0x09, KEY_EN_F8}, //8
    {0x0A, KEY_EN_F9}, //9
    {0x0B, KEY_EN_F10}, //10
    {0x0C, KEY_EN_F11}, //11
    {0x0D, KEY_EN_F12}, //12
    {0x0E, KEY_EN_INSERT}, //13
    {0x0F, KEY_EN_PRINT_SCREEN}, //14
    {0x10, KEY_EN_DELETE}, //15
    {0x11, KEY_EN_HOME}, //16
    {0x12, KEY_EN_END}, //17
    {0x13, KEY_EN_PAGE_UP}, //18
    {0x14, KEY_EN_PAGE_DOWN}, //19

    //row 2
    {0x16, KEY_JP_ZENKAKU}, //20
    {0x17, "Key: !"}, //21
    {0x18, "Key: \""}, //22
    {0x19, KEY_EN_POUND}, //23
    {0x1A, "Key: $"}, //24
    {0x1B, "Key: %"}, //25
    {0x1C, "Key: &"}, //26
    {0x1D, "Key: '"}, //27
    {0x1E, "Key: ("}, //28
    {0x1F, "Key: )"}, //29
    {0x20, "Key: wo"}, //30
    {0x21, KEY_EN_EQUALS}, //31
    {0x22, "Key: ~"}, //32
    {0x23, "Key: |"}, //33
    {0x26, KEY_EN_NUMPAD_LOCK}, //34
    {0x27, KEY_EN_NUMPAD_DIVIDE}, //35
    {0x28, KEY_EN_NUMPAD_TIMES}, //36
    {0x29, KEY_EN_NUMPAD_MINUS}, //37

    //row 3
    {0x2B, "Key: kanji"}, //38
    {0x2C, KEY_EN_1}, //39
    {0x2D, KEY_EN_2}, //40
    {0x2E, KEY_EN_3}, //41
    {0x2F, KEY_EN_4}, //42
    {0x30, KEY_EN_5}, //43
    {0x31, KEY_EN_6}, //44
    {0x32, KEY_EN_7}, //45
    {0x33, KEY_EN_8}, //46
    {0x34, KEY_EN_9}, //47
    {0x35, KEY_EN_0}, //48
    {0x36, KEY_EN_MINUS}, //49
    {0x37, KEY_JP_CHEVRON}, //50
    {0x38, KEY_JP_YEN}, //51
    {0x25, KEY_EN_BACKSPACE}, //52
    {0x3A, KEY_EN_BACKSPACE}, //53
    {0x3B, KEY_EN_NUMPAD_LOCK}, //54

    //row 4
    {0x40, KEY_EN_TAB}, //55
    {0x42, KEY_EN_Q}, //56
    {0x43, KEY_EN_W}, //57
    {0x44, KEY_EN_E}, //58
    {0x45, KEY_EN_R}, //59
    {0x46, KEY_EN_T}, //60
    {0x47, KEY_EN_Y}, //61
    {0x48, KEY_EN_U}, //62
    {0x49, KEY_EN_I}, //63
    {0x4A, KEY_EN_O}, //64
    {0x4B, KEY_EN_P}, //65
    {0x4C, KEY_EN_BACK_TICK}, //66
    {0x61, KEY_JP_AT}, //67
    {0x4D, "Key: {"}, //68
    {0x62, KEY_EN_LEFT_BRACKET}, //69
    {0x4E, KEY_EN_ISO_ENTER}, //70
    {0x4F, KEY_EN_NUMPAD_7}, //71
    {0x50, KEY_EN_NUMPAD_8}, //72
    {0x51, KEY_EN_NUMPAD_9}, //73
    {0x67, KEY_EN_NUMPAD_PLUS}, //74

    //row 5
    {0x55, KEY_EN_CAPS_LOCK}, //75
    {0x56, KEY_EN_CAPS_LOCK}, //76
    {0x57, KEY_EN_CAPS_LOCK}, //77
    {0x6D, KEY_EN_A}, //78
    {0x6E, KEY_EN_S}, //79
    {0x58, KEY_EN_D}, //80
    {0x59, KEY_EN_F}, //81
    {0x5A, KEY_EN_G}, //82
    {0x71, KEY_EN_H}, //83
    {0x72, KEY_EN_J}, //84
    {0x5B, KEY_EN_K}, //85
    {0x5C, KEY_EN_L}, //86
    {0x5D, "Key: +"}, //87
    {0x5E, KEY_EN_SEMICOLON}, //88
    {0x5F, "Key: *"}, //89
    {0x60, KEY_JP_COLON}, //90
    {0x77, "Key: }"}, //91
    {0x78, KEY_EN_RIGHT_BRACKET}, //92
    {0x63, KEY_EN_ISO_ENTER}, //93
    {0x64, "Key: Number Pad Home"}, //94
    {0x65, "Key: Number Pad Up Arrow"}, //95
    {0x66, "Key: Number Pad Page Up"}, //96
    {0x68, KEY_EN_NUMPAD_PLUS}, //97

    //row 6
    {0x6A, KEY_EN_LEFT_SHIFT}, //98
    {0x6B, KEY_EN_LEFT_SHIFT}, //99
    {0x82, KEY_EN_Z}, //100
    {0x83, KEY_EN_X}, //101
    {0x6F, KEY_EN_C}, //102
    {0x70, KEY_EN_V}, //103
    {0x87, KEY_EN_B}, //104
    {0x88, KEY_EN_N}, //105
    {0x73, KEY_EN_M}, //106
    {0x74, "Key: <"}, //107
    {0x89, KEY_EN_COMMA}, //108
    {0x75, "Key: >"}, //109
    {0x8A, KEY_EN_PERIOD}, //110
    {0x76, "Key: ?"}, //111
    {0x8B, KEY_EN_FORWARD_SLASH}, //112
    {0x8C, KEY_JP_RO}, //113
    {0x8D, KEY_EN_RIGHT_SHIFT}, //114
    {0xA2, KEY_EN_RIGHT_SHIFT}, //115
    {0x79, KEY_EN_NUMPAD_4}, //116
    {0x7A, "Key: Number Pad Right Arrow"}, //117
    {0x7B, KEY_EN_NUMPAD_5}, //118
    {0x7C, KEY_EN_NUMPAD_6}, //119
    {0x7D, "Key: Number Pad Left Arrow"}, //120

    //row 7
    {0x8E, KEY_EN_NUMPAD_1}, //121
    {0x8F, "Key: Number Pad End"}, //122
    {0x90, KEY_EN_NUMPAD_2}, //123
    {0x91, "Key: Number Pad Down Arrow"}, //124
    {0x92, KEY_EN_NUMPAD_3}, //125
    {0x93, "Key: Number Pad Page Down"}, //126
    {0xA7, KEY_EN_NUMPAD_ENTER}, //127

    //row 8
    {0x7F, KEY_EN_LEFT_CONTROL}, //128
    {0x80, KEY_EN_LEFT_FUNCTION}, //129
    {0x96, KEY_EN_LEFT_WINDOWS}, //130
    {0x97, KEY_EN_LEFT_ALT}, //131
    {0x98, KEY_JP_MUHENKAN}, //132
    {0x85, KEY_EN_SPACE}, //133
    {0x86, KEY_EN_SPACE}, //134
    {0x99, KEY_JP_HENKAN}, //135
    {0x9A, KEY_JP_KANA}, //136
    {0x9B, KEY_EN_RIGHT_CONTROL}, //137
    {0x9C, KEY_EN_LEFT_ARROW}, //138
    {0x9D, KEY_EN_UP_ARROW}, //139
    {0x9E, "Key: Brightness +"}, //140
    {0x9F, KEY_EN_DOWN_ARROW}, //141
    {0xA0, "Key: Brightness -"}, //142
    {0xA1, KEY_EN_RIGHT_ARROW}, //143
    {0xA3, KEY_EN_NUMPAD_0}, //144
    {0xA4, "Key: Number Pad Insert"}, //145
    {0xA5, KEY_EN_NUMPAD_PERIOD}, //146
    {0xA6, "Key: Number Pad Delete"}, //147
    {0xA8, KEY_EN_NUMPAD_ENTER}, //148
};

    /*----------------*\
    | zone 2, logo     |
    \*----------------*/
const lenovo_led legion_Y760_logo[]
{
    {0x01, "Logo Bottom Left"},//0
    {0x02, "Logo LED 2"},
    {0x03, "Logo LED 3"},
    {0x04, "Logo LED 4"},
    {0x05, "Logo LED Top Left"},
    {0x06, "Logo LED 6"},
    {0x07, "Logo LED 7"},
    {0x08, "Logo LED 8"},
    {0x09, "Logo LED Top Right"},
    {0x0A, "Logo LED 10"},
    {0x0B, "Logo LED 11"},
    {0x0C, "Logo LED 12"},
    {0x0D, "Logo Bottom Right"},//12
};

    /*----------------*\
    | zone 3, vents    |
    \*----------------*/
const lenovo_led legion_Y760_vents[]
{
    //left
    {0x01, "Left Vent Front"},//0
    {0x02, "Left Vent LED 2"},
    {0x03, "Left Vent LED 3"},
    {0x04, "Left Vent LED 4"},
    {0x05, "Left Vent LED 5"},
    {0x06, "Left Vent LED 6"},
    {0x07, "Left Vent LED 7"},
    {0x08, "Left Vent LED 8"},
    {0x09, "Left Vent LED 9"},
    {0x0A, "Left Vent LED 10"},
    {0x0B, "Left Vent LED 11"},
    {0x0C, "Left Vent LED 12"},
    {0x0D, "Left Vent LED 13"},
    {0x0E, "Left Vent LED 14"},
    {0x0F, "Left Vent LED 15"},
    {0x10, "Left Vent LED 16"},
    {0x11, "Left Vent LED 17"},
    {0x12, "Left Vent LED 18"},
    {0x13, "Left Vent LED 19"},
    {0x14, "Left Vent LED 20"},
    {0x15, "Left Vent LED 21"},
    {0x16, "Left Vent LED 22"},
    {0x17, "Left Vent LED 23"},
    {0x18, "Left Vent LED 24"},
    {0x19, "Left Vent LED 25"},
    {0x1A, "Left Vent LED 26"},
    {0x1B, "Left Vent Back"},//26

    //right
    {0x1C, "Right Vent Front"},//27
    {0x1D, "Right Vent LED 2"},
    {0x1E, "Right Vent LED 3"},
    {0x1F, "Right Vent LED 4"},
    {0x20, "Right Vent LED 5"},
    {0x21, "Right Vent LED 6"},
    {0x22, "Right Vent LED 7"},
    {0x23, "Right Vent LED 8"},
    {0x24, "Right Vent LED 9"},
    {0x25, "Right Vent LED 10"},
    {0x26, "Right Vent LED 11"},
    {0x27, "Right Vent LED 12"},
    {0x28, "Right Vent LED 13"},
    {0x29, "Right Vent LED 14"},
    {0x2A, "Right Vent LED 15"},
    {0x2B, "Right Vent LED 16"},
    {0x2C, "Right Vent LED 17"},
    {0x2D, "Right Vent LED 18"},
    {0x2E, "Right Vent LED 19"},
    {0x2F, "Right Vent LED 20"},
    {0x30, "Right Vent LED 21"},
    {0x31, "Right Vent LED 22"},
    {0x32, "Right Vent LED 23"},
    {0x33, "Right Vent LED 24"},
    {0x34, "Right Vent LED 25"},
    {0x35, "Right Vent LED 26"},
    {0x36, "Right Vent Back"},//53

    //back right vent
    {0x37, "Back Right Vent Left"},//54
    {0x38, "Back Right Vent 2"},
    {0x39, "Back Right Vent 3"},
    {0x3A, "Back Right Vent 4"},
    {0x3B, "Back Right Vent 5"},
    {0x3C, "Back Right Vent 6"},
    {0x3D, "Back Right Vent 7"},
    {0x3E, "Back Right Vent 8"},
    {0x3F, "Back Right Vent 10"},
    {0x40, "Back Right Vent 11"},
    {0x41, "Back Right Vent 12"},
    {0x42, "Back Right Vent 13"},
    {0x43, "Back Right Vent 14"},
    {0x44, "Back Right Vent 15"},
    {0x45, "Back Right Vent 16"},
    {0x46, "Back Right Vent 17"},
    {0x47, "Back Right Vent 18"},
    {0x48, "Back Right Vent 19"},
    {0x49, "Back Right Vent 20"},
    {0x4A, "Back Right Vent 21"},
    {0x4B, "Back Right Vent 22"},
    {0x4C, "Back Right Vent 23"},
    {0x4D, "Back Right Vent 24"},
    {0x4E, "Back Right Vent 25"},
    {0x4F, "Back Right Vent 26"},
    {0x50, "Back Right Vent Right"},//79

    //back left vent
    {0x51, "Back Left Vent Right"},//80
    {0x52, "Back Left Vent 2"},
    {0x53, "Back Left Vent 3"},
    {0x54, "Back Left Vent 4"},
    {0x55, "Back Left Vent 5"},
    {0x56, "Back Left Vent 6"},
    {0x57, "Back Left Vent 7"},
    {0x58, "Back Left Vent 8"},
    {0x59, "Back Left Vent 10"},
    {0x5A, "Back Left Vent 11"},
    {0x5B, "Back Left Vent 12"},
    {0x5C, "Back Left Vent 13"},
    {0x5D, "Back Left Vent 14"},
    {0x5E, "Back Left Vent 15"},
    {0x5F, "Back Left Vent 16"},
    {0x60, "Back Left Vent 17"},
    {0x61, "Back Left Vent 18"},
    {0x62, "Back Left Vent 19"},
    {0x63, "Back Left Vent 20"},
    {0x64, "Back Left Vent 21"},
    {0x65, "Back Left Vent 22"},
    {0x66, "Back Left Vent 23"},
    {0x67, "Back Left Vent 24"},
    {0x68, "Back Left Vent 25"},
    {0x69, "Back Left Vent 26"},
    {0x6A, "Back Left Vent Left"},//105
};
    /*-----------------*\
    | zone 4, neon      |
    \*-----------------*/
const lenovo_led legion_Y760_neon[]
{
    //left side
    {0x01, "Neon LED 1 Top Left"},//0
    {0x02, "Neon LED 2"},
    {0x03, "Neon LED 3"},
    {0x04, "Neon LED 4"},
    {0x05, "Neon LED 5"},
    {0x06, "Neon LED 6"},
    {0x07, "Neon LED 7"},
    {0x08, "Neon LED 8"},
    {0x09, "Neon LED 9"},
    {0x0A, "Neon LED 10"},
    {0x0B, "Neon LED 11"},
    {0x0C, "Neon LED 12"},
    {0x0D, "Neon LED 13"},
    {0x0E, "Neon LED 14"},
    {0x0F, "Neon LED 15"},
    {0x10, "Neon LED 16"},
    {0x11, "Neon LED 17"},
    {0x12, "Neon LED 18"},
    {0x13, "Neon LED 19"},
    {0x14, "Neon LED 20"},
    {0x15, "Neon LED 21 Left Corner"},//20

    //front
    {0x16, "Neon LED 22 Left Corner"},//21
    {0x17, "Neon LED 23"},
    {0x18, "Neon LED 24"},
    {0x19, "Neon LED 25"},
    {0x1A, "Neon LED 26"},
    {0x1B, "Neon LED 27"},
    {0x1C, "Neon LED 28"},
    {0x1D, "Neon LED 29"},
    {0x1E, "Neon LED 30"},
    {0x1F, "Neon LED 31"},
    {0x20, "Neon LED 32"},
    {0x21, "Neon LED 33"},
    {0x22, "Neon LED 34"},
    {0x23, "Neon LED 35"},
    {0x24, "Neon LED 36"},
    {0x25, "Neon LED 37"},
    {0x26, "Neon LED 38"},
    {0x27, "Neon LED 39"},
    {0x28, "Neon LED 40"},
    {0x29, "Neon LED 41"},
    {0x2A, "Neon LED 42"},
    {0x2B, "Neon LED 43"},
    {0x2C, "Neon LED 44"},
    {0x2D, "Neon LED 45"},
    {0x2E, "Neon LED 46"},
    {0x2F, "Neon LED 47"},
    {0x30, "Neon LED 48"},
    {0x31, "Neon LED 49"},
    {0x32, "Neon LED 50"},
    {0x33, "Neon LED 51"},
    {0x34, "Neon LED 52"},
    {0x35, "Neon LED 53"},
    {0x36, "Neon LED 54"},
    {0x37, "Neon LED 55"},
    {0x38, "Neon LED 56"},
    {0x39, "Neon LED 57"},
    {0x3A, "Neon LED 58"},
    {0x3B, "Neon LED 59"},
    {0x3C, "Neon LED 60"},
    {0x3D, "Neon LED 61"},
    {0x3E, "Neon LED 62"},
    {0x3F, "Neon LED 63"},
    {0x40, "Neon LED 64"},
    {0x41, "Neon LED 65"},
    {0x42, "Neon LED 66"},
    {0x43, "Neon LED 67"},
    {0x44, "Neon LED 68"},
    {0x45, "Neon LED 69"},
    {0x46, "Neon LED 70"},
    {0x47, "Neon LED 71"},
    {0x48, "Neon LED 72"},
    {0x49, "Neon LED 73"},
    {0x4A, "Neon LED 74"},
    {0x4B, "Neon LED 75"},
    {0x4C, "Neon LED 76"},
    {0x4D, "Neon LED 77"},
    {0x4E, "Neon LED 78 Right Corner"},//77

    //right side
    {0x4F, "Neon LED 79 Right Corner"},//78
    {0x50, "Neon LED 80"},
    {0x51, "Neon LED 81"},
    {0x52, "Neon LED 82"},
    {0x53, "Neon LED 83"},
    {0x54, "Neon LED 84"},
    {0x55, "Neon LED 85"},
    {0x56, "Neon LED 86"},
    {0x57, "Neon LED 87"},
    {0x58, "Neon LED 88"},
    {0x59, "Neon LED 89"},
    {0x5A, "Neon LED 90"},
    {0x5B, "Neon LED 91"},
    {0x5C, "Neon LED 92"},
    {0x5D, "Neon LED 93"},
    {0x5E, "Neon LED 94"},
    {0x5F, "Neon LED 95"},
    {0x60, "Neon LED 96"},
    {0x61, "Neon LED 97"},
    {0x62, "Neon LED 98"},
    {0x63, "Neon LED 99 Top Right"},//98
};


/*--------------------------------------------------------*\
| Additional LEDs for Legion Y750, Y750S and Y760S         |
\*--------------------------------------------------------*/
const lenovo_led legion_legion_Y750_additional_leds[]
{
    {0xAA, "Logo"},
    {0xAD, "Vents"},
    {0xAC, "Neon"}
};

/*--------------------------------------------------------*\
| LEDs for Legion Y740 17"                                |
\*--------------------------------------------------------*/

const lenovo_led legion_Y740_17_ansi_leds[]
{
    //Row 1
    {0x6E, KEY_EN_ESCAPE},           //0
    {0x70, KEY_EN_F1},               //1
    {0x71, KEY_EN_F2},               //2
    {0x72, KEY_EN_F3},               //3
    {0x73, KEY_EN_F4},               //4
    {0x74, KEY_EN_F5},               //5
    {0x75, KEY_EN_F6},               //6
    {0x76, KEY_EN_F7},               //7
    {0x77, KEY_EN_F8},               //8
    {0x78, KEY_EN_F9},               //9
    {0x79, KEY_EN_F10},              //10
    {0x7A, KEY_EN_F11},              //11
    {0x7B, KEY_EN_F12},              //12
    {0x7D, KEY_EN_DELETE},           //13
    {0x5B, KEY_EN_NUMPAD_7},         //14
    {0x60, KEY_EN_NUMPAD_8},         //15
    {0x65, KEY_EN_NUMPAD_9},         //16
    {0x5F, KEY_EN_NUMPAD_DIVIDE},    //17

    //Row 2
    {0x01, KEY_EN_BACK_TICK},        //18
    {0x02, KEY_EN_1},                //19
    {0x03, KEY_EN_2},                //20
    {0x04, KEY_EN_3},                //21
    {0x05, KEY_EN_4},                //22
    {0x06, KEY_EN_5},                //23
    {0x07, KEY_EN_6},                //24
    {0x08, KEY_EN_7},                //25
    {0x09, KEY_EN_8},                //26
    {0x0A, KEY_EN_9},                //27
    {0x0B, KEY_EN_0},                //28
    {0x0C, KEY_EN_MINUS},            //29
    {0x0D, KEY_EN_EQUALS},           //30
    {0x0F, KEY_EN_BACKSPACE},        //31 left led
    {0x8C, KEY_EN_BACKSPACE},        //32 right led
    {0x5C, KEY_EN_NUMPAD_4},         //33
    {0x61, KEY_EN_NUMPAD_5},         //34
    {0x66, KEY_EN_NUMPAD_6},         //35
    {0x64, KEY_EN_NUMPAD_TIMES},     //36

    //Row 3
    {0x10, KEY_EN_TAB},              //37
    {0x11, KEY_EN_Q},                //38
    {0x12, KEY_EN_W},                //39
    {0x13, KEY_EN_E},                //40
    {0x14, KEY_EN_R},                //41
    {0x15, KEY_EN_T},                //42
    {0x16, KEY_EN_Y},                //43
    {0x17, KEY_EN_U},                //44
    {0x18, KEY_EN_I},                //45
    {0x19, KEY_EN_O},                //46
    {0x1A, KEY_EN_P},                //47
    {0x1B, KEY_EN_LEFT_BRACKET},     //48
    {0x1C, KEY_EN_RIGHT_BRACKET},    //49
    {0x1D, KEY_EN_ANSI_BACK_SLASH},  //50
    {0x5D, KEY_EN_NUMPAD_1},         //51
    {0x62, KEY_EN_NUMPAD_2},         //52
    {0x67, KEY_EN_NUMPAD_3},         //53
    {0x69, KEY_EN_NUMPAD_MINUS},     //54

    //Row 4
    {0x1E, KEY_EN_CAPS_LOCK},        //55 left led
    {0x8D, KEY_EN_CAPS_LOCK},        //56 right led
    {0x1F, KEY_EN_A},                //57
    {0x20, KEY_EN_S},                //58
    {0x21, KEY_EN_D},                //59
    {0x22, KEY_EN_F},                //60
    {0x23, KEY_EN_G},                //61
    {0x24, KEY_EN_H},                //62
    {0x25, KEY_EN_J},                //63
    {0x26, KEY_EN_K},                //64
    {0x27, KEY_EN_L},                //65
    {0x28, KEY_EN_SEMICOLON},        //66
    {0x29, KEY_EN_QUOTE},            //67
    {0x2B, KEY_EN_ANSI_ENTER},       //68 left led
    {0x8F, KEY_EN_ANSI_ENTER},       //69 right led
    {0x5A, KEY_EN_NUMPAD_LOCK},      //70 top led
    {0x63, KEY_EN_NUMPAD_0},         //71
    {0x68, KEY_EN_NUMPAD_PERIOD},    //72
    {0x6A, KEY_EN_NUMPAD_PLUS},      //73

    //Row 5
    {0x2C, KEY_EN_LEFT_SHIFT},       //74
    {0x8E, KEY_EN_LEFT_SHIFT},       //75
    {0x2E, KEY_EN_Z},                //76
    {0x2F, KEY_EN_X},                //77
    {0x30, KEY_EN_C},                //78
    {0x31, KEY_EN_V},                //79
    {0x32, KEY_EN_B},                //80
    {0x33, KEY_EN_N},                //81
    {0x34, KEY_EN_M},                //82
    {0x35, KEY_EN_COMMA},            //83
    {0x36, KEY_EN_PERIOD},           //84
    {0x37, KEY_EN_FORWARD_SLASH},    //85
    {0x39, KEY_EN_RIGHT_SHIFT},      //86 left led
    {0x90, KEY_EN_RIGHT_SHIFT},      //87 right led
    {0x53, KEY_EN_UP_ARROW},         //88
    {0x93, KEY_EN_NUMPAD_LOCK},      //89 bottom led

    //Row 6
    {0x3A, KEY_EN_LEFT_CONTROL},     //90
    {0x3B, KEY_EN_LEFT_FUNCTION},    //91
    {0x7F, KEY_EN_LEFT_WINDOWS},     //92
    {0x3C, KEY_EN_LEFT_ALT},         //93
    {0x3D, KEY_EN_SPACE},            //94 left led
    {0x91, KEY_EN_SPACE},            //95 right led
    {0x3E, KEY_EN_RIGHT_ALT},        //96
    {0x40, KEY_EN_PRINT_SCREEN},     //97 left led
    {0x92, KEY_EN_PRINT_SCREEN},     //98 left led
    {0x86, "Record"},                //99
    {0x81, KEY_EN_RIGHT_CONTROL},    //100
    {0x4F, KEY_EN_LEFT_ARROW},       //101
    {0x54, KEY_EN_DOWN_ARROW},       //102
    {0x59, KEY_EN_RIGHT_ARROW},      //103
};

const lenovo_led legion_Y740_17_iso_leds[]
{
    //Row 1
    {0x6E, KEY_EN_ESCAPE},           //0
    {0x70, KEY_EN_F1},               //1
    {0x71, KEY_EN_F2},               //2
    {0x72, KEY_EN_F3},               //3
    {0x73, KEY_EN_F4},               //4
    {0x74, KEY_EN_F5},               //5
    {0x75, KEY_EN_F6},               //6
    {0x76, KEY_EN_F7},               //7
    {0x77, KEY_EN_F8},               //8
    {0x78, KEY_EN_F9},               //9
    {0x79, KEY_EN_F10},              //10
    {0x7A, KEY_EN_F11},              //11
    {0x7B, KEY_EN_F12},              //12
    {0x7D, KEY_EN_DELETE},           //13
    {0x5B, KEY_EN_NUMPAD_7},         //14
    {0x60, KEY_EN_NUMPAD_8},         //15
    {0x65, KEY_EN_NUMPAD_9},         //16
    {0x5F, KEY_EN_NUMPAD_DIVIDE},    //17

    //Row 2
    {0x01, KEY_EN_BACK_TICK},        //18
    {0x02, KEY_EN_1},                //19
    {0x03, KEY_EN_2},                //20
    {0x04, KEY_EN_3},                //21
    {0x05, KEY_EN_4},                //22
    {0x06, KEY_EN_5},                //23
    {0x07, KEY_EN_6},                //24
    {0x08, KEY_EN_7},                //25
    {0x09, KEY_EN_8},                //26
    {0x0A, KEY_EN_9},                //27
    {0x0B, KEY_EN_0},                //28
    {0x0C, KEY_EN_MINUS},            //29
    {0x0D, KEY_EN_EQUALS},           //30
    {0x0F, KEY_EN_BACKSPACE},        //31 left led
    {0x8C, KEY_EN_BACKSPACE},        //32 right led
    {0x5C, KEY_EN_NUMPAD_4},         //33
    {0x61, KEY_EN_NUMPAD_5},         //34
    {0x66, KEY_EN_NUMPAD_6},         //35
    {0x64, KEY_EN_NUMPAD_TIMES},     //36

    //Row 3
    {0x10, KEY_EN_TAB},              //37
    {0x11, KEY_EN_Q},                //38
    {0x12, KEY_EN_W},                //39
    {0x13, KEY_EN_E},                //40
    {0x14, KEY_EN_R},                //41
    {0x15, KEY_EN_T},                //42
    {0x16, KEY_EN_Y},                //43
    {0x17, KEY_EN_U},                //44
    {0x18, KEY_EN_I},                //45
    {0x19, KEY_EN_O},                //46
    {0x1A, KEY_EN_P},                //47
    {0x1B, KEY_EN_LEFT_BRACKET},     //48
    {0x1C, KEY_EN_RIGHT_BRACKET},    //49
    {0x94, KEY_EN_ISO_ENTER},        //50
    {0x5D, KEY_EN_NUMPAD_1},         //51
    {0x62, KEY_EN_NUMPAD_2},         //52
    {0x67, KEY_EN_NUMPAD_3},         //53
    {0x69, KEY_EN_NUMPAD_MINUS},     //54

    //Row 4
    {0x1E, KEY_EN_CAPS_LOCK},        //55 left led
    {0x8D, KEY_EN_CAPS_LOCK},        //56 right led
    {0x1F, KEY_EN_A},                //57
    {0x20, KEY_EN_S},                //58
    {0x21, KEY_EN_D},                //59
    {0x22, KEY_EN_F},                //60
    {0x23, KEY_EN_G},                //61
    {0x24, KEY_EN_H},                //62
    {0x25, KEY_EN_J},                //63
    {0x26, KEY_EN_K},                //64
    {0x27, KEY_EN_L},                //65
    {0x28, KEY_EN_SEMICOLON},        //66
    {0x29, KEY_EN_QUOTE},            //67
    {0x2B, KEY_EN_POUND},            //68
    {0x5A, KEY_EN_NUMPAD_LOCK},      //69 top led
    {0x63, KEY_EN_NUMPAD_0},         //70
    {0x68, KEY_EN_NUMPAD_PERIOD},    //71
    {0x6A, KEY_EN_NUMPAD_PLUS},      //72

    //Row 5
    {0x2C, KEY_EN_LEFT_SHIFT},       //73
    {0x2D, KEY_EN_ISO_BACK_SLASH},   //74
    {0x2E, KEY_EN_Z},                //75
    {0x2F, KEY_EN_X},                //76
    {0x30, KEY_EN_C},                //77
    {0x31, KEY_EN_V},                //78
    {0x32, KEY_EN_B},                //79
    {0x33, KEY_EN_N},                //80
    {0x34, KEY_EN_M},                //81
    {0x35, KEY_EN_COMMA},            //82
    {0x36, KEY_EN_PERIOD},           //83
    {0x37, KEY_EN_FORWARD_SLASH},    //84
    {0x39, KEY_EN_RIGHT_SHIFT},      //85 left led
    {0x90, KEY_EN_RIGHT_SHIFT},      //86 right led
    {0x53, KEY_EN_UP_ARROW},         //87
    {0x93, KEY_EN_NUMPAD_LOCK},      //88 bottom led

    //Row 6
    {0x3A, KEY_EN_LEFT_CONTROL},     //89
    {0x3B, KEY_EN_LEFT_FUNCTION},    //90
    {0x7F, KEY_EN_LEFT_WINDOWS},     //91
    {0x3C, KEY_EN_LEFT_ALT},         //92
    {0x3D, KEY_EN_SPACE},            //93 left led
    {0x91, KEY_EN_SPACE},            //94 right led
    {0x3E, KEY_EN_RIGHT_ALT},        //95
    {0x40, KEY_EN_PRINT_SCREEN},     //96 left led
    {0x92, KEY_EN_PRINT_SCREEN},     //97 left led
    {0x86, "Record"},                //98
    {0x81, KEY_EN_RIGHT_CONTROL},    //99
    {0x4F, KEY_EN_LEFT_ARROW},       //100
    {0x54, KEY_EN_DOWN_ARROW},       //101
    {0x59, KEY_EN_RIGHT_ARROW},      //102
};

/*--------------------------------------------------------*\
| LEDs for Legion Y740 15"                                |
\*--------------------------------------------------------*/

const lenovo_led legion_Y740_15_ansi_leds[]
{
    {0x6E, KEY_EN_ESCAPE},           //0
    {0x70, KEY_EN_F1},               //1
    {0x71, KEY_EN_F2},               //2
    {0x72, KEY_EN_F3},               //3
    {0x73, KEY_EN_F4},               //4
    {0x74, KEY_EN_F5},               //5
    {0x75, KEY_EN_F6},               //6
    {0x76, KEY_EN_F7},               //7
    {0x77, KEY_EN_F8},               //8
    {0x78, KEY_EN_F9},               //9
    {0x79, KEY_EN_F10},              //10
    {0x7A, KEY_EN_F11},              //11
    {0x7B, KEY_EN_F12},              //12
    {0x4C, KEY_EN_DELETE},           //13

    //Row 2
    {0x01, KEY_EN_BACK_TICK},        //14
    {0x02, KEY_EN_1},                //15
    {0x03, KEY_EN_2},                //16
    {0x04, KEY_EN_3},                //17
    {0x05, KEY_EN_4},                //18
    {0x06, KEY_EN_5},                //19
    {0x07, KEY_EN_6},                //20
    {0x08, KEY_EN_7},                //21
    {0x09, KEY_EN_8},                //22
    {0x0A, KEY_EN_9},                //23
    {0x0B, KEY_EN_0},                //24
    {0x0C, KEY_EN_MINUS},            //25
    {0x0D, KEY_EN_EQUALS},           //26
    {0x0F, KEY_EN_BACKSPACE},        //27
    {0x8C, KEY_EN_BACKSPACE},        //28

    //Row 3
    {0x10, KEY_EN_TAB},              //29
    {0x11, KEY_EN_Q},                //30
    {0x12, KEY_EN_W},                //31
    {0x13, KEY_EN_E},                //32
    {0x14, KEY_EN_R},                //33
    {0x15, KEY_EN_T},                //34
    {0x16, KEY_EN_Y},                //35
    {0x17, KEY_EN_U},                //36
    {0x18, KEY_EN_I},                //37
    {0x19, KEY_EN_O},                //38
    {0x1A, KEY_EN_P},                //39
    {0x1B, KEY_EN_LEFT_BRACKET},     //40
    {0x1C, KEY_EN_RIGHT_BRACKET},    //41
    {0x1D, KEY_EN_ANSI_BACK_SLASH},  //42

    //Row 4
    {0x1E, KEY_EN_CAPS_LOCK},        //43
    {0x8D, KEY_EN_CAPS_LOCK},        //44
    {0x1F, KEY_EN_A},                //45
    {0x20, KEY_EN_S},                //46
    {0x21, KEY_EN_D},                //47
    {0x22, KEY_EN_F},                //48
    {0x23, KEY_EN_G},                //49
    {0x24, KEY_EN_H},                //50
    {0x25, KEY_EN_J},                //51
    {0x26, KEY_EN_K},                //52
    {0x27, KEY_EN_L},                //53
    {0x28, KEY_EN_SEMICOLON},        //54
    {0x29, KEY_EN_QUOTE},            //55
    {0x2B, KEY_EN_ANSI_ENTER},       //56
    {0x8F, KEY_EN_ANSI_ENTER},       //57

    //Row 5
    {0x2C, KEY_EN_LEFT_SHIFT},       //58
    {0x8E, KEY_EN_LEFT_SHIFT},       //59
    {0x2E, KEY_EN_Z},                //60
    {0x2F, KEY_EN_X},                //61
    {0x30, KEY_EN_C},                //62
    {0x31, KEY_EN_V},                //63
    {0x32, KEY_EN_B},                //64
    {0x33, KEY_EN_N},                //65
    {0x34, KEY_EN_M},                //66
    {0x35, KEY_EN_COMMA},            //67
    {0x36, KEY_EN_PERIOD},           //68
    {0x37, KEY_EN_FORWARD_SLASH},    //69
    {0x39, KEY_EN_RIGHT_SHIFT},      //70
    {0x90, KEY_EN_RIGHT_SHIFT},      //71

    //Row 6
    {0x3A, KEY_EN_LEFT_CONTROL},     //72
    {0x3B, KEY_EN_LEFT_FUNCTION},    //73
    {0x7F, KEY_EN_LEFT_WINDOWS},     //74
    {0x3C, KEY_EN_LEFT_ALT},         //75
    {0x3D, KEY_EN_SPACE},            //76
    {0x91, KEY_EN_SPACE},            //77
    {0x3E, KEY_EN_RIGHT_ALT},        //78
    {0x40, KEY_EN_RIGHT_CONTROL},    //79
    {0x53, KEY_EN_UP_ARROW},         //80

    //Row 7
    {0x4F, KEY_EN_LEFT_ARROW},       //81
    {0x54, KEY_EN_DOWN_ARROW},       //82
    {0x59, KEY_EN_RIGHT_ARROW},      //83

    //Left Column
    {0xC1, "Key: Vantage"},          //84
    {0xC2, "Key: Capture"},          //85
    {0xC3, "Key: M1"},               //86
    {0xC4, "Key: M2"},               //87
    {0xC5, "Key: Kb Brightness Up"}, //88
    {0xC6, "Key: Kb Brightness Down"}, //89
};

const lenovo_led legion_Y740_15_iso_leds[]
{
    {0x6E, KEY_EN_ESCAPE},           //0
    {0x70, KEY_EN_F1},               //1
    {0x71, KEY_EN_F2},               //2
    {0x72, KEY_EN_F3},               //3
    {0x73, KEY_EN_F4},               //4
    {0x74, KEY_EN_F5},               //5
    {0x75, KEY_EN_F6},               //6
    {0x76, KEY_EN_F7},               //7
    {0x77, KEY_EN_F8},               //8
    {0x78, KEY_EN_F9},               //9
    {0x79, KEY_EN_F10},              //10
    {0x7A, KEY_EN_F11},              //11
    {0x7B, KEY_EN_F12},              //12
    {0x7D, KEY_EN_DELETE},           //13

    //Row 2
    {0x01, KEY_EN_BACK_TICK},        //14
    {0x02, KEY_EN_1},                //15
    {0x03, KEY_EN_2},                //16
    {0x04, KEY_EN_3},                //17
    {0x05, KEY_EN_4},                //18
    {0x06, KEY_EN_5},                //19
    {0x07, KEY_EN_6},                //20
    {0x08, KEY_EN_7},                //21
    {0x09, KEY_EN_8},                //22
    {0x0A, KEY_EN_9},                //23
    {0x0B, KEY_EN_0},                //24
    {0x0C, KEY_EN_MINUS},            //25
    {0x0D, KEY_EN_EQUALS},           //26
    {0x0F, KEY_EN_BACKSPACE},        //27
    {0x8C, KEY_EN_BACKSPACE},        //28

    //Row 3
    {0x10, KEY_EN_TAB},              //29
    {0x11, KEY_EN_Q},                //30
    {0x12, KEY_EN_W},                //31
    {0x13, KEY_EN_E},                //32
    {0x14, KEY_EN_R},                //33
    {0x15, KEY_EN_T},                //34
    {0x16, KEY_EN_Y},                //35
    {0x17, KEY_EN_U},                //36
    {0x18, KEY_EN_I},                //37
    {0x19, KEY_EN_O},                //38
    {0x1A, KEY_EN_P},                //39
    {0x1B, KEY_EN_LEFT_BRACKET},     //40
    {0x1C, KEY_EN_RIGHT_BRACKET},    //41
    {0x94, KEY_EN_ISO_ENTER},        //42

    //Row 4
    {0x1E, KEY_EN_CAPS_LOCK},        //43
    {0x8D, KEY_EN_CAPS_LOCK},        //44
    {0x1F, KEY_EN_A},                //45
    {0x20, KEY_EN_S},                //46
    {0x21, KEY_EN_D},                //47
    {0x22, KEY_EN_F},                //48
    {0x23, KEY_EN_G},                //49
    {0x24, KEY_EN_H},                //50
    {0x25, KEY_EN_J},                //51
    {0x26, KEY_EN_K},                //52
    {0x27, KEY_EN_L},                //53
    {0x28, KEY_EN_SEMICOLON},        //54
    {0x29, KEY_EN_QUOTE},            //55
    {0x2B, KEY_EN_POUND},            //56

    //Row 5
    {0x2C, KEY_EN_LEFT_SHIFT},       //57
    {0x2D, KEY_EN_ISO_BACK_SLASH},   //58
    {0x2E, KEY_EN_Z},                //59
    {0x2F, KEY_EN_X},                //60
    {0x30, KEY_EN_C},                //61
    {0x31, KEY_EN_V},                //62
    {0x32, KEY_EN_B},                //63
    {0x33, KEY_EN_N},                //64
    {0x34, KEY_EN_M},                //65
    {0x35, KEY_EN_COMMA},            //66
    {0x36, KEY_EN_PERIOD},           //67
    {0x37, KEY_EN_FORWARD_SLASH},    //68
    {0x39, KEY_EN_RIGHT_SHIFT},      //69
    {0x90, KEY_EN_RIGHT_SHIFT},      //70

    //Row 6
    {0x3A, KEY_EN_LEFT_CONTROL},     //71
    {0x3B, KEY_EN_LEFT_FUNCTION},    //72
    {0x7F, KEY_EN_LEFT_WINDOWS},     //73
    {0x3C, KEY_EN_LEFT_ALT},         //74
    {0x3D, KEY_EN_SPACE},            //75
    {0x91, KEY_EN_SPACE},            //76
    {0x3E, KEY_EN_RIGHT_ALT},        //77
    {0x40, KEY_EN_RIGHT_CONTROL},    //78
    {0x53, KEY_EN_UP_ARROW},         //79

    //Row 7
    {0x4F, KEY_EN_LEFT_ARROW},       //80
    {0x54, KEY_EN_DOWN_ARROW},       //81
    {0x59, KEY_EN_RIGHT_ARROW},      //82

    //Left Column
    {0xC1, "Key: Vantage"},          //83
    {0xC2, "Key: Capture"},          //84
    {0xC3, "Key: M1"},               //85
    {0xC4, "Key: M2"},               //86
    {0xC5, "Key: Kb Brightness Up"}, //87
    {0xC6, "Key: Kb Brightness Down"}, //88
};

/*--------------------------------------------------------*\
| Additional LEDs for Legion Y740                          |
\*--------------------------------------------------------*/
const lenovo_led legion_legion_Y740_additional_leds[]
{
    {0x97, "Power button"},          //104
    {0x99, "Vents"},                 //105
    {0x98, "USB ports"},             //106
    {0x96, "Legion Y Logo"}          //107
};


/*--------------------------------------------------------*\
| Legion 7 gen 6: 7 zones                                  |
|                                                          |
|Note: the device has 4 zones in the protocol however, the |
|vent lights have been split into 4 zones to improve ease  |
|of use                                                    |
\*--------------------------------------------------------*/

/*------*\
|keyboard|
\*------*/
static lenovo_zone lenovo_legion_Y760_kbd_ansi
{
    "Keyboard",
    ZONE_TYPE_MATRIX,
    1,
    13,
    21,
    legion_Y760_ansi_leds_map,
    legion_Y760_ansi_leds,
    0,
    144,
};

static lenovo_zone lenovo_legion_Y760_kbd_iso
{
    "Keyboard",
    ZONE_TYPE_MATRIX,
    1,
    13,
    21,
    legion_Y760_iso_leds_map,
    legion_Y760_iso_leds,
    0,
    142,
};

static lenovo_zone lenovo_legion_Y760_kbd_jp
{
    "Keyboard",
    ZONE_TYPE_MATRIX,
    1,
    13,
    21,
    legion_Y760_jp_leds_map,
    legion_Y760_jp_leds,
    0,
    148,
};

/*------*\
|logo    |
\*------*/
static lenovo_zone lenovo_legion_Y760_logo
{
    "Logo",
    ZONE_TYPE_LINEAR,
    2,
    1,
    13,
    NULL,
    legion_Y760_logo,
    0,
    12,
};

/*------*\
|vents   |
\*------*/
static lenovo_zone lenovo_legion_Y760_vent_left
{
    "Left vent",
    ZONE_TYPE_LINEAR,
    3,
    1,
    26,
    NULL,
    legion_Y760_vents,
    0,
    26,
};

static lenovo_zone lenovo_legion_Y760_vent_right
{
    "Right vent",
    ZONE_TYPE_LINEAR,
    3,
    1,
    26,
    NULL,
    legion_Y760_vents,
    27,
    53,
};

static lenovo_zone lenovo_legion_Y760_vent_back_right
{
    "Back Right vent",
    ZONE_TYPE_LINEAR,
    3,
    1,
    25,
    NULL,
    legion_Y760_vents,
    54,
    79,
};

static lenovo_zone lenovo_legion_Y760_vent_back_left
{
    "Back Left vent",
    ZONE_TYPE_LINEAR,
    3,
    1,
    25,
    NULL,
    legion_Y760_vents,
    80,
    105,
};

/*------*\
|neon    |
\*------*/
static lenovo_zone lenovo_legion_Y760_neon
{
    "Neon",
    ZONE_TYPE_LINEAR,
    4,
    1,
    99,
    NULL,
    legion_Y760_neon,
    0,
    98,
};

/*--------------------------------------------------------*\
| Legion Y750, Y750S and Y760S: 4 zones                    |
\*--------------------------------------------------------*/

/*----------------------------------*\
|keyboard                            |
|                                    |
| Note: keyboard is shared with Y760 |
\*----------------------------------*/

/*------*\
|logo    |
\*------*/
static lenovo_zone lenovo_legion_Y750_logo
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1,
    1,
    NULL,
    legion_legion_Y750_additional_leds,
    0,
    0,
};

/*------*\
|vents   |
\*------*/
static lenovo_zone lenovo_legion_Y750_vents
{
    "Vents",
    ZONE_TYPE_SINGLE,
    1,
    1,
    1,
    NULL,
    legion_legion_Y750_additional_leds,
    1,
    1,
};

/*------*\
|neon    |
\*------*/
static lenovo_zone lenovo_legion_Y750_neon
{
    "Neon",
    ZONE_TYPE_SINGLE,
    1,
    1,
    1,
    NULL,
    legion_legion_Y750_additional_leds,
    2,
    2,
};

/*--------------------------------------------------------*\
| Legion Y740 17": 5 zones                                 |
\*--------------------------------------------------------*/

/*------*\
|keyboard|
\*------*/
static lenovo_zone lenovo_legion_Y740_17_kbd_ansi
{
    "Keyboard",
    ZONE_TYPE_MATRIX,
    1,
    6,
    19,
    legion_Y740_17_ansi_leds_map,
    legion_Y740_17_ansi_leds,
    0,
    103,
};

static lenovo_zone lenovo_legion_Y740_17_kbd_iso
{
    "Keyboard",
    ZONE_TYPE_MATRIX,
    1,
    6,
    19,
    legion_Y740_17_iso_leds_map,
    legion_Y740_17_iso_leds,
    0,
    102,
};

/*------*\
|logo    |
\*------*/
static lenovo_zone lenovo_legion_Y740_logo
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1,
    1,
    NULL,
    legion_legion_Y740_additional_leds,
    3,
    3,
};

/*-----------*\
|Power button |
\*-----------*/
static lenovo_zone lenovo_legion_Y740_pwrbtn
{
    "Power Button",
    ZONE_TYPE_SINGLE,
    1,
    1,
    1,
    NULL,
    legion_legion_Y740_additional_leds,
    0,
    0,
};

/*------*\
|vents   |
\*------*/
static lenovo_zone lenovo_legion_Y740_vents
{
    "Vents",
    ZONE_TYPE_SINGLE,
    1,
    1,
    1,
    NULL,
    legion_legion_Y740_additional_leds,
    1,
    1,
};

/*--------*\
|USB Ports |
\*--------*/
static lenovo_zone lenovo_legion_Y740_ports
{
    "USB Ports",
    ZONE_TYPE_SINGLE,
    1,
    1,
    1,
    NULL,
    legion_legion_Y740_additional_leds,
    2,
    2,
};

/*--------------------------------------------------------*\
| Legion Y740 15": 5 zones                                 |
\*--------------------------------------------------------*/

/*------*\
|keyboard|
\*------*/
static lenovo_zone lenovo_legion_Y740_15_kbd_ansi
{
    "Keyboard",
    ZONE_TYPE_MATRIX,
    1,
    7,
    16,
    legion_Y740_15_ansi_leds_map,
    legion_Y740_15_ansi_leds,
    0,
    89,
};

static lenovo_zone lenovo_legion_Y740_15_kbd_iso
{
    "Keyboard",
    ZONE_TYPE_MATRIX,
    1,
    7,
    16,
    legion_Y740_15_iso_leds_map,
    legion_Y740_15_iso_leds,
    0,
    88,
};

/*--------------------------------------------------------*\
| Legion 7 gen7: 4 zones                                   |
\*--------------------------------------------------------*/

static const unsigned int legion7_gen7and8_ansi_leds_map[] =
    {    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
        20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  NA,  30,  31,  32,  33,  NA,  34,  35,  36,  37,
        38,  39,  40,  41,  NA,  42,  43,  44,  45,  46,  47,  48,  49,  50,  NA,  51,  52,  53,  54,  NA,
        55,  56,  NA,  57,  58,  59,  60,  61,  62,  63,  64,  65,  NA,  66,  67,  NA,  68,  69,  70,  71,
        72,  NA,  73,  74,  75,  NA,  76,  77,  78,  79,  80,  81,  82,  NA,  83,  NA,  84,  85,  86,  NA,
        87,  88,  89,  90,  91,  NA,  NA,  NA,  NA,  NA,  92,  93,  NA,  94,  NA,  NA,  95,  NA,  96,  97,
        NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  98,  99,  NA, 100,  NA,  NA,  NA,  NA};

/*---------------------*\
| zone 1, keyboard ANSI |
\*---------------------*/
const lenovo_led legion7_gen7and8_ansi_leds[]
{
    //row 1
    {0x01, KEY_EN_ESCAPE},//0
    {0x02, KEY_EN_F1},//1
    {0x03, KEY_EN_F2},//2
    {0x04, KEY_EN_F3},//3
    {0x05, KEY_EN_F4},//4
    {0x06, KEY_EN_F5},//5
    {0x07, KEY_EN_F6},//6
    {0x08, KEY_EN_F7},//7
    {0x09, KEY_EN_F8},//8
    {0x0A, KEY_EN_F9},//9
    {0x0B, KEY_EN_F10},//10
    {0x0C, KEY_EN_F11},//11
    {0x0D, KEY_EN_F12},//12
    {0x0E, KEY_EN_INSERT},//13
    {0x0F, KEY_EN_PRINT_SCREEN},//14
    {0x10, KEY_EN_DELETE},//15
    {0x11, KEY_EN_HOME},//16
    {0x12, KEY_EN_END},//17
    {0x13, KEY_EN_PAGE_UP},//18
    {0x14, KEY_EN_PAGE_DOWN},//19

    //row 2
    {0x16, KEY_EN_BACK_TICK},//20
    {0x17, KEY_EN_1},//21
    {0x18, KEY_EN_2},//22
    {0x19, KEY_EN_3},//23
    {0x1A, KEY_EN_4},//24
    {0x1B, KEY_EN_5},//25
    {0x1C, KEY_EN_6},//26
    {0x1D, KEY_EN_7},//27
    {0x1E, KEY_EN_8},//28
    {0x1F, KEY_EN_9},//29
    {0x20, KEY_EN_0},//30
    {0x21, KEY_EN_MINUS},//31
    {0x22, KEY_EN_EQUALS},//32
    {0x38, KEY_EN_BACKSPACE},//33
    {0x26, KEY_EN_NUMPAD_LOCK},//34
    {0x27, KEY_EN_NUMPAD_DIVIDE},//35
    {0x28, KEY_EN_NUMPAD_TIMES},//36
    {0x29, KEY_EN_NUMPAD_MINUS},//37

    //row 3
    {0x40, KEY_EN_TAB},//38
    {0x42, KEY_EN_Q},//39
    {0x43, KEY_EN_W},//40
    {0x44, KEY_EN_E},//41
    {0x45, KEY_EN_R},//42
    {0x46, KEY_EN_T},//43
    {0x47, KEY_EN_Y},//44
    {0x48, KEY_EN_U},//45
    {0x49, KEY_EN_I},//46
    {0x4A, KEY_EN_O},//47
    {0x4B, KEY_EN_P},//48
    {0x4C, KEY_EN_LEFT_BRACKET},//49
    {0x4D, KEY_EN_RIGHT_BRACKET},//50
    {0x4E, KEY_EN_ANSI_BACK_SLASH},//51
    {0x4F, KEY_EN_NUMPAD_7},//52
    {0x50, KEY_EN_NUMPAD_8},//53
    {0x51, KEY_EN_NUMPAD_9},//54

    //row 4
    {0x55, KEY_EN_CAPS_LOCK},//55
    {0x6D, KEY_EN_A},//56
    {0x6E, KEY_EN_S},//57
    {0x58, KEY_EN_D},//58
    {0x59, KEY_EN_F},//59
    {0x5A, KEY_EN_G},//60
    {0x71, KEY_EN_H},//61
    {0x72, KEY_EN_J},//62
    {0x5B, KEY_EN_K},//63
    {0x5C, KEY_EN_L},//64
    {0x5D, KEY_EN_SEMICOLON},//65
    {0x5F, KEY_EN_QUOTE},//66
    {0x77, KEY_EN_ANSI_ENTER},//67
    {0x79, KEY_EN_NUMPAD_4},//68
    {0x7B, KEY_EN_NUMPAD_5},//69
    {0x7C, KEY_EN_NUMPAD_6},//70
    {0x68, KEY_EN_NUMPAD_PLUS},//71

    //row 5
    {0x6A, KEY_EN_LEFT_SHIFT},//72
    {0x82, KEY_EN_Z},//73
    {0x83, KEY_EN_X},//74
    {0x6F, KEY_EN_C},//75
    {0x70, KEY_EN_V},//76
    {0x87, KEY_EN_B},//77
    {0x88, KEY_EN_N},//78
    {0x73, KEY_EN_M},//79
    {0x74, KEY_EN_COMMA},//80
    {0x75, KEY_EN_PERIOD},//81
    {0x76, KEY_EN_FORWARD_SLASH},//82
    {0x8D, KEY_EN_RIGHT_SHIFT},//83
    {0x8E, KEY_EN_NUMPAD_1},//84
    {0x90, KEY_EN_NUMPAD_2},//85
    {0x92, KEY_EN_NUMPAD_3},//86

    //row 6
    {0x7F, KEY_EN_LEFT_CONTROL},//87
    {0x80, KEY_EN_LEFT_FUNCTION},//88
    {0x96, KEY_EN_LEFT_WINDOWS},//89
    {0x97, KEY_EN_LEFT_ALT},//90
    {0x98, KEY_EN_SPACE},//91
    {0x9A, KEY_EN_RIGHT_ALT},//92
    {0x9B, KEY_EN_RIGHT_CONTROL},//93
    {0x9D, KEY_EN_UP_ARROW},//94
    {0xA3, KEY_EN_NUMPAD_0},//95
    {0xA5, KEY_EN_NUMPAD_PERIOD},//96
    {0xA7, KEY_EN_NUMPAD_ENTER},//97

    //row 7
    {0x9C, KEY_EN_LEFT_ARROW},//98
    {0x9F, KEY_EN_DOWN_ARROW},//99
    {0xA1, KEY_EN_RIGHT_ARROW},//100
};

const lenovo_led legion7_gen7and8_neon_leds[]
{
    {0xF5, "Neon group 1"},//0
    {0xF6, "Neon group 2"},//1
    {0xF7, "Neon group 3"},//2
    {0xF8, "Neon group 4"},//3
    {0xF9, "Neon group 5"},//4
    {0xFA, "Neon group 6"},//5
    {0xFB, "Neon group 7"},//6
    {0xFC, "Neon group 8"},//7
    {0xFD, "Neon group 9"},//8
    {0xFE, "Neon group 10"},//9
};

const lenovo_led legion_7gen7_vents_leds[]
{
    {0xE9, "Vent group 1"},//0
    {0xEA, "Vent group 2"},//1
    {0xEB, "Vent group 3"},//2
    {0xEC, "Vent group 4"},//3
    {0xED, "Vent group 5"},//4
    {0xEE, "Vent group 6"},//5
    {0xEF, "Vent group 7"},//6
    {0xF0, "Vent group 8"},//7
};

const lenovo_led legion_7gen7_logo_leds[]
{
    {0xDD, "Logo"},//0
};

/*------*\
|keyboard|
\*------*/
static lenovo_zone legion7_gen7and8_kbd_ansi
{
    "Keyboard",
    ZONE_TYPE_MATRIX,
    0,
    7,
    20,
    legion7_gen7and8_ansi_leds_map,
    legion7_gen7and8_ansi_leds,
    0,
    100,
};

/*------*\
|logo    |
\*------*/
static lenovo_zone lenovo_legion_7gen7_logo
{
    "Logo",
    ZONE_TYPE_LINEAR,
    5,
    1,
    1,
    NULL,
    legion_7gen7_logo_leds,
    0,
    0,
};

/*------*\
|vents   |
\*------*/
static lenovo_zone lenovo_legion_7gen7_vents
{
    "Vents",
    ZONE_TYPE_LINEAR,
    3,
    1,
    8,
    NULL,
    legion_7gen7_vents_leds,
    0,
    7,
};

/*------*\
|neon    |
\*------*/
static lenovo_zone legion7_gen7and8_neon
{
    "Neon",
    ZONE_TYPE_LINEAR,
    1,
    1,
    10,
    NULL,
    legion7_gen7and8_neon_leds,
    0,
    9,
};
