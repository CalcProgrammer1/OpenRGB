/*---------------------------------------------------------*\
| RGBController_HyperXMicrophone.h                          |
|                                                           |
|   RGBController for HyperX microphone                     |
|                                                           |
|   Matt Silva (thesilvanator)                              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include "RGBController.h"
#include "HyperXMicrophoneController.h"

class RGBController_HyperXMicrophone : public RGBController
{
public:
    RGBController_HyperXMicrophone(HyperXMicrophoneController* controller_ptr);
    ~RGBController_HyperXMicrophone();

    void        SetupZones();

    void        DeviceResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

    void        KeepaliveThread();

private:
    HyperXMicrophoneController*                         controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
