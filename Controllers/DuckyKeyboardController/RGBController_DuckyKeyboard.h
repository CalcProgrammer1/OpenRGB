/*---------------------------------------------------------*\
| RGBController_DuckyKeyboard.h                             |
|                                                           |
|   RGBController for Ducky keyboard                        |
|                                                           |
|   Adam Honse (CalcProgrammer1)                04 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "DuckyKeyboardController.h"

class RGBController_DuckyKeyboard : public RGBController
{
public:
    RGBController_DuckyKeyboard(DuckyKeyboardController* controller_ptr);
    ~RGBController_DuckyKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    DuckyKeyboardController* controller;
};
