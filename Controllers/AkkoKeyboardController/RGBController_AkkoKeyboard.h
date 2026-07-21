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
#include "AkkoKeyboardController.h"

class RGBController_AkkoKeyboard : public RGBController
{
public:
    RGBController_AkkoKeyboard(AkkoKeyboardController* controller_ptr);
    ~RGBController_AkkoKeyboard();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AkkoKeyboardController* controller;
};
