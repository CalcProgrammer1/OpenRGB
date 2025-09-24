/*---------------------------------------------------------*\
| RGBController_GainwardGPUv2.h                             |
|                                                           |
|   RGBController for Gainward v2 GPU                       |
|                                                           |
|   KundaPanda                                  04 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GainwardGPUv2Controller.h"

class RGBController_GainwardGPUv2 : public RGBController
{
public:
    RGBController_GainwardGPUv2(GainwardGPUv2Controller* controller_ptr);
    ~RGBController_GainwardGPUv2();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    GainwardGPUv2Controller* controller;
};
