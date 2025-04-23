/*---------------------------------------------------------*\
| RGBController_Razer.h                                     |
|                                                           |
|   RGBController for Razer Hanbo devices                   |
|                                                           |
|   Joseph East (dripsnek)                      12 Apr 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RazerHanboController.h"

enum
{
    RAZER_HANBO_MODE_DIRECT,
    RAZER_HANBO_MODE_OFF,
    RAZER_HANBO_MODE_SPECTRUM_CYCLE,
};

class RGBController_RazerHanbo : public RGBController
{
public:
    RGBController_RazerHanbo(RazerHanboController* controller_ptr);
    ~RGBController_RazerHanbo();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RazerHanboController*    controller;
    int                      local_mode;
};
