/*-----------------------------------------*\
|  FanController_NZXTHue2.cpp               |
|                                           |
|  Generic Fan Interface for NZXT Hue 2     |
|  (Smart Device V2, Fan & RGB) controller  |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/22/2020   |
\*-----------------------------------------*/

#include "FanController_NZXTHue2.h"

FanController_NZXTHue2::FanController_NZXTHue2(NZXTHue2Controller* hue2_ptr)
{
    hue2 = hue2_ptr;

    name        = "NZXT Hue 2 Device";
    description = "NZXT Hue 2 Device";
    version     = hue2->GetFirmwareVersion();

    for(std::size_t fan_index = 0; fan_index < hue2->GetNumFanChannels(); fan_index++)
    {
        fan new_fan;

        hue2->UpdateStatus();

        new_fan.name        = "NZXT Fan ";
        new_fan.name.append(std::to_string(fan_index + 1));
        new_fan.speed_min   = 0;
        new_fan.speed_max   = 100;
        new_fan.speed_cmd   = hue2->GetFanCommand(fan_index);
        new_fan.rpm_rdg     = hue2->GetFanRPM(fan_index);

        fans.push_back(new_fan);
    }

    UpdateControl();
}

void FanController_NZXTHue2::UpdateControl()
{
    for(std::size_t fan_index = 0; fan_index < fans.size(); fan_index++)
    {
        hue2->SendFan(fan_index, 0, fans[fan_index].speed_cmd);
    }
}

void FanController_NZXTHue2::UpdateReading()
{
    hue2->UpdateStatus();

    for(std::size_t fan_index = 0; fan_index < fans.size(); fan_index++)
    {
        fans[fan_index].rpm_rdg = hue2->GetFanRPM(fan_index);
    }
}