/*---------------------------------------------------------*\
| RGBController_CorsairWireless.h                           |
|                                                           |
|   RGBController for Corsair wireless keyboard             |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairWirelessController.h"

class RGBController_CorsairWireless : public RGBController
{
public:
    RGBController_CorsairWireless(CorsairWirelessController* controller_ptr);
    ~RGBController_CorsairWireless();

    int         physical_layout;
    int         logical_layout;

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();

private:
    CorsairWirelessController*                          controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
