/*-----------------------------------------*\
|  RGBController_HyperXAlloyElite.h         |
|                                           |
|  Generic RGB Interface for HyperX Alloy   |
|  Elite Keyboard                           |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/2/2020    |
\*-----------------------------------------*/

#pragma once
#include <atomic>
#include <chrono>
#include <thread>

#include "RGBController.h"
#include "HyperXAlloyEliteController.h"

class RGBController_HyperXAlloyElite : public RGBController
{
public:
    RGBController_HyperXAlloyElite(HyperXAlloyEliteController* hyperx_ptr);
    ~RGBController_HyperXAlloyElite();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

    void        KeepaliveThreadFunction();
    
private:
    HyperXAlloyEliteController*                         controller;
    std::atomic<bool>                                   keepalive_thread_run;
    std::thread*                                        keepalive_thread;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
