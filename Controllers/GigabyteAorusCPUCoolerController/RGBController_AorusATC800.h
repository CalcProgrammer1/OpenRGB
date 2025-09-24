/*---------------------------------------------------------*\
| RGBController_AorusATC800.h                               |
|                                                           |
|   RGBController for Aorus ATC800 cooler                   |
|                                                           |
|   Felipe Cavalcanti                           13 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ATC800Controller.h"

class RGBController_AorusATC800 : public RGBController
{
public:
    RGBController_AorusATC800(ATC800Controller* controller_ptr);
    ~RGBController_AorusATC800();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ATC800Controller* controller;
};
