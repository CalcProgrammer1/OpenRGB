/*-----------------------------------------*\
|  RGBController_EVGAGPU.h                  |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  EVGA GPU RGB Driver                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/11/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVGAGPUController.h"

class RGBController_EVGAGPU : public RGBController
{
public:
    RGBController_EVGAGPU(EVGAGPUController* evga_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    EVGAGPUController* evga;
};
