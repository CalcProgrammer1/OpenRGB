/*---------------------------------------------------------*\
| RGBController_GalaxGPU.h                                  |
|                                                           |
|   RGBController for Galax/KFA2 GPU                        |
|                                                           |
|   Niels Westphal (crashniels)                 12 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GalaxGPUController.h"

class RGBController_GalaxGPU : public RGBController
{
public:
    RGBController_GalaxGPU(GalaxGPUController* controller_ptr);
    ~RGBController_GalaxGPU();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    GalaxGPUController* controller;

    int        GetDeviceMode();
};
