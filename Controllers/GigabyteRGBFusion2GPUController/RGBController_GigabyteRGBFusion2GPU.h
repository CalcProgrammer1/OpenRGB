/*-----------------------------------------*\
|  RGBController_GigabyteRGBFusion2GPU.h    |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion2 GPU Driver          |
|                                           |
|  based on original by                     |
|  Adam Honse (CalcProgrammer1) 2/23/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteRGBFusion2GPUController.h"

class RGBController_RGBFusion2GPU : public RGBController
{
public:
    RGBController_RGBFusion2GPU(RGBFusion2GPUController* controller_ptr);
    ~RGBController_RGBFusion2GPU();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    uint8_t     getModeIndex(uint8_t mode_value);
    
    RGBFusion2GPUController* controller;
    std::vector<uint8_t>     zoneIndexMap;
};
