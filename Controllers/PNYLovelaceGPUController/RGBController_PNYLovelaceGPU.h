/*---------------------------------------------------------*\
| RGBController_PNYLovelaceGPU.h                            |
|                                                           |
|   RGBController for PNY Lovelace GPU                      |
|                                                           |
|   yufan                                       01 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PNYLovelaceGPUController.h"

class RGBController_PNYLovelaceGPU : public RGBController
{
public:
    RGBController_PNYLovelaceGPU(PNYLovelaceGPUController* controller_ptr);

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PNYLovelaceGPUController* controller;
};
