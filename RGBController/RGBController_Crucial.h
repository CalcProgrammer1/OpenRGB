/*-----------------------------------------*\
|  RGBController_Crucial.h                  |
|                                           |
|  Generic RGB Interface for Crucial        |
|  Ballistix RGB controller                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/19/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CrucialController.h"

class RGBController_Crucial : public RGBController
{
public:
    RGBController_Crucial(CrucialController* crucial_ptr);
    int         GetMode();
    void        SetMode(int mode);
    void        SetCustomMode();
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

private:
    CrucialController*  crucial;
};
