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

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AlienwareController*                    controller;
    std::chrono::steady_clock::time_point   last_packet_ts;
};
