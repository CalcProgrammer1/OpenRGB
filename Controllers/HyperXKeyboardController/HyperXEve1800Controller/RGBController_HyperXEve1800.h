/*---------------------------------------------------------*\
| RGBController_HyperXEve1800.h                            |
|                                                           |
|   RGBController for HyperX Eve 1800 keyboard              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "HyperXEve1800Controller.h"

class RGBController_HyperXEve1800 : public RGBController
{
public:
    RGBController_HyperXEve1800(HyperXEve1800Controller* controller_ptr);
    ~RGBController_HyperXEve1800();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();

private:
    HyperXEve1800Controller*                            controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
