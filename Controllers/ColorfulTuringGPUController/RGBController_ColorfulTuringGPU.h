#pragma once

#include "RGBController.h"
#include "ColorfulTuringGPUController.h"

class RGBController_ColorfulTuringGPU : public RGBController
{
public:
    RGBController_ColorfulTuringGPU(ColorfulTuringGPUController* colorful_gpu_ptr);
    ~RGBController_ColorfulTuringGPU();

    void                         SetupZones();
    void                         ResizeZone(int zone, int new_size);
    void                         DeviceUpdateLEDs();
    void                         UpdateZoneLEDs(int zone);
    void                         UpdateSingleLED(int led);
    void                         DeviceUpdateMode();

private:
    ColorfulTuringGPUController* controller;
    int                          getModeIndex(int mode_value);

};


