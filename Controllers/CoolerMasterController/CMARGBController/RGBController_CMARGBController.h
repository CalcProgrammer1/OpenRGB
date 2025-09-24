/*---------------------------------------------------------*\
| RGBController_CMARGBController.h                          |
|                                                           |
|   RGBController for Cooler Master ARGB controller         |
|                                                           |
|   Chris M (Dr_No)                             14 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "CMARGBController.h"
#include "RGBController.h"

class RGBController_CMARGBController : public RGBController
{
public:
    RGBController_CMARGBController(CMARGBController* controller_ptr);
    ~RGBController_CMARGBController();

    void        SetupModes();
    void        SetupZones();

    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceUpdateZoneMode(int zone);

private:
    CMARGBController*           controller;
    std::vector<unsigned int>   leds_channel;
};
