/*-----------------------------------------*\
|  RGBController_Corsair.h                  |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Corsair Vengeance RGB driver             |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/13/2019   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairController.h"

class RGBController_Corsair : public RGBController
{
public:
    RGBController_Corsair(CorsairController* corsair_ptr);
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    CorsairController* corsair;
};
