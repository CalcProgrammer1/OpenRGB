/*---------------------------------------------------------*\
| RGBController_SteelSeriesSiberia.h                        |
|                                                           |
|   RGBController for SteelSeries Siberia                   |
|                                                           |
|   E Karlsson (pilophae)                       18 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SteelSeriesSiberiaController* controller;
};
