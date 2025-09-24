/*---------------------------------------------------------*\
| RGBController_Alienware.h                                 |
|                                                           |
|   RGBController for Dell Alienware RGB USB controller     |
|                                                           |
|   Gabriel Marcano (gemarcano)                 19 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "AlienwareController.h"

class RGBController_Alienware : public RGBController
{
public:
    RGBController_Alienware(AlienwareController* controller_ptr);

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AlienwareController*                    controller;
    std::chrono::steady_clock::time_point   last_packet_ts;
};
