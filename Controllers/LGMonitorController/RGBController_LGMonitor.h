/*---------------------------------------------------------*\
| RGBController_LGMonitor.h                                 |
|                                                           |
|   RGBController for LG monitor                            |
|                                                           |
|   Morgan Guimard (morg)                       11 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "LGMonitorController.h"

class RGBController_LGMonitor : public RGBController
{
public:
    RGBController_LGMonitor(LGMonitorController* controller_ptr);
    ~RGBController_LGMonitor();

    void SetupZones();
    void ResizeZone(int zone, int new_size);
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);
    void DeviceUpdateMode();

private:
    LGMonitorController*                                controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;

    void KeepaliveThread();
};
