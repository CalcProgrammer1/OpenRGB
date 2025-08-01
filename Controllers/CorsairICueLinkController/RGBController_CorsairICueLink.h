/*---------------------------------------------------------*\
| RGBController_CorsairICueLink.h                           |
|                                                           |
|   RGBController for Corsair iCue Link Devices             |
|                                                           |
|   Aiden Vigue (acvigue)                       02 Mar 2025 |
|   Adam Honse <calcprogrammer1@gmail.com>      01 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <chrono>
#include <thread>
#include "CorsairICueLinkController.h"
#include "RGBController.h"

class RGBController_CorsairICueLink : public RGBController
{
public:
    RGBController_CorsairICueLink(CorsairICueLinkController* controller);
    ~RGBController_CorsairICueLink();

    void SetupZones();

    void ResizeZone(int zone, int new_size);
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    CorsairICueLinkController*                          controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_commit_time;

    void KeepaliveThread();
};
