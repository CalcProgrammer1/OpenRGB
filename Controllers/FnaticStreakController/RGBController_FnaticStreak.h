/*---------------------------------------------------------*\
| RGBController_FnaticStreak.h                              |
|                                                           |
|   RGBController for Fnatic Streak and miniStreak keyboard |
|                                                           |
|   Based on leddy project by Hanna Czenczek                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "FnaticStreakController.h"

class RGBController_FnaticStreak : public RGBController
{
public:
    RGBController_FnaticStreak(FnaticStreakController* controller_ptr);
    ~RGBController_FnaticStreak();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();

private:
    FnaticStreakController*                             controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
