/*---------------------------------------------------------*\
| RGBController_ThermaltakeRiingQuad.h                      |
|                                                           |
|   RGBController for Thermaltake Riing Quad                |
|                                                           |
|   Chris M (Dr_No)                             15 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ThermaltakeRiingQuadController.h"

class RGBController_ThermaltakeRiingQuad : public RGBController
{
public:
    RGBController_ThermaltakeRiingQuad(ThermaltakeRiingQuadController* controller_ptr);
    ~RGBController_ThermaltakeRiingQuad();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ThermaltakeRiingQuadController* controller;
    std::vector<unsigned int>       leds_channel;
    std::vector<unsigned int>       zones_channel;
};
