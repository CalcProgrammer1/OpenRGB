/*---------------------------------------------------------*\
| RGBController_SteelSeriesApex.h                           |
|                                                           |
|   RGBController for SteelSeries Apex 7                    |
|                                                           |
|   Eric Samuelson (edbgon)                     05 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "SteelSeriesApexBaseController.h"
#include "SteelSeriesGeneric.h"

class RGBController_SteelSeriesApex : public RGBController
{
public:
    RGBController_SteelSeriesApex(SteelSeriesApexBaseController* controller_ptr);
    ~RGBController_SteelSeriesApex();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SteelSeriesApexBaseController*  controller;
    steelseries_type                proto_type;

    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
