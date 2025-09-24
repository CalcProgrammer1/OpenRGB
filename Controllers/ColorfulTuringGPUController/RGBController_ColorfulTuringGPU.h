/*---------------------------------------------------------*\
| RGBController_ColorfulTuringGPU.h                         |
|                                                           |
|   RGBController for Colorful Turing GPU                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ColorfulTuringGPUController.h"

class RGBController_ColorfulTuringGPU : public RGBController
{
public:
    RGBController_ColorfulTuringGPU(ColorfulTuringGPUController* colorful_gpu_ptr);
    ~RGBController_ColorfulTuringGPU();

    void                         SetupZones();

    void                         DeviceUpdateLEDs();
    void                         DeviceUpdateZoneLEDs(int zone);
    void                         DeviceUpdateSingleLED(int led);

    void                         DeviceUpdateMode();
    void                         DeviceSaveMode();

private:
    ColorfulTuringGPUController* controller;
    int                          getModeIndex(int mode_value);
};


