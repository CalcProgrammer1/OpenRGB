/*-----------------------------------------*\
|  AsusAuraTUFKeyboardController.h          |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Mola19 10/02/2021                        |
\*-----------------------------------------*/

#include "RGBControllerKeyNames.h"
#include <vector>
#include <map>
#include "RGBController.h"

#pragma once

enum
{
    ASUS_TUF_K7_LAYOUT_CA    = 1,
    ASUS_TUF_K7_LAYOUT_AR    = 2,
    ASUS_TUF_K7_LAYOUT_DE    = 3,
    ASUS_TUF_K7_LAYOUT_UK    = 4,
    ASUS_TUF_K7_LAYOUT_FR    = 5,
    ASUS_TUF_K7_LAYOUT_CN    = 6,
    ASUS_TUF_K7_LAYOUT_HU    = 7,
    ASUS_TUF_K7_LAYOUT_IT    = 8,
    ASUS_TUF_K7_LAYOUT_TH    = 9,
    ASUS_TUF_K7_LAYOUT_UA    = 10,
    ASUS_TUF_K7_LAYOUT_NO    = 11,
    ASUS_TUF_K7_LAYOUT_PT    = 12,
    ASUS_TUF_K7_LAYOUT_HE    = 13,
    ASUS_TUF_K7_LAYOUT_RU    = 14,
    ASUS_TUF_K7_LAYOUT_ES    = 15,
    ASUS_TUF_K7_LAYOUT_TW    = 16,
    ASUS_TUF_K7_LAYOUT_US    = 17,
    ASUS_TUF_K7_LAYOUT_TR    = 18,
    ASUS_TUF_K7_LAYOUT_CZ    = 19,
    ASUS_TUF_K7_LAYOUT_BE    = 20,
    ASUS_TUF_K7_LAYOUT_JP    = 21,
    ASUS_TUF_K7_LAYOUT_KR    = 22,
    ASUS_TUF_K7_LAYOUT_IS    = 23,
    ASUS_TUF_K7_LAYOUT_WB    = 24,
    ASUS_TUF_K7_LAYOUT_SW_CH = 25
};

#define NA  0xFFFFFFFF

typedef struct
{
    unsigned int *                matrix_map;
    std::vector<std::string>      led_names;
} layout_type;


static unsigned int ASUS_TUF_K7_LAYOUT_KEYS_104[6][23] = {
    {   0,  NA,  18,  24,  30,  36,  NA,  48,  54,  60,  66,  72,  78,  84,  90,  NA,  96, 102, 108,  NA,  NA,  NA,  NA },
    {   1,  13,  19,  25,  31,  37,  43,  49,  55,  61,  67,  73,  79,  91,  NA,  NA,  97, 103, 109, 115, 121, 127, 133 },
    {   2,  NA,  14,  20,  26,  32,  38,  44,  50,  56,  62,  68,  74,  80,  92,  NA,  98, 104, 110, 116, 122, 128, 134 },
    {   3,  NA,  15,  21,  27,  33,  39,  45,  51,  57,  63,  69,  75,  NA,  93,  NA,  NA,  NA,  NA, 117, 123, 129,  NA },
    {   4,  NA,  16,  22,  28,  34,  40,  46,  52,  58,  64,  70,  NA,  94,  NA,  NA,  NA, 106,  NA, 118, 124, 130, 136 },
    {   5,  11,  17,  NA,  NA,  NA,  41,  NA,  NA,  NA,  59,  71,  77,  NA,  95,  NA, 101, 107, 113, 119,  NA, 131,  NA }
};

static unsigned int ASUS_TUF_K7_LAYOUT_KEYS_105[6][23] = {
    {   0,  NA,  18,  24,  30,  36,  NA,  48,  54,  60,  66,  72,  78,  84,  90,  NA,  96, 102, 108,  NA,  NA,  NA,  NA },
    {   1,  13,  19,  25,  31,  37,  43,  49,  55,  61,  67,  73,  79,  91,  NA,  NA,  97, 103, 109, 115, 121, 127, 133 },
    {   2,  NA,  14,  20,  26,  32,  38,  44,  50,  56,  62,  68,  74,  80,  NA,  NA,  98, 104, 110, 116, 122, 128, 134 },
    {   3,  NA,  15,  21,  27,  33,  39,  45,  51,  57,  63,  69,  75,  81,  93,  NA,  NA,  NA,  NA, 117, 123, 129,  NA },
    {   4,  10,  16,  22,  28,  34,  40,  46,  52,  58,  64,  70,  NA,  94,  NA,  NA,  NA, 106,  NA, 118, 124, 130, 136 },
    {   5,  11,  17,  NA,  NA,  NA,  41,  NA,  NA,  NA,  59,  71,  77,  NA,  95,  NA, 101, 107, 113, 119,  NA, 131,  NA }
};


static std::map<int,layout_type> AsusTUFK7Layouts =
{
    {
        ASUS_TUF_K7_LAYOUT_UK,
        {
            (unsigned int *) ASUS_TUF_K7_LAYOUT_KEYS_105,
            {
                KEY_EN_ESCAPE,
                KEY_EN_BACK_TICK,
                KEY_EN_TAB,
                KEY_EN_CAPS_LOCK,
                KEY_EN_LEFT_SHIFT,
                KEY_EN_LEFT_CONTROL,

                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                "Key: \\",
                KEY_EN_LEFT_WINDOWS,

                KEY_EN_UNUSED,
                KEY_EN_1,
                KEY_EN_Q,
                KEY_EN_A,
                KEY_EN_Z,
                KEY_EN_LEFT_ALT,

                KEY_EN_F1,
                KEY_EN_2,
                KEY_EN_W,
                KEY_EN_S,
                KEY_EN_X,
                KEY_EN_UNUSED,

                KEY_EN_F2,
                KEY_EN_3,
                KEY_EN_E,
                KEY_EN_D,
                KEY_EN_C,
                KEY_EN_UNUSED,

                KEY_EN_F3,
                KEY_EN_4,
                KEY_EN_R,
                KEY_EN_F,
                KEY_EN_V,
                KEY_EN_UNUSED,

                KEY_EN_F4,
                KEY_EN_5,
                KEY_EN_T,
                KEY_EN_G,
                KEY_EN_B,
                KEY_EN_SPACE,

                KEY_EN_UNUSED,
                KEY_EN_6,
                KEY_EN_Y,
                KEY_EN_H,
                KEY_EN_N,
                KEY_EN_UNUSED,

                KEY_EN_F5,
                KEY_EN_7,
                KEY_EN_U,
                KEY_EN_J,
                KEY_EN_M,
                KEY_EN_UNUSED,

                KEY_EN_F6,
                KEY_EN_8,
                KEY_EN_I,
                KEY_EN_K,
                KEY_EN_COMMA,
                KEY_EN_RIGHT_ALT,

                KEY_EN_F7,
                KEY_EN_9,
                KEY_EN_O,
                KEY_EN_L,
                KEY_EN_PERIOD,
                KEY_EN_UNUSED,

                KEY_EN_F8,
                KEY_EN_0,
                KEY_EN_P,
                KEY_EN_SEMICOLON,
                KEY_EN_FORWARD_SLASH,
                KEY_EN_RIGHT_FUNCTION,

                KEY_EN_F9,
                KEY_EN_MINUS,
                KEY_EN_LEFT_BRACKET,
                KEY_EN_QUOTE,
                KEY_EN_UNUSED,
                KEY_EN_MENU,

                KEY_EN_F10,
                KEY_EN_EQUALS,
                KEY_EN_RIGHT_BRACKET,
                KEY_EN_POUND,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,

                KEY_EN_F11,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,

                KEY_EN_F12,
                KEY_EN_BACKSPACE,
                KEY_EN_UNUSED,
                KEY_EN_ANSI_ENTER,
                KEY_EN_RIGHT_SHIFT,
                KEY_EN_RIGHT_CONTROL,

                KEY_EN_PRINT_SCREEN,
                KEY_EN_INSERT,
                KEY_EN_DELETE,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_LEFT_ARROW,

                KEY_EN_SCROLL_LOCK,
                KEY_EN_HOME,
                KEY_EN_END,
                KEY_EN_UNUSED,
                KEY_EN_UP_ARROW,
                KEY_EN_DOWN_ARROW,

                KEY_EN_PAUSE_BREAK,
                KEY_EN_PAGE_UP,
                KEY_EN_PAGE_DOWN,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_RIGHT_ARROW,

                KEY_EN_UNUSED,
                KEY_EN_NUMPAD_LOCK,
                KEY_EN_NUMPAD_7,
                KEY_EN_NUMPAD_4,
                KEY_EN_NUMPAD_1,
                KEY_EN_NUMPAD_0,

                KEY_EN_UNUSED,
                KEY_EN_NUMPAD_DIVIDE,
                KEY_EN_NUMPAD_8,
                KEY_EN_NUMPAD_5,
                KEY_EN_NUMPAD_2,
                KEY_EN_UNUSED,

                KEY_EN_UNUSED,
                KEY_EN_NUMPAD_TIMES,
                KEY_EN_NUMPAD_9,
                KEY_EN_NUMPAD_6,
                KEY_EN_NUMPAD_3,
                "Key: Number Pad ,",

                KEY_EN_UNUSED,
                KEY_EN_NUMPAD_MINUS,
                KEY_EN_NUMPAD_PLUS,
                KEY_EN_UNUSED,
                KEY_EN_NUMPAD_ENTER,
                KEY_EN_UNUSED,
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            (unsigned int *) ASUS_TUF_K7_LAYOUT_KEYS_104,
            {
                KEY_EN_ESCAPE,
                KEY_EN_BACK_TICK,
                KEY_EN_TAB,
                KEY_EN_CAPS_LOCK,
                KEY_EN_LEFT_SHIFT,
                KEY_EN_LEFT_CONTROL,

                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_LEFT_WINDOWS,

                KEY_EN_UNUSED,
                KEY_EN_1,
                KEY_EN_Q,
                KEY_EN_A,
                KEY_EN_Z,
                KEY_EN_LEFT_ALT,

                KEY_EN_F1,
                KEY_EN_2,
                KEY_EN_W,
                KEY_EN_S,
                KEY_EN_X,
                KEY_EN_UNUSED,

                KEY_EN_F2,
                KEY_EN_3,
                KEY_EN_E,
                KEY_EN_D,
                KEY_EN_C,
                KEY_EN_UNUSED,

                KEY_EN_F3,
                KEY_EN_4,
                KEY_EN_R,
                KEY_EN_F,
                KEY_EN_V,
                KEY_EN_UNUSED,

                KEY_EN_F4,
                KEY_EN_5,
                KEY_EN_T,
                KEY_EN_G,
                KEY_EN_B,
                KEY_EN_SPACE,

                KEY_EN_UNUSED,
                KEY_EN_6,
                KEY_EN_Y,
                KEY_EN_H,
                KEY_EN_N,
                KEY_EN_UNUSED,

                KEY_EN_F5,
                KEY_EN_7,
                KEY_EN_U,
                KEY_EN_J,
                KEY_EN_M,
                KEY_EN_UNUSED,

                KEY_EN_F6,
                KEY_EN_8,
                KEY_EN_I,
                KEY_EN_K,
                KEY_EN_COMMA,
                KEY_EN_RIGHT_ALT,

                KEY_EN_F7,
                KEY_EN_9,
                KEY_EN_O,
                KEY_EN_L,
                KEY_EN_PERIOD,
                KEY_EN_UNUSED,

                KEY_EN_F8,
                KEY_EN_0,
                KEY_EN_P,
                KEY_EN_SEMICOLON,
                KEY_EN_FORWARD_SLASH,
                KEY_EN_RIGHT_FUNCTION,

                KEY_EN_F9,
                KEY_EN_MINUS,
                KEY_EN_LEFT_BRACKET,
                KEY_EN_QUOTE,
                KEY_EN_UNUSED,
                KEY_EN_MENU,

                KEY_EN_F10,
                KEY_EN_EQUALS,
                KEY_EN_RIGHT_BRACKET,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,

                KEY_EN_F11,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,

                KEY_EN_F12,
                KEY_EN_BACKSPACE,
                "Key: \\",
                KEY_EN_ANSI_ENTER,
                KEY_EN_RIGHT_SHIFT,
                KEY_EN_RIGHT_CONTROL,

                KEY_EN_PRINT_SCREEN,
                KEY_EN_INSERT,
                KEY_EN_DELETE,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_LEFT_ARROW,

                KEY_EN_SCROLL_LOCK,
                KEY_EN_HOME,
                KEY_EN_END,
                KEY_EN_UNUSED,
                KEY_EN_UP_ARROW,
                KEY_EN_DOWN_ARROW,

                KEY_EN_PAUSE_BREAK,
                KEY_EN_PAGE_UP,
                KEY_EN_PAGE_DOWN,
                KEY_EN_UNUSED,
                KEY_EN_UNUSED,
                KEY_EN_RIGHT_ARROW,

                KEY_EN_UNUSED,
                KEY_EN_NUMPAD_LOCK,
                KEY_EN_NUMPAD_7,
                KEY_EN_NUMPAD_4,
                KEY_EN_NUMPAD_1,
                KEY_EN_NUMPAD_0,

                KEY_EN_UNUSED,
                KEY_EN_NUMPAD_DIVIDE,
                KEY_EN_NUMPAD_8,
                KEY_EN_NUMPAD_5,
                KEY_EN_NUMPAD_2,
                KEY_EN_UNUSED,

                KEY_EN_UNUSED,
                KEY_EN_NUMPAD_TIMES,
                KEY_EN_NUMPAD_9,
                KEY_EN_NUMPAD_6,
                KEY_EN_NUMPAD_3,
                "Key: Number Pad ,",

                KEY_EN_UNUSED,
                KEY_EN_NUMPAD_MINUS,
                KEY_EN_NUMPAD_PLUS,
                KEY_EN_UNUSED,
                KEY_EN_NUMPAD_ENTER,
                KEY_EN_UNUSED,
            }
        }
    },
};
