/*-----------------------------------------*\
|  RGBController_GainwardGPU.h              |
|                                           |
|  Driver for Gainward RGB v1 on GPUs       |
|                                           |
|  TheRogueZeta 11/05/2020                  |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GainwardGPUv1Controller.h"

class RGBController_GainwardGPUv1 : public RGBController
{
public:
    RGBController_GainwardGPUv1(GainwardGPUv1Controller* gainward_gpu_ptr);
    ~RGBController_GainwardGPUv1();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    GainwardGPUv1Controller* gainward_gpu;

    int        GetDeviceMode();
};
