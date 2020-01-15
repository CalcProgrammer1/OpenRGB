/*-----------------------------------------*\
|  RGBController_RGBFusion2.h               |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion 2.0 Driver           |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/15/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RGBFusion2Controller.h"

class RGBController_RGBFusion2 : public RGBController
{
public:
    RGBController_RGBFusion2(RGBFusion2Controller* rgb_fusion_ptr);
    int         GetMode();
    void        SetMode(int mode);
    void        SetCustomMode();
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

private:
    RGBFusion2Controller* rgb_fusion;
};
