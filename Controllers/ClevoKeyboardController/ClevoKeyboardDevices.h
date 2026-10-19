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
#include <vector>
#include "KeyboardLayoutManager.h"

/*---------------------------------------------------------*\
| Clevo keyboard layout definitions                         |
\*---------------------------------------------------------*/
extern keyboard_keymap_overlay_values clevo_keyboard_layout;
extern keyboard_keymap_overlay_values clevo_stellaris16a07_keyboard_layout;
extern keyboard_keymap_overlay_values clevo_829x_keyboard_layout;

/*---------------------------------------------------------*\
| ITE 8291 LED maps                                         |
|   Keyboards with the same PID (0x600B) ship with at least |
|   two PCB revisions that wire a few keys to different     |
|   LEDs. The USB descriptors are identical, so the map is  |
|   chosen in the device's Edit Device dialog.              |
\*---------------------------------------------------------*/
#define CLEVO_ITE8291_LED_MAP_DEFAULT   "Default (TongFang X6AR5xxY)"
#define CLEVO_ITE8291_LED_MAP_ALTERNATE "Alternate (Game Garaj SLAYER R9T)"

typedef struct
{
    const char*     name;
    unsigned int    value;
} clevo_led_override;

extern const std::vector<clevo_led_override> clevo_ite8291_alternate_led_map;
