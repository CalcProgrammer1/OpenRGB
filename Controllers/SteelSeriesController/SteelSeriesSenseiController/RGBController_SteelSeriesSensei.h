/*---------------------------------------------------------*\
| RGBController_SteelSeriesSensei.h                         |
|                                                           |
|   RGBController for SteelSeries Sensei                    |
|                                                           |
|   Based on SteelSeries Rival controller                   |
|   B Horn (bahorn)                             13 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SteelSeriesSenseiController* controller;
};
