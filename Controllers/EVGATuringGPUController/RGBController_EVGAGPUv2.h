/*---------------------------------------------------------*\
| RGBController_EVGAGPUv2.h                                 |
|                                                           |
|   RGBController for EVGA V2 (Turing) GPU                  |
|                                                           |
|   TheRogueZeta                                15 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVGAGPUv2Controller.h"

class RGBController_EVGAGPUv2 : public RGBController
{
public:
    RGBController_EVGAGPUv2(EVGAGPUv2Controller* controller_ptr);
    ~RGBController_EVGAGPUv2();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    EVGAGPUv2Controller* controller;

    int         getModeIndex(unsigned char mode_value);
};
