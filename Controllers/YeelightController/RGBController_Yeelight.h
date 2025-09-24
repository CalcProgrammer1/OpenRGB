/*---------------------------------------------------------*\
| RGBController_Yeelight.h                                  |
|                                                           |
|   RGBController for Yeelight                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                18 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    YeelightController* controller;
};
