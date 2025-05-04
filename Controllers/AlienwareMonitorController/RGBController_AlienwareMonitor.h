/*---------------------------------------------------------*\
| RGBController_AlienwareMonitor.h                          |
|                                                           |
|   RGBController for Alienware monitors                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AlienwareMonitorController.h"

class RGBController_AlienwareMonitor : public RGBController
{
public:
RGBController_AlienwareMonitor(AlienwareMonitorController* controller_ptr);
    ~RGBController_AlienwareMonitor();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
AlienwareMonitorController* controller;
};
