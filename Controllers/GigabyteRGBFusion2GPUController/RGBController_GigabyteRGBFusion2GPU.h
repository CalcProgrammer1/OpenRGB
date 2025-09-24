/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2GPU.h                     |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 GPU       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    RGBFusion2GPUController* controller;
};
