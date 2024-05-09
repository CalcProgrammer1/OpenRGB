/*---------------------------------------------------------*\
| RGBController_AsusAuraMonitor.h                           |
|                                                           |
|   RGBController for ASUS Aura monitor                     |
|                                                           |
|   Mola19                                      08 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusAuraMonitorController.h"

class RGBController_AuraMonitor : public RGBController
{
public:
    RGBController_AuraMonitor(AuraMonitorController* controller_ptr);
    ~RGBController_AuraMonitor();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AuraMonitorController*  controller;
};
