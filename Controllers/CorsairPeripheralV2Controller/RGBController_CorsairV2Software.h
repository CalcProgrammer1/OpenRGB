/*---------------------------------------------------------*\
| RGBController_CorsairV2SoftwareController.h               |
|                                                           |
|   RGBController for Corsair V2 peripherals - software     |
|   modes                                                   |
|                                                           |
|   Chris M (Dr_No)                             11 Aug 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

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
