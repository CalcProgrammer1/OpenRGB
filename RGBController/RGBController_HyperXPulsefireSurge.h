/*-----------------------------------------*\
|  RGBController_HyperXPulsefireSurge.h     |
|                                           |
|  Generic RGB Interface for HyperX         |
|  Pulsefire Surge                          |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/25/2020   |
\*-----------------------------------------*/

#pragma once
#include <chrono>

#include "RGBController.h"
#include "HyperXPulsefireSurgeController.h"

class RGBController_HyperXPulsefireSurge : public RGBController
{
public:
    RGBController_HyperXPulsefireSurge(HyperXPulsefireSurgeController* hyperx_ptr);
    ~RGBController_HyperXPulsefireSurge();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

    void        KeepaliveThread();
    
private:
    HyperXPulsefireSurgeController* hyperx;
};
