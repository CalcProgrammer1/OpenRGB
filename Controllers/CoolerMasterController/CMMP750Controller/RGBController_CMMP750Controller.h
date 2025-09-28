/*---------------------------------------------------------*\
| RGBController_CMMP750Controller.h                         |
|                                                           |
|   RGBController for Cooler Master MP750 mousemat          |
|                                                           |
|   Chris M (Dr_No)                             18 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CMMP750Controller.h"

class RGBController_CMMP750Controller : public RGBController
{
public:
    RGBController_CMMP750Controller(CMMP750Controller* controller_ptr);
    ~RGBController_CMMP750Controller();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CMMP750Controller*  controller;

    int         GetDeviceMode();
};
