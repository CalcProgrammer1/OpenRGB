/*---------------------------------------------------------*\
| RGBController_AsusAuraMonitor.h                           |
|                                                           |
|   RGBController for ASUS Aura monitor                     |
|                                                           |
|   Mola19                                      08 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AuraMonitorController*  controller;
};
