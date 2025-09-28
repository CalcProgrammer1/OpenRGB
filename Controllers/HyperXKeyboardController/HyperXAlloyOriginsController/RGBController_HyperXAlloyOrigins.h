/*---------------------------------------------------------*\
| RGBController_HyperXAlloyOrigins.h                        |
|                                                           |
|   RGBController for HyperX Alloy Origins keyboard         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "HyperXAlloyOriginsController.h"

class RGBController_HyperXAlloyOrigins : public RGBController
{
public:
    RGBController_HyperXAlloyOrigins(HyperXAlloyOriginsController* controller_ptr);
    ~RGBController_HyperXAlloyOrigins();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();

private:
    HyperXAlloyOriginsController*                       controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
