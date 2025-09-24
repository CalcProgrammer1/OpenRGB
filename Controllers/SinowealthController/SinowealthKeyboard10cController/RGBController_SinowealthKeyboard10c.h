/*---------------------------------------------------------*\
| RGBController_SinowealthKeyboard10c.h                     |
|                                                           |
|   RGBController for Sinowealth Keyboards with PID 010C    |
|                                                           |
|   Rodrigo Tavares                             27 Nov 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SinowealthKeyboard10cController.h"

class RGBController_SinowealthKeyboard10c : public RGBController
{
public:
    RGBController_SinowealthKeyboard10c(SinowealthKeyboard10cController* controller_ptr, unsigned char model_id);
    ~RGBController_SinowealthKeyboard10c();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    void KeepaliveThreadFunction();

    std::chrono::time_point<std::chrono::steady_clock> last_update_time;
    unsigned char                                      model_id;
    SinowealthKeyboard10cController*                   controller;
    std::atomic<bool>                                  keepalive_thread_run;
    std::thread*                                       keepalive_thread;
};
