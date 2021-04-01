/*-----------------------------------------*\
|  RGBController_GainwardGPUv2.h            |
|                                           |
|  Driver for Gainward RGB v2 on GPUs       |
|                                           |
|  KundaPanda 01/04/2021                    |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GainwardGPUv2Controller.h"

class RGBController_GainwardGPUv2 : public RGBController
{
public:
    RGBController_GainwardGPUv2(GainwardGPUv2Controller* gainward_gpu_ptr);
    ~RGBController_GainwardGPUv2();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    GainwardGPUv2Controller* gainward_gpu;
};
