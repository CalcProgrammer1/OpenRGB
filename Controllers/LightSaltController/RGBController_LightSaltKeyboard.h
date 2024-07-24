/*---------------------------------------------------------*\
| RGBController_LightSaltKeyboard.h                         |
|                                                           |
|   RGBController for LightSalt Keyboard                    |
|                                                           |
|   James Buren (braewoods)                     23 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController_LightSalt.h"

class RGBController_LightSaltKeyboard : public RGBController_LightSalt
{
public:
    RGBController_LightSaltKeyboard(LightSaltController* controller_ptr);
};
