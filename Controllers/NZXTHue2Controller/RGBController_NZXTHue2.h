/*---------------------------------------------------------*\
| RGBController_NZXTHue2.h                                  |
|                                                           |
|   RGBController for NZXT Hue 2                            |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      29 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "NZXTHue2Controller.h"

class RGBController_NZXTHue2 : public RGBController
{
public:
    RGBController_NZXTHue2(NZXTHue2Controller* controller_ptr);
    ~RGBController_NZXTHue2();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    NZXTHue2Controller*         controller;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
