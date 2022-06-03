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

#ifndef LENOVODEVICES_H
#define LENOVODEVICES_H
/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define LEGION_7_6_PID_1                        0xC968
#define LEGION_7_6_PID_2                        0xC957
#define LEGION_7_6_PID_3                        0xC956

struct lenovo_led
{
    uint8_t zone_num;
    uint8_t led_num;
    std::string name;
};

/*---------*\
| LED MAPS  |
\*---------*/
const lenovo_led legion_7_g_6_ansi_leds[]
{
    /*----------------*\
    | zone 1, keyboard |
    \*----------------*/
    //row 1
    {1, 0x01, KEY_EN_ESCAPE},//0
    {1, 0x02, KEY_EN_F1},//1
    {1, 0x03, KEY_EN_F2},//2
    {1, 0x04, KEY_EN_F3},//3
    {1, 0x05, KEY_EN_F4},//4
    {1, 0x06, KEY_EN_F5},//5
    {1, 0x07, KEY_EN_F6},//6
    {1, 0x08, KEY_EN_F7},//7
    {1, 0x09, KEY_EN_F8},//8
    {1, 0x0A, KEY_EN_F9},//9
    {1, 0x0B, KEY_EN_F10},//10
    {1, 0x0C, KEY_EN_F11},//11
    {1, 0x0D, KEY_EN_F12},//12
    {1, 0x0E, KEY_EN_INSERT},//13
    {1, 0x0F, KEY_EN_PRINT_SCREEN},//14
    {1, 0x10, KEY_EN_DELETE},//15
    {1, 0x11, KEY_EN_HOME},//16
    {1, 0x12, KEY_EN_END},//17
    {1, 0x13, KEY_EN_PAGE_UP},//18
    {1, 0x14, KEY_EN_PAGE_DOWN},//19

    //row 2
    {1, 0x16, "Key: ~"},//20
    {1, 0x17, "Key: !"},//21
    {1, 0x18, "Key: @"},//22
    {1, 0x19, "Key: #"},//23
    {1, 0x1A, "Key: $"},//24
    {1, 0x1B, "Key: %"},//25
    {1, 0x1C, "Key: ^"},//26
    {1, 0x1D, "Key: &"},//27
    {1, 0x1E, "Key: *"},//28
    {1, 0x1F, "Key: ("},//29
    {1, 0x20, "Key: )"},//30
    {1, 0x21, "Key: _"},//31
    {1, 0x22, "Key: +"},//32
    {1, 0x26, KEY_EN_NUMPAD_LOCK},//33
    {1, 0x27, KEY_EN_NUMPAD_DIVIDE},//34
    {1, 0x28, KEY_EN_NUMPAD_TIMES},//35
    {1, 0x29, KEY_EN_NUMPAD_MINUS},//36

    //row 3
    {1, 0x2B, KEY_EN_BACK_TICK},//37
    {1, 0x2C, KEY_EN_1},//38
    {1, 0x2D, KEY_EN_2},//39
    {1, 0x2E, KEY_EN_3},//40
    {1, 0x2F, KEY_EN_4},//41
    {1, 0x30, KEY_EN_5},//42
    {1, 0x31, KEY_EN_6},//43
    {1, 0x32, KEY_EN_7},//44
    {1, 0x33, KEY_EN_8},//45
    {1, 0x34, KEY_EN_9},//46
    {1, 0x35, KEY_EN_0},//47
    {1, 0x36, KEY_EN_MINUS},//48
    {1, 0x37, KEY_EN_EQUALS},//49
    {1, 0x38, KEY_EN_BACKSPACE},//50
    {1, 0x39, KEY_EN_BACKSPACE},//51
    {1, 0x3A, KEY_EN_BACKSPACE},//52
    {1, 0x3B, KEY_EN_NUMPAD_LOCK},//53

    //row 4
    {1, 0x40, KEY_EN_TAB},//54
    {1, 0x42, KEY_EN_Q},//55
    {1, 0x43, KEY_EN_W},//56
    {1, 0x44, KEY_EN_E},//57
    {1, 0x45, KEY_EN_R},//58
    {1, 0x46, KEY_EN_T},//59
    {1, 0x47, KEY_EN_Y},//60
    {1, 0x48, KEY_EN_U},//61
    {1, 0x49, KEY_EN_I},//62
    {1, 0x4A, KEY_EN_O},//63
    {1, 0x4B, KEY_EN_P},//64
    {1, 0x4C, "Key: {"},//65
    {1, 0x4D, "Key: }"},//66
    {1, 0x4E, "Key: |"},//67
    {1, 0x4F, KEY_EN_NUMPAD_7},//68
    {1, 0x50, KEY_EN_NUMPAD_8},//69
    {1, 0x51, KEY_EN_NUMPAD_9},//70
    {1, 0x67, KEY_EN_NUMPAD_PLUS},//71

    //row 5
    {1, 0x55, KEY_EN_CAPS_LOCK},//72
    {1, 0x56, KEY_EN_CAPS_LOCK},//73
    {1, 0x57, KEY_EN_CAPS_LOCK},//74
    {1, 0x6D, KEY_EN_A},//75
    {1, 0x6E, KEY_EN_S},//76
    {1, 0x58, KEY_EN_D},//77
    {1, 0x59, KEY_EN_F},//78
    {1, 0x5A, KEY_EN_G},//79
    {1, 0x71, KEY_EN_H},//80
    {1, 0x72, KEY_EN_J},//81
    {1, 0x5B, KEY_EN_K},//82
    {1, 0x5C, KEY_EN_L},//83
    {1, 0x5D, "Key: :"},//84
    {1, 0x5E, KEY_EN_SEMICOLON},//85
    {1, 0x5F, "Key: \""},//86
    {1, 0x60, KEY_EN_QUOTE},//87
    {1, 0x77, KEY_EN_ANSI_ENTER},//88
    {1, 0x78, KEY_EN_ANSI_ENTER},//89
    {1, 0x61, KEY_EN_LEFT_BRACKET},//90
    {1, 0x62, KEY_EN_RIGHT_BRACKET},//91
    {1, 0x63, KEY_EN_ANSI_BACK_SLASH},//92
    {1, 0x64, "Key: Numpad Home"},//93
    {1, 0x65, "Key: Numpad Up Arrow"},//94
    {1, 0x66, "Key: Numpad PgUP"},//95
    {1, 0x68, KEY_EN_NUMPAD_PLUS},//96

    //row 6
    {1, 0x6A, KEY_EN_LEFT_SHIFT},//97
    {1, 0x6B, KEY_EN_LEFT_SHIFT},//98
    {1, 0x82, KEY_EN_Z},//99
    {1, 0x83, KEY_EN_X},//100
    {1, 0x6F, KEY_EN_C},//101
    {1, 0x70, KEY_EN_V},//102
    {1, 0x87, KEY_EN_B},//103
    {1, 0x88, KEY_EN_N},//104
    {1, 0x73, KEY_EN_M},//105
    {1, 0x74, "Key: <"},//106
    {1, 0x75, "Key: >"},//107
    {1, 0x76, "Key: ?"},//108
    {1, 0x8D, KEY_EN_RIGHT_SHIFT},//109
    {1, 0xa2, KEY_EN_RIGHT_SHIFT},//110
    {1, 0x79, KEY_EN_NUMPAD_4},//111
    {1, 0x7A, "Key: Numpad Right Arrow"},//112
    {1, 0x7B, KEY_EN_NUMPAD_5},//113
    {1, 0x7C, KEY_EN_NUMPAD_6},//114
    {1, 0x7D, "Key: Numpad Left Arrow"},//115

    //row 7
    {1, 0x89, KEY_EN_COMMA},//116
    {1, 0x8A, KEY_EN_PERIOD},//117
    {1, 0x8B, KEY_EN_FORWARD_SLASH},//118
    {1, 0x8E, KEY_EN_NUMPAD_1},//119
    {1, 0x8F, "Key: Numpad End"},//120
    {1, 0x90, KEY_EN_NUMPAD_2},//121
    {1, 0x91, "Key: Numpad Down Arrow"},//122
    {1, 0x92, KEY_EN_NUMPAD_3},//123
    {1, 0x93, "Key: Numpad PgDn"},//124
    {1, 0xa7, KEY_EN_NUMPAD_ENTER},//125

    //row 8
    {1, 0x7F, KEY_EN_LEFT_CONTROL},//126
    {1, 0x80, KEY_EN_LEFT_FUNCTION},//127
    {1, 0x96, KEY_EN_LEFT_WINDOWS},//128
    {1, 0x97, KEY_EN_LEFT_ALT},//129
    {1, 0x98, KEY_EN_SPACE},//130
    {1, 0x99, KEY_EN_SPACE},//131
    {1, 0x9A, KEY_EN_RIGHT_ALT},//132
    {1, 0x9B, KEY_EN_RIGHT_CONTROL},//133
    {1, 0x9C, KEY_EN_LEFT_ARROW},//134
    {1, 0x9D, KEY_EN_UP_ARROW},//135
    {1, 0x9E, "Key: Up Brightness"},//136
    {1, 0x9F, KEY_EN_DOWN_ARROW},//137
    {1, 0xA0, "Key: Down Brightness"},//138
    {1, 0xA1, KEY_EN_RIGHT_ARROW},//139
    {1, 0xA3, KEY_EN_NUMPAD_0},//140
    {1, 0xA4, "Key: Numpad Insert"},//141
    {1, 0xA5, KEY_EN_NUMPAD_PERIOD},//142
    {1, 0xA6, "Key: Numpad Delete"},//143
    {1, 0xA8, KEY_EN_NUMPAD_ENTER},//144

    //ISO
    {1, 0x6C, "Key: | (ISO)"},//145
    {1, 0x81, KEY_EN_ISO_BACK_SLASH},//146

    /*----------------*\
    | zone 2, logo     |
    \*----------------*/

    {2, 0x01, "Logo Bottom Left"},//147
    {2, 0x02, "Logo LED 2"},
    {2, 0x03, "Logo LED 3"},
    {2, 0x04, "Logo LED 4"},
    {2, 0x05, "Logo LED Top Left"},
    {2, 0x06, "Logo LED 6"},
    {2, 0x07, "Logo LED 7"},
    {2, 0x08, "Logo LED 8"},
    {2, 0x09, "Logo LED Top Right"},
    {2, 0x0A, "Logo LED 10"},
    {2, 0x0B, "Logo LED 11"},
    {2, 0x0C, "Logo LED 12"},
    {2, 0x0D, "Logo Bottom Right"},//159

    /*----------------*\
    | zone 3, vents    |
    \*----------------*/

    //left
    {3, 0x01, "Left Vent Front"},//160
    {3, 0x02, "Left Vent LED 2"},
    {3, 0x03, "Left Vent LED 3"},
    {3, 0x04, "Left Vent LED 4"},
    {3, 0x05, "Left Vent LED 5"},
    {3, 0x06, "Left Vent LED 6"},
    {3, 0x07, "Left Vent LED 7"},
    {3, 0x08, "Left Vent LED 8"},
    {3, 0x09, "Left Vent LED 9"},
    {3, 0x0A, "Left Vent LED 10"},
    {3, 0x0B, "Left Vent LED 11"},
    {3, 0x0C, "Left Vent LED 12"},
    {3, 0x0D, "Left Vent LED 13"},
    {3, 0x0E, "Left Vent LED 14"},
    {3, 0x0F, "Left Vent LED 15"},
    {3, 0x10, "Left Vent LED 16"},
    {3, 0x11, "Left Vent LED 17"},
    {3, 0x12, "Left Vent LED 18"},
    {3, 0x13, "Left Vent LED 19"},
    {3, 0x14, "Left Vent LED 20"},
    {3, 0x15, "Left Vent LED 21"},
    {3, 0x16, "Left Vent LED 22"},
    {3, 0x17, "Left Vent LED 23"},
    {3, 0x18, "Left Vent LED 24"},
    {3, 0x19, "Left Vent LED 25"},
    {3, 0x1A, "Left Vent LED 26"},
    {3, 0x1B, "Left Vent Back"},//186

    //right
    {3, 0x1C, "Right Vent Front"},//187
    {3, 0x1D, "Right Vent LED 2"},
    {3, 0x1E, "Right Vent LED 3"},
    {3, 0x1F, "Right Vent LED 4"},
    {3, 0x20, "Right Vent LED 5"},
    {3, 0x21, "Right Vent LED 6"},
    {3, 0x22, "Right Vent LED 7"},
    {3, 0x23, "Right Vent LED 8"},
    {3, 0x24, "Right Vent LED 9"},
    {3, 0x25, "Right Vent LED 10"},
    {3, 0x26, "Right Vent LED 11"},
    {3, 0x27, "Right Vent LED 12"},
    {3, 0x28, "Right Vent LED 13"},
    {3, 0x29, "Right Vent LED 14"},
    {3, 0x2A, "Right Vent LED 15"},
    {3, 0x2B, "Right Vent LED 16"},
    {3, 0x2C, "Right Vent LED 17"},
    {3, 0x2D, "Right Vent LED 18"},
    {3, 0x2E, "Right Vent LED 19"},
    {3, 0x2F, "Right Vent LED 20"},
    {3, 0x30, "Right Vent LED 21"},
    {3, 0x31, "Right Vent LED 22"},
    {3, 0x32, "Right Vent LED 23"},
    {3, 0x33, "Right Vent LED 24"},
    {3, 0x34, "Right Vent LED 25"},
    {3, 0x35, "Right Vent LED 26"},
    {3, 0x36, "Right Vent Back"},//213
    
    //back right vent
    {3, 0x37, "Back Right Vent Left"},//214
    {3, 0x38, "Back Right Vent 2"},
    {3, 0x39, "Back Right Vent 3"},
    {3, 0x3A, "Back Right Vent 4"},
    {3, 0x3B, "Back Right Vent 5"},
    {3, 0x3C, "Back Right Vent 6"},
    {3, 0x3D, "Back Right Vent 7"},
    {3, 0x3E, "Back Right Vent 8"},
    {3, 0x3F, "Back Right Vent 10"},
    {3, 0x40, "Back Right Vent 11"},
    {3, 0x41, "Back Right Vent 12"},
    {3, 0x42, "Back Right Vent 13"},
    {3, 0x43, "Back Right Vent 14"},
    {3, 0x44, "Back Right Vent 15"},
    {3, 0x45, "Back Right Vent 16"},
    {3, 0x46, "Back Right Vent 17"},
    {3, 0x47, "Back Right Vent 18"},
    {3, 0x48, "Back Right Vent 19"},
    {3, 0x49, "Back Right Vent 20"},
    {3, 0x4A, "Back Right Vent 21"},
    {3, 0x4B, "Back Right Vent 22"},
    {3, 0x4C, "Back Right Vent 23"},
    {3, 0x4D, "Back Right Vent 24"},
    {3, 0x4E, "Back Right Vent 25"},
    {3, 0x4F, "Back Right Vent 26"},
    {3, 0x50, "Back Right Vent Right"},//239

    //back left vent
    {3, 0x51, "Back Left Vent Right"},//240
    {3, 0x52, "Back Left Vent 2"},
    {3, 0x53, "Back Left Vent 3"},
    {3, 0x54, "Back Left Vent 4"},
    {3, 0x55, "Back Left Vent 5"},
    {3, 0x56, "Back Left Vent 6"},
    {3, 0x57, "Back Left Vent 7"},
    {3, 0x58, "Back Left Vent 8"},
    {3, 0x59, "Back Left Vent 10"},
    {3, 0x5A, "Back Left Vent 11"},
    {3, 0x5B, "Back Left Vent 12"},
    {3, 0x5C, "Back Left Vent 13"},
    {3, 0x5D, "Back Left Vent 14"},
    {3, 0x5E, "Back Left Vent 15"},
    {3, 0x5F, "Back Left Vent 16"},
    {3, 0x60, "Back Left Vent 17"},
    {3, 0x61, "Back Left Vent 18"},
    {3, 0x62, "Back Left Vent 19"},
    {3, 0x63, "Back Left Vent 20"},
    {3, 0x64, "Back Left Vent 21"},
    {3, 0x65, "Back Left Vent 22"},
    {3, 0x66, "Back Left Vent 23"},
    {3, 0x67, "Back Left Vent 24"},
    {3, 0x68, "Back Left Vent 25"},
    {3, 0x69, "Back Left Vent 26"},
    {3, 0x6A, "Back Left Vent Left"},//265

    /*-----------------*\
    | zone 4, neon      |
    \*-----------------*/
    //left side
    {4, 0x01, "Neon LED 1 Top Left"},//266
    {4, 0x02, "Neon LED 2"},
    {4, 0x03, "Neon LED 3"},
    {4, 0x04, "Neon LED 4"},
    {4, 0x05, "Neon LED 5"},
    {4, 0x06, "Neon LED 6"},
    {4, 0x07, "Neon LED 7"},
    {4, 0x08, "Neon LED 8"},
    {4, 0x09, "Neon LED 9"},
    {4, 0x0A, "Neon LED 10"},
    {4, 0x0B, "Neon LED 11"},
    {4, 0x0C, "Neon LED 12"},
    {4, 0x0D, "Neon LED 13"},
    {4, 0x0E, "Neon LED 14"},
    {4, 0x0F, "Neon LED 15"},
    {4, 0x10, "Neon LED 16"},
    {4, 0x11, "Neon LED 17"},
    {4, 0x12, "Neon LED 18"},
    {4, 0x13, "Neon LED 19"},
    {4, 0x14, "Neon LED 20"},
    {4, 0x15, "Neon LED 21 Left Corner"},//286

    //front
    {4, 0x16, "Neon LED 22 Left Corner"},//287
    {4, 0x17, "Neon LED 23"},
    {4, 0x18, "Neon LED 24"},
    {4, 0x19, "Neon LED 25"},
    {4, 0x1A, "Neon LED 26"},
    {4, 0x1B, "Neon LED 27"},
    {4, 0x1C, "Neon LED 28"},
    {4, 0x1D, "Neon LED 29"},
    {4, 0x1E, "Neon LED 30"},
    {4, 0x1F, "Neon LED 31"},
    {4, 0x20, "Neon LED 32"},
    {4, 0x21, "Neon LED 33"},
    {4, 0x22, "Neon LED 34"},
    {4, 0x23, "Neon LED 35"},
    {4, 0x24, "Neon LED 36"},
    {4, 0x25, "Neon LED 37"},
    {4, 0x26, "Neon LED 38"},
    {4, 0x27, "Neon LED 39"},
    {4, 0x28, "Neon LED 40"},
    {4, 0x29, "Neon LED 41"},
    {4, 0x2A, "Neon LED 42"},
    {4, 0x2B, "Neon LED 43"},
    {4, 0x2C, "Neon LED 44"},
    {4, 0x2D, "Neon LED 45"},
    {4, 0x2E, "Neon LED 46"},
    {4, 0x2F, "Neon LED 47"},
    {4, 0x30, "Neon LED 48"},
    {4, 0x31, "Neon LED 49"},
    {4, 0x32, "Neon LED 50"},
    {4, 0x33, "Neon LED 51"},
    {4, 0x34, "Neon LED 52"},
    {4, 0x35, "Neon LED 53"},
    {4, 0x36, "Neon LED 54"},
    {4, 0x37, "Neon LED 55"},
    {4, 0x38, "Neon LED 56"},
    {4, 0x39, "Neon LED 57"},
    {4, 0x3A, "Neon LED 58"},
    {4, 0x3B, "Neon LED 59"},
    {4, 0x3C, "Neon LED 60"},
    {4, 0x3D, "Neon LED 61"},
    {4, 0x3E, "Neon LED 62"},
    {4, 0x3F, "Neon LED 63"},
    {4, 0x40, "Neon LED 64"},
    {4, 0x41, "Neon LED 65"},
    {4, 0x42, "Neon LED 66"},
    {4, 0x43, "Neon LED 67"},
    {4, 0x44, "Neon LED 68"},
    {4, 0x45, "Neon LED 69"},
    {4, 0x46, "Neon LED 70"},
    {4, 0x47, "Neon LED 71"},
    {4, 0x48, "Neon LED 72"},
    {4, 0x49, "Neon LED 73"},
    {4, 0x4A, "Neon LED 74"},
    {4, 0x4B, "Neon LED 75"},
    {4, 0x4C, "Neon LED 76"},
    {4, 0x4D, "Neon LED 77"},
    {4, 0x4E, "Neon LED 78 Right Corner"},//343
    
    //right side
    {4, 0x4F, "Neon LED 79 Right Corner"},//344
    {4, 0x50, "Neon LED 80"},
    {4, 0x51, "Neon LED 81"},
    {4, 0x52, "Neon LED 82"},
    {4, 0x53, "Neon LED 83"},
    {4, 0x54, "Neon LED 84"},
    {4, 0x55, "Neon LED 85"},
    {4, 0x56, "Neon LED 86"},
    {4, 0x57, "Neon LED 87"},
    {4, 0x58, "Neon LED 88"},
    {4, 0x59, "Neon LED 89"},
    {4, 0x5A, "Neon LED 90"},
    {4, 0x5B, "Neon LED 91"},
    {4, 0x5C, "Neon LED 92"},
    {4, 0x5D, "Neon LED 93"},
    {4, 0x5E, "Neon LED 94"},
    {4, 0x5F, "Neon LED 95"},
    {4, 0x60, "Neon LED 96"},
    {4, 0x61, "Neon LED 97"},
    {4, 0x62, "Neon LED 98"},
    {4, 0x63, "Neon LED 99 Top Right"},//364
};

/*-------------------*\
| Zone Definitions    |
\*-------------------*/

struct lenovo_zone
{
    std::string name;
    zone_type type;
    unsigned int height;
    unsigned int width;
    unsigned int start; //index to start reading the list of leds
    unsigned int end; //end index
};

#endif
