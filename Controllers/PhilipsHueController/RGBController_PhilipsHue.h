/*---------------------------------------------------------*\
| RGBController_PhilipsHue.h                                |
|                                                           |
|   RGBController for Philips Hue                           |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PhilipsHueController.h"

class RGBController_PhilipsHue : public RGBController
{
public:
    RGBController_PhilipsHue(PhilipsHueController* controller_ptr);

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    PhilipsHueController* controller;
};
