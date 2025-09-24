/*---------------------------------------------------------*\
| RGBController_AsusAuraCore.h                              |
|                                                           |
|   RGBController for ASUS ROG Aura Core                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                13 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusAuraCoreController.h"

class RGBController_AuraCore : public RGBController
{
public:
    RGBController_AuraCore(AuraCoreController* controller_ptr);
    ~RGBController_AuraCore();

    void        SetupKeyboard();
    void        SetupGA15DH();
    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AuraCoreController* controller;
};
