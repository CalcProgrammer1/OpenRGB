/*---------------------------------------------------------*\
| RGBController_PalitGPUv1.h                                |
|                                                           |
|   RGBController for Palit v1 GPU                          |
|                                                           |
|   TheRogueZeta                                05 Nov 2020 |
|   Manatsawin Hanmongkolchai                   11 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PalitGPUv1Controller.h"

class RGBController_PalitGPUv1 : public RGBController
{
public:
    RGBController_PalitGPUv1(PalitGPUv1Controller* controller_ptr);
    ~RGBController_PalitGPUv1();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PalitGPUv1Controller* controller;

    int        GetDeviceMode();
};
