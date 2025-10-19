/*---------------------------------------------------------*\
| RGBController_AsusMonitor.h                               |
|                                                           |
|   RGBController for Asus monitors                         |
|                                                           |
|   Morgan Guimard (morg)                       19 oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusMonitorController.h"

class RGBController_AsusMonitor : public RGBController
{
public:
    RGBController_AsusMonitor(AsusMonitorController* controller_ptr);
    ~RGBController_AsusMonitor();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    AsusMonitorController* controller;
    unsigned int number_of_leds;
};
