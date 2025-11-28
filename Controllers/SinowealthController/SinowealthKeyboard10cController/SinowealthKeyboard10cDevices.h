/*---------------------------------------------------------*\
| SinowealthKeyboard10cDevices.cpp                          |
|                                                           |
|   Device list for Sinowealth Keyboards with PID 010C      |
|                                                           |
|   Rodrigo Tavares                             27 Nov 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <map>
#include <utility>

#include "KeyboardLayoutManager.h"

typedef std::pair<const char*, unsigned char> led_pair;

typedef struct
{
    std::string                    device_name;
    keyboard_keymap_overlay_values keyboard_layout;
} sinowealth_device;

typedef std::map<unsigned char, sinowealth_device> sinowealth_device_map;

extern const sinowealth_device_map sinowealth_10c_keyboards;
