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
    int         GetMode();
    void        SetMode(int mode);
    void        SetCustomMode();
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

private:
    RGBFusionController* rgb_fusion;
};
