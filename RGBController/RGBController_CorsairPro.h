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
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    CorsairProController* corsair;
};
