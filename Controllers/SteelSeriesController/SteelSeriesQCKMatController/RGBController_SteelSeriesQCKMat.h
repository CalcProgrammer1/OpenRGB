/*---------------------------------------------------------*\
| RGBController_SteelSeriesQCKMat.h                         |
|                                                           |
|   RGBController for SteelSeries Mouse                     |
|                                                           |
|   Edbgon                                      22 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesQCKMatController.h"

class RGBController_SteelSeriesQCKMat : public RGBController
{
public:
    RGBController_SteelSeriesQCKMat(SteelSeriesQCKMatController* controller_ptr);
    ~RGBController_SteelSeriesQCKMat();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SteelSeriesQCKMatController* controller;
};
