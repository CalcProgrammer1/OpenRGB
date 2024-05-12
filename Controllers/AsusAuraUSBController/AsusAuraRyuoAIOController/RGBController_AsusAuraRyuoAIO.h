/*---------------------------------------------------------*\
| RGBController_AsusAuraRyuoAIO.h                           |
|                                                           |
|   RGBController for ASUS Aura Ryuo                        |
|                                                           |
|   Cooper Hall (geobot19 / Geo_bot)            08 Apr 2022 |
|   using snipets from Chris M (Dr.No)                      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "LogManager.h"
#include "RGBController.h"
#include "AsusAuraRyuoAIOController.h"

class RGBController_AsusAuraRyuoAIO : public RGBController
{
public:
    RGBController_AsusAuraRyuoAIO(AsusAuraRyuoAIOController* controller_ptr);
    ~RGBController_AsusAuraRyuoAIO();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    int  GetDeviceMode();
    int  GetLED_Zone(int led_idx);

    AsusAuraRyuoAIOController* controller;
};

