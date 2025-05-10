/*-----------------------------------------*\
|  RGBController_Govee.h                    |
|                                           |
|  Generic RGB Interface for Govee          |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/1/2023   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GoveeController.h"

class RGBController_Govee : public RGBController
{
public:
    RGBController_Govee(GoveeController* controller_ptr);
    ~RGBController_Govee();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();

private:
    GoveeController*                                    controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
