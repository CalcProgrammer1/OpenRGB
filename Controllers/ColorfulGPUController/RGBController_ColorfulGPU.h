#pragma once

#include "RGBController.h"
#include "ColorfulGPUController.h"

class RGBController_ColorfulGPU : public RGBController
{
public:
    RGBController_ColorfulGPU(ColorfulGPUController* colorful_gpu_ptr);
    ~RGBController_ColorfulGPU();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ColorfulGPUController* controller;

};


