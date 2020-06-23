/*-----------------------------------------*\
|  FanController_NZXTHue2.h                 |
|                                           |
|  Generic Fan Interface for NZXT Hue 2     |
|  (Smart Device V2, Fan & RGB) controller  |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/22/2020   |
\*-----------------------------------------*/

#pragma once
#include "FanController.h"
#include "NZXTHue2Controller.h"

class FanController_NZXTHue2 : public FanController
{
public:
    FanController_NZXTHue2(NZXTHue2Controller* hue2_ptr);

    void        UpdateControl();
    void        UpdateReading();

private:
    NZXTHue2Controller* hue2;
};
