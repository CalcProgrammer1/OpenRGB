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
    int GetMode();
    void SetMode(int mode);
    void SetAllLEDs(RGBColor color);

private:
    CorsairController* corsair;
};