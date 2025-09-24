/*---------------------------------------------------------*\
| RGBController_GainwardGPUv1.h                             |
|                                                           |
|   RGBController for Gainward v1 GPU                       |
|                                                           |
|   TheRogueZeta                                05 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GainwardGPUv1Controller.h"

class RGBController_GainwardGPUv1 : public RGBController
{
public:
    RGBController_GainwardGPUv1(GainwardGPUv1Controller* controller_ptr);
    ~RGBController_GainwardGPUv1();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    GainwardGPUv1Controller* controller;

    int        GetDeviceMode();
};
