/*-----------------------------------------*\
|  FanController_CorsairCommander.cpp       |
|                                           |
|  Generic Fan Interface for Corsair        |
|  Commander controller                     |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/7/2020    |
\*-----------------------------------------*/

#include "FanController_CorsairCommander.h"

FanController_CorsairCommander::FanController_CorsairCommander(CorsairLightingNodeController* corsair_ptr)
{
    corsair = corsair_ptr;

    name        = "Corsair Commander";
    description = "Corsair Commander Device";
    version     = corsair->GetFirmwareString();

    for(std::size_t fan_index = 0; fan_index < CORSAIR_LIGHTING_NODE_NUM_FAN_CHANNELS; fan_index++)
    {
        fan new_fan;

        new_fan.name        = "Corsair Commander Fan ";
        new_fan.name.append(std::to_string(fan_index + 1));
        new_fan.speed_min   = 0;
        new_fan.speed_max   = 100;
        new_fan.speed_cmd   = corsair->GetFanPercent(fan_index);
        new_fan.rpm_rdg     = corsair->GetFanRPM(fan_index);

        fans.push_back(new_fan);

        corsair->SetFanConfiguration(fan_index, CORSAIR_LIGHTING_NODE_FAN_CONFIG_4_PIN);
    }

    UpdateControl();
}

void FanController_CorsairCommander::UpdateControl()
{
    for(std::size_t fan_index = 0; fan_index < fans.size(); fan_index++)
    {
        corsair->SetFanPercent(fan_index, fans[fan_index].speed_cmd);
    }
}

void FanController_CorsairCommander::UpdateReading()
{
    for(std::size_t fan_index = 0; fan_index < fans.size(); fan_index++)
    {
        unsigned short rpm;

        rpm = corsair->GetFanRPM(fan_index);

        fans[fan_index].rpm_rdg = rpm;
    }
}