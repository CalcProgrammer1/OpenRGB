/*---------------------------------------------------------*\
| RGBController_CorsairV2SoftwareController.h               |
|                                                           |
|   RGBController for Corsair V2 peripherals - software     |
|   modes                                                   |
|                                                           |
|   Chris M (Dr_No)                             11 Aug 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairPeripheralV2Controller.h"
#include "CorsairPeripheralV2HardwareController.h"
#include "CorsairPeripheralV2SoftwareController.h"

class RGBController_CorsairV2SW : public RGBController
{
public:
    RGBController_CorsairV2SW(CorsairPeripheralV2Controller* controller_ptr);
    ~RGBController_CorsairV2SW();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();
    void KeepaliveThread();

private:
    CorsairPeripheralV2Controller*          controller;

    RGBColor                                null_color              = 0;
    std::vector<RGBColor *>                 buffer_map;

    std::thread*                            keepalive_thread;
    std::atomic<bool>                       keepalive_thread_run;
    std::chrono::time_point
        <std::chrono::steady_clock>         last_update_time;

};
