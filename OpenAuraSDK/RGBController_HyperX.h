/*-----------------------------------------*\
|  RGBController_HyperX.h                   |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  HyperX Predator RGB interface            |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/29/2019   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "HyperXController.h"

class RGBController_HyperX : public RGBController
{
public:
    RGBController_HyperX(HyperXController* hyperx_ptr);
    int GetMode();
    void SetMode(int mode);
    void SetCustomMode();
    void SetAllLEDs(RGBColor color);
    void SetAllZoneLEDs(int zone, RGBColor color);
    void SetLED(int led, RGBColor color);

private:
    HyperXController* hyperx;
};