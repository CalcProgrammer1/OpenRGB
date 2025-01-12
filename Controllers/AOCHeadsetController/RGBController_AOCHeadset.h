/*---------------------------------------------------------*\
| RGBController_AOCHeadset.h                                |
|                                                           |
|   RGBController for AOC headset                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                12 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AOCHeadsetController.h"

class RGBController_AOCHeadset : public RGBController
{
public:
    RGBController_AOCHeadset(AOCHeadsetController* controller_ptr);
    ~RGBController_AOCHeadset();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AOCHeadsetController*  controller;
};
