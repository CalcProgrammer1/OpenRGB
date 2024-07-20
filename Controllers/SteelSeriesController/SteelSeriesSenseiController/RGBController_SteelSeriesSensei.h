/*---------------------------------------------------------*\
| RGBController_SteelSeriesSensei.h                         |
|                                                           |
|   RGBController for SteelSeries Sensei                    |
|                                                           |
|   Based on SteelSeries Rival controller                   |
|   B Horn (bahorn)                             13 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesSenseiController.h"

class RGBController_SteelSeriesSensei : public RGBController
{
public:
    RGBController_SteelSeriesSensei(SteelSeriesSenseiController* controller_ptr);
    ~RGBController_SteelSeriesSensei();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SteelSeriesSenseiController* controller;
};
