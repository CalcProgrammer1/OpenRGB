/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusionGPU.h                      |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion GPU         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                23 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteRGBFusionGPUController.h"

class RGBController_RGBFusionGPU : public RGBController
{
public:
    RGBController_RGBFusionGPU(RGBFusionGPUController* controller_ptr);
    ~RGBController_RGBFusionGPU();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    RGBFusionGPUController* controller;
};
