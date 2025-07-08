/*---------------------------------------------------------*\
| RGBController_TForceXtreem.h                              |
|                                                           |
|   RGBController for TeamGroup T-Force Xtreem RAM          |
|                                                           |
|   Milan Cermak (krysmanta)                    28 Dec 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "TForceXtreemController.h"

class RGBController_TForceXtreem : public RGBController
{
public:
    RGBController_TForceXtreem(TForceXtreemController* controller_ptr);
    ~RGBController_TForceXtreem();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    TForceXtreemController* controller;

    int         GetDeviceMode();
};
