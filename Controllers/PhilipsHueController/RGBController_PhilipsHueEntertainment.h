/*---------------------------------------------------------*\
| RGBController_PhilipsHueEntertainment.h                   |
|                                                           |
|   RGBController for Philips Hue Entertainment Mode        |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      07 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <thread>
#include "RGBController.h"
#include "PhilipsHueEntertainmentController.h"

class RGBController_PhilipsHueEntertainment : public RGBController
{
public:
    RGBController_PhilipsHueEntertainment(PhilipsHueEntertainmentController* controller_ptr);

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThreadFunction();

private:
    PhilipsHueEntertainmentController* controller;

    std::atomic<bool>   KeepaliveThreadRunning;
    std::thread*        KeepaliveThread;

    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
