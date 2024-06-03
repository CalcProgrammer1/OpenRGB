/*---------------------------------------------------------*\
| RGBController_PalitGPU.h                                  |
|                                                           |
|   RGBController for Palit GPU                             |
|                                                           |
|   Manatsawin Hanmongkolchai                   11 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PalitGPUController.h"

class RGBController_PalitGPU : public RGBController
{
public:
    RGBController_PalitGPU(PalitGPUController* controller_ptr);

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PalitGPUController* controller;
};
