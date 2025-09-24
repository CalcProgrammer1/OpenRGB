/*---------------------------------------------------------*\
| RGBController_HyperXPulsefireHaste.h                      |
|                                                           |
|   RGBController for HyperX Pulsefire Haste                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();

private:
    HyperXPulsefireHasteController*                     controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
