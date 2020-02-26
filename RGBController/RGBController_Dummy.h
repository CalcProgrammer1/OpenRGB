/*-----------------------------------------*\
|  RGBController_Dummy.h                    |
|                                           |
|  Generic RGB Interface Dummy Class        |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/25/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"

class RGBController_Dummy : public RGBController
{
public:
    RGBController_Dummy();
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();
};