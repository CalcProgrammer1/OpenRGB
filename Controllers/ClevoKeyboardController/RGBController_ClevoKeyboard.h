/*---------------------------------------------------------*\
| RGBController_ClevoKeyboard.h                             |
|                                                           |
|   RGBController for Clevo per-key RGB keyboard (ITE 8291) |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ClevoKeyboardController.h"
#include "ClevoKeyboardDevices.h"

class RGBController_ClevoKeyboard : public RGBController
{
public:
    RGBController_ClevoKeyboard(ClevoKeyboardController* controller_ptr);
    ~RGBController_ClevoKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ClevoKeyboardController*        controller;
    std::vector<RGBColor*>          buffer_map;
    RGBColor                        null_color;
};
