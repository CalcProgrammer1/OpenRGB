/*---------------------------------------------------------*\
| ClevoKeyboardDevices.h                                    |
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

#pragma once

#include <string>
#include "KeyboardLayoutManager.h"

/*---------------------------------------------------------*\
| Clevo keyboard layout definitions                         |
\*---------------------------------------------------------*/
extern keyboard_keymap_overlay_values clevo_keyboard_layout;
extern keyboard_keymap_overlay_values clevo_829x_keyboard_layout;
