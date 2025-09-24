/*---------------------------------------------------------*\
| RGBController_NZXTHue1.h                                  |
|                                                           |
|   RGBController for NZXT Hue 1 (Smart Device V1)          |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      16 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "NZXTHue1Controller.h"

class RGBController_NZXTHue1 : public RGBController
{
public:
    RGBController_NZXTHue1(NZXTHue1Controller* controller_ptr);
    ~RGBController_NZXTHue1();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    NZXTHue1Controller*         controller;
};
