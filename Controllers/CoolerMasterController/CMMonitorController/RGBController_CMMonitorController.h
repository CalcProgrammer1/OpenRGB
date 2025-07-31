/*---------------------------------------------------------*\
| RGBController_CMMonitorController.h                       |
|                                                           |
|   RGBController for Cooler Master monitor                 |
|                                                           |
|   Morgan Guimard (morg)                       18 Sep 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CMMonitorController.h"

class RGBController_CMMonitorController : public RGBController
{
public:
    RGBController_CMMonitorController(CMMonitorController* controller_ptr);
    ~RGBController_CMMonitorController();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    CMMonitorController*   controller;
};
