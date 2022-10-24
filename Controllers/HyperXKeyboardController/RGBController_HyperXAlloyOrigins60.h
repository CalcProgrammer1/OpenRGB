/*-----------------------------------------*\
|  RGBController_HyperXAlloyOrigins60.h     |
|                                           |
|  Generic RGB Interface for HyperX Alloy   |
|  Origins 60 RGB Keyboard                  |
|                                           |
|  Derek Huber 10/24/2022                   |
\*-----------------------------------------*/

#pragma once
#include <chrono>

#include "RGBController.h"
#include "HyperXAlloyOrigins60Controller.h"

class RGBController_HyperXAlloyOrigins60 : public RGBController
{
public:
    RGBController_HyperXAlloyOrigins60(HyperXAlloyOrigins60Controller* controller_ptr);
    ~RGBController_HyperXAlloyOrigins60();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();

private:
    HyperXAlloyOrigins60Controller*                     controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
