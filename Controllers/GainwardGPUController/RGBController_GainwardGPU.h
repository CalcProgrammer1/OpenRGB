/*-----------------------------------------*\
|  RGBController_GainwardGPU.h              |
|                                           |
|  Driver for Gainward RGB on GPUs          |
|                                           |
|  TheRogueZeta 11/05/2020                  |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GainwardGPUController.h"

class RGBController_GainwardGPU : public RGBController
{
public:
    RGBController_GainwardGPU(GainwardGPUController* gainward_gpu_ptr);
    ~RGBController_GainwardGPU();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    GainwardGPUController* gainward_gpu;

    int        GetDeviceMode();
};
