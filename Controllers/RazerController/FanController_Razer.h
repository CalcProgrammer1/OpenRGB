/*-----------------------------------------*\
|  FanController_Razer.h                    |
|                                           |
|  Generic Fan Interface for Razer Blade    |
|  laptops that support fan control         |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/23/2021   |
\*-----------------------------------------*/

#pragma once
#include "FanController.h"
#include "RazerController.h"

class FanController_Razer : public FanController
{
public:
    FanController_Razer(RazerController* controller_ptr);

    void        UpdateControl();
    void        UpdateReading();

private:
    RazerController*    controller;
};
