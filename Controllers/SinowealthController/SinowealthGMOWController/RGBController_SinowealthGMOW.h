/*---------------------------------------------------------*\
| RGBController_SinowealthGMOW.h                            |
|                                                           |
|   RGBController for Glorious Model O Wireless             |
|                                                           |
|   Matt Silva (thesilvanator)                     May 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SinowealthGMOWController* controller;
};
