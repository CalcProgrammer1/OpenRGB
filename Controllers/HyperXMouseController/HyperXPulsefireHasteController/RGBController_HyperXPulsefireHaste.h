/*-----------------------------------------*\
|  RGBController_HyperXPulsefireHaste.h     |
|                                           |
|  Generic RGB Interface for HyperX         |
|  Pulsefire Haste                          |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/19/2021   |
\*-----------------------------------------*/

#pragma once
#include <chrono>

#include "RGBController.h"
#include "HyperXPulsefireHasteController.h"

class RGBController_HyperXPulsefireHaste : public RGBController
{
public:
    RGBController_HyperXPulsefireHaste(HyperXPulsefireHasteController* controller_ptr);
    ~RGBController_HyperXPulsefireHaste();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();
    
private:
    HyperXPulsefireHasteController*                     controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
