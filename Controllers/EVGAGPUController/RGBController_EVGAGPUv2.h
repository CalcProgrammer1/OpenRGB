/*-----------------------------------------*\
|  RGBController_EVGAGPUv2.h                |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  EVGA GPU RGB V2 (Turing) Driver          |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/13/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVGAGPUv2Controller.h"

class RGBController_EVGAGPUv2 : public RGBController
{
public:
    RGBController_EVGAGPUv2(EVGAGPUv2Controller* evga_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    EVGAGPUv2Controller* evga;
};
