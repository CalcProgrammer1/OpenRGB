/*---------------------------------------------------------*\
| RGBController_PalitGPU.h                                  |
|                                                           |
|   RGBController for Palit GPU                             |
|                                                           |
|   Manatsawin Hanmongkolchai                   11 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PalitGPUController.h"

class RGBController_PalitGPU : public RGBController
{
public:
    RGBController_PalitGPU(PalitGPUController* controller_ptr);

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PalitGPUController* controller;
};
