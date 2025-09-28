/*---------------------------------------------------------*\
| RGBController_HyperXPulsefireSurge.h                      |
|                                                           |
|   RGBController for HyperX Pulsefire Surge                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "HyperXPulsefireSurgeController.h"

class RGBController_HyperXPulsefireSurge : public RGBController
{
public:
    RGBController_HyperXPulsefireSurge(HyperXPulsefireSurgeController* controller_ptr);
    ~RGBController_HyperXPulsefireSurge();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();

private:
    HyperXPulsefireSurgeController*                     controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
