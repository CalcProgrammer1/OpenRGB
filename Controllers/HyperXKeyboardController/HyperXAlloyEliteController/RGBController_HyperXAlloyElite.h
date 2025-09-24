/*---------------------------------------------------------*\
| RGBController_HyperXAlloyElite.h                          |
|                                                           |
|   RGBController for HyperX Alloy Elite keyboard           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                02 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <chrono>
#include <thread>
#include "RGBController.h"
#include "HyperXAlloyEliteController.h"

class RGBController_HyperXAlloyElite : public RGBController
{
public:
    RGBController_HyperXAlloyElite(HyperXAlloyEliteController* controller_ptr);
    ~RGBController_HyperXAlloyElite();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThreadFunction();

private:
    HyperXAlloyEliteController*                         controller;
    std::atomic<bool>                                   keepalive_thread_run;
    std::thread*                                        keepalive_thread;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
