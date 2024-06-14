/*---------------------------------------------------------*\
| RGBController_RoccatKonePro.h                             |
|                                                           |
|   RGBController for Roccat Kone Pro                       |
|                                                           |
|   Garrett Denham (GardenOfWyers)              12 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatKoneProController* controller;
};
