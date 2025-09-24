/*---------------------------------------------------------*\
| RGBController_CryorigH7QuadLumi.h                         |
|                                                           |
|   RGBController for Cryorig H7 Quad Lumi                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CryorigH7QuadLumiController.h"

class RGBController_CryorigH7QuadLumi : public RGBController
{
public:
    RGBController_CryorigH7QuadLumi(CryorigH7QuadLumiController* controller_ptr);
    ~RGBController_CryorigH7QuadLumi();

    void        SetupModes();
    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceUpdateZoneMode(int zone);

private:
    CryorigH7QuadLumiController*    controller;
    std::vector<unsigned int>       leds_channel;
    std::vector<unsigned int>       zones_channel;
};
