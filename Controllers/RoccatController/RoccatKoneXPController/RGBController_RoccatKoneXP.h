/*---------------------------------------------------------*\
| RGBController_RoccatKoneXP.h                              |
|                                                           |
|   RGBController for Roccat Kone XP                        |
|                                                           |
|   Mola19                                      12 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatKoneXPController.h"

class RGBController_RoccatKoneXP : public RGBController
{
public:
    RGBController_RoccatKoneXP(RoccatKoneXPController* controller_ptr);
    ~RGBController_RoccatKoneXP();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatKoneXPController* controller;
};
