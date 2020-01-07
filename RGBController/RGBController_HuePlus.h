/*-----------------------------------------*\
|  RGBController_HuePlus.h                  |
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
    int         GetMode();
    void        SetMode(int mode);
    void        SetCustomMode();
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

private:
    HuePlusController*          hueplus;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};
