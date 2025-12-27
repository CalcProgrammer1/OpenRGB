/*---------------------------------------------------------*\
| RGBController_ManliGPU.h                                  |
|                                                           |
|   RGBController for Manli GPU                             |
|                                                           |
|   Based on RGBController_ZotacV2GPU                       |
|   Adapted for Manli RTX 4090 Gallardo                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ManliGPUController.h"

class RGBController_ManliGPU : public RGBController
{
public:
    RGBController_ManliGPU(ManliGPUController* controller_ptr);
    ~RGBController_ManliGPU();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

    ManliGPUConfig config;

private:
    ManliGPUController* controller;
};

