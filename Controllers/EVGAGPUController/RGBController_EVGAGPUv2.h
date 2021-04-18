/*-----------------------------------------*\
|  RGBController_EVGAGPUv2.h                |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  EVGA GPU RGB V2 (Turing) Driver          |
|                                           |
|  TheRogueZeta                 4/15/2021   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVGAGPUv2Controller.h"

class RGBController_EVGAGPUv2 : public RGBController
{
public:
    RGBController_EVGAGPUv2(EVGAGPUv2Controller* evga_ptr);
    ~RGBController_EVGAGPUv2();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    int         getModeIndex(unsigned char mode_value);
    EVGAGPUv2Controller* evga;
};
