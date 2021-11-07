/*-----------------------------------------*\
|  AsusAuraTUFKeyboardController.h          |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Mola19 10/02/2021                        |
\*-----------------------------------------*/

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
                "Key: Escape",
                "Key: `",
                "Key: Tab",
                "Key: Caps Lock",
                "Key: Left Shift",
                "Key: Left Control",

                "Unused",
                "Unused",
                "Unused",
                "Unused",
                "Key: \\",
                "Key: Left Windows",

                "Unused",
                "Key: 1",
                "Key: Q",
                "Key: A",
                "Key: Z",
                "Key: Left Alt",

                "Key: F1",
                "Key: 2",
                "Key: W",
                "Key: S",
                "Key: X",
                "Unused",

                "Key: F2",
                "Key: 3",
                "Key: E",
                "Key: D",
                "Key: C",
                "Unused",

                "Key: F3",
                "Key: 4",
                "Key: R",
                "Key: F",
                "Key: V",
                "Unused",

                "Key: F4",
                "Key: 5",
                "Key: T",
                "Key: G",
                "Key: B",
                "Key: Space",

                "Unused",
                "Key: 6",
                "Key: Y",
                "Key: H",
                "Key: N",
                "Unused",

                "Key: F5",
                "Key: 7",
                "Key: U",
                "Key: J",
                "Key: M",
                "Unused",

                "Key: F6",
                "Key: 8",
                "Key: I",
                "Key: K",
                "Key: ,",
                "Key: Right Alt",

                "Key: F7",
                "Key: 9",
                "Key: O",
                "Key: L",
                "Key: .",
                "Unused",

                "Key: F8",
                "Key: 0",
                "Key: P",
                "Key: ;",
                "Key: /",
                "Key: Right Fn",

                "Key: F9",
                "Key: -",
                "Key: [",
                "Key: '",
                "Unused",
                "Key: Menu",

                "Key: F10",
                "Key: =",
                "Key: ]",
                "Key: #",
                "Unused",
                "Unused",

                "Key: F11",
                "Unused",
                "Unused",
                "Unused",
                "Unused",
                "Unused",

                "Key: F12",
                "Key: Backspace",
                "Unused",
                "Key: Enter",
                "Key: Right Shift",
                "Key: Right Control",

                "Key: Print Screen",
                "Key: Insert",
                "Key: Delete",
                "Unused",
                "Unused",
                "Key: Left Arrow",

                "Key: Scroll Lock",
                "Key: Home",
                "Key: End",
                "Unused",
                "Key: Up Arrow",
                "Key: Down Arrow",

                "Key: Pause/Break",
                "Key: Page Up",
                "Key: Page Down",
                "Unused",
                "Unused",
                "Key: Right Arrow",

                "Unused",
                "Key: Num Lock",
                "Key: Number Pad 7",
                "Key: Number Pad 4",
                "Key: Number Pad 1",
                "Key: Number Pad 0",

                "Unused",
                "Key: Number Pad /",
                "Key: Number Pad 8",
                "Key: Number Pad 5",
                "Key: Number Pad 2",
                "Unused",

                "Unused",
                "Key: Number Pad *",
                "Key: Number Pad 9",
                "Key: Number Pad 6",
                "Key: Number Pad 3",
                "Key: Number Pad ,",

                "Unused",
                "Key: Number Pad -",
                "Key: Number Pad +",
                "Unused",
                "Key: Number Pad Enter",
                "Unused",
            }
        }
    },
    {
        ASUS_TUF_K7_LAYOUT_US,
        {
            (unsigned int *) ASUS_TUF_K7_LAYOUT_KEYS_104,
            {
                "Key: Escape",
                "Key: `",
                "Key: Tab",
                "Key: Caps Lock",
                "Key: Left Shift",
                "Key: Left Control",

                "Unused",
                "Unused",
                "Unused",
                "Unused",
                "Unused",
                "Key: Left Windows",

                "Unused",
                "Key: 1",
                "Key: Q",
                "Key: A",
                "Key: Z",
                "Key: Left Alt",

                "Key: F1",
                "Key: 2",
                "Key: W",
                "Key: S",
                "Key: X",
                "Unused",

                "Key: F2",
                "Key: 3",
                "Key: E",
                "Key: D",
                "Key: C",
                "Unused",

                "Key: F3",
                "Key: 4",
                "Key: R",
                "Key: F",
                "Key: V",
                "Unused",

                "Key: F4",
                "Key: 5",
                "Key: T",
                "Key: G",
                "Key: B",
                "Key: Space",

                "Unused",
                "Key: 6",
                "Key: Y",
                "Key: H",
                "Key: N",
                "Unused",

                "Key: F5",
                "Key: 7",
                "Key: U",
                "Key: J",
                "Key: M",
                "Unused",

                "Key: F6",
                "Key: 8",
                "Key: I",
                "Key: K",
                "Key: ,",
                "Key: Right Alt",

                "Key: F7",
                "Key: 9",
                "Key: O",
                "Key: L",
                "Key: .",
                "Unused",

                "Key: F8",
                "Key: 0",
                "Key: P",
                "Key: ;",
                "Key: /",
                "Key: Right Fn",

                "Key: F9",
                "Key: -",
                "Key: [",
                "Key: '",
                "Unused",
                "Key: Menu",

                "Key: F10",
                "Key: =",
                "Key: ]",
                "Unused",
                "Unused",
                "Unused",

                "Key: F11",
                "Unused",
                "Unused",
                "Unused",
                "Unused",
                "Unused",

                "Key: F12",
                "Key: Backspace",
                "Key: \\",
                "Key: Enter",
                "Key: Right Shift",
                "Key: Right Control",

                "Key: Print Screen",
                "Key: Insert",
                "Key: Delete",
                "Unused",
                "Unused",
                "Key: Left Arrow",

                "Key: Scroll Lock",
                "Key: Home",
                "Key: End",
                "Unused",
                "Key: Up Arrow",
                "Key: Down Arrow",

                "Key: Pause/Break",
                "Key: Page Up",
                "Key: Page Down",
                "Unused",
                "Unused",
                "Key: Right Arrow",

                "Unused",
                "Key: Num Lock",
                "Key: Number Pad 7",
                "Key: Number Pad 4",
                "Key: Number Pad 1",
                "Key: Number Pad 0",

                "Unused",
                "Key: Number Pad /",
                "Key: Number Pad 8",
                "Key: Number Pad 5",
                "Key: Number Pad 2",
                "Unused",

                "Unused",
                "Key: Number Pad *",
                "Key: Number Pad 9",
                "Key: Number Pad 6",
                "Key: Number Pad 3",
                "Key: Number Pad ,",

                "Unused",
                "Key: Number Pad -",
                "Key: Number Pad +",
                "Unused",
                "Key: Number Pad Enter",
                "Unused",
            }
        }
    },
};
