/*---------------------------------------------------------*\
| RGBController_SapphireNitroGlowV1.h                       |
|                                                           |
|   RGBController for Sapphire Nitro Glow V1                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SapphireNitroGlowV1Controller.h"

class RGBController_SapphireNitroGlowV1 : public RGBController
{
public:
    RGBController_SapphireNitroGlowV1(SapphireNitroGlowV1Controller* controller_ptr);
    ~RGBController_SapphireNitroGlowV1();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SapphireNitroGlowV1Controller* controller;

    void        ReadConfiguration();
};
