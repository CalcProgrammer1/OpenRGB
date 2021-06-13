/*------------------------------------------*\
|  RGBController_HyperXAlloyOriginsCore.h    |
|                                            |
|  Generic RGB Interface for HyperX Alloy    |
|  Origins Core RGB Keyboard                 |
|                                            |
|  Volodymyr Nazarchuk (Vavooon) 4/28/2021   |
\*------------------------------------------*/

#pragma once
#include <chrono>

#include "RGBController.h"
#include "HyperXAlloyOriginsCoreController.h"

class RGBController_HyperXAlloyOriginsCore : public RGBController
{
public:
    RGBController_HyperXAlloyOriginsCore(HyperXAlloyOriginsCoreController* hyperx_ptr);
    ~RGBController_HyperXAlloyOriginsCore();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
    
    void        KeepaliveThread();
    
private:
    HyperXAlloyOriginsCoreController*                   hyperx;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
