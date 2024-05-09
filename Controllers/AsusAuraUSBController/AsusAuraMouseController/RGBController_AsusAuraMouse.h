/*---------------------------------------------------------*\
| RGBController_AsusAuraMouse.h                             |
|                                                           |
|   RGBController for ASUS Aura mouse                       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AuraMouseController*  controller;
    uint16_t              pid;
};
