/*---------------------------------------------------------*\
| RGBController_Nollie.h                                    |
|                                                           |
|   RGBController for Nollie                                |
|                                                           |
|   Name (cnn1236661)                           25 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "NollieController.h"

class RGBController_Nollie : public RGBController
{
public:
    RGBController_Nollie(NollieController* controller_ptr);
    ~RGBController_Nollie();

    void        SetupZones();
    void        DeviceConfigureZone(int zone_idx);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    NollieController*           controller;
    std::vector<unsigned int>   leds_channel;
};
