/*---------------------------------------------------------*\
| RGBController_PowerColorRedDevilGPU.cpp                   |
|                                                           |
|   Driver for PowerColor Red Devil GPU                     |
|                                                           |
|   Jana Rettig (SapphicKitten)                 14 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PowerColorRedDevilGPUController.h"

class RGBController_RedDevilGPU : public RGBController
{
public:
    RGBController_RedDevilGPU(RedDevilGPUController* controller_ptr);
    ~RGBController_RedDevilGPU();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RedDevilGPUController* controller;
};
