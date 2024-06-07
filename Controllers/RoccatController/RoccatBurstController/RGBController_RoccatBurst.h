/*---------------------------------------------------------*\
| RGBController_RoccatBurst.h                               |
|                                                           |
|   RGBController for Roccat Burst                          |
|                                                           |
|   Morgan Guimard (morg)                       01 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatBurstController.h"

class RGBController_RoccatBurst : public RGBController
{
public:
    RGBController_RoccatBurst(RoccatBurstController* controller_ptr, unsigned int leds_count);
    ~RGBController_RoccatBurst();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatBurstController* controller;
    unsigned int leds_count;
};
