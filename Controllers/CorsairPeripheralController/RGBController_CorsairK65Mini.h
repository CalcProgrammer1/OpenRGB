/*---------------------------------------------------------*\
| RGBController_CorsairK65Mini.h                            |
|                                                           |
|   RGBController for Corsair K65 Mini keyboard             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairK65MiniController.h"

class RGBController_CorsairK65Mini : public RGBController
{
public:
    RGBController_CorsairK65Mini(CorsairK65MiniController* controller_ptr);
    ~RGBController_CorsairK65Mini();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

    void KeepaliveThread();

private:
    CorsairK65MiniController*                           controller;

    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
    std::vector<unsigned int>                           led_positions;
};
