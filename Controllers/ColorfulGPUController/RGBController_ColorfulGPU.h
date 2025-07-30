/*---------------------------------------------------------*\
| RGBController_ColorfulGPU.h                               |
|                                                           |
|   RGBController for Colorful GPU                          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ColorfulGPUController.h"

class RGBController_ColorfulGPU : public RGBController
{
public:
    RGBController_ColorfulGPU(ColorfulGPUController* colorful_gpu_ptr);
    ~RGBController_ColorfulGPU();

    void        SetupZones();
    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ColorfulGPUController* controller;
};
