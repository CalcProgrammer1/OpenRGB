/*-----------------------------------------*\
|  RGBController_Hue2.h                     |
|                                           |
|  Generic RGB Interface for NZXT Hue 2     |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/29/2019  |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "serial_port.h"
#include "Hue2Controller.h"

class RGBController_Hue2 : public RGBController
{
public:
    RGBController_Hue2(Hue2Controller* hue2_ptr);
    int GetMode();
    void SetMode(int mode);
    void SetCustomMode();
    void SetAllLEDs(RGBColor color);
    void SetAllZoneLEDs(int zone, RGBColor color);
    void SetLED(int led, RGBColor color);
    void UpdateLEDs();

private:
    Hue2Controller*             hue2;
    std::vector<unsigned int>   leds_channel;
    std::vector<unsigned int>   zones_channel;
};