/*---------------------------------------------------------*\
| RGBController_SkyloongGK104Pro.h                          |
|                                                           |
|   RGBController for Skyloong GK104 Pro                    |
|                                                           |
|   Givo (givowo)                               30 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SkyloongGK104ProController.h"

#define BRIGHTNESS_MIN      0
#define BRIGHTNESS_MAX      127

class RGBController_SkyloongGK104Pro : public RGBController
{
public:
    RGBController_SkyloongGK104Pro(SkyloongGK104ProController* controller_ptr);
    ~RGBController_SkyloongGK104Pro();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SkyloongGK104ProController*   controller;
};
