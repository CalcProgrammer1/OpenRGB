/*---------------------------------------------------------*\
| RGBController_SinowealthGMOW.h                            |
|                                                           |
|   RGBController for Glorious Model O Wireless             |
|                                                           |
|   Matt Silva (thesilvanator)                     May 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SinowealthGMOWController.h"

class RGBController_GMOW : public RGBController
{
public:
    RGBController_GMOW(SinowealthGMOWController* sinowealth_ptr);
    ~RGBController_GMOW();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SinowealthGMOWController* controller;
};
