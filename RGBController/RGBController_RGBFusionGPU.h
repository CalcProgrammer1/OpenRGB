/*-----------------------------------------*\
|  RGBController_RGBFusionGPU.h             |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion GPU Driver           |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/23/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RGBFusionGPUController.h"

class RGBController_RGBFusionGPU : public RGBController
{
public:
    RGBController_RGBFusionGPU(RGBFusionGPUController* rgb_fusion_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    RGBFusionGPUController* rgb_fusion;
};
