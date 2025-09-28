/*---------------------------------------------------------*\
| RGBController_AsusAuraGPU.h                               |
|                                                           |
|   RGBController for ASUS Aura GPU                         |
|                                                           |
|   Jan Rettig (Klapstuhl)                      14 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusAuraGPUController.h"

class RGBController_AuraGPU : public RGBController
{
public:
    RGBController_AuraGPU(AuraGPUController* controller_ptr);
    ~RGBController_AuraGPU();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AuraGPUController* controller;

    int        GetDeviceMode();
};
