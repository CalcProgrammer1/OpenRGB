/*---------------------------------------------------------*\
| RGBController_HyperXMicrophoneV2.h                        |
|                                                           |
|   RGBController for HyperX QuadCast 2 S Microphone        |
|                                                           |
|   Morgan Guimard (morg)                                   |
|   Logan Phillips (Eclipse)                    23 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "HyperXMicrophoneV2Controller.h"

class RGBController_HyperXMicrophoneV2 : public RGBController
{
public:
    RGBController_HyperXMicrophoneV2(HyperXMicrophoneV2Controller* controller_ptr);
    ~RGBController_HyperXMicrophoneV2();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

    void        KeepaliveThread();

private:
    HyperXMicrophoneV2Controller*                       controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
