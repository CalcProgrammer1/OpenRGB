/*---------------------------------------------------------*\
| RGBController_AkkoKeyboard.h                              |
|                                                           |
|   RGBController for Akko Multi-modes keyboards            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoyuanKeyboardController.h"

class RGBController_AkkoKeyboard : public RGBController
{
public:
    RGBController_AkkoKeyboard(RoyuanKeyboardController* controller_ptr);
    ~RGBController_AkkoKeyboard();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs(std::vector<RGBColor>& update_colors);
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoyuanKeyboardController* controller;
};
