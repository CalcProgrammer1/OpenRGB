/*---------------------------------------------------------*\
| RGBController_PNYGPU.h                                    |
|                                                           |
|   RGBController for PNY Turing GPU                        |
|                                                           |
|   KendallMorgan                               17 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PNYGPUController.h"

class RGBController_PNYGPU : public RGBController
{
public:
    RGBController_PNYGPU(PNYGPUController* controller_ptr);

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PNYGPUController* controller;
};
