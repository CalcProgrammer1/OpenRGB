/*---------------------------------------------------------*\
| RGBController_CorsairLightingNode.h                       |
|                                                           |
|   RGBController for Corsair Lighting Node devices         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairLightingNodeController.h"

class RGBController_CorsairLightingNode : public RGBController
{
public:
    RGBController_CorsairLightingNode(CorsairLightingNodeController* controller_ptr);
    ~RGBController_CorsairLightingNode();

    void        SetupModes();
    void        SetupZones();

    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceUpdateZoneMode(int zone);

private:
    CorsairLightingNodeController*  controller;
    std::vector<unsigned int>       leds_channel;
    std::vector<unsigned int>       zones_channel;
};
