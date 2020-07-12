/*-----------------------------------------*\
|  RGBController_HyperXAlloyOrigins.h       |
|                                           |
|  Generic RGB Interface for HyperX Alloy   |
|  Origins RGB Keyboard                     |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/11/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "HyperXAlloyOriginsController.h"

class RGBController_HyperXAlloyOrigins : public RGBController
{
public:
    RGBController_HyperXAlloyOrigins(HyperXAlloyOriginsController* hyperx_ptr);
    ~RGBController_HyperXAlloyOrigins();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();
    
private:
    HyperXAlloyOriginsController*   hyperx;
};
