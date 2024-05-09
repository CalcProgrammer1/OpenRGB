/*---------------------------------------------------------*\
| RGBController_AOCMousemat.h                               |
|                                                           |
|   RGBController for AOC mousemat                          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AOCMousematController.h"

class RGBController_AOCMousemat : public RGBController
{
public:
    RGBController_AOCMousemat(AOCMousematController* controller_ptr);
    ~RGBController_AOCMousemat();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AOCMousematController*  controller;
};
