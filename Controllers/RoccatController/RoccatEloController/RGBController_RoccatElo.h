/*---------------------------------------------------------*\
| RGBController_RoccatElo.h                                 |
|                                                           |
|   RGBController for Roccat Elo                            |
|                                                           |
|   Flora Aubry                                 02 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatEloController.h"

class RGBController_RoccatElo : public RGBController
{
public:
    RGBController_RoccatElo(RoccatEloController* controller_ptr);
    ~RGBController_RoccatElo();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    RoccatEloController* controller;
};
