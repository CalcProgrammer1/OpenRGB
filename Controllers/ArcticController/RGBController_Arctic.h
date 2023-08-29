/*-----------------------------------------*\
|  RGBController_Arctic.h                   |
|                                           |
|  Generic RGB Interface for Arctic devices |
|                                           |
|  Armin Wolf (Wer-Wolf) 27/08/2023         |
\*-----------------------------------------*/

#pragma once
#include "ArcticController.h"
#include "RGBController.h"
#include "serial_port.h"
#include <chrono>
#include <thread>

class RGBController_Arctic : public RGBController
{
public:
    RGBController_Arctic(ArcticController* controller_ptr);
    ~RGBController_Arctic();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThreadFunction();

private:
    ArcticController*                                   controller;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
    std::atomic<bool>                                   keepalive_thread_run;
    std::thread                                         keepalive_thread;
};

