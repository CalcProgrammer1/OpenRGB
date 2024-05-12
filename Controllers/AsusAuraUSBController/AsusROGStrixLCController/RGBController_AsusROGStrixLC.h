/*---------------------------------------------------------*\
| RGBController_AsusROGStrixLC.h                            |
|                                                           |
|   RGBController for ASUS Aura liquid cooler               |
|                                                           |
|   Chris M (Dr_No)                             17 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "LogManager.h"
#include "RGBController.h"
#include "AsusROGStrixLCController.h"

class RGBController_AsusROGStrixLC : public RGBController
{
public:
    RGBController_AsusROGStrixLC(AsusROGStrixLCController* controller_ptr);
    ~RGBController_AsusROGStrixLC();

    void                SetupZones();
    void                ResizeZone(int zone, int new_size);

    void                DeviceUpdateLEDs();
    void                UpdateZoneLEDs(int zone);
    void                UpdateSingleLED(int led);

    void                DeviceUpdateMode();
private:
    int                 GetDeviceMode();
    int                 GetLED_Zone(int led_idx);

    AsusROGStrixLCController* controller;
};
