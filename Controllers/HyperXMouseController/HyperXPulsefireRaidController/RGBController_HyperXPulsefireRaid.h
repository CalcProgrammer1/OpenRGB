/*---------------------------------------------------------*\
| RGBController_HyperXPulsefireRaid.h                       |
|                                                           |
|   RGBController for HyperX Pulsefire Raid                 |
|                                                           |
|   Morgan Guimard (morg)                       06 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "HyperXPulsefireRaidController.h"

class RGBController_HyperXPulsefireRaid : public RGBController
{
public:
    RGBController_HyperXPulsefireRaid(HyperXPulsefireRaidController* controller_ptr);
    ~RGBController_HyperXPulsefireRaid();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();
    void DeviceSaveMode();

private:
    HyperXPulsefireRaidController*                      controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
    void                                                KeepaliveThread();
};
