/*---------------------------------------------------------*\
| RGBController_SteelSeriesArctis5.h                        |
|                                                           |
|   RGBController for SteelSeries Arctis 5                  |
|                                                           |
|   Morgan Guimard                              04 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    SteelSeriesArctis5Controller* controller;
};
