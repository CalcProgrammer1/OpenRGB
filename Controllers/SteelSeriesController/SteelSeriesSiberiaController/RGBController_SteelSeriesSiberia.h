/*---------------------------------------------------------*\
| RGBController_SteelSeriesSiberia.h                        |
|                                                           |
|   RGBController for SteelSeries Siberia                   |
|                                                           |
|   E Karlsson (pilophae)                       18 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesSiberiaController.h"

class RGBController_SteelSeriesSiberia : public RGBController
{
public:
    RGBController_SteelSeriesSiberia(SteelSeriesSiberiaController* controller_ptr);
    ~RGBController_SteelSeriesSiberia();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SteelSeriesSiberiaController* controller;
};
