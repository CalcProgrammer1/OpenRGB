/*---------------------------------------------------------*\
| RGBController_MSIMonitor.h                                |
|                                                           |
|   RGBController for MSI monitor (gaming controller)       |
|                                                           |
|   Andy Herbert                              2026 May 16   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIMonitorController.h"

class RGBController_MSIMonitor : public RGBController
{
public:
    RGBController_MSIMonitor(MSIMonitorController* controller_ptr);
    ~RGBController_MSIMonitor();

    void SetupZones();

    void DeviceUpdateLEDs(std::vector<RGBColor>& update_colors);
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    MSIMonitorController*                               controller;
};
