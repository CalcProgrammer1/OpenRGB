/*---------------------------------------------------------*\
| RGBController_HoltekA070.h                                |
|                                                           |
|   RGBController for Holtek mouse                          |
|                                                           |
|   Santeri Pikarinen (santeri3700)             01 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "HoltekA070Controller.h"

class RGBController_HoltekA070 : public RGBController
{
public:
    RGBController_HoltekA070(HoltekA070Controller* controller_ptr);
    ~RGBController_HoltekA070();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    HoltekA070Controller* controller;
};
