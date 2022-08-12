/*-----------------------------------------*\
|  RGBController_EVGAGPUv3.h                |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  EVGA GPU RGB V3 (Ampere) Driver          |
|                                           |
|  TheRogueZeta                 7/15/2021   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVGAGPUv3Controller.h"

class RGBController_EVGAGPUv3 : public RGBController
{
public:
    RGBController_EVGAGPUv3(EVGAGPUv3Controller* evga_ptr);
    ~RGBController_EVGAGPUv3();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    uint8_t     getModeIndex(uint8_t mode_value);
    EVGAGPUv3Controller* evga;
    std::vector<uint8_t> zoneIndexMap;
};
