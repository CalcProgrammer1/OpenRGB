/*---------------------------------------------------------*\
| RGBController_NZXTHuePlus.h                               |
|                                                           |
|   RGBController for NZXT Hue Plus                         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      20 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "serial_port.h"
#include "NZXTHuePlusController.h"

class RGBController_HuePlus : public RGBController
{
public:
    RGBController_HuePlus(HuePlusController* controller_ptr);
    ~RGBController_HuePlus();

    void        SetupModes();
    void        SetupZones();

    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceUpdateZoneMode(int zone);

private:
    HuePlusController*          controller;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
