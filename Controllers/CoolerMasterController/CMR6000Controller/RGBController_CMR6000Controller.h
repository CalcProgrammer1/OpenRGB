/*---------------------------------------------------------*\
| RGBController_CMR6000Controller.h                         |
|                                                           |
|   RGBController for Cooler Master AMD Radeon 6000 series  |
|   GPU                                                     |
|                                                           |
|   Eric S (edbgon)                             02 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CMR6000Controller.h"

class RGBController_CMR6000Controller : public RGBController
{
public:
    RGBController_CMR6000Controller(CMR6000Controller* controller_ptr);
    ~RGBController_CMR6000Controller();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
private:
    CMR6000Controller*  controller;

    int         GetDeviceMode();
};
