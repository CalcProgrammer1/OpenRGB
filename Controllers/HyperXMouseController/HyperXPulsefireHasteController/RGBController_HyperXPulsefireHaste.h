/*---------------------------------------------------------*\
| RGBController_HyperXPulsefireHaste.h                      |
|                                                           |
|   RGBController for HyperX Pulsefire Haste                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "HyperXPulsefireHasteController.h"

class RGBController_HyperXPulsefireHaste : public RGBController
{
public:
    RGBController_HyperXPulsefireHaste(HyperXPulsefireHasteController* controller_ptr);
    ~RGBController_HyperXPulsefireHaste();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();

private:
    HyperXPulsefireHasteController*                     controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
