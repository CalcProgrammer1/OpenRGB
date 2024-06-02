/*---------------------------------------------------------*\
| RGBController_NZXTMouse.h                                 |
|                                                           |
|   RGBController for NZXT Mouse                            |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      16 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "NZXTMouseController.h"

class RGBController_NZXTMouse : public RGBController
{
public:
    RGBController_NZXTMouse(NZXTMouseController* controller_ptr);
    ~RGBController_NZXTMouse();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    NZXTMouseController*        controller;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
