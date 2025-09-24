/*---------------------------------------------------------*\
| RGBController_AsusROGStrixLC.h                            |
|                                                           |
|   RGBController for ASUS Aura liquid cooler               |
|                                                           |
|   Chris M (Dr_No)                             17 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void                DeviceUpdateLEDs();
    void                DeviceUpdateZoneLEDs(int zone);
    void                DeviceUpdateSingleLED(int led);

    void                DeviceUpdateMode();
private:
    int                 GetDeviceMode();
    int                 GetLED_Zone(int led_idx);

    AsusROGStrixLCController* controller;
};
