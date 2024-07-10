/*---------------------------------------------------------*\
| RGBController_SteelSeriesApex3.h                          |
|                                                           |
|   RGBController for SteelSeries Apex 3                    |
|                                                           |
|   Chris M (Dr_No)                             23 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesApex3Controller.h"

enum class APEX3_MODES
{
    DIRECT          = 0,
    RAINBOW_WAVE    = 1
};

class RGBController_SteelSeriesApex3 : public RGBController
{
public:
    RGBController_SteelSeriesApex3(SteelSeriesApex3Controller* controller_ptr);
    ~RGBController_SteelSeriesApex3();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    SteelSeriesApex3Controller*     controller;
};
