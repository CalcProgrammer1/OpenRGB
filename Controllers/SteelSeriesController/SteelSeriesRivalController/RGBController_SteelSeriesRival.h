/*---------------------------------------------------------*\
| RGBController_SteelSeriesRival.h                          |
|                                                           |
|   RGBController for SteelSeries Rival                     |
|                                                           |
|   B Horn (bahorn)                             13 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesRivalController.h"

class RGBController_SteelSeriesRival : public RGBController
{
public:
    RGBController_SteelSeriesRival(SteelSeriesRivalController* controller_ptr);
    ~RGBController_SteelSeriesRival();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    SteelSeriesRivalController* controller;
};
