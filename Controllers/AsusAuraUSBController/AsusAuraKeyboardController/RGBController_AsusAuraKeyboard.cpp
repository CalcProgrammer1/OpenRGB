/*---------------------------------------------------------*\
| RGBController_AsusAuraKeyboard.cpp                        |
|                                                           |
|   RGBController for ASUS Aura keyboard                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Mar 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_AsusAuraKeyboard.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int flare_matrix_map[6][22] =
    { {   0,  NA,  13,  18,  23,  28,  38,  43,  49,  54,  60,  65,  69,  70,  NA,  76,  80,  85,  NA,  NA,  NA,  NA },
      {   1,   8,  14,  19,  24,  29,  34,  39,  44,  50,  55,  61,  66,  71,  NA,  77,  81,  86,  89,  94,  98, 103 },
      {   2,  NA,   9,  15,  20,  25,  30,  35,  40,  45,  51,  56,  62,  67,  72,  78,  82,  87,  90,  95,  99, 104 },
      {   3,  NA,  10,  16,  21,  26,  31,  36,  41,  46,  52,  57,  63,  68,  73,  NA,  NA,  NA,  91,  96, 100,  NA },
      {   4,   6,  11,  17,  22,  27,  32,  37,  42,  47,  53,  58,  74,  NA,  NA,  NA,  83,  NA,  92,  97, 101, 105 },
      {   5,   7,  12,  NA,  NA,  NA,  NA,  33,  NA,  48,  NA,  59,  64,  75,  NA,  79,  84,  88,  93,  NA, 102,  NA } };

static unsigned int scope_matrix_map[6][22] =
    { {   0,  NA,  13,  18,  23,  28,  38,  43,  49,  54,  60,  65,  69,  70,  NA,  76,  80,  85,  NA,  NA,  NA,  NA },
      {   1,   8,  14,  19,  24,  29,  34,  39,  44,  50,  55,  61,  66,  71,  NA,  77,  81,  86,  89,  94,  98, 103 },
      {   2,  NA,   9,  15,  20,  25,  30,  35,  40,  45,  51,  56,  62,  67,  72,  78,  82,  87,  90,  95,  99, 104 },
      {   3,  NA,  10,  16,  21,  26,  31,  36,  41,  46,  52,  57,  63,  68,  73,  NA,  NA,  NA,  91,  96, 100,  NA },
      {   4,   6,  11,  17,  22,  27,  32,  37,  42,  47,  53,  58,  74,  NA,  NA,  NA,  83,  NA,  92,  97, 101, 105 },
      {   5,   NA,  7,  12,  NA,  NA,  NA,  33,  NA,  48,  NA,  59,  64,  75,  NA,  79,  84,  88,  93,  NA, 102,  NA } };

static unsigned int scope_tkl_matrix_map[6][18] =
    { {   0,  NA,  13,  18,  23,  28,  38,  43,  49,  54,  60,  65,  69,  70,  NA,  NA,  NA,  NA },
      {   1,   8,  14,  19,  24,  29,  34,  39,  44,  50,  55,  61,  66,  71,  NA,  76,  79,  83 },
      {   2,  NA,   9,  15,  20,  25,  30,  35,  40,  45,  51,  56,  62,  67,  72,  77,  80,  84 },
      {   3,  NA,  10,  16,  21,  26,  31,  36,  41,  46,  52,  57,  63,  68,  73,  NA,  NA,  NA },
      {   4,   6,  11,  17,  22,  27,  32,  37,  42,  47,  53,  58,  74,  NA,  NA,  NA,  81,  NA },
      {   5,   NA,  7,  12,  NA,  NA,  NA,  33,  NA,  48,  NA,  59,  64,  75,  NA,  78,  82,  85 } };

static unsigned int falchion_matrix_map[5][16] =
    { {   0,   5,   9,  14,  18,  22,  26,  31,  35,  39,  44,  49,  54,  58,  NA,  63 },
      {   1,  NA,   6,  10,  15,  19,  23,  27,  32,  36,  40,  45,  50,  55,  59,  64 },
      {   2,  NA,   7,  11,  16,  20,  24,  28,  33,  37,  41,  46,  51,  60,  NA,  65 },
      {   3,  NA,  12,  17,  21,  25,  29,  34,  38,  42,  47,  52,  56,  NA,  61,  66 },
      {   4,   8,  13,  NA,  NA,  NA,  30,  NA,  NA,  NA,  43,  48,  53,  57,  62,  67 } };

static const std::vector<aura_keyboard_led> default_led_names =
{
    /* Key Label                Index  */
    { KEY_EN_ESCAPE,            0x00    },
    { KEY_EN_BACK_TICK,         0x01    },
    { KEY_EN_TAB,               0x02    },
    { KEY_EN_CAPS_LOCK,         0x03    },
    { KEY_EN_LEFT_SHIFT,        0x04    },
    { KEY_EN_LEFT_CONTROL,      0x05    },
    { KEY_EN_ISO_BACK_SLASH,    0x0C    },
    { KEY_EN_1,                 0x11    },
    { KEY_EN_Q,                 0x12    },
    { KEY_EN_A,                 0x13    },
    { KEY_EN_Z,                 0x14    },
    { KEY_EN_F1,                0x18    },
    { KEY_EN_2,                 0x19    },
    { KEY_EN_W,                 0x1A    },
    { KEY_EN_S,                 0x1B    },
    { KEY_EN_X,                 0x1C    },
    { KEY_EN_F2,                0x20    },
    { KEY_EN_3,                 0x21    },
    { KEY_EN_E,                 0x22    },
    { KEY_EN_D,                 0x23    },
    { KEY_EN_C,                 0x24    },
    { KEY_EN_F3,                0x28    },
    { KEY_EN_4,                 0x29    },
    { KEY_EN_R,                 0x2A    },
    { KEY_EN_F,                 0x2B    },
    { KEY_EN_V,                 0x2C    },
    { KEY_EN_F4,                0x30    },
    { KEY_EN_5,                 0x31    },
    { KEY_EN_T,                 0x32    },
    { KEY_EN_G,                 0x33    },
    { KEY_EN_B,                 0x34    },
    { KEY_EN_SPACE,             0x35    },
    { KEY_EN_6,                 0x39    },
    { KEY_EN_Y,                 0x3A    },
    { KEY_EN_H,                 0x3B    },
    { KEY_EN_N,                 0x3C    },
    { KEY_EN_F5,                0x40    },
    { KEY_EN_7,                 0x41    },
    { KEY_EN_U,                 0x42    },
    { KEY_EN_J,                 0x43    },
    { KEY_EN_M,                 0x44    },
    { KEY_EN_F6,                0x48    },
    { KEY_EN_8,                 0x49    },
    { KEY_EN_I,                 0x4A    },
    { KEY_EN_K,                 0x4B    },
    { KEY_EN_COMMA,             0x4C    },
    { KEY_EN_RIGHT_ALT,         0x4D    },
    { KEY_EN_F7,                0x50    },
    { KEY_EN_9,                 0x51    },
    { KEY_EN_O,                 0x52    },
    { KEY_EN_L,                 0x53    },
    { KEY_EN_PERIOD,            0x54    },
    { KEY_EN_F8,                0x58    },
    { KEY_EN_0,                 0x59    },
    { KEY_EN_P,                 0x5A    },
    { KEY_EN_SEMICOLON,         0x5B    },
    { KEY_EN_FORWARD_SLASH,     0x5C    },
    { KEY_EN_RIGHT_FUNCTION,    0x5D    },
    { KEY_EN_F9,                0x60    },
    { KEY_EN_MINUS,             0x61    },
    { KEY_EN_LEFT_BRACKET,      0x62    },
    { KEY_EN_QUOTE,             0x63    },
    { KEY_EN_MENU,              0x65    },
    { KEY_EN_F10,               0x68    },
    { KEY_EN_EQUALS,            0x69    },
    { KEY_EN_RIGHT_BRACKET,     0x6A    },
    { KEY_EN_POUND,             0x6B    },
    { KEY_EN_F11,               0x70    },
    { KEY_EN_F12,               0x78    },
    { KEY_EN_BACKSPACE,         0x79    },
    { KEY_EN_ANSI_BACK_SLASH,   0x7A    },
    { KEY_EN_ANSI_ENTER,        0x7B    },
    { KEY_EN_RIGHT_SHIFT,       0x7C    },
    { KEY_EN_RIGHT_CONTROL,     0x7D    },
    { KEY_EN_PRINT_SCREEN,      0x80    },
    { KEY_EN_INSERT,            0x81    },
    { KEY_EN_DELETE,            0x82    },
    { KEY_EN_LEFT_ARROW,        0x85    },
    { KEY_EN_SCROLL_LOCK,       0x88    },
    { KEY_EN_HOME,              0x89    },
    { KEY_EN_END,               0x8A    },
    { KEY_EN_UP_ARROW,          0x8C    },
    { KEY_EN_DOWN_ARROW,        0x8D    },
    { KEY_EN_PAUSE_BREAK,       0x90    },
    { KEY_EN_PAGE_UP,           0x91    },
    { KEY_EN_PAGE_DOWN,         0x92    },
    { KEY_EN_RIGHT_ARROW,       0x95    },
    { KEY_EN_NUMPAD_LOCK,       0x99    },
    { KEY_EN_NUMPAD_7,          0x9A    },
    { KEY_EN_NUMPAD_4,          0x9B    },
    { KEY_EN_NUMPAD_1,          0x9C    },
    { KEY_EN_NUMPAD_0,          0x9D    },
    { KEY_EN_NUMPAD_DIVIDE,     0xA1    },
    { KEY_EN_NUMPAD_8,          0xA2    },
    { KEY_EN_NUMPAD_5,          0xA3    },
    { KEY_EN_NUMPAD_2,          0xA4    },
    { KEY_EN_NUMPAD_TIMES,      0xA9    },
    { KEY_EN_NUMPAD_9,          0xAA    },
    { KEY_EN_NUMPAD_6,          0xAB    },
    { KEY_EN_NUMPAD_3,          0xAC    },
    { KEY_EN_NUMPAD_PERIOD,     0xAD    },
    { KEY_EN_NUMPAD_MINUS,      0xB1    },
    { KEY_EN_NUMPAD_PLUS,       0xB2    },
    { KEY_EN_NUMPAD_ENTER,      0xB4    },
};

static const std::vector<aura_keyboard_led> default_tkl_led_names =
{
    /* Key Label                Index  */
    { KEY_EN_ESCAPE,            0x00    },
    { KEY_EN_BACK_TICK,         0x01    },
    { KEY_EN_TAB,               0x02    },
    { KEY_EN_CAPS_LOCK,         0x03    },
    { KEY_EN_LEFT_SHIFT,        0x04    },
    { KEY_EN_LEFT_CONTROL,      0x05    },
    { KEY_EN_ISO_BACK_SLASH,    0x0C    },
    { KEY_EN_1,                 0x11    },
    { KEY_EN_Q,                 0x12    },
    { KEY_EN_A,                 0x13    },
    { KEY_EN_Z,                 0x14    },
    { KEY_EN_F1,                0x18    },
    { KEY_EN_2,                 0x19    },
    { KEY_EN_W,                 0x1A    },
    { KEY_EN_S,                 0x1B    },
    { KEY_EN_X,                 0x1C    },
    { KEY_EN_F2,                0x20    },
    { KEY_EN_3,                 0x21    },
    { KEY_EN_E,                 0x22    },
    { KEY_EN_D,                 0x23    },
    { KEY_EN_C,                 0x24    },
    { KEY_EN_F3,                0x28    },
    { KEY_EN_4,                 0x29    },
    { KEY_EN_R,                 0x2A    },
    { KEY_EN_F,                 0x2B    },
    { KEY_EN_V,                 0x2C    },
    { KEY_EN_F4,                0x30    },
    { KEY_EN_5,                 0x31    },
    { KEY_EN_T,                 0x32    },
    { KEY_EN_G,                 0x33    },
    { KEY_EN_B,                 0x34    },
    { KEY_EN_SPACE,             0x35    },
    { KEY_EN_6,                 0x39    },
    { KEY_EN_Y,                 0x3A    },
    { KEY_EN_H,                 0x3B    },
    { KEY_EN_N,                 0x3C    },
    { KEY_EN_F5,                0x40    },
    { KEY_EN_7,                 0x41    },
    { KEY_EN_U,                 0x42    },
    { KEY_EN_J,                 0x43    },
    { KEY_EN_M,                 0x44    },
    { KEY_EN_F6,                0x48    },
    { KEY_EN_8,                 0x49    },
    { KEY_EN_I,                 0x4A    },
    { KEY_EN_K,                 0x4B    },
    { KEY_EN_COMMA,             0x4C    },
    { KEY_EN_RIGHT_ALT,         0x4D    },
    { KEY_EN_F7,                0x50    },
    { KEY_EN_9,                 0x51    },
    { KEY_EN_O,                 0x52    },
    { KEY_EN_L,                 0x53    },
    { KEY_EN_PERIOD,            0x54    },
    { KEY_EN_F8,                0x58    },
    { KEY_EN_0,                 0x59    },
    { KEY_EN_P,                 0x5A    },
    { KEY_EN_SEMICOLON,         0x5B    },
    { KEY_EN_FORWARD_SLASH,     0x5C    },
    { KEY_EN_RIGHT_FUNCTION,    0x5D    },
    { KEY_EN_F9,                0x60    },
    { KEY_EN_MINUS,             0x61    },
    { KEY_EN_LEFT_BRACKET,      0x62    },
    { KEY_EN_QUOTE,             0x63    },
    { KEY_EN_MENU,              0x65    },
    { KEY_EN_F10,               0x68    },
    { KEY_EN_EQUALS,            0x69    },
    { KEY_EN_RIGHT_BRACKET,     0x6A    },
    { KEY_EN_POUND,             0x6B    },
    { KEY_EN_F11,               0x70    },
    { KEY_EN_F12,               0x78    },
    { KEY_EN_BACKSPACE,         0x79    },
    { KEY_EN_ANSI_BACK_SLASH,   0x7A    },
    { KEY_EN_ANSI_ENTER,        0x7B    },
    { KEY_EN_RIGHT_SHIFT,       0x7C    },
    { KEY_EN_RIGHT_CONTROL,     0x7D    },
    { KEY_EN_INSERT,            0x81    },
    { KEY_EN_DELETE,            0x82    },
    { KEY_EN_LEFT_ARROW,        0x85    },
    { KEY_EN_HOME,              0x89    },
    { KEY_EN_END,               0x8A    },
    { KEY_EN_UP_ARROW,          0x8C    },
    { KEY_EN_DOWN_ARROW,        0x8D    },
    { KEY_EN_PAGE_UP,           0x91    },
    { KEY_EN_PAGE_DOWN,         0x92    },
    { KEY_EN_RIGHT_ARROW,       0x95    },
    { "Logo 1",                 0x80    },
    { "Logo 2",                 0x90    },
    { "Underglow 1",            0x06    },
    { "Underglow 2",            0x0E    },
    { "Underglow 3",            0x16    },
    { "Underglow 4",            0x1E    },
    { "Underglow 5",            0x26    },
    { "Underglow 6",            0x2E    },
    { "Underglow 7",            0x36    },
    { "Underglow 8",            0x3E    },
    { "Underglow 9",            0x46    },
    { "Underglow 10",           0x4E    },
    { "Underglow 11",           0x56    },
    { "Underglow 12",           0x5E    },
    { "Underglow 13",           0x66    },
    { "Underglow 14",           0x6E    },
    { "Underglow 15",           0x76    },
    { "Underglow 16",           0x7E    },
    { "Underglow 17",           0x86    },
    { "Underglow 18",           0x8E    },
    { "Underglow 19",           0x96    },
    { "Underglow 20",           0x9E    },
    { "Underglow 21",           0xA6    },
    { "Underglow 22",           0xAE    },
    { "Underglow 23",           0xB6    },
    { "Underglow 24",           0xBE    },
    { "Underglow 25",           0xC6    },
    { "Underglow 26",           0xCE    },
};

static const std::vector<aura_keyboard_led> default_65pct_led_names =
{
    /* Key Label                Index  */
    { KEY_EN_ESCAPE,            0x00    },
    { KEY_EN_TAB,               0x01    },
    { KEY_EN_CAPS_LOCK,         0x02    },
    { KEY_EN_LEFT_SHIFT,        0x03    },
    { KEY_EN_LEFT_CONTROL,      0x04    },
    { KEY_EN_1,                 0x08    },
    { KEY_EN_Q,                 0x09    },
    { KEY_EN_A,                 0x0A    },
    { KEY_EN_LEFT_WINDOWS,      0x0C    },
    { KEY_EN_2,                 0x10    },
    { KEY_EN_W,                 0x11    },
    { KEY_EN_S,                 0x12    },
    { KEY_EN_Z,                 0x13    },
    { KEY_EN_LEFT_ALT,          0x14    },
    { KEY_EN_3,                 0x18    },
    { KEY_EN_E,                 0x19    },
    { KEY_EN_D,                 0x1A    },
    { KEY_EN_X,                 0x1B    },
    { KEY_EN_4,                 0x20    },
    { KEY_EN_R,                 0x21    },
    { KEY_EN_F,                 0x22    },
    { KEY_EN_C,                 0x23    },
    { KEY_EN_5,                 0x28    },
    { KEY_EN_T,                 0x29    },
    { KEY_EN_G,                 0x2A    },
    { KEY_EN_V,                 0x2B    },
    { KEY_EN_6,                 0x30    },
    { KEY_EN_Y,                 0x31    },
    { KEY_EN_H,                 0x32    },
    { KEY_EN_B,                 0x33    },
    { KEY_EN_SPACE,             0x34    },
    { KEY_EN_7,                 0x38    },
    { KEY_EN_U,                 0x39    },
    { KEY_EN_J,                 0x3A    },
    { KEY_EN_N,                 0x3B    },
    { KEY_EN_8,                 0x40    },
    { KEY_EN_I,                 0x41    },
    { KEY_EN_K,                 0x42    },
    { KEY_EN_M,                 0x43    },
    { KEY_EN_9,                 0x48    },
    { KEY_EN_O,                 0x49    },
    { KEY_EN_L,                 0x4A    },
    { KEY_EN_COMMA,             0x4B    },
    { KEY_EN_RIGHT_ALT,         0x4C    },
    { KEY_EN_0,                 0x50    },
    { KEY_EN_P,                 0x51    },
    { KEY_EN_SEMICOLON,         0x52    },
    { KEY_EN_PERIOD,            0x53    },
    { KEY_EN_RIGHT_FUNCTION,    0x54    },
    { KEY_EN_MINUS,             0x58    },
    { KEY_EN_LEFT_BRACKET,      0x59    },
    { KEY_EN_QUOTE,             0x5A    },
    { KEY_EN_FORWARD_SLASH,     0x5B    },
    { KEY_EN_RIGHT_CONTROL,     0x5C    },
    { KEY_EN_EQUALS,            0x60    },
    { KEY_EN_RIGHT_BRACKET,     0x61    },
    { KEY_EN_RIGHT_SHIFT,       0x63    },
    { KEY_EN_LEFT_ARROW,        0x64    },
    { KEY_EN_BACKSPACE,         0x68    },
    { KEY_EN_ANSI_BACK_SLASH,   0x69    },
    { KEY_EN_ANSI_ENTER,        0x6A    },
    { KEY_EN_UP_ARROW,          0x6B    },
    { KEY_EN_DOWN_ARROW,        0x6C    },
    { KEY_EN_INSERT,            0x70    },
    { KEY_EN_DELETE,            0x71    },
    { KEY_EN_PAGE_UP,           0x72    },
    { KEY_EN_PAGE_DOWN,         0x73    },
    { KEY_EN_RIGHT_ARROW,       0x74    },
};

/**------------------------------------------------------------------*\
    @name Asus Aura Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectAsusAuraUSBKeyboards
    @comment
\*-------------------------------------------------------------------*/

RGBController_AuraKeyboard::RGBController_AuraKeyboard(AuraKeyboardController* controller_ptr, AuraKeyboardMappingLayoutType keyboard_layout)
{
    controller          = controller_ptr;
    layout              = keyboard_layout;

    name                = controller->GetNameString();
    vendor              = "ASUS";
    type                = DEVICE_TYPE_KEYBOARD;
    description         = "ASUS Aura Keyboard Device";
    location            = controller->GetDeviceLocation();
    serial              = controller->GetSerialString();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_AuraKeyboard::~RGBController_AuraKeyboard()
{
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        if(zones[zone_idx].type == ZONE_TYPE_MATRIX)
        {
            delete zones[zone_idx].matrix_map;
        }
    }

    delete controller;
}

void RGBController_AuraKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    std::vector<led_zone> led_zones;
    std::vector<aura_keyboard_led> led_names;

    switch(layout)
    {
        /*-----------------------------------------------------*\
        | On the ROG Scope keyboards Ctrl key is double sized,  |
        | so there is a layout shift                            |
        \*-----------------------------------------------------*/
        case SCOPE_LAYOUT:
            led_names = default_led_names;
            led_zones.push_back({ZONE_EN_KEYBOARD,      ZONE_TYPE_MATRIX, 106, new matrix_map_type{6, 22, (unsigned int *)&scope_matrix_map}});

            led_names.insert(led_names.begin() + 7,     {KEY_EN_LEFT_WINDOWS,   0x15});
            led_names.insert(led_names.begin() + 12,    {KEY_EN_LEFT_ALT,       0x1D});
            break;

        case SCOPE_RX_LAYOUT:
            led_names = default_led_names;
            led_zones.push_back({ZONE_EN_KEYBOARD,      ZONE_TYPE_MATRIX, 106, new matrix_map_type{6, 22, (unsigned int *)&scope_matrix_map}});
            led_zones.push_back({"Logo",                ZONE_TYPE_SINGLE, 1, NULL});

            led_names.insert(led_names.begin() + 7,     {KEY_EN_LEFT_WINDOWS,   0x15});
            led_names.insert(led_names.begin() + 12,    {KEY_EN_LEFT_ALT,       0x1D});
            led_names.push_back({ "Logo", 0xB0});
            break;

        case SCOPE_TKL_LAYOUT:
            led_names = default_tkl_led_names;
            led_zones.push_back({ZONE_EN_KEYBOARD,      ZONE_TYPE_MATRIX, 86, new matrix_map_type{6, 18, (unsigned int *)&scope_tkl_matrix_map}});
            led_zones.push_back({"Logo",                ZONE_TYPE_LINEAR, 2, NULL});
            led_zones.push_back({"Underglow",           ZONE_TYPE_LINEAR, 26, NULL});

            led_names.insert(led_names.begin() + 7,     {KEY_EN_LEFT_WINDOWS,   0x15});
            led_names.insert(led_names.begin() + 12,    {KEY_EN_LEFT_ALT,       0x1D});
            break;

        case FLARE_LAYOUT:
            led_names = default_led_names;
            led_zones.push_back({ZONE_EN_KEYBOARD,      ZONE_TYPE_MATRIX, 106, new matrix_map_type{6, 22, (unsigned int *)&flare_matrix_map}});
            led_zones.push_back({"Logo",                ZONE_TYPE_SINGLE, 1, NULL});
            led_zones.push_back({"Underglow",           ZONE_TYPE_SINGLE, 2, NULL});

            led_names.insert(led_names.begin() + 7,     {KEY_EN_LEFT_WINDOWS,   0x0D});
            led_names.insert(led_names.begin() + 12,    {KEY_EN_LEFT_ALT,       0x15});

            led_names.push_back({ "Logo", 0xB8});
            led_names.push_back({ "Left Underglow",     0xB9});
            led_names.push_back({ "Right Underglow",    0xBA});
            break;

        case FALCHION_LAYOUT:
            led_names = default_65pct_led_names;
            led_zones.push_back({ZONE_EN_KEYBOARD,      ZONE_TYPE_MATRIX, 68, new matrix_map_type{5, 16, (unsigned int *)&falchion_matrix_map}});
            break;
    }

    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < led_zones.size(); zone_idx++)
    {
        zone new_zone;
        new_zone.name                   = led_zones[zone_idx].name;
        new_zone.type                   = led_zones[zone_idx].type;
        new_zone.leds_min               = led_zones[zone_idx].size;
        new_zone.leds_max               = led_zones[zone_idx].size;
        new_zone.leds_count             = led_zones[zone_idx].size;

        if(led_zones[zone_idx].type == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map         = led_zones[zone_idx].matrix;
        }
        else
        {
            new_zone.matrix_map         = NULL;
        }

        zones.push_back(new_zone);

        total_led_count += led_zones[zone_idx].size;
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name  = led_names[led_idx].name;
        new_led.value = led_names[led_idx].idx;

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_AuraKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AuraKeyboard::DeviceUpdateLEDs()
{
    std::vector<unsigned char> frame_buf;

    /*---------------------------------------------------------*\
    | Resize the frame buffer, 4 bytes per LED                  |
    \*---------------------------------------------------------*/
    frame_buf.resize(leds.size() * 4);

    /*---------------------------------------------------------*\
    | TODO: Send packets with multiple LED frames               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        frame_buf[(led_idx * 4) + 0] = leds[led_idx].value;
        frame_buf[(led_idx * 4) + 1] = RGBGetRValue(colors[led_idx]);
        frame_buf[(led_idx * 4) + 2] = RGBGetGValue(colors[led_idx]);
        frame_buf[(led_idx * 4) + 3] = RGBGetBValue(colors[led_idx]);
    }

    controller->SendDirect((unsigned char)leds.size(), frame_buf.data());
}

void RGBController_AuraKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraKeyboard::DeviceUpdateMode()
{

}
