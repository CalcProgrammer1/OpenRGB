/*---------------------------------------------------------*\
| RGBController_RoccatKonePro.h                             |
|                                                           |
|   RGBController for Roccat Kone Pro                       |
|                                                           |
|   Garrett Denham (GardenOfWyers)              12 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatKoneProController.h"

class RGBController_RoccatKonePro : public RGBController
{
public:
    RGBController_RoccatKonePro(RoccatKoneProController* controller_ptr);
    ~RGBController_RoccatKonePro();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatKoneProController* controller;
};
