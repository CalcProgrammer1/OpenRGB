/*---------------------------------------------------------*\
| RGBController_MadCatzCyborg.h                             |
|                                                           |
|   RGB Controller for MadCatz Cyborg Gaming Light          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MadCatzCyborgController.h"

class RGBController_MadCatzCyborg : public RGBController
{
public:
    RGBController_MadCatzCyborg(MadCatzCyborgController* controller_ptr);
    ~RGBController_MadCatzCyborg();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    MadCatzCyborgController* controller;
};
