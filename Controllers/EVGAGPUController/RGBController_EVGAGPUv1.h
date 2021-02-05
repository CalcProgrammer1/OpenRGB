/*-----------------------------------------*\
|  RGBController_EVGAGPUv1.h                |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  EVGA GPU RGB V1 (Pascal) Driver          |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/11/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "EVGAGPUv1Controller.h"

class RGBController_EVGAGPUv1 : public RGBController
{
public:
    RGBController_EVGAGPUv1(EVGAGPUv1Controller* evga_ptr);
    ~RGBController_EVGAGPUv1();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    EVGAGPUv1Controller* evga;
};
