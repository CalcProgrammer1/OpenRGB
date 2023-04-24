/*-----------------------------------------*\
|  RGBController_HyperXMousemat.h           |
|                                           |
|  Generic RGB Interface for HyperX         |
|  mousemat                                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/25/2020  |
\*-----------------------------------------*/

#pragma once
#include <chrono>

#include "RGBController.h"
#include "HyperXMousematController.h"

class RGBController_HyperXMousemat : public RGBController
{
public:
    RGBController_HyperXMousemat(HyperXMousematController* controller_ptr, unsigned int first_zone_leds_count_arg, unsigned int second_zone_leds_count_arg);
    ~RGBController_HyperXMousemat();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThread();
    
private:
    HyperXMousematController*                           controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;

    unsigned int first_zone_leds_count;
    unsigned int second_zone_leds_count;
};
