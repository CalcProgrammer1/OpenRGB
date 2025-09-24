/*---------------------------------------------------------*\
| RGBController_PNYLovelaceGPU.h                            |
|                                                           |
|   RGBController for PNY Lovelace GPU                      |
|                                                           |
|   yufan                                       01 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PNYLovelaceGPUController.h"

class RGBController_PNYLovelaceGPU : public RGBController
{
public:
    RGBController_PNYLovelaceGPU(PNYLovelaceGPUController* controller_ptr);

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PNYLovelaceGPUController* controller;
};
