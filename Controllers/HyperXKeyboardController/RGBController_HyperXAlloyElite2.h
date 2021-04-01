/*-----------------------------------------*\
|  RGBController_HyperXAlloyElite2.h        |
|                                           |
|  Generic RGB Interface for HyperX Alloy   |
|  Elite2 RGB Keyboard                      |
|                                           |
|  KundaPanda (vojdo) 02/04/2021            |
\*-----------------------------------------*/

#pragma once
#include <atomic>
#include <chrono>
#include <thread>

#include "RGBController.h"
#include "HyperXAlloyElite2Controller.h"

class RGBController_HyperXAlloyElite2 : public RGBController
{
public:
    RGBController_HyperXAlloyElite2(HyperXAlloyElite2Controller* hyperx_ptr);
    ~RGBController_HyperXAlloyElite2();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

    void        KeepaliveThreadFunction();

private:
    HyperXAlloyElite2Controller*                        hyperx;
    std::atomic<bool>                                   keepalive_thread_run;
    std::thread*                                        keepalive_thread;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
