/*---------------------------------------------------------*\
| RGBController_HyperXOrigins2_65.h                         |
|                                                           |
|   RGBController for HyperX Origins 2 65 keyboard          |
|                                                           |
|   Ricardo Amorim                              28 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "HyperXOrigins2_65Controller.h"

class RGBController_HyperXOrigins2_65 : public RGBController
{
public:
    RGBController_HyperXOrigins2_65(HyperXOrigins2_65Controller* controller_ptr);
    ~RGBController_HyperXOrigins2_65();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();

private:
    HyperXOrigins2_65Controller*                        controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};

