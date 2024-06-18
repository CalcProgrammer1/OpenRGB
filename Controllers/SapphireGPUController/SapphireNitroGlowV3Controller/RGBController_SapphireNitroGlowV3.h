/*---------------------------------------------------------*\
| RGBController_SapphireNitroGlowV3.h                       |
|                                                           |
|   RGBController for Sapphire Nitro Glow V3                |
|                                                           |
|   K900                                        03 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SapphireNitroGlowV3Controller.h"

class RGBController_SapphireNitroGlowV3 : public RGBController
{
public:
    RGBController_SapphireNitroGlowV3(SapphireNitroGlowV3Controller* controller_ptr);
    ~RGBController_SapphireNitroGlowV3();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SapphireNitroGlowV3Controller* controller;

    void        ReadConfiguration();
};
