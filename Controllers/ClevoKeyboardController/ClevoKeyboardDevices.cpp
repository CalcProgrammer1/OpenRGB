/*---------------------------------------------------------*\
| ClevoKeyboardDevices.cpp                                  |
|                                                           |
|   Device list for Clevo per-key RGB keyboards             |
|   Supports ITE 8291 (PID 0x600B) and ITE 829x (0x8910)    |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          21 Jan 2026 |
|   Valentin Lobstein (balgogan@protonmail.com) 27 Mar 2026 |
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
\*---------------------------------------------------------*/

/*---------------------------------------------------------*\
| LED values, in physical (row, then column) order.         |
|                                                           |
| This must match the order KeyboardLayoutManager assigns   |
| default_values against: the structural keymap sorted by   |
| (row, col) across main+fn_row+extras combined, NOT the    |
| fn_row-then-main-then-extras order the zone tables below  |
| are declared in. Getting this wrong silently misassigns   |
| almost every key from Print Screen onward, not just a few.|
|                                                           |
| For ANSI-only keys (not present on this ISO keyboard),    |
| use 0 as a placeholder - they won't be displayed.         |
| Numpad values are added via edit_keys.                    |
\*---------------------------------------------------------*/
static const std::vector<unsigned int> clevo_tkl_values =
{
    /*-----------------------------------------------------*\
    | Row 0: fn_row + Print Screen/Scroll Lock/Pause        |
    \*-----------------------------------------------------*/
    105,                                                        // Escape
    106, 107, 108, 109,                                         // F1-F4
    110, 111, 112, 113,                                         // F5-F8
    114, 115, 116, 117,                                         // F9-F12
    118,                                                        // Print Screen
    0,                                                          // Scroll Lock (removed via edit_keys)
    0,                                                          // Pause (removed via edit_keys)

    /*-----------------------------------------------------*\
    | Row 1: numbers row + Insert/Home/Page Up              |
    \*-----------------------------------------------------*/
    84,                                                         // Back tick
    85, 86, 87, 88, 89, 90, 91, 92, 93, 94,                     // 1-0
    95, 96,                                                     // Minus, Equals
    98,                                                         // Backspace
    119,                                                        // Insert
    121,                                                        // Home
    122,                                                        // Page Up

    /*-----------------------------------------------------*\
    | Row 2: Tab/QWERTY row + Delete/End/PageDown           |
    \*-----------------------------------------------------*/
    63,                                                         // Tab
    65, 66, 67, 68, 69, 70, 71, 72, 73, 74,                     // Q-P
    75, 76,                                                     // [ ]
    0,                                                          // ANSI backslash (not on ISO)
    120,                                                        // Delete
    124,                                                        // End
    123,                                                        // Page Down

    /*-----------------------------------------------------*\
    | Row 3: Caps Lock/home row/Enter                       |
    \*-----------------------------------------------------*/
    42,                                                         // Caps Lock
    44, 45, 46, 47, 48, 49, 50, 51, 52,                         // A-L
    53, 54,                                                     // ; '
    55,                                                         // ISO # (POUND)
    56,                                                         // Enter

    /*-----------------------------------------------------*\
    | Row 4: Left Shift/ISO backslash/Z row + Up            |
    \*-----------------------------------------------------*/
    21,                                                         // Left Shift
    77,                                                         // ISO backslash
    24, 25, 26, 27, 28, 29, 30, 31, 32,                         // Z-. (9 keys)
    33,                                                         // /
    35,                                                         // Right Shift
    14,                                                         // Up

    /*-----------------------------------------------------*\
    | Row 5: bottom modifier row + Left/Down/Right          |
    \*-----------------------------------------------------*/
    0,                                                          // Left Ctrl
    3,                                                          // Left Win
    4,                                                          // Left Alt
    7,                                                          // Space
    10,                                                         // Right Alt
    0,                                                          // Right Fn (removed via edit_keys)
    0,                                                          // Menu (removed via edit_keys)
    12,                                                         // Right Ctrl
    13,                                                         // Left
    18,                                                         // Down
    15,                                                         // Right
};

/*---------------------------------------------------------*\
| ITE 829x LED value mapping                                |
| LED ID = ((row & 7) << 5) | col                           |
| Row 5 (F-keys) = 0xA0+, Row 4 (numbers) = 0x80+           |
| Row 3 (QWERTY) = 0x60+, Row 2 (home)    = 0x40+           |
| Row 1 (Z row)  = 0x20+, Row 0 (mods)    = 0x00+           |
\*---------------------------------------------------------*/
static const std::vector<unsigned int> clevo_829x_full_values =
{
    /*---------------------------------------------------------*\
    | Values in FULL layout visual order (same as CM keyboard)  |
    | LED ID = ((row & 7) << 5) | col                           |
    | Row counts: 16, 21, 21, 17, 18, 13                        |
    \*---------------------------------------------------------*/
/*  ESC   F1    F2    F3    F4    F5    F6    F7    F8    F9    F10   F11   F12   PRSC  SCLK  PSBK */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0,    0,
/*  BKTK  1     2     3     4     5     6     7     8     9     0     -     =     BSPC  INS   HOME  PGUP  NMLK  NMDV  NMTM  NMMI */
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2D, 0x2E, 0x0E, 0x10, 0x12, 0x30, 0x31, 0x32, 0x33,
/*  TAB   Q     W     E     R     T     Y     U     I     O     P     [     ]     ANSI\ DEL   END   PGDN  NM7   NM8   NM9   NMPL */
    0x40, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x0F, 0x11, 0x13, 0x50, 0x51, 0x52, 0x53,
/*  CPLK  A     S     D     F     G     H     J     K     L     ;     '     #     ENTR  NM4   NM5   NM6        */
    0x60, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0,    0x6E, 0x70, 0x71, 0x72,
/*  LSFT  ISO\  Z     X     C     V     B     N     M     ,     .     /     RSFT  ARWU  NM1   NM2   NM3   NMER */
    0x80, 0,    0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8F, 0x90, 0x91, 0x92, 0x93,
/*  LCTL  LFNC  LWIN  LALT  SPACE                         RALT  RMNU  RCTL  ARWL  ARDN  ARWR  NM0   NMPD */
    0xA0, 0xA2, 0xA3, 0xA4, 0xA5,                         0xAA, 0xAB, 0xAC, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2,
};

keyboard_keymap_overlay_values clevo_829x_keyboard_layout
{
    KEYBOARD_SIZE_FULL,
    {
        clevo_829x_full_values,
        {
            /* No regional overlays needed */
        }
    },
    {
        /* No edit keys needed - all keys defined in full layout */
    }
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
        /*---------------------------------------------------------*\
        | Edit Keys                                                 |
        \*---------------------------------------------------------*/

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
