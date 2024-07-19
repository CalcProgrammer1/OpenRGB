/*---------------------------------------------------------*\
| RGBController_SteelSeriesRival3.h                         |
|                                                           |
|   RGBController for SteelSeries Rival 3                   |
|                                                           |
|   B Horn (bahorn)                             29 Aug 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesAerox3Controller.h"
#include "SteelSeriesRival3Controller.h"

class RGBController_SteelSeriesRival3 : public RGBController
{
public:
    RGBController_SteelSeriesRival3(SteelSeriesMouseController* controller_ptr);
    ~RGBController_SteelSeriesRival3();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    SteelSeriesMouseController* controller;
};
