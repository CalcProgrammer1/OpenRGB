/*---------------------------------------------------------*\
| RGBController_RoccatBurstProAir.h                         |
|                                                           |
|   RGBController for Roccat Burst Pro Air                  |
|                                                           |
|   Morgan Guimard (morg)                       16 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatBurstProAirController.h"

class RGBController_RoccatBurstProAir : public RGBController
{
public:
    RGBController_RoccatBurstProAir(RoccatBurstProAirController* controller_ptr);
    ~RGBController_RoccatBurstProAir();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);
    void        DeviceUpdateMode();

private:
    RoccatBurstProAirController* controller;
};
