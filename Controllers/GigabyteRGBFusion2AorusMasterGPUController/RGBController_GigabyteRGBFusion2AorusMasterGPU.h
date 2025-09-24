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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    RGBFusion2AorusMasterGPUController* controller;
};
