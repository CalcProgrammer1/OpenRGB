/*---------------------------------------------------------*\
| RGBController_AOCKeyboard.h                               |
|                                                           |
|   RGBController for AOC keyboard                          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                10 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AOCKeyboardController.h"

class RGBController_AOCKeyboard : public RGBController
{
public:
    RGBController_AOCKeyboard(AOCKeyboardController* controller_ptr);
    ~RGBController_AOCKeyboard();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AOCKeyboardController*  controller;
};
