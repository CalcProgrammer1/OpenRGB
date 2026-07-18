/*---------------------------------------------------------*\
| RGBController_SteelSeriesArctisNova3.h                    |
|                                                           |
|   RGBController for SteelSeries Arctis Nova 3             |
|                                                           |
|   Tomasz Gorczyca (eldiablo123)               18 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/
#pragma once

#include "RGBController.h"
#include "SteelSeriesArctisNova3Controller.h"

class RGBController_SteelSeriesArctisNova3 : public RGBController
{
public:
    RGBController_SteelSeriesArctisNova3(SteelSeriesArctisNova3Controller* controller_ptr);
    ~RGBController_SteelSeriesArctisNova3();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();
    void DeviceSaveMode();

private:
    SteelSeriesArctisNova3Controller* controller;
};
