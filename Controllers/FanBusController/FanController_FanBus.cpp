/*-----------------------------------------*\
|  FanController_FanBus.cpp                 |
|                                           |
|  Generic Fan Interface for FanBus         |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/1/2020    |
\*-----------------------------------------*/

#include "FanController_FanBus.h"

FanController_FanBus::FanController_FanBus(FanBusController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "FanBus Device";
    description = "FanBus Device";
    location    = controller->GetLocation();

    fan new_fan;

    new_fan.name        = "FanBus Fan";
    new_fan.speed_min   = 0;
    new_fan.speed_max   = 255;
    new_fan.speed_cmd   = controller->GetFanPWM();
    new_fan.rpm_rdg     = controller->GetFanRPM();

    fans.push_back(new_fan);
}

void FanController_FanBus::UpdateControl()
{
    controller->SetFanPWM(fans[0].speed_cmd);
}

void FanController_FanBus::UpdateReading()
{
    unsigned short rpm;

    rpm = controller->GetFanRPM();

    fans[0].rpm_rdg = rpm;
}