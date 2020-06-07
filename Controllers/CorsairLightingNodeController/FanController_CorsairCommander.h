/*-----------------------------------------*\
|  FanController_CorsairCommander.h         |
|                                           |
|  Generic Fan Interface for Corsair        |
|  Commander controller                     |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/7/2020    |
\*-----------------------------------------*/

#pragma once
#include "FanController.h"
#include "CorsairLightingNodeController.h"

class FanController_CorsairCommander : public FanController
{
public:
    FanController_CorsairCommander(CorsairLightingNodeController* corsair_ptr);

    void        UpdateControl();
    void        UpdateReading();

private:
    CorsairLightingNodeController* corsair;
};
