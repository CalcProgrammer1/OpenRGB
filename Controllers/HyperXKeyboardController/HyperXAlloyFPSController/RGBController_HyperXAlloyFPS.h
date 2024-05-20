/*---------------------------------------------------------*\
| RGBController_HyperXAlloyFPS.h                            |
|                                                           |
|   RGBController for HyperX Alloy FPS keyboard             |
|                                                           |
|   Adam Honse (CalcProgrammer1)                02 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <chrono>
#include <thread>
#include "RGBController.h"
#include "HyperXAlloyFPSController.h"

class RGBController_HyperXAlloyFPS : public RGBController
{
public:
    RGBController_HyperXAlloyFPS(HyperXAlloyFPSController* controller_ptr);
    ~RGBController_HyperXAlloyFPS();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThreadFunction();

private:
    HyperXAlloyFPSController*                           controller;
    std::atomic<bool>                                   keepalive_thread_run;
    std::thread*                                        keepalive_thread;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
