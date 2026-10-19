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

/*---------------------------------------------------------*\
| Alternate ITE 8291 LED map                                |
|   Only these keys differ from clevo_tkl_values. LED index |
|   is row * 21 + col: 22 and 23 are row 1, columns 1-2     |
|   (before Z at 24), 77 is the top half of the ISO Enter.  |
|   The layout manager names the ISO Enter "Key: Enter".    |
\*---------------------------------------------------------*/
const std::vector<clevo_led_override> clevo_ite8291_alternate_led_map =
{
    { KEY_EN_ANSI_ENTER,        77  },
    { KEY_EN_LEFT_SHIFT,        22  },
    { KEY_EN_ISO_BACK_SLASH,    23  },
};

/*---------------------------------------------------------*\
| TUXEDO Stellaris 16 Gen7 AMD (STELLARIS16A07)             |
| ITE 8291 (USB 048d:600b), 102 physical ISO keys.          |
| Coordinates are visual; values are hardware LED IDs.      |
| Rows: 0=Fn/nav, 1=numbers, 2=Q, 3=A,                      |
|       4=bottom letters, 5=modifiers/Up, 6=arrows.         |
| Descending order allows SwapKey to build a sorted         |
| keymap without INSERT_SHIFT_RIGHT side effects.           |
\*---------------------------------------------------------*/
keyboard_keymap_overlay_values clevo_stellaris16a07_keyboard_layout =
{
    KEYBOARD_SIZE_EMPTY,
    { {}, {} },
    {
        {   0,      6,      15,          15,        KEY_EN_RIGHT_ARROW,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      6,      14,          18,        KEY_EN_DOWN_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      6,      13,          13,        KEY_EN_LEFT_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      5,      18,          17,        KEY_EN_NUMPAD_PERIOD,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      5,      16,          16,        KEY_EN_NUMPAD_0,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      5,      14,          14,        KEY_EN_UP_ARROW,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      5,      11,          12,        KEY_EN_RIGHT_CONTROL,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      5,      10,          10,        KEY_EN_RIGHT_ALT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      5,       6,           7,        KEY_EN_SPACE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      5,       3,           4,        KEY_EN_LEFT_ALT,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      5,       2,           3,        KEY_EN_LEFT_WINDOWS,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      5,       1,           2,        KEY_EN_LEFT_FUNCTION,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      5,       0,           0,        KEY_EN_LEFT_CONTROL,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,      19,          39,        KEY_EN_NUMPAD_ENTER,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,      18,          38,        KEY_EN_NUMPAD_3,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,      17,          37,        KEY_EN_NUMPAD_2,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,      16,          36,        KEY_EN_NUMPAD_1,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,      12,          35,        KEY_EN_RIGHT_SHIFT,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,      11,          33,        KEY_EN_FORWARD_SLASH,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,      10,          32,        KEY_EN_PERIOD,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,       9,          31,        KEY_EN_COMMA,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,       8,          30,        KEY_EN_M,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,       7,          29,        KEY_EN_N,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,       6,          28,        KEY_EN_B,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,       5,          27,        KEY_EN_V,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,       4,          26,        KEY_EN_C,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,       3,          25,        KEY_EN_X,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,       2,          24,        KEY_EN_Z,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,       1,          23,        KEY_EN_ISO_BACK_SLASH,     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      4,       0,          22,        KEY_EN_LEFT_SHIFT,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,      18,          59,        KEY_EN_NUMPAD_6,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,      17,          58,        KEY_EN_NUMPAD_5,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,      16,          57,        KEY_EN_NUMPAD_4,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,      13,          77,        KEY_EN_ISO_ENTER,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,      12,          55,        KEY_EN_POUND,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,      11,          54,        KEY_EN_QUOTE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,      10,          53,        KEY_EN_SEMICOLON,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,       9,          52,        KEY_EN_L,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,       8,          51,        KEY_EN_K,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,       7,          50,        KEY_EN_J,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,       6,          49,        KEY_EN_H,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,       5,          48,        KEY_EN_G,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,       4,          47,        KEY_EN_F,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,       3,          46,        KEY_EN_D,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,       2,          45,        KEY_EN_S,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,       1,          44,        KEY_EN_A,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      3,       0,          42,        KEY_EN_CAPS_LOCK,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,      19,          81,        KEY_EN_NUMPAD_PLUS,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,      18,          80,        KEY_EN_NUMPAD_9,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,      17,          79,        KEY_EN_NUMPAD_8,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,      16,          78,        KEY_EN_NUMPAD_7,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,      12,          76,        KEY_EN_RIGHT_BRACKET,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,      11,          75,        KEY_EN_LEFT_BRACKET,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,      10,          74,        KEY_EN_P,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,       9,          73,        KEY_EN_O,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,       8,          72,        KEY_EN_I,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,       7,          71,        KEY_EN_U,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,       6,          70,        KEY_EN_Y,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,       5,          69,        KEY_EN_T,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,       4,          68,        KEY_EN_R,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,       3,          67,        KEY_EN_E,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,       2,          66,        KEY_EN_W,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,       1,          65,        KEY_EN_Q,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      2,       0,          63,        KEY_EN_TAB,                KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,      19,         102,        KEY_EN_NUMPAD_MINUS,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,      18,         101,        KEY_EN_NUMPAD_TIMES,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,      17,         100,        KEY_EN_NUMPAD_DIVIDE,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,      16,          99,        KEY_EN_NUMPAD_LOCK,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,      13,          98,        KEY_EN_BACKSPACE,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,      12,          96,        KEY_EN_EQUALS,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,      11,          95,        KEY_EN_MINUS,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,      10,          94,        KEY_EN_0,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,       9,          93,        KEY_EN_9,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,       8,          92,        KEY_EN_8,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,       7,          91,        KEY_EN_7,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,       6,          90,        KEY_EN_6,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,       5,          89,        KEY_EN_5,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,       4,          88,        KEY_EN_4,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,       3,          87,        KEY_EN_3,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,       2,          86,        KEY_EN_2,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,       1,          85,        KEY_EN_1,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      1,       0,          84,        KEY_EN_BACK_TICK,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,      19,         124,        KEY_EN_PAGE_DOWN,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,      18,         123,        KEY_EN_PAGE_UP,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,      17,         122,        KEY_EN_END,                KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,      16,         121,        KEY_EN_HOME,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,      15,         120,        KEY_EN_DELETE,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,      14,         119,        KEY_EN_INSERT,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,      13,         118,        KEY_EN_PRINT_SCREEN,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,      12,         117,        KEY_EN_F12,                KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,      11,         116,        KEY_EN_F11,                KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,      10,         115,        KEY_EN_F10,                KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,       9,         114,        KEY_EN_F9,                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,       8,         113,        KEY_EN_F8,                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,       7,         112,        KEY_EN_F7,                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,       6,         111,        KEY_EN_F6,                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,       5,         110,        KEY_EN_F5,                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,       4,         109,        KEY_EN_F4,                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,       3,         108,        KEY_EN_F3,                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,       2,         107,        KEY_EN_F2,                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,       1,         106,        KEY_EN_F1,                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
        {   0,      0,       0,         105,        KEY_EN_ESCAPE,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY, },
    }
};
