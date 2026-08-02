/*---------------------------------------------------------*\
| RGBController_SteelSeriesRival5.h                         |
|                                                           |
|   RGBController for SteelSeries Rival 5                   |
|                                                           |
|   A Ingram (Woodsy900)                        20 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesRival5Controller.h"

class RGBController_SteelSeriesRival5 : public RGBController
{
public:
    RGBController_SteelSeriesRival5(SteelSeriesMouseController* controller_ptr);
    ~RGBController_SteelSeriesRival5();

    void        SetupZones();
    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);
    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    SteelSeriesMouseController* controller;
};
