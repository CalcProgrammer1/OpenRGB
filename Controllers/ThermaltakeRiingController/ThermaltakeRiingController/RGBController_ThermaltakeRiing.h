/*---------------------------------------------------------*\
| RGBController_ThermaltakeRiing.h                          |
|                                                           |
|   RGBController for Thermaltake Riing                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ThermaltakeRiingController.h"

class RGBController_ThermaltakeRiing : public RGBController
{
public:
    RGBController_ThermaltakeRiing(ThermaltakeRiingController* controller_ptr);
    ~RGBController_ThermaltakeRiing();

    void        SetupZones();
    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ThermaltakeRiingController* controller;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
