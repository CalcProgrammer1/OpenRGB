/*-----------------------------------------*\
|  RGBController_MSIGPUv2.h                 |
|                                           |
|  ITE9 RGB Interface for MSI GPU           |
|  Wojciech Lazarski 03/Jan/2023            |
|                                           |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIGPUv2Controller.h"

class RGBController_MSIGPUv2 : public RGBController
{
public:
    RGBController_MSIGPUv2(MSIGPUv2Controller* msi_gpu_ptr);
    ~RGBController_MSIGPUv2();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    MSIGPUv2Controller* msi_gpu;
    void       DeviceUpdateAll(const mode& current_mode);
};
