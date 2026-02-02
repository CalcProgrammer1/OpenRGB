/*---------------------------------------------------------*\
| RGBController_PNYGPU.h                                    |
|                                                           |
|   RGBController for PNY Turing GPU                        |
|                                                           |
|   KendallMorgan                               17 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PNYGPUController.h"

class RGBController_PNYGPU : public RGBController
{
public:
    RGBController_PNYGPU(PNYGPUController* controller_ptr);
    ~RGBController_PNYGPU();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PNYGPUController* controller;
};
