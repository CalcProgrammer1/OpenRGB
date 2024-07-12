/*---------------------------------------------------------*\
| RGBController_SteelSeriesOldApex.h                        |
|                                                           |
|   RGBController for older SteelSeries Apex keyboards      |
|   (Apex/Apex Fnatic/Apex 350)                             |
|                                                           |
|   Based on findings in ApexCtl by Audrius/tuxmark5, et.   |
|   al, https://github.com/tuxmark5/ApexCtl                 |
|                                                           |
|   David Lee (RAMChYLD)                        15 Nov 2020 |
|   Based on work by B Horn (bahorn)            13 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesOldApexController.h"
#include "color32.h"

class RGBController_SteelSeriesOldApex : public RGBController
{
public:
    RGBController_SteelSeriesOldApex(SteelSeriesOldApexController* controller_ptr);
    ~RGBController_SteelSeriesOldApex();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SteelSeriesOldApexController* controller;
};
