/*---------------------------------------------------------*\
| RGBController_AsusStrixClaw.h                             |
|                                                           |
|   RGBController for ASUS Strix Claw mouse                 |
|                                                           |
|   Mola19                                      06 Aug 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusStrixClawController.h"

class RGBController_StrixClaw : public RGBController
{
public:
    RGBController_StrixClaw(StrixClawController* controller_ptr);
    ~RGBController_StrixClaw();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    StrixClawController*  controller;
};
