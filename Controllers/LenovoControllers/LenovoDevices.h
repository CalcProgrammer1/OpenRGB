/*-------------------------------------------------------------------*\
|  LenovoDevices.h                                                    |
|                                                                     |
|  Describes zones for various Lenovo Legion Devices                  |
|                                                                     |
|  Cooper Hall (geobot19)          17 Apr 2022                        |
|                                                                     |
| Note: additions here must be adeed to RGBController_LenovoUSB.cpp in|
| the switch statements which are on lines 28 and 60 at time of       |
| writing                                                             |
\*-------------------------------------------------------------------*/
 #include <string>
 #include "RGBControllerKeyNames.h"
 #include "RGBController.h"

#define NA 0xFFFFFFFF
#ifndef LENOVODEVICES_H
#define LENOVODEVICES_H
/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define LEGION_Y740                             0xC935
#define LEGION_Y750                             0xC956
#define LEGION_Y750S                            0xC957
#define LEGION_Y760                             0xC968
#define LEGION_Y760S                            0xC967

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
    {  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  NA,  NA,  16,  17,  18,  19,
      20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  NA,  NA,  NA,  NA,  53,  33,  34,  35,  36,
      37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  NA,  NA,  68,  69,  70,  NA,
      54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  90,  65,  91,  66,  92,  67,  NA,  93,  94,  95,  71,
      72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  85,  84,  87,  86,  88,  89, 111, 113, 114,  96,
      97,  98,  99, 100, 101, 102, 103, 104, 105, 116, 106, 117, 107, 118, 108, 109, 110,  NA, 112,  NA, 115,  NA,
     126, 127, 128, 129, 130,  NA,  NA,  NA,  NA, 131, 132, 133,  NA,  NA,  NA, 136,  NA,  NA, 119, 121, 123, 125,
      NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 135,  NA,  NA, 120, 122, 124, 144,
      NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 134, 137, 139,  NA, 140, 141, 143,  NA,
      NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 138,  NA,  NA,  NA,  NA, 142,  NA };

static const unsigned int legion_Y760_iso_leds_map[] =
    {  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  NA,  NA,  16,  17,  18,  19,
      20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  NA,  NA,  NA,  NA,  51,  33,  34,  35,  36,
      37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  NA,  NA,  50,  NA,  NA,  66,  67,  68,  NA,
      52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  88,  63,  89,  64,  NA,  NA,  65,  90,  91,  92,  69,
      70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  83,  82,  85,  84,  87,  86, 109, 111, 112,  93,
      94,  96,  95,  97,  98,  99, 100, 101, 102, 103, 114, 104, 115, 105, 116, 106, 107, 108, 110,  NA, 113,  NA,
     124, 125, 126, 127, 128,  NA,  NA,  NA,  NA, 129, 130, 131,  NA,  NA,  NA, 134,  NA,  NA, 117, 119, 121, 123,
      NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 133,  NA,  NA, 118, 120, 122, 142,
      NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 132, 135, 137,  NA, 138, 139, 141,  NA,
      NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 136,  NA,  NA,  NA,  NA, 140,  NA };

static const unsigned int legion_Y760_jp_leds_map[] =
    {  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  NA,  NA,  16,  17,  18,  19,
      20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  NA,  NA,  NA,  54,  34,  35,  36,  37,
      38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  NA,  NA,  71,  72,  73,  NA,
      55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  NA,  91,  70,  94,  95,  96,  74,
      75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  92,  93, 116, 118, 119,  97,
      98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 117,  NA, 120,  NA,
     128, 129, 130, 131, 132, 133,  NA,  NA, 134, 135, 136, 137,  NA,  NA,  NA, 139,  NA,  NA, 121, 123, 125, 127,
      NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 140,  NA,  NA, 122, 124, 126, 148,
      NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 138, 141, 143,  NA, 144, 145, 146,  NA,
      NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 142,  NA,  NA,  NA,  NA, 147,  NA };

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
    {0x19, "Key: #"},//23
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
    {0xa2, KEY_EN_RIGHT_SHIFT},//110
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
    {0xa7, KEY_EN_NUMPAD_ENTER},//125

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
    {0x17, "Key: ¬"},//20
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
    {0xa2, KEY_EN_RIGHT_SHIFT},//108
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
    {0xa7, KEY_EN_NUMPAD_ENTER},//123

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
    {0x16, "Key: halfwidth"}, //20
    {0x17, "Key: !"}, //21
    {0x18, "Key: \""}, //22
    {0x19, "Key: #"}, //23
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
    {0x2B, "Key: fullwidth"}, //38
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
    {0x37, "Key: ~"}, //50
    {0x38, "Key: Yen"}, //51
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
    {0x61, "Key: @"}, //67
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
    {0x60, "Key: :"}, //90
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
    {0x8C, KEY_EN_ANSI_BACK_SLASH}, //113
    {0x8D, KEY_EN_RIGHT_SHIFT}, //114
    {0xa2, KEY_EN_RIGHT_SHIFT}, //115
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
    {0xa7, KEY_EN_NUMPAD_ENTER}, //127

    //row 8
    {0x7F, KEY_EN_LEFT_CONTROL}, //128
    {0x80, KEY_EN_LEFT_FUNCTION}, //129
    {0x96, KEY_EN_LEFT_WINDOWS}, //130
    {0x97, KEY_EN_LEFT_ALT}, //131
    {0x98, "Key: Muhenkan"}, //132
    {0x85, KEY_EN_SPACE}, //133
    {0x86, KEY_EN_SPACE}, //134
    {0x99, "Key: Henkan"}, //135
    {0x9A, "Key: Katakana/Hiragana/Rōmaji"}, //136
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
    10,
    22,
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
    10,
    22,
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
    10,
    22,
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


#endif
