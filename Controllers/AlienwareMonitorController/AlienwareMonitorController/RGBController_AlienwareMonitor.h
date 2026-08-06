/*---------------------------------------------------------*\
| RGBController_AlienwareMonitor.h                          |
|                                                           |
|   RGBController for Alienware monitors                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
AlienwareMonitorController* controller;
};
