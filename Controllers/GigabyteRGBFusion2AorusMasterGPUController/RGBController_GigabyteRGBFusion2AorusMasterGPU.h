/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2AorusMasterGPU.h          |
|                                                           |
|   RGBController for Gigabyte AORUS MASTER GPU             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteRGBFusion2AorusMasterGPUController.h"

class RGBController_RGBFusion2AorusMasterGPU : public RGBController
{
public:
    RGBController_RGBFusion2AorusMasterGPU(RGBFusion2AorusMasterGPUController* controller_ptr);
    ~RGBController_RGBFusion2AorusMasterGPU();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    RGBFusion2AorusMasterGPUController* controller;
};
