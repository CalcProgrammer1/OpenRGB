/*---------------------------------------------------------*\
| RGBController_SteelSeriesArctis5.h                        |
|                                                           |
|   RGBController for SteelSeries Arctis 5                  |
|                                                           |
|   Morgan Guimard                              04 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesArctis5Controller.h"

class RGBController_SteelSeriesArctis5 : public RGBController
{
public:
    RGBController_SteelSeriesArctis5(SteelSeriesArctis5Controller* controller_ptr);
    ~RGBController_SteelSeriesArctis5();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    SteelSeriesArctis5Controller* controller;
};
