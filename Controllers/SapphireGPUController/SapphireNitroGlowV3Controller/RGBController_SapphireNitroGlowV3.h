/*---------------------------------------------------------*\
| RGBController_SapphireNitroGlowV3.h                       |
|                                                           |
|   RGBController for Sapphire Nitro Glow V3                |
|                                                           |
|   K900                                        03 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SapphireNitroGlowV3Controller* controller;

    void        ReadConfiguration();
};
