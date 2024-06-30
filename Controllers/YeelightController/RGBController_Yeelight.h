/*---------------------------------------------------------*\
| RGBController_Yeelight.h                                  |
|                                                           |
|   RGBController for Yeelight                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                18 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "YeelightController.h"

class RGBController_Yeelight : public RGBController
{
public:
    RGBController_Yeelight(YeelightController* controller_ptr);
    ~RGBController_Yeelight();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    YeelightController* controller;
};
