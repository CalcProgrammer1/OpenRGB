/*---------------------------------------------------------*\
| RGBController_Razer.h                                     |
|                                                           |
|   RGBController for Razer devices                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                22 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RazerController.h"

#define NA      0xFFFFFFFF

enum
{
    RAZER_MODE_DIRECT,
    RAZER_MODE_OFF,
    RAZER_MODE_STATIC,
    RAZER_MODE_BREATHING,
    RAZER_MODE_SPECTRUM_CYCLE,
    RAZER_MODE_WAVE,
    RAZER_MODE_REACTIVE,
};

class RGBController_Razer : public RGBController
{
public:
    RGBController_Razer(RazerController* controller_ptr);
    ~RGBController_Razer();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RazerController*    controller;
};
