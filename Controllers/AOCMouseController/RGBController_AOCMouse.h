/*---------------------------------------------------------*\
| RGBController_AOCMouse.h                                  |
|                                                           |
|   RGBController for AOC mouse                             |
|                                                           |
|   Adam Honse (CalcProgrammer1)                20 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AOCMouseController.h"

class RGBController_AOCMouse : public RGBController
{
public:
    RGBController_AOCMouse(AOCMouseController* controller_ptr);
    ~RGBController_AOCMouse();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AOCMouseController*  controller;
};
