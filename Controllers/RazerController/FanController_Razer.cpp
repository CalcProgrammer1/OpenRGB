/*-----------------------------------------*\
|  FanController_Razer.cpp                  |
|                                           |
|  Generic Fan Interface for Razer Blade    |
|  laptops that support fan control         |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/23/2021   |
\*-----------------------------------------*/

#include "FanController_Razer.h"

FanController_Razer::FanController_Razer(RazerController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetName();
    description = "Razer Device";
    version     = controller->GetFirmwareString();

    fan new_fan;

    new_fan.name        = "Razer Blade Laptop Fan";
    new_fan.speed_min   = 0;
    new_fan.speed_max   = 5400;
    new_fan.speed_cmd   = 0;
    new_fan.rpm_rdg     = 0;

    fans.push_back(new_fan);
}

void FanController_Razer::UpdateControl()
{
    controller->SetFanRPM(fans[0].speed_cmd);
}

void FanController_Razer::UpdateReading()
{

}