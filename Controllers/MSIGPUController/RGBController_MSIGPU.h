/*-----------------------------------------*\
|  RGBController_MSIGPU.h                  |
|                                           |
|  Generic RGB Interface for MSI GPU        |
|                                           |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIGPUController.h"

class RGBController_MSIGPU : public RGBController
{
public:
    RGBController_MSIGPU(MSIGPUController* msi_gpu_ptr);
    ~RGBController_MSIGPU();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    MSIGPUController* msi_gpu;

    int        GetDeviceMode();
};
