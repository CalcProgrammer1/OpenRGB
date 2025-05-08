/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2BlackwellGPU.h            |
|                                                           |
|   RGBController for Gigabyte RGB Fusion 2 Blackwell GPU   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteRGBFusion2BlackwellGPUController.h"

class RGBController_RGBFusion2BlackwellGPU : public RGBController
{
public:
    RGBController_RGBFusion2BlackwellGPU(RGBFusion2BlackwellGPUController* controller_ptr, uint8_t led_layout);
    ~RGBController_RGBFusion2BlackwellGPU();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    RGBFusion2BlackwellGPUController* controller;
    uint8_t     gpu_layout;
};
