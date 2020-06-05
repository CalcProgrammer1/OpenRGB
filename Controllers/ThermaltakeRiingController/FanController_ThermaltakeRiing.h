/*-----------------------------------------*\
|  FanController_ThermaltakeRiing.h         |
|                                           |
|  Generic Fan Interface for Thermaltake    |
|  Riing controller                         |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/5/2020    |
\*-----------------------------------------*/

#pragma once
#include "FanController.h"
#include "ThermaltakeRiingController.h"

class FanController_ThermaltakeRiing : public FanController
{
public:
    FanController_ThermaltakeRiing(ThermaltakeRiingController* riing_ptr);

    void        UpdateControl();
    void        UpdateReading();

private:
    ThermaltakeRiingController* riing;
};
