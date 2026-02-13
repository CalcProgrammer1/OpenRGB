/*---------------------------------------------------------*\
| ClevoKeyboardDevices.cpp                                  |
|                                                           |
|   Device list for Clevo per-key RGB keyboards (ITE 8291)  |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          21 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "ClevoKeyboardDevices.h"

/*---------------------------------------------------------*\
| Clevo Keyboard Layout                                     |
|                                                           |
| Based on KEYBOARD_SIZE_TKL with numpad added and          |
| navigation cluster adjusted to match Clevo's layout.      |
|                                                           |
| Hardware LED indices (value field):                       |
| - Row 5 (F-keys):     105-124                             |
| - Row 4 (numbers):    84-102                              |
| - Row 3 (QWERTY):     63-81                               |
| - Row 2 (home):       42-59                               |
| - Row 1 (Z row):      22-39                               |
| - Row 0 (modifiers):  0-18                                |
\*---------------------------------------------------------*/

/*---------------------------------------------------------*\
| LED values in TKL order (fn_row + main + extras)          |
|                                                           |
| Values follow the key order in KeyboardLayoutManager.cpp. |
| For ANSI-only keys (not present on this ISO keyboard),    |
| use 0 as a placeholder - they won't be displayed.         |
| Numpad values are added via edit_keys.                    |
\*---------------------------------------------------------*/
static const std::vector<unsigned int> clevo_tkl_values =
{
    /*---------------------------------------------------------*\
    | Function row (keyboard_zone_fn_row)                       |
    \*---------------------------------------------------------*/
    105,                                                        // Escape
    106, 107, 108, 109,                                         // F1-F4
    110, 111, 112, 113,                                         // F5-F8
    114, 115, 116, 117,                                         // F9-F12

    /*---------------------------------------------------------*\
    | Main block - Row 1 (keyboard_zone_main)                   |
    \*---------------------------------------------------------*/
    84,                                                         // Back tick
    85, 86, 87, 88, 89, 90, 91, 92, 93, 94,                     // 1-0
    95, 96,                                                     // Minus, Equals
    98,                                                         // Backspace

    /*---------------------------------------------------------*\
    | Main block - Row 2                                        |
    \*---------------------------------------------------------*/
    63,                                                         // Tab
    65, 66, 67, 68, 69, 70, 71, 72, 73, 74,                     // Q-P
    75, 76,                                                     // [ ]
    0,                                                          // ANSI backslash (not on ISO)

    /*---------------------------------------------------------*\
    | Main block - Row 3                                        |
    \*---------------------------------------------------------*/
    42,                                                         // Caps Lock
    44, 45, 46, 47, 48, 49, 50, 51, 52,                         // A-L
    53, 54,                                                     // ; '
    55,                                                         // ISO # (POUND)
    77,                                                         // Enter (ANSI/ISO share same LED)

    /*---------------------------------------------------------*\
    | Main block - Row 4                                        |
    \*---------------------------------------------------------*/
    22,                                                         // Left Shift
    23,                                                         // ISO backslash
    24, 25, 26, 27, 28, 29, 30, 31, 32,                         // Z-. (9 keys)
    33,                                                         // /
    35,                                                         // Right Shift

    /*---------------------------------------------------------*\
    | Main block - Row 5                                        |
    \*---------------------------------------------------------*/
    0,                                                          // Left Ctrl
    3,                                                          // Left Win
    4,                                                          // Left Alt
    7,                                                          // Space
    10,                                                         // Right Alt
    0,                                                          // Right Fn (removed via edit_keys)
    0,                                                          // Menu (removed via edit_keys)
    12,                                                         // Right Ctrl

    /*---------------------------------------------------------*\
    | Extras - Navigation cluster (keyboard_zone_extras)        |
    \*---------------------------------------------------------*/
    118,                                                        // Print Screen
    0,                                                          // Scroll Lock (removed via edit_keys)
    0,                                                          // Pause (removed via edit_keys)
    119,                                                        // Insert
    121,                                                        // Home
    123,                                                        // Page Up
    120,                                                        // Delete
    122,                                                        // End
    124,                                                        // Page Down

    /*---------------------------------------------------------*\
    | Extras - Arrow keys                                       |
    \*---------------------------------------------------------*/
    14,                                                         // Up
    13,                                                         // Left
    18,                                                         // Down
    15,                                                         // Right
};

keyboard_keymap_overlay_values clevo_keyboard_layout
{
    KEYBOARD_SIZE_TKL,
    {
        clevo_tkl_values,
        {
            /* No regional overlays needed */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/

        /*---------------------------------------------------------*\
        | Remove keys not present on Clevo keyboard                 |
        \*---------------------------------------------------------*/
        {   0,      0,      15,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Scroll Lock
        {   0,      0,      15,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Pause
        {   0,      5,      11,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Right Fn
        {   0,      5,      11,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Menu

        /*---------------------------------------------------------*\
        | Add Left Function key                                     |
        \*---------------------------------------------------------*/
        {   0,      5,       1,           2,        KEY_EN_LEFT_FUNCTION,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },

        /*---------------------------------------------------------*\
        | Add Numpad                                                |
        \*---------------------------------------------------------*/
        {   0,      1,      15,          99,        KEY_EN_NUMPAD_LOCK,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      16,         100,        KEY_EN_NUMPAD_DIVIDE,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      17,         101,        KEY_EN_NUMPAD_TIMES,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      18,         102,        KEY_EN_NUMPAD_MINUS,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      15,          78,        KEY_EN_NUMPAD_7,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      16,          79,        KEY_EN_NUMPAD_8,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      17,          80,        KEY_EN_NUMPAD_9,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      18,          81,        KEY_EN_NUMPAD_PLUS,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      15,          57,        KEY_EN_NUMPAD_4,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      16,          58,        KEY_EN_NUMPAD_5,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      17,          59,        KEY_EN_NUMPAD_6,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      15,          36,        KEY_EN_NUMPAD_1,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      16,          37,        KEY_EN_NUMPAD_2,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      17,          38,        KEY_EN_NUMPAD_3,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      18,          39,        KEY_EN_NUMPAD_ENTER,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      15,          16,        KEY_EN_NUMPAD_0,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      16,          17,        KEY_EN_NUMPAD_PERIOD,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    }
};
