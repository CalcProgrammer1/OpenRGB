/*---------------------------------------------------------*\
| RGBController_EVGAGPUv1.h                                 |
|                                                           |
|   RGBController for EVGA V1 (Pascal) GPU                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVGAGPUv1Controller.h"

class RGBController_EVGAGPUv1 : public RGBController
{
public:
    RGBController_EVGAGPUv1(EVGAGPUv1Controller* controller_ptr);
    ~RGBController_EVGAGPUv1();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    EVGAGPUv1Controller* controller;
};
