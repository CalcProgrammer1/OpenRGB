/*---------------------------------------------------------*\
| RGBController_AsusAuraMouse.h                             |
|                                                           |
|   RGBController for ASUS Aura mouse                       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusAuraMouseController.h"

class RGBController_AuraMouse : public RGBController
{
public:
    RGBController_AuraMouse(AuraMouseController* controller_ptr);
    ~RGBController_AuraMouse();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AuraMouseController*  controller;
    uint16_t              pid;
};
