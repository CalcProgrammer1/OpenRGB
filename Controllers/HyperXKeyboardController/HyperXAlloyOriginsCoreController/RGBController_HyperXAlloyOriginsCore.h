/*---------------------------------------------------------*\
| RGBController_HyperXAlloyOriginsCore.h                    |
|                                                           |
|   RGBController for HyperX Alloy Origins Core keyboard    |
|                                                           |
|   Volodymyr Nazarchuk (Vavooon)               28 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "HyperXAlloyOriginsCoreController.h"

#define HYPERX_ALLOY_ORIGINS_CORE_ANSI      0x09
#define HYPERX_ALLOY_ORIGINS_CORE_ABNT2     0x10

class RGBController_HyperXAlloyOriginsCore : public RGBController
{
public:
    RGBController_HyperXAlloyOriginsCore(HyperXAlloyOriginsCoreController* controller_ptr);
    ~RGBController_HyperXAlloyOriginsCore();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();

private:
    HyperXAlloyOriginsCoreController*                   controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
    unsigned int                                        variant;
};
