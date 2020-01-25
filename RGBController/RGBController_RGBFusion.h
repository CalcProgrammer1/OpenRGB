/*-----------------------------------------*\
|  RGBController_RGBFusion.h                |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion Driver               |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/11/2019  |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RGBFusionController.h"

class RGBController_RGBFusion : public RGBController
{
public:
    RGBController_RGBFusion(RGBFusionController* rgb_fusion_ptr);
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    RGBFusionController* rgb_fusion;

    int         GetDeviceMode();
};
