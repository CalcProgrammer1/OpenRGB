/*---------------------------------------------------------*\
| RGBController_MSIMonitor.cpp                              |
|                                                           |
|   RGBController for MSI monitor (gaming controller)       |
|                                                           |
|   Andy Herbert                              2026 May 16   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "MSIMonitorController.h"

class RGBController_MSIMonitor : public RGBController
{
public:
    RGBController_MSIMonitor(MSIMonitorController* controller_ptr);
    ~RGBController_MSIMonitor();

    void SetupZones();
    void ResizeZone(int zone, int new_size);
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);
    void DeviceUpdateMode();

private:
    MSIMonitorController*                                controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;

    void KeepaliveThread();
};
