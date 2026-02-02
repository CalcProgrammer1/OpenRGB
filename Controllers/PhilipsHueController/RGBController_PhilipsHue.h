/*---------------------------------------------------------*\
| RGBController_PhilipsHue.h                                |
|                                                           |
|   RGBController for Philips Hue                           |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PhilipsHueController.h"

class RGBController_PhilipsHue : public RGBController
{
public:
    RGBController_PhilipsHue(PhilipsHueController* controller_ptr);
    ~RGBController_PhilipsHue();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PhilipsHueController* controller;
};
