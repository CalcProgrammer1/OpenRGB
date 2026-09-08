/*---------------------------------------------------------*\
| RGBController_PalitGPUv2.h                                |
|                                                           |
|   RGBController for Palit v2 GPU                          |
|                                                           |
|   KundaPanda                                  04 Jan 2021 |
|   KendallMorgan                               17 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PalitGPUv2Controller.h"

class RGBController_PalitGPUv2 : public RGBController
{
public:
    RGBController_PalitGPUv2(PalitGPUv2Controller* controller_ptr);
    ~RGBController_PalitGPUv2();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PalitGPUv2Controller* controller;
};