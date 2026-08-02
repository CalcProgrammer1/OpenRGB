/*---------------------------------------------------------*\
| RGBController_ClevoKeyboard.h                             |
|                                                           |
|   RGBController for Clevo per-key RGB keyboard            |
|   Supports ITE 8291 (PID 0x600B) and ITE 829x (0x8910)    |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|   Valentin Lobstein (balgogan@protonmail.com) 27 Mar 2026 |
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
    explicit RGBController_ClevoKeyboard(ClevoKeyboardController* controller_ptr);
    ~RGBController_ClevoKeyboard();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    ClevoKeyboardController*        controller;
    std::vector<RGBColor*>          buffer_map;
    RGBColor                        null_color;
};
