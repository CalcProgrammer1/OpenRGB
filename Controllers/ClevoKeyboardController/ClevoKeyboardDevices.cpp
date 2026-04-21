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
| LED values in FINAL LED ORDER                              |
|                                                           |
| CRITICAL: This array MUST be in the order LEDs appear   |
| in OpenRGB (row-major order after sorting by row,col). |
|                                                           |
| The KeyboardLayoutManager sorts keys by (row, col), so |
| this array must match that sorted order, not zone order!   |
\*---------------------------------------------------------*/
static const std::vector<unsigned int> clevo_tkl_values =
{
    /*---------------------------------------------------------*\
    | Row 0: F-row + extras                                |
    \*---------------------------------------------------------*/
    105,                                                        // Escape
    106, 107, 108, 109,                                         // F1-F4
    110, 111, 112, 113,                                         // F5-F8
    114, 115, 116, 117,                                         // F9-F12
    118,                                                        // Print Screen

    /*---------------------------------------------------------*\
    | Row 1: Number row + extras                              |
    \*---------------------------------------------------------*/
    84,                                                         // Back tick
    85, 86, 87, 88, 89, 90, 91, 92, 93, 94,                     // 1-0
    95, 96,                                                     // Minus, Equals
    98,                                                         // Backspace
    119,                                                        // Insert
    99,                                                         // Num Lock
    100,                                                        // Numpad /
    101,                                                        // Numpad *
    102,                                                        // Numpad -
    121,                                                        // Home
    123,                                                        // Page Up

    /*---------------------------------------------------------*\
    | Row 2: QWERTY row + extras                           |
    \*---------------------------------------------------------*/
    63,                                                         // Tab
    65,                                                         // Q
    66,                                                         // W
    67,                                                         // E
    68,                                                         // R
    69,                                                         // T
    70,                                                         // Y
    71,                                                         // U
    72,                                                         // I
    73,                                                         // O
    74,                                                         // P
    75,                                                         // [
    76,                                                         // ]
    120,                                                        // Delete
    78,                                                         // Numpad 7
    79,                                                         // Numpad 8
    80,                                                         // Numpad 9
    81,                                                         // Numpad +
    122,                                                        // End
    124,                                                        // Page Down

    /*---------------------------------------------------------*\
    | Row 3: ASDF row + numpad                              |
    \*---------------------------------------------------------*/
    42,                                                         // Caps Lock
    44,                                                         // A
    45,                                                         // S
    46,                                                         // D
    47,                                                         // F
    48,                                                         // G
    49,                                                         // H
    50,                                                         // J
    51,                                                         // K
    52,                                                         // L
    53,                                                         // ;
    54,                                                         // '
    55,                                                         // ISO #
    77,                                                         // Enter
    57,                                                         // Numpad 4
    58,                                                         // Numpad 5
    59,                                                         // Numpad 6

    /*---------------------------------------------------------*\
    | Row 4: ZXCV row + numpad + Up arrow                     |
    \*---------------------------------------------------------*/
    22,                                                         // Left Shift
    23,                                                         // ISO backslash
    24,                                                         // Z
    25,                                                         // X
    26,                                                         // C
    27,                                                         // V
    28,                                                         // B
    29,                                                         // N
    30,                                                         // M
    31,                                                         // ,
    32,                                                         // .
    33,                                                         // /
    35,                                                         // Right Shift
    36,                                                         // Numpad 1
    37,                                                         // Numpad 2
    38,                                                         // Numpad 3
    14,                                                         // Up arrow

    /*---------------------------------------------------------*\
    | Row 5: Modifiers + arrows + numpad                      |
    \*---------------------------------------------------------*/
    3,                                                          // Left Win
    4,                                                          // Left Alt
    7,                                                          // Space
    10,                                                         // Right Alt
    12,                                                         // Right Ctrl
    16,                                                         // Numpad 0
    17,                                                         // Numpad .
    13,                                                         // Left arrow
    18,                                                         // Down arrow
    15,                                                         // Right arrow
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
