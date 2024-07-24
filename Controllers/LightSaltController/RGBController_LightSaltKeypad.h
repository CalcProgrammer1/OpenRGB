/*---------------------------------------------------------*\
| RGBController_LightSaltKeypad.h                           |
|                                                           |
|   RGBController for LightSalt Keypad                      |
|                                                           |
|   James Buren (braewoods)                     23 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController_LightSalt.h"

class RGBController_LightSaltKeypad : public RGBController_LightSalt
{
public:
    RGBController_LightSaltKeypad(LightSaltController* controller_ptr);
};
