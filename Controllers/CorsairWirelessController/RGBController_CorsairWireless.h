/*-----------------------------------------*\
|  RGBController_CorsairWireless.h          |
|                                           |
|  Generic RGB Interface for Corsair RGB    |
|  wireless keyboard devices                |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/8/2021    |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CorsairWirelessController.h"

class RGBController_CorsairWireless : public RGBController
{
public:
    RGBController_CorsairWireless(CorsairWirelessController* corsair_ptr);
    ~RGBController_CorsairWireless();

    int         physical_layout;
    int         logical_layout;

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

    void        KeepaliveThread();

private:
    CorsairWirelessController*      corsair;
    std::thread*                    keepalive_thread;
    std::atomic<bool>               keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
