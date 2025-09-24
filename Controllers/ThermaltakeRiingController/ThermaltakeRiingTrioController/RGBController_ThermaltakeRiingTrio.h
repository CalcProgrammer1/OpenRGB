/*---------------------------------------------------------*\
| RGBController_ThermaltakeRiingTrio.h                      |
|                                                           |
|   RGBController for Thermaltake Riing Trio                |
|                                                           |
|   Sam B (4rcheria)                            24 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ThermaltakeRiingTrioController.h"

class RGBController_ThermaltakeRiingTrio : public RGBController
{
public:
    RGBController_ThermaltakeRiingTrio(ThermaltakeRiingTrioController* controller_ptr);
    ~RGBController_ThermaltakeRiingTrio();

    void        SetupZones();
    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ThermaltakeRiingTrioController* controller;
    std::vector<unsigned int>       leds_channel;
    std::vector<unsigned int>       zones_channel;
};
