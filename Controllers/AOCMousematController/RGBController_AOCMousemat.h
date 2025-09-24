/*---------------------------------------------------------*\
| RGBController_AOCMousemat.h                               |
|                                                           |
|   RGBController for AOC mousemat                          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AOCMousematController*  controller;
};
