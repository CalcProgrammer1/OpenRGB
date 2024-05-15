/*---------------------------------------------------------*\
| RGBController_CorsairPeripheral.cpp                       |
|                                                           |
|   RGBController for Corsair peripherals                   |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_CorsairPeripheral.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   0,  NA,  10,  18,  28,  36,  NA,  46,  55,  64,  74,  NA,  84,  93, 102,   6,  15,  24,  33,  26,  35,  44,  53 },
      {   1,  11,  19,  29,  37,  47,  56,  65,  75,  85,  94,  NA, 103,   7,  25,  NA,  42,  51,  60,  62,  72,  82,  91 },
      {   2,  NA,  12,  20,  30,  38,  NA,  48,  57,  66,  76,  86,  95, 104,  70,  80,  34,  43,  52,  9,   17,  27, 100 },
      {   3,  NA,  13,  21,  31,  39,  NA,  49,  58,  67,  77,  87,  96, 105,  98, 112,  NA,  NA,  NA,  45,  54,  63,  NA },
      {   4, 111,  22,  32,  40,  50,  NA,  59,  NA,  68,  78,  88,  97, 106,  61,  NA,  NA,  81,  NA,  73,  83,  92, 109 },
      {   5,  14,  23,  NA,  NA,  NA,  NA,  41,  NA,  NA,  NA,  NA,  69,  79,  89,  71,  90,  99, 108, 101,  NA, 110,  NA } };

static unsigned int matrix_map_k70_mk2[7][23] =
    { {  NA,  NA,  NA, 115, 107,   8,  NA,  NA,  NA,  NA,  NA, 113, 114,  NA,  NA,  NA,  NA,  NA,  NA,  16,  NA,  NA,  NA,},
      {   0,  NA,  10,  18,  28,  36,  NA,  46,  55,  64,  74,  NA,  84,  93, 102,   6,  15,  24,  33,  26,  35,  44,  53 },
      {   1,  11,  19,  29,  37,  47,  56,  65,  75,  85,  94,  NA, 103,   7,  25,  NA,  42,  51,  60,  62,  72,  82,  91 },
      {   2,  NA,  12,  20,  30,  38,  NA,  48,  57,  66,  76,  86,  95, 104,  70,  80,  34,  43,  52,  9,   17,  27, 100 },
      {   3,  NA,  13,  21,  31,  39,  NA,  49,  58,  67,  77,  87,  96, 105,  98, 112,  NA,  NA,  NA,  45,  54,  63,  NA },
      {   4, 111,  22,  32,  40,  50,  NA,  59,  NA,  68,  78,  88,  97, 106,  61,  NA,  NA,  81,  NA,  73,  83,  92, 109 },
      {   5,  14,  23,  NA,  NA,  NA,  NA,  41,  NA,  NA,  NA,  NA,  69,  79,  89,  71,  90,  99, 108, 101,  NA, 110,  NA } };

static unsigned int matrix_map_k95_platinum[7][24] =
    { {  NA,  NA,  NA, 119, 107,   8,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  16,  NA,  NA,  NA,},
      { 113,   0,  NA,  10,  18,  28,  36,  NA,  46,  55,  64,  74,  NA,  84,  93, 102,   6,  15,  24,  33,  26,  35,  44,  53 },
      { 114,   1,  11,  19,  29,  37,  47,  56,  65,  75,  85,  94,  NA, 103,   7,  25,  NA,  42,  51,  60,  62,  72,  82,  91 },
      { 115,   2,  NA,  12,  20,  30,  38,  NA,  48,  57,  66,  76,  86,  95, 104,  70,  80,  34,  43,  52,  9,   17,  27, 100 },
      { 116,   3,  NA,  13,  21,  31,  39,  NA,  49,  58,  67,  77,  87,  96, 105,  98, 112,  NA,  NA,  NA,  45,  54,  63,  NA },
      { 117,   4, 111,  22,  32,  40,  50,  NA,  59,  NA,  68,  78,  88,  97, 106,  61,  NA,  NA,  81,  NA,  73,  83,  92, 109 },
      { 118,   5,  14,  23,  NA,  NA,  NA,  NA,  41,  NA,  NA,  NA,  NA,  69,  79,  89,  71,  90,  99, 108, 101,  NA, 110,  NA } };

static unsigned int matrix_map_k95[7][26] =
    { {  NA,  NA,  NA, 131, 132, 133, 134,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 107,   8,  NA,  NA,  16,  NA,  NA,},
      { 113, 114, 115,   0,  NA,  10,  18,  28,  36,  NA,  46,  55,  64,  74,  NA,  84,  93, 102,   6,  15,  24,  33,  26,  35,  44,  53 },
      { 116, 117, 118,   1,  11,  19,  29,  37,  47,  56,  65,  75,  85,  94,  NA, 103,   7,  25,  NA,  42,  51,  60,  62,  72,  82,  91 },
      { 119, 120, 121,   2,  NA,  12,  20,  30,  38,  NA,  48,  57,  66,  76,  86,  95, 104,  70,  80,  34,  43,  52,  9,   17,  27, 100 },
      { 122, 123, 124,   3,  NA,  13,  21,  31,  39,  NA,  49,  58,  67,  77,  87,  96, 105,  98, 112,  NA,  NA,  NA,  45,  54,  63,  NA },
      { 125, 126, 127,   4, 111,  22,  32,  40,  50,  NA,  59,  NA,  68,  78,  88,  97, 106,  61,  NA,  NA,  81,  NA,  73,  83,  92, 109 },
      { 128, 129, 130,   5,  14,  23,  NA,  NA,  NA,  NA,  41,  NA,  NA,  NA,  NA,  69,  79,  89,  71,  90,  99, 108, 101,  NA, 110,  NA } };

/*---------------------------------------------------------*\
| Normal Corsair Layout                                     |
\*---------------------------------------------------------*/
static const char* zone_names[] =
{
    ZONE_EN_KEYBOARD,
};

static const unsigned int zone_sizes[] =
{
    113
};

static const zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

/*---------------------------------------------------------*\
| K70 MK2 Corsair Layout                                    |
\*---------------------------------------------------------*/
static const char* zone_names_k70_mk2[] =
{
    ZONE_EN_KEYBOARD,
};

static const unsigned int zone_sizes_k70_mk2[] =
{
    116
};

static const zone_type zone_types_k70_mk2[] =
{
    ZONE_TYPE_MATRIX,
};

/*---------------------------------------------------------*\
| K95 Platinum                                              |
\*---------------------------------------------------------*/
static const char* zone_names_k95_platinum[] =
{
    ZONE_EN_KEYBOARD,
    "Light Bar"
};

static const unsigned int zone_sizes_k95_platinum[] =
{
    120,
    19
};

static const zone_type zone_types_k95_platinum[] =
{
    ZONE_TYPE_MATRIX,
    ZONE_TYPE_LINEAR
};

/*---------------------------------------------------------*\
| K95 non-Platinum                                          |
\*---------------------------------------------------------*/
static const char* zone_names_k95[] =
{
    ZONE_EN_KEYBOARD,
};

static const unsigned int zone_sizes_k95[] =
{
    135
};

static const zone_type zone_types_k95[] =
{
    ZONE_TYPE_MATRIX
};

/*---------------------------------------------------------*\
| K55                                                       |
\*---------------------------------------------------------*/
static const char* zone_names_k55[] =
{
    "Left",
    "Middle",
    "Right",
};

static const unsigned int zone_sizes_k55[] =
{
    1,
    1,
    1
};

static const zone_type zone_types_k55[] =
{
    ZONE_TYPE_SINGLE,
    ZONE_TYPE_SINGLE,
    ZONE_TYPE_SINGLE
};

static const char* led_names[] =
{
    KEY_EN_ESCAPE,              //0
    KEY_EN_BACK_TICK,           //1
    KEY_EN_TAB,                 //2
    KEY_EN_CAPS_LOCK,           //3
    KEY_EN_LEFT_SHIFT,          //4
    KEY_EN_LEFT_CONTROL,        //5
    KEY_EN_F12,                 //6
    KEY_EN_EQUALS,              //7
    "Key: Lock",                //8
    KEY_EN_NUMPAD_7,            //9
    KEY_EN_F1,                  //12
    KEY_EN_1,                   //13
    KEY_EN_Q,                   //14
    KEY_EN_A,                   //15
    KEY_EN_LEFT_WINDOWS,        //17
    KEY_EN_PRINT_SCREEN,        //18
    KEY_EN_MEDIA_MUTE,          //20
    KEY_EN_NUMPAD_8,            //21
    KEY_EN_F2,                  //24
    KEY_EN_2,                   //25
    KEY_EN_W,                   //26
    KEY_EN_S,                   //27
    KEY_EN_Z,                   //28
    KEY_EN_LEFT_ALT,            //29
    KEY_EN_SCROLL_LOCK,         //30
    KEY_EN_BACKSPACE,           //31
    KEY_EN_MEDIA_STOP,          //32
    KEY_EN_NUMPAD_9,            //33
    KEY_EN_F3,                  //36
    KEY_EN_3,                   //37
    KEY_EN_E,                   //38
    KEY_EN_D,                   //39
    KEY_EN_X,                   //40
    KEY_EN_PAUSE_BREAK,         //42
    KEY_EN_DELETE,              //43
    KEY_EN_MEDIA_PREVIOUS,      //44
    KEY_EN_F4,                  //48
    KEY_EN_4,                   //49
    KEY_EN_R,                   //50
    KEY_EN_F,                   //51
    KEY_EN_C,                   //52
    KEY_EN_SPACE,               //53
    KEY_EN_INSERT,              //54
    KEY_EN_END,                 //55
    KEY_EN_MEDIA_PLAY_PAUSE,    //56
    KEY_EN_NUMPAD_4,            //57
    KEY_EN_F5,                  //60
    KEY_EN_5,                   //61
    KEY_EN_T,                   //62
    KEY_EN_G,                   //63
    KEY_EN_V,                   //64
    KEY_EN_HOME,                //66
    KEY_EN_PAGE_DOWN,           //67
    KEY_EN_MEDIA_NEXT,          //68
    KEY_EN_NUMPAD_5,            //69
    KEY_EN_F6,                  //72
    KEY_EN_6,                   //73
    KEY_EN_Y,                   //74
    KEY_EN_H,                   //75
    KEY_EN_B,                   //76
    KEY_EN_PAGE_UP,             //78
    KEY_EN_RIGHT_SHIFT,         //79
    KEY_EN_NUMPAD_LOCK,         //80
    KEY_EN_NUMPAD_6,            //81
    KEY_EN_F7,                  //84
    KEY_EN_7,                   //85
    KEY_EN_U,                   //86
    KEY_EN_J,                   //87
    KEY_EN_N,                   //88
    KEY_EN_RIGHT_ALT,           //89
    KEY_EN_RIGHT_BRACKET,       //90
    KEY_EN_RIGHT_CONTROL,       //91
    KEY_EN_NUMPAD_DIVIDE,       //92
    KEY_EN_NUMPAD_1,            //93
    KEY_EN_F8,                  //96
    KEY_EN_8,                   //97
    KEY_EN_I,                   //98
    KEY_EN_K,                   //99
    KEY_EN_M,                   //100
    KEY_EN_RIGHT_WINDOWS,       //101
    KEY_EN_ANSI_BACK_SLASH,     //102
    KEY_EN_UP_ARROW,            //103
    KEY_EN_NUMPAD_TIMES,        //104
    KEY_EN_NUMPAD_2,            //105
    KEY_EN_F9,                  //108
    KEY_EN_9,                   //109
    KEY_EN_O,                   //110
    KEY_EN_L,                   //111
    KEY_EN_COMMA,               //112
    KEY_EN_MENU,                //113
    KEY_EN_LEFT_ARROW,          //115
    KEY_EN_NUMPAD_MINUS,        //116
    KEY_EN_NUMPAD_3,            //117
    KEY_EN_F10,                 //120
    KEY_EN_0,                   //121
    KEY_EN_P,                   //122
    KEY_EN_SEMICOLON,           //123
    KEY_EN_PERIOD,              //124
    KEY_EN_ANSI_ENTER,          //126
    KEY_EN_DOWN_ARROW,          //127
    KEY_EN_NUMPAD_PLUS,         //128
    KEY_EN_NUMPAD_0,            //129
    KEY_EN_F11,                 //132
    KEY_EN_MINUS,               //133
    KEY_EN_LEFT_BRACKET,        //134
    KEY_EN_QUOTE,               //135
    KEY_EN_FORWARD_SLASH,       //136
    "Key: Brightness",          //137
    KEY_EN_RIGHT_ARROW,         //139
    KEY_EN_NUMPAD_ENTER,        //140
    KEY_EN_NUMPAD_PERIOD,       //141
    "Key: / (ISO)",
    KEY_EN_ISO_BACK_SLASH,
};

static const char* led_names_k70_mk2[] =
{
    KEY_EN_ESCAPE,              //0
    KEY_EN_BACK_TICK,           //1
    KEY_EN_TAB,                 //2
    KEY_EN_CAPS_LOCK,           //3
    KEY_EN_LEFT_SHIFT,          //4
    KEY_EN_LEFT_CONTROL,        //5
    KEY_EN_F12,                 //6
    KEY_EN_EQUALS,              //7
    "Key: Lock",                //8
    KEY_EN_NUMPAD_7,            //9
    KEY_EN_F1,                  //12
    KEY_EN_1,                   //13
    KEY_EN_Q,                   //14
    KEY_EN_A,                   //15
    //"Key: / (ISO)",           //16
    KEY_EN_LEFT_WINDOWS,        //17
    KEY_EN_PRINT_SCREEN,        //18
    KEY_EN_MEDIA_MUTE,          //20
    KEY_EN_NUMPAD_8,            //21
    KEY_EN_F2,                  //24
    KEY_EN_2,                   //25
    KEY_EN_W,                   //26
    KEY_EN_S,                   //27
    KEY_EN_Z,                   //28
    KEY_EN_LEFT_ALT,            //29
    KEY_EN_SCROLL_LOCK,         //30
    KEY_EN_BACKSPACE,           //31
    KEY_EN_MEDIA_STOP,          //32
    KEY_EN_NUMPAD_9,            //33
    KEY_EN_F3,                  //36
    KEY_EN_3,                   //37
    KEY_EN_E,                   //38
    KEY_EN_D,                   //39
    KEY_EN_X,                   //40
    KEY_EN_PAUSE_BREAK,         //42
    KEY_EN_DELETE,              //43
    KEY_EN_MEDIA_PREVIOUS,      //44
    //"Key: Logo Left",         //047
    KEY_EN_F4,                  //48
    KEY_EN_4,                   //49
    KEY_EN_R,                   //50
    KEY_EN_F,                   //51
    KEY_EN_C,                   //52
    KEY_EN_SPACE,               //53
    KEY_EN_INSERT,              //54
    KEY_EN_END,                 //55
    KEY_EN_MEDIA_PLAY_PAUSE,    //56
    KEY_EN_NUMPAD_4,            //57
    //"Key: Logo Right",        //059
    KEY_EN_F5,                  //60
    KEY_EN_5,                   //61
    KEY_EN_T,                   //62
    KEY_EN_G,                   //63
    KEY_EN_V,                   //64
    KEY_EN_HOME,                //66
    KEY_EN_PAGE_DOWN,           //67
    KEY_EN_MEDIA_NEXT,          //68
    KEY_EN_NUMPAD_5,            //69
    KEY_EN_F6,                  //72
    KEY_EN_6,                   //73
    KEY_EN_Y,                   //74
    KEY_EN_H,                   //75
    KEY_EN_B,                   //76
    KEY_EN_PAGE_UP,             //78
    KEY_EN_RIGHT_SHIFT,         //79
    KEY_EN_NUMPAD_LOCK,         //80
    KEY_EN_NUMPAD_6,            //81
    KEY_EN_F7,                  //84
    KEY_EN_7,                   //85
    KEY_EN_U,                   //86
    KEY_EN_J,                   //87
    KEY_EN_N,                   //88
    KEY_EN_RIGHT_ALT,           //89
    KEY_EN_RIGHT_BRACKET,       //90
    KEY_EN_RIGHT_CONTROL,       //91
    KEY_EN_NUMPAD_DIVIDE,       //92
    KEY_EN_NUMPAD_1,            //93
    KEY_EN_F8,                  //96
    KEY_EN_8,                   //97
    KEY_EN_I,                   //98
    KEY_EN_K,                   //99
    KEY_EN_M,                   //100
    KEY_EN_RIGHT_WINDOWS,       //101
    KEY_EN_ANSI_BACK_SLASH,     //102
    KEY_EN_UP_ARROW,            //103
    KEY_EN_NUMPAD_TIMES,        //104
    KEY_EN_NUMPAD_2,            //105
    KEY_EN_F9,                  //108
    KEY_EN_9,                   //109
    KEY_EN_O,                   //110
    KEY_EN_L,                   //111
    KEY_EN_COMMA,               //112
    KEY_EN_MENU,                //113
    //KEY_EN_ISO_BACK_SLASH,    //114
    KEY_EN_LEFT_ARROW,          //115
    KEY_EN_NUMPAD_MINUS,        //116
    KEY_EN_NUMPAD_3,            //117
    KEY_EN_F10,                 //120
    KEY_EN_0,                   //121
    KEY_EN_P,                   //122
    KEY_EN_SEMICOLON,           //123
    //"Key: Profile",           //125
    KEY_EN_PERIOD,              //124
    KEY_EN_ANSI_ENTER,          //126
    KEY_EN_DOWN_ARROW,          //127
    KEY_EN_NUMPAD_PLUS,         //128
    KEY_EN_NUMPAD_0,            //129
    KEY_EN_F11,                 //132
    KEY_EN_MINUS,               //133
    KEY_EN_LEFT_BRACKET,        //134
    KEY_EN_QUOTE,               //135
    KEY_EN_FORWARD_SLASH,       //136
    "Key: Brightness",          //137
    KEY_EN_RIGHT_ARROW,         //139
    KEY_EN_NUMPAD_ENTER,        //140
    KEY_EN_NUMPAD_PERIOD,       //141
    "Key: / (ISO)",             //16
    KEY_EN_ISO_BACK_SLASH,      //114
    "Key: Logo Left",           //047
    "Key: Logo Right",          //059
    "Key: Profile",             //125
};

static const char* led_names_k95_plat[] =
{
    KEY_EN_ESCAPE,              //0
    KEY_EN_BACK_TICK,           //1
    KEY_EN_TAB,                 //2
    KEY_EN_CAPS_LOCK ,          //3
    KEY_EN_LEFT_SHIFT,          //4
    KEY_EN_LEFT_CONTROL,        //5
    KEY_EN_F12,                 //6
    KEY_EN_EQUALS,              //7
    "Key: Lock",                //8
    KEY_EN_NUMPAD_7,            //9
    KEY_EN_F1,                  //12
    KEY_EN_1,                   //13
    KEY_EN_Q,                   //14
    KEY_EN_A,                   //15
    KEY_EN_LEFT_WINDOWS,        //17
    KEY_EN_PRINT_SCREEN,        //18
    KEY_EN_MEDIA_MUTE,          //20
    KEY_EN_NUMPAD_8,            //21
    KEY_EN_F2,                  //24
    KEY_EN_2,                   //25
    KEY_EN_W,                   //26
    KEY_EN_S,                   //27
    KEY_EN_Z,                   //28
    KEY_EN_LEFT_ALT,            //29
    KEY_EN_SCROLL_LOCK,         //30
    KEY_EN_BACKSPACE,           //31
    KEY_EN_MEDIA_STOP,          //32
    KEY_EN_NUMPAD_9,            //33
    KEY_EN_F3,                  //36
    KEY_EN_3,                   //37
    KEY_EN_E,                   //38
    KEY_EN_D,                   //39
    KEY_EN_X,                   //40
    KEY_EN_PAUSE_BREAK,         //42
    KEY_EN_DELETE,              //43
    KEY_EN_MEDIA_PREVIOUS,      //44
    KEY_EN_F4,                  //48
    KEY_EN_4,                   //49
    KEY_EN_R,                   //50
    KEY_EN_F,                   //51
    KEY_EN_C,                   //52
    KEY_EN_SPACE,               //53
    KEY_EN_INSERT,              //54
    KEY_EN_END,                 //55
    KEY_EN_MEDIA_PLAY_PAUSE,    //56
    KEY_EN_NUMPAD_4,            //57
    KEY_EN_F5,                  //60
    KEY_EN_5,                   //61
    KEY_EN_T,                   //62
    KEY_EN_G,                   //63
    KEY_EN_V,                   //64
    KEY_EN_HOME,                //66
    KEY_EN_PAGE_DOWN,           //67
    KEY_EN_MEDIA_NEXT,          //68
    KEY_EN_NUMPAD_5,            //69
    KEY_EN_F6,                  //72
    KEY_EN_6,                   //73
    KEY_EN_Y,                   //74
    KEY_EN_H,                   //75
    KEY_EN_B,                   //76
    KEY_EN_PAGE_UP,             //78
    KEY_EN_RIGHT_SHIFT,         //79
    KEY_EN_NUMPAD_LOCK,         //80
    KEY_EN_NUMPAD_6,            //81
    KEY_EN_F7,                  //84
    KEY_EN_7,                   //85
    KEY_EN_U,                   //86
    KEY_EN_J,                   //87
    KEY_EN_N,                   //88
    KEY_EN_RIGHT_ALT,           //89
    KEY_EN_RIGHT_BRACKET,       //90
    KEY_EN_RIGHT_CONTROL,       //91
    KEY_EN_NUMPAD_DIVIDE,       //92
    KEY_EN_NUMPAD_1,            //93
    KEY_EN_F8,                  //96
    KEY_EN_8,                   //97
    KEY_EN_I,                   //98
    KEY_EN_K,                   //99
    KEY_EN_M,                   //100
    KEY_EN_RIGHT_WINDOWS,       //101
    KEY_EN_ANSI_BACK_SLASH,     //102
    KEY_EN_UP_ARROW,            //103
    KEY_EN_NUMPAD_TIMES,        //104
    KEY_EN_NUMPAD_2,            //105
    KEY_EN_F9,                  //108
    KEY_EN_9,                   //109
    KEY_EN_O,                   //110
    KEY_EN_L,                   //111
    KEY_EN_COMMA,               //112
    KEY_EN_MENU,                //113
    KEY_EN_LEFT_ARROW,          //115
    KEY_EN_NUMPAD_MINUS,        //116
    KEY_EN_NUMPAD_3,            //117
    KEY_EN_F10,                 //120
    KEY_EN_0,                   //121
    KEY_EN_P,                   //122
    KEY_EN_SEMICOLON,           //123
    KEY_EN_PERIOD,              //124
    KEY_EN_ANSI_ENTER,          //126
    KEY_EN_DOWN_ARROW,          //127
    KEY_EN_NUMPAD_PLUS,         //128
    KEY_EN_NUMPAD_0,            //129
    KEY_EN_F11,                 //132
    KEY_EN_MINUS,               //133
    KEY_EN_LEFT_BRACKET,        //134
    KEY_EN_QUOTE,               //135
    KEY_EN_FORWARD_SLASH,       //136
    "Key: Brightness",          //137
    KEY_EN_RIGHT_ARROW,         //139
    KEY_EN_NUMPAD_ENTER,        //140
    KEY_EN_NUMPAD_PERIOD,       //141
    "Key: / (ISO)",
    KEY_EN_ISO_BACK_SLASH,
    "Key: Macro G1",
    "Key: Macro G2",
    "Key: Macro G3",
    "Key: Macro G4",
    "Key: Macro G5",
    "Key: Macro G6",
    "Key: Preset",
    "Light Bar 1",
    "Light Bar 2",
    "Light Bar 3",
    "Light Bar 4",
    "Light Bar 5",
    "Light Bar 6",
    "Light Bar 7",
    "Light Bar 8",
    "Light Bar 9",
    "Light Bar 10",
    "Light Bar 11",
    "Light Bar 12",
    "Light Bar 13",
    "Light Bar 14",
    "Light Bar 15",
    "Light Bar 16",
    "Light Bar 17",
    "Light Bar 18",
    "Light Bar 19"
};

static const char* led_names_k95[] =
{
    KEY_EN_ESCAPE,              //0
    KEY_EN_BACK_TICK,           //1
    KEY_EN_TAB,                 //2
    KEY_EN_CAPS_LOCK,           //3
    KEY_EN_LEFT_SHIFT,          //4
    KEY_EN_LEFT_CONTROL,        //5
    KEY_EN_F12,                 //6
    KEY_EN_EQUALS,              //7
    "Key: Lock",                //8
    KEY_EN_NUMPAD_7,            //9
    KEY_EN_F1,                  //12
    KEY_EN_1,                   //13
    KEY_EN_Q,                   //14
    KEY_EN_A,                   //15
    KEY_EN_LEFT_WINDOWS,        //17
    KEY_EN_PRINT_SCREEN,        //18
    KEY_EN_MEDIA_MUTE,          //20
    KEY_EN_NUMPAD_8,            //21
    KEY_EN_F2,                  //24
    KEY_EN_2,                   //25
    KEY_EN_W,                   //26
    KEY_EN_S,                   //27
    KEY_EN_Z,                   //28
    KEY_EN_LEFT_ALT,            //29
    KEY_EN_SCROLL_LOCK,         //30
    KEY_EN_BACKSPACE,           //31
    KEY_EN_MEDIA_STOP,          //32
    KEY_EN_NUMPAD_9,            //33
    KEY_EN_F3,                  //36
    KEY_EN_3,                   //37
    KEY_EN_E,                   //38
    KEY_EN_D,                   //39
    KEY_EN_X,                   //40
    KEY_EN_PAUSE_BREAK,         //42
    KEY_EN_DELETE,              //43
    KEY_EN_MEDIA_PREVIOUS,      //44
    KEY_EN_F4,                  //48
    KEY_EN_4,                   //49
    KEY_EN_R,                   //50
    KEY_EN_F,                   //51
    KEY_EN_C,                   //52
    KEY_EN_SPACE,               //53
    KEY_EN_INSERT,              //54
    KEY_EN_END,                 //55
    KEY_EN_MEDIA_PLAY_PAUSE,    //56
    KEY_EN_NUMPAD_4,            //57
    KEY_EN_F5,                  //60
    KEY_EN_5,                   //61
    KEY_EN_T,                   //62
    KEY_EN_G,                   //63
    KEY_EN_V,                   //64
    KEY_EN_HOME,                //66
    KEY_EN_PAGE_DOWN,           //67
    KEY_EN_MEDIA_NEXT,          //68
    KEY_EN_NUMPAD_5,            //69
    KEY_EN_F6,                  //72
    KEY_EN_6,                   //73
    KEY_EN_Y,                   //74
    KEY_EN_H,                   //75
    KEY_EN_B,                   //76
    KEY_EN_PAGE_UP,             //78
    KEY_EN_RIGHT_SHIFT,         //79
    KEY_EN_NUMPAD_LOCK,         //80
    KEY_EN_NUMPAD_6,            //81
    KEY_EN_F7,                  //84
    KEY_EN_7,                   //85
    KEY_EN_U,                   //86
    KEY_EN_J,                   //87
    KEY_EN_N,                   //88
    KEY_EN_RIGHT_ALT,           //89
    KEY_EN_RIGHT_BRACKET,       //90
    KEY_EN_RIGHT_CONTROL,       //91
    KEY_EN_NUMPAD_DIVIDE,       //92
    KEY_EN_NUMPAD_1,            //93
    KEY_EN_F8,                  //96
    KEY_EN_8,                   //97
    KEY_EN_I,                   //98
    KEY_EN_K,                   //99
    KEY_EN_M,                   //100
    KEY_EN_RIGHT_WINDOWS,       //101
    KEY_EN_ANSI_BACK_SLASH,     //102
    KEY_EN_UP_ARROW,            //103
    KEY_EN_NUMPAD_TIMES,        //104
    KEY_EN_NUMPAD_2,            //105
    KEY_EN_F9,                  //108
    KEY_EN_9,                   //109
    KEY_EN_O,                   //110
    KEY_EN_L,                   //111
    KEY_EN_COMMA,               //112
    KEY_EN_MENU,                //113
    KEY_EN_LEFT_ARROW,          //115
    KEY_EN_NUMPAD_MINUS,        //116
    KEY_EN_NUMPAD_3,            //117
    KEY_EN_F10,                 //120
    KEY_EN_0,                   //121
    KEY_EN_P,                   //122
    KEY_EN_SEMICOLON,           //123
    KEY_EN_PERIOD,              //124
    KEY_EN_ANSI_ENTER,          //126
    KEY_EN_DOWN_ARROW,          //127
    KEY_EN_NUMPAD_PLUS,         //128
    KEY_EN_NUMPAD_0,            //129
    KEY_EN_F11,                 //132
    KEY_EN_MINUS,               //133
    KEY_EN_LEFT_BRACKET,        //134
    KEY_EN_QUOTE,               //135
    KEY_EN_FORWARD_SLASH,       //136
    "Key: Brightness",          //137
    KEY_EN_RIGHT_ARROW,         //139
    KEY_EN_NUMPAD_ENTER,        //140
    KEY_EN_NUMPAD_PERIOD,       //141
    "Key: / (ISO)",
    KEY_EN_ISO_BACK_SLASH,
    "Key: Macro G1",
    "Key: Macro G2",
    "Key: Macro G3",
    "Key: Macro G4",
    "Key: Macro G5",
    "Key: Macro G6",
    "Key: Macro G7",
    "Key: Macro G8",
    "Key: Macro G9",
    "Key: Macro G10",
    "Key: Macro G11",
    "Key: Macro G12",
    "Key: Macro G13",
    "Key: Macro G14",
    "Key: Macro G15",
    "Key: Macro G16",
    "Key: Macro G17",
    "Key: Macro G18",
    "Key: MR",
    "Key: M1",
    "Key: M2",
    "Key: M3",
};

static const char* corsair_mouse_leds[] =
{
    "Mouse LED 1",
    "Mouse LED 2",
    "Mouse LED 3",
    "Mouse LED 4",
    "Mouse LED 5",
    "Mouse LED 6",
    "Mouse LED 7",
    "Mouse LED 8",
    "Mouse LED 9",
    "Mouse LED 10",
    "Mouse LED 11",
    "Mouse LED 12",
    "Mouse LED 13",
    "Mouse LED 14",
    "Mouse LED 15",
};

static const char* led_names_k55[] =
{
    "LEFT",
    "MIDDLE",
    "RIGHT",
};

static const char* corsair_m65_elite_leds[] =
{
    "",
    "",
    "Logo",
    "DPI",
    "Scroll Wheel",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

static const char* corsair_sabre_rgb_leds[] =
{
    "",
    "Underglow",
    "Logo",
    "DPI",
    "Scroll Wheel",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    ""
};

static const char* corsair_harpoon_pro_leds[] =
{
    "",
    "",
    "",
    "Logo",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

/**------------------------------------------------------------------*\
    @name Corsair Peripheral
    @category Keyboard,Mouse,Mousemat,HeadsetStand
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :tools:
    @detectors DetectCorsairPeripheralControllers
    @comment
    All controllers support `Direct` mode
    Currently HW modes are implemented for the following devices:
    * Corsair K70 RGB MK.2
    * Corsair K70 RGB MK.2 Low Profile
\*-------------------------------------------------------------------*/

RGBController_CorsairPeripheral::RGBController_CorsairPeripheral(CorsairPeripheralController* controller_ptr, bool supports_hardware_modes)
{
    controller      = controller_ptr;

    name            = controller->GetName();
    vendor          = "Corsair";
    description     = "Corsair RGB Peripheral Device";
    type            = controller->GetDeviceType();
    version         = controller->GetFirmwareString();
    location        = controller->GetDeviceLocation();
    serial          = controller->GetSerialString();

    physical_layout = controller->GetPhysicalLayout();
    logical_layout  = controller->GetLogicalLayout();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = CORSAIR_MODE_DIRECT_VALUE;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    if(supports_hardware_modes)
    {
        mode ColorPulse;
        ColorPulse.name                 = "ColorPulse";
        ColorPulse.value                = CORSAIR_HW_MODE_COLOR_PULSE_VALUE;
        ColorPulse.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        ColorPulse.color_mode           = MODE_COLORS_RANDOM;
        ColorPulse.speed                = CORSAIR_HW_MODE_SPEED_MIN;
        ColorPulse.speed_min            = CORSAIR_HW_MODE_SPEED_MIN;
        ColorPulse.speed_max            = CORSAIR_HW_MODE_SPEED_MAX;
        ColorPulse.brightness           = CORSAIR_HW_MODE_BRIGHTNESS_MAX / 2;
        ColorPulse.brightness_min       = CORSAIR_HW_MODE_BRIGHTNESS_MIN;
        ColorPulse.brightness_max       = CORSAIR_HW_MODE_BRIGHTNESS_MAX;
        ColorPulse.colors.resize(2);
        modes.push_back(ColorPulse);

        mode ColorShift;
        ColorShift.name                 = "ColorShift";
        ColorShift.value                = CORSAIR_HW_MODE_COLOR_SHIFT_VALUE;
        ColorShift.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        ColorShift.color_mode           = MODE_COLORS_RANDOM;
        ColorShift.speed                = CORSAIR_HW_MODE_SPEED_MIN;
        ColorShift.speed_min            = CORSAIR_HW_MODE_SPEED_MIN;
        ColorShift.speed_max            = CORSAIR_HW_MODE_SPEED_MAX;
        ColorShift.brightness           = CORSAIR_HW_MODE_BRIGHTNESS_MAX / 2;
        ColorShift.brightness_min       = CORSAIR_HW_MODE_BRIGHTNESS_MIN;
        ColorShift.brightness_max       = CORSAIR_HW_MODE_BRIGHTNESS_MAX;
        ColorShift.colors.resize(2);
        modes.push_back(ColorShift);

        mode ColorWave;
        ColorWave.name                  = "ColorWave";
        ColorWave.value                 = CORSAIR_HW_MODE_COLOR_WAVE_VALUE;
        ColorWave.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        ColorWave.color_mode            = MODE_COLORS_RANDOM;
        ColorWave.direction             = MODE_DIRECTION_LEFT;
        ColorWave.speed                 = CORSAIR_HW_MODE_SPEED_MIN;
        ColorWave.speed_min             = CORSAIR_HW_MODE_SPEED_MIN;
        ColorWave.speed_max             = CORSAIR_HW_MODE_SPEED_MAX;
        ColorWave.brightness            = CORSAIR_HW_MODE_BRIGHTNESS_MAX / 2;
        ColorWave.brightness_min        = CORSAIR_HW_MODE_BRIGHTNESS_MIN;
        ColorWave.brightness_max        = CORSAIR_HW_MODE_BRIGHTNESS_MAX;
        ColorWave.colors.resize(2);
        modes.push_back(ColorWave);

        mode RainbowWave;
        RainbowWave.name                = "RainbowWave";
        RainbowWave.value               = CORSAIR_HW_MODE_RAINBOW_WAVE_VALUE;
        RainbowWave.flags               = MODE_FLAG_HAS_SPEED |  MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        RainbowWave.color_mode          = MODE_COLORS_NONE;
        RainbowWave.direction           = MODE_DIRECTION_LEFT;
        RainbowWave.speed               = CORSAIR_HW_MODE_SPEED_MIN;
        RainbowWave.speed_min           = CORSAIR_HW_MODE_SPEED_MIN;
        RainbowWave.speed_max           = CORSAIR_HW_MODE_SPEED_MAX;
        RainbowWave.brightness          = CORSAIR_HW_MODE_BRIGHTNESS_MAX / 2;
        RainbowWave.brightness_min      = CORSAIR_HW_MODE_BRIGHTNESS_MIN;
        RainbowWave.brightness_max      = CORSAIR_HW_MODE_BRIGHTNESS_MAX;
        modes.push_back(RainbowWave);

        mode Rain;
        Rain.name                       = "Rain";
        Rain.value                      = CORSAIR_HW_MODE_RAIN_VALUE;
        Rain.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        Rain.color_mode                 = MODE_COLORS_RANDOM;
        Rain.speed                      = CORSAIR_HW_MODE_SPEED_MIN;
        Rain.speed_min                  = CORSAIR_HW_MODE_SPEED_MIN;
        Rain.speed_max                  = CORSAIR_HW_MODE_SPEED_MAX;
        Rain.brightness                 = CORSAIR_HW_MODE_BRIGHTNESS_MAX / 2;
        Rain.brightness_min             = CORSAIR_HW_MODE_BRIGHTNESS_MIN;
        Rain.brightness_max             = CORSAIR_HW_MODE_BRIGHTNESS_MAX;
        Rain.colors.resize(2);
        modes.push_back(Rain);

        mode Spiral;
        Spiral.name                     = "Spiral";
        Spiral.value                    = CORSAIR_HW_MODE_SPIRAL_VALUE;
        Spiral.flags                    = MODE_FLAG_HAS_SPEED |  MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        Spiral.color_mode               = MODE_COLORS_NONE;
        Spiral.speed                    = CORSAIR_HW_MODE_SPEED_MIN;
        Spiral.speed_min                = CORSAIR_HW_MODE_SPEED_MIN;
        Spiral.speed_max                = CORSAIR_HW_MODE_SPEED_MAX;
        Spiral.brightness               = CORSAIR_HW_MODE_BRIGHTNESS_MAX / 2;
        Spiral.brightness_min           = CORSAIR_HW_MODE_BRIGHTNESS_MIN;
        Spiral.brightness_max           = CORSAIR_HW_MODE_BRIGHTNESS_MAX;
        Spiral.direction                = MODE_DIRECTION_LEFT;
        modes.push_back(Spiral);

        mode TypeKey;
        TypeKey.name                    = "TypeKey";
        TypeKey.value                   = CORSAIR_HW_MODE_TYPE_KEY_VALUE;
        TypeKey.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        TypeKey.color_mode              = MODE_COLORS_RANDOM;
        TypeKey.speed                   = CORSAIR_HW_MODE_SPEED_MIN;
        TypeKey.speed_min               = CORSAIR_HW_MODE_SPEED_MIN;
        TypeKey.speed_max               = CORSAIR_HW_MODE_SPEED_MAX;
        TypeKey.brightness              = CORSAIR_HW_MODE_BRIGHTNESS_MAX / 2;
        TypeKey.brightness_min          = CORSAIR_HW_MODE_BRIGHTNESS_MIN;
        TypeKey.brightness_max          = CORSAIR_HW_MODE_BRIGHTNESS_MAX;
        TypeKey.colors.resize(2);
        modes.push_back(TypeKey);

        mode TypeRipple;
        TypeRipple.name                 = "TypeRipple";
        TypeRipple.value                = CORSAIR_HW_MODE_TYPE_RIPPLE_VALUE;
        TypeRipple.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        TypeRipple.color_mode           = MODE_COLORS_RANDOM;
        TypeRipple.speed                = CORSAIR_HW_MODE_SPEED_MIN;
        TypeRipple.speed_min            = CORSAIR_HW_MODE_SPEED_MIN;
        TypeRipple.speed_max            = CORSAIR_HW_MODE_SPEED_MAX;
        TypeRipple.brightness           = CORSAIR_HW_MODE_BRIGHTNESS_MAX / 2;
        TypeRipple.brightness_min       = CORSAIR_HW_MODE_BRIGHTNESS_MIN;
        TypeRipple.brightness_max       = CORSAIR_HW_MODE_BRIGHTNESS_MAX;
        TypeRipple.colors.resize(2);
        modes.push_back(TypeRipple);

        mode Visor;
        Visor.name                      = "Visor";
        Visor.value                     = CORSAIR_HW_MODE_VISOR_VALUE;
        Visor.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        Visor.color_mode                = MODE_COLORS_RANDOM;
        Visor.speed                     = CORSAIR_HW_MODE_SPEED_MIN;
        Visor.speed_min                 = CORSAIR_HW_MODE_SPEED_MIN;
        Visor.speed_max                 = CORSAIR_HW_MODE_SPEED_MAX;
        Visor.brightness                = CORSAIR_HW_MODE_BRIGHTNESS_MAX / 2;
        Visor.brightness_min            = CORSAIR_HW_MODE_BRIGHTNESS_MIN;
        Visor.brightness_max            = CORSAIR_HW_MODE_BRIGHTNESS_MAX;
        Visor.colors.resize(2);
        modes.push_back(Visor);
    }


    SetupZones();
}

RGBController_CorsairPeripheral::~RGBController_CorsairPeripheral()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_CorsairPeripheral::SetupZones()
{
    /*---------------------------------------------------------*\
    | Determine number of zones                                 |
    |   For now, keyboard has 2 zones and mousemat has 1        |
    \*---------------------------------------------------------*/
    unsigned int num_zones = 0;

    switch(type)
    {
        case DEVICE_TYPE_KEYBOARD:
            if (logical_layout == CORSAIR_TYPE_K95_PLAT)
            {
                num_zones = 2;
                break;
            }
            if (logical_layout == CORSAIR_TYPE_K55)
            {
                num_zones = 3;
                break;
            }
            num_zones = 1;
            break;

        case DEVICE_TYPE_MOUSE:
        case DEVICE_TYPE_MOUSEMAT:
            num_zones = 1;
            break;

        case DEVICE_TYPE_HEADSET_STAND:
            num_zones = 2;
            break;
    }

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < num_zones; zone_idx++)
    {
        zone new_zone;
        switch(type)
        {
            case DEVICE_TYPE_KEYBOARD:
                if (logical_layout == CORSAIR_TYPE_K95_PLAT)
                {
                    new_zone.name                   = zone_names_k95_platinum[zone_idx];
                    new_zone.type                   = zone_types_k95_platinum[zone_idx];
                    new_zone.leds_min               = zone_sizes_k95_platinum[zone_idx];
                    new_zone.leds_max               = zone_sizes_k95_platinum[zone_idx];
                    new_zone.leds_count             = zone_sizes_k95_platinum[zone_idx];

                    if(zone_types[zone_idx] == ZONE_TYPE_MATRIX)
                    {
                        new_zone.matrix_map         = new matrix_map_type;
                        new_zone.matrix_map->height = 7;
                        new_zone.matrix_map->width  = 24;
                        new_zone.matrix_map->map    = (unsigned int *)&matrix_map_k95_platinum;
                    }
                    else
                    {
                        new_zone.matrix_map         = NULL;
                    }
                }
                else if (logical_layout == CORSAIR_TYPE_K95)
                {
                    new_zone.name                   = zone_names_k95[zone_idx];
                    new_zone.type                   = zone_types_k95[zone_idx];
                    new_zone.leds_min               = zone_sizes_k95[zone_idx];
                    new_zone.leds_max               = zone_sizes_k95[zone_idx];
                    new_zone.leds_count             = zone_sizes_k95[zone_idx];

                    if(zone_types[zone_idx] == ZONE_TYPE_MATRIX)
                    {
                        new_zone.matrix_map         = new matrix_map_type;
                        new_zone.matrix_map->height = 7;
                        new_zone.matrix_map->width  = 26;
                        new_zone.matrix_map->map    = (unsigned int *)&matrix_map_k95;
                    }
                    else
                    {
                        new_zone.matrix_map         = NULL;
                    }
                }
                else if (logical_layout == CORSAIR_TYPE_K55)
                {
                    new_zone.name                   = zone_names_k55[zone_idx];
                    new_zone.type                   = zone_types_k55[zone_idx];
                    new_zone.leds_min               = zone_sizes_k55[zone_idx];
                    new_zone.leds_max               = zone_sizes_k55[zone_idx];
                    new_zone.leds_count             = zone_sizes_k55[zone_idx];
                    new_zone.matrix_map             = NULL;
                }
                else if (logical_layout == CORSAIR_TYPE_K70_MK2)
                {
                    new_zone.name                   = zone_names_k70_mk2[zone_idx];
                    new_zone.type                   = zone_types_k70_mk2[zone_idx];
                    new_zone.leds_min               = zone_sizes_k70_mk2[zone_idx];
                    new_zone.leds_max               = zone_sizes_k70_mk2[zone_idx];
                    new_zone.leds_count             = zone_sizes_k70_mk2[zone_idx];

                    if(zone_types[zone_idx] == ZONE_TYPE_MATRIX)
                    {
                        new_zone.matrix_map         = new matrix_map_type;
                        new_zone.matrix_map->height = 7;
                        new_zone.matrix_map->width  = 23;
                        new_zone.matrix_map->map    = (unsigned int *)&matrix_map_k70_mk2;
                    }
                    else
                    {
                        new_zone.matrix_map         = NULL;
                    }
                }
                else //default layout
                {
                    new_zone.name                   = zone_names[zone_idx];
                    new_zone.type                   = zone_types[zone_idx];
                    new_zone.leds_min               = zone_sizes[zone_idx];
                    new_zone.leds_max               = zone_sizes[zone_idx];
                    new_zone.leds_count             = zone_sizes[zone_idx];

                    if(zone_types[zone_idx] == ZONE_TYPE_MATRIX)
                    {
                        new_zone.matrix_map         = new matrix_map_type;
                        new_zone.matrix_map->height = 6;
                        new_zone.matrix_map->width  = 23;
                        new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
                    }
                    else
                    {
                        new_zone.matrix_map         = NULL;
                    }
                }
                break;


            case DEVICE_TYPE_MOUSE:
                new_zone.name           = "Mouse Zone";
                new_zone.type           = ZONE_TYPE_SINGLE;
                new_zone.leds_min       = 15;
                new_zone.leds_max       = 15;
                new_zone.leds_count     = 15;
                new_zone.matrix_map     = NULL;
                break;

            case DEVICE_TYPE_MOUSEMAT:
                new_zone.name           = "Mousemat Zone";
                new_zone.type           = ZONE_TYPE_LINEAR;
                new_zone.leds_min       = 15;
                new_zone.leds_max       = 15;
                new_zone.leds_count     = 15;
                new_zone.matrix_map     = NULL;
                break;

            case DEVICE_TYPE_HEADSET_STAND:
                if(zone_idx == 0)
                {
                    new_zone.name           = "Base LED Strip";
                    new_zone.type           = ZONE_TYPE_LINEAR;
                    new_zone.leds_min       = 8;
                    new_zone.leds_max       = 8;
                    new_zone.leds_count     = 8;
                    new_zone.matrix_map     = NULL;
                }
                else
                {
                    new_zone.name           = "Logo";
                    new_zone.type           = ZONE_TYPE_SINGLE;
                    new_zone.leds_min       = 1;
                    new_zone.leds_max       = 1;
                    new_zone.leds_count     = 1;
                    new_zone.matrix_map     = NULL;
                }
                break;
        }

        zones.push_back(new_zone);

        total_led_count += new_zone.leds_count;
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;

        switch(type)
        {
            case DEVICE_TYPE_KEYBOARD:
                if(logical_layout == CORSAIR_TYPE_K95_PLAT)
                {
                    new_led.name = led_names_k95_plat[led_idx];
                }
                else if(logical_layout == CORSAIR_TYPE_K95)
                {
                    new_led.name = led_names_k95[led_idx];
                }
                else if(logical_layout == CORSAIR_TYPE_K55)
                {
                    new_led.name = led_names_k55[led_idx];
                }
                else if(logical_layout == CORSAIR_TYPE_K70_MK2)
                {
                    new_led.name = led_names_k70_mk2[led_idx];
                }
                else
                {
                    new_led.name = led_names[led_idx];
                }
                break;

            case DEVICE_TYPE_MOUSE:
                if(name == "Corsair M65 RGB Elite")
                {
                    new_led.name = corsair_m65_elite_leds[led_idx];
                }
                else if(name == "Corsair Harpoon RGB PRO")
                {
                    new_led.name = corsair_harpoon_pro_leds[led_idx];
                }
                if(name == "Corsair Ironclaw RGB")
                {
                    new_led.name = corsair_m65_elite_leds[led_idx];
                }
                else if(name == "Corsair Sabre RGB")
                {
                    new_led.name = corsair_sabre_rgb_leds[led_idx];
                }
                else
                {
                    new_led.name = corsair_mouse_leds[led_idx];
                }
                break;

            case DEVICE_TYPE_MOUSEMAT:
            case DEVICE_TYPE_HEADSET_STAND:
                new_led.name = "Mousemat LED ";
                new_led.name.append(std::to_string(led_idx + 1));
                break;
        }

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_CorsairPeripheral::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairPeripheral::DeviceUpdateLEDs()
{
    controller->SetLEDs(colors);
}

void RGBController_CorsairPeripheral::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetLEDs(colors);
}

void RGBController_CorsairPeripheral::UpdateSingleLED(int /*led*/)
{
    controller->SetLEDs(colors);
}

void RGBController_CorsairPeripheral::DeviceUpdateMode()
{

    if(modes[active_mode].value == CORSAIR_MODE_DIRECT_VALUE)
    {
        controller->SwitchMode(true);
    }
    else
    {
        const mode& active = modes[active_mode];

        unsigned int direction = active.direction;

        if(active.flags & MODE_FLAG_HAS_DIRECTION_LR || active.flags & MODE_FLAG_HAS_DIRECTION_UD)
        {
            direction += 1;

            if(active.value == CORSAIR_HW_MODE_SPIRAL_VALUE)
            {
                direction += 4;
            }
        }

        controller->SetHardwareMode(active.value, active.color_mode, active.colors, active.speed, direction, active.brightness);

        controller->SwitchMode(false);
    }

}
