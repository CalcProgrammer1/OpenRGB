/*---------------------------------------------------------*\
| RGBController_BlinkController.h                           |
|                                                           |
|   RGBController for ThingM Blink                          |
|                                                           |
|   Eric S (edbgon)                             01 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "BlinkController.h"

class RGBController_BlinkController : public RGBController
{
public:
    RGBController_BlinkController(BlinkController* controller_ptr);
    ~RGBController_BlinkController();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    BlinkController* controller;
};
