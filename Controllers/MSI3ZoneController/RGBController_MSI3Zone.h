/*---------------------------------------------------------*\
| RGBController_MSI3Zone.h                                  |
|                                                           |
|   RGBController for MSI/SteelSeries 3-Zone keyboard       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSI3ZoneController.h"

class RGBController_MSI3Zone : public RGBController
{
public:
    RGBController_MSI3Zone(MSI3ZoneController* controller_ptr);
    ~RGBController_MSI3Zone();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    MSI3ZoneController* controller;
};
