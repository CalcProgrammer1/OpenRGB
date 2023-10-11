/*-----------------------------------------*\
|  RGBController_LGMonitor.h                |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  LG monitor RGB USB Driver                |
|                                           |
|  Guimard Morgan (morg) 10/11/2023         |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LGMonitorController.h"
#include <chrono>

class RGBController_LGMonitor : public RGBController
{
public:
    RGBController_LGMonitor(LGMonitorController* controller_ptr);
    ~RGBController_LGMonitor();

    void SetupZones();
    void ResizeZone(int zone, int new_size);
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);
    void DeviceUpdateMode();

private:
    LGMonitorController*                                controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;

    void KeepaliveThread();
};
