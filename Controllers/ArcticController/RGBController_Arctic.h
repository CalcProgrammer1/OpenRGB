/*---------------------------------------------------------*\
| RGBController_Arctic.h                                    |
|                                                           |
|   RGBController for Arctic devices                        |
|                                                           |
|   Armin Wolf (Wer-Wolf)                       09 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include <thread>
#include "ArcticController.h"
#include "RGBController.h"
#include "serial_port.h"

class RGBController_Arctic : public RGBController
{
public:
    RGBController_Arctic(ArcticController* controller_ptr);
    ~RGBController_Arctic();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThreadFunction();

private:
    ArcticController*                                   controller;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
    std::atomic<bool>                                   keepalive_thread_run;
    std::thread                                         keepalive_thread;
};

