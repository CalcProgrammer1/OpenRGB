/*---------------------------------------------------------*\
| RGBController_RoccatElo.h                                 |
|                                                           |
|   RGBController for Roccat Elo                            |
|                                                           |
|   Flora Aubry                                 02 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    RoccatEloController* controller;
};
