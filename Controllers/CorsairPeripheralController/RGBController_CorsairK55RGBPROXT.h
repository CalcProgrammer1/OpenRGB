/*---------------------------------------------------------*\
| CorsairK55RGBPROXTController.h                            |
|                                                           |
|   Driver for Corsair K55 RGB PRO XT keyboard              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairK55RGBPROXTController.h"

class RGBController_CorsairK55RGBPROXT : public RGBController
{
public:
    RGBController_CorsairK55RGBPROXT(CorsairK55RGBPROXTController* controller_ptr);
    ~RGBController_CorsairK55RGBPROXT();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();
    void KeepaliveThread();

private:
    CorsairK55RGBPROXTController*                       controller;

    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
