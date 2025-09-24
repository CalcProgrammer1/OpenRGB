/*---------------------------------------------------------*\
| RGBController_RoccatKoneProAir.h                          |
|                                                           |
|   RGBController for Roccat Kone Pro Air                   |
|                                                           |
|   Plunti                                      10 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatKoneProAirController.h"

class RGBController_RoccatKoneProAir : public RGBController
{
public:
    RGBController_RoccatKoneProAir(RoccatKoneProAirController* controller_ptr);
    ~RGBController_RoccatKoneProAir();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatKoneProAirController* controller;
};
