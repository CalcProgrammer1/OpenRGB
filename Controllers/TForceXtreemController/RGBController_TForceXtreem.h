/*---------------------------------------------------------*\
| RGBController_TForceXtreem.h                              |
|                                                           |
|   RGBController for TeamGroup T-Force Xtreem RAM          |
|                                                           |
|   Milan Cermak (krysmanta)                    28 Dec 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    TForceXtreemController* controller;

    int         GetDeviceMode();
};
