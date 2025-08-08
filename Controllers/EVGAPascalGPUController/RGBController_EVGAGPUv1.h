/*---------------------------------------------------------*\
| RGBController_EVGAGPUv1.h                                 |
|                                                           |
|   RGBController for EVGA V1 (Pascal) GPU                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    EVGAGPUv1Controller* controller;
};
