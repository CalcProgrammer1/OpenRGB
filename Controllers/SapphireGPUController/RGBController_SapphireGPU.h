/*-----------------------------------------*\
|  RGBController_SapphireGPU.h              |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Sapphire GPU RGB Driver                  |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/15/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SapphireGPUController.h"

class RGBController_SapphireGPU : public RGBController
{
public:
    RGBController_SapphireGPU(SapphireGPUController* sapphire_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    SapphireGPUController* sapphire;
};
