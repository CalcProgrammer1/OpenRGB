/*-----------------------------------------*\
|  RGBController_PhilipsHueEntertainment.h  |
|                                           |
|  Generic RGB Interface for Philips Hue    |
|  Entertainment Mode                       |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/7/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "PhilipsHueEntertainmentController.h"

#include <atomic>
#include <thread>

class RGBController_PhilipsHueEntertainment : public RGBController
{
public:
    RGBController_PhilipsHueEntertainment(PhilipsHueEntertainmentController* light_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

    void        KeepaliveThreadFunction();

private:
    PhilipsHueEntertainmentController* light;

    std::atomic<bool>   KeepaliveThreadRunning;
    std::thread*        KeepaliveThread;

    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
