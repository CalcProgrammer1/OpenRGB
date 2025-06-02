/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2GPU.h                     |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 GPU       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteRGBFusion2GPUController.h"

#define RGB_FUSION_2_GPU_NUMBER_OF_ZONES 5

class RGBController_RGBFusion2GPU : public RGBController
{
public:
    RGBController_RGBFusion2GPU(RGBFusion2GPUController* controller_ptr);
    ~RGBController_RGBFusion2GPU();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    RGBFusion2GPUController* controller;
};
