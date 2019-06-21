/*-----------------------------------------*\
|  RGBController_LEDStrip.h                 |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  KeyboardVisualizer LED strip interface   |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/20/2019   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "serial_port.h"
#include "LEDStrip.h"

class RGBController_LEDStrip : public RGBController
{
public:
    RGBController_LEDStrip();
    int GetMode();
    void SetMode(int mode);
    void SetCustomMode();
    void SetAllLEDs(RGBColor color);
    void SetAllZoneLEDs(int zone, RGBColor color);
    void SetLED(int led, RGBColor color);

private:
    std::vector<RGBColor> colors;
    LEDStrip* strip;
};