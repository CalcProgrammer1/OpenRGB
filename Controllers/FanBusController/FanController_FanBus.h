/*-----------------------------------------*\
|  FanController_FanBus.h                   |
|                                           |
|  Generic Fan Interface for FanBus         |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/1/2020    |
\*-----------------------------------------*/

#pragma once
#include "FanController.h"
#include "FanBusController.h"

class FanController_FanBus : public FanController
{
public:
    FanController_FanBus(FanBusController* controller_ptr);

    void        UpdateControl();
    void        UpdateReading();

private:
    FanBusController* controller;
};
