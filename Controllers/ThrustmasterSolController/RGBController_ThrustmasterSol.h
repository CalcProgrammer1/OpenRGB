/*---------------------------------------------------------*\
| RGBController_ThrustmasterSol.h                           |
|                                                           |
|   RGBController for Thrustmaster Sol series joysticks     |
|                                                           |
|   Ken Sanislo                                 02 Apr 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ThrustmasterSolController.h"

class RGBController_ThrustmasterSol : public RGBController
{
public:
    RGBController_ThrustmasterSol(ThrustmasterSolController* controller_ptr);
    ~RGBController_ThrustmasterSol();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    ThrustmasterSolController* controller;
};
