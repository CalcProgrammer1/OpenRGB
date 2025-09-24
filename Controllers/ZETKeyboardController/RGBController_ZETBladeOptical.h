/*---------------------------------------------------------*\
| RGBController_ZETBladeOptical.h                           |
|                                                           |
|   RGBController for ZET Blade                             |
|                                                           |
|   Based on HyperX Alloy Elite2 implementation by          |
|   KundaPanda                                              |
|                                                           |
|   Moon_darker (Vaker)                         23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <chrono>
#include <thread>
#include "RGBController.h"
#include "ZETBladeOpticalController.h"

#define ZET_BLADE_OPTICAL_ROWS           6
#define ZET_BLADE_OPTICAL_COLUMNS       22

class RGBController_ZETBladeOptical : public RGBController
{
public:
    RGBController_ZETBladeOptical(ZETBladeOpticalController* controller_ptr);
    ~RGBController_ZETBladeOptical();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ZETBladeOpticalController*                          controller;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
