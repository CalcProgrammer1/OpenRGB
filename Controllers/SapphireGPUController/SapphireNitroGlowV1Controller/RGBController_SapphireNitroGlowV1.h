/*---------------------------------------------------------*\
| RGBController_SapphireNitroGlowV1.h                       |
|                                                           |
|   RGBController for Sapphire Nitro Glow V1                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SapphireNitroGlowV1Controller* controller;

    void        ReadConfiguration();
};
