/*-----------------------------------------*\
|  RGBController_HuePlus.h                 |
|                                           |
|  Generic RGB Interface for NZXT Hue+      |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/20/2019   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "serial_port.h"
#include "HuePlusController.h"

class RGBController_HuePlus : public RGBController
{
public:
    RGBController_HuePlus(HuePlusController* hueplus_ptr);
    int GetMode();
    void SetMode(int mode);
    void SetCustomMode();
    void SetAllLEDs(RGBColor color);
    void SetAllZoneLEDs(int zone, RGBColor color);
    void SetLED(int led, RGBColor color);
    void UpdateLEDs();

private:
    HuePlusController* strip;
};