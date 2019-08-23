/*-----------------------------------------*\
|  RGBController_CorsairPro.h               |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Corsair Vengeance Pro RGB driver         |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/30/2019   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairProController.h"

class RGBController_CorsairPro : public RGBController
{
public:
    RGBController_CorsairPro(CorsairProController* corsair_ptr);
    int GetMode();
    void SetMode(int mode);
    void SetCustomMode();
    void SetAllLEDs(RGBColor color);
    void SetAllZoneLEDs(int zone, RGBColor color);
    void SetLED(int led, RGBColor color);
    void UpdateLEDs();

private:
    CorsairProController* corsair;
};