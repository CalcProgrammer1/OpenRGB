/*---------------------------------------------------------*\
| RGBController_RedragonMouse.h                             |
|                                                           |
|   RGBController for Redragon mouse                        |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Mar 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RedragonMouseController.h"

class RGBController_RedragonMouse : public RGBController
{
public:
    RGBController_RedragonMouse(RedragonMouseController* controller_ptr);
    ~RGBController_RedragonMouse();

    void        SetupZones();
    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RedragonMouseController*   controller;
};
