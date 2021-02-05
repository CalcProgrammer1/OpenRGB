/*-----------------------------------------*\
|  RGBController_GigabyteRGBFusionGPU.h     |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion GPU Driver           |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/23/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteRGBFusionGPUController.h"

class RGBController_RGBFusionGPU : public RGBController
{
public:
    RGBController_RGBFusionGPU(RGBFusionGPUController* rgb_fusion_ptr);
    ~RGBController_RGBFusionGPU();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    RGBFusionGPUController* rgb_fusion;
};
