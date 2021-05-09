/*-----------------------------------------*\
|  FanController_NZXTKraken.cpp             |
|                                           |
|  Generic Fan Interface for NZXT Kraken    |
|                                           |
|  swilde 5/2021                            |
\*-----------------------------------------*/

#include "FanController_NZXTKraken.h"

FanController_NZXTKraken::FanController_NZXTKraken(NZXTKrakenController* kraken_ptr)
{
    kraken      = kraken_ptr;

    name        = "NZXT Kraken X2";
    description = "NZXT Kraken X42/X52/X62/X72";
    version     = kraken->GetFirmwareVersion();
    location    = kraken->GetLocation();
    serial      = kraken->GetSerialString();

    kraken->UpdateStatus();

    fan new_fan;

    /*---------------------------------------------------------*\
    | Add fan                                                   |
    | X52 has a max fan RPM of 200, the X42 and X62 max is 1800 |
    \*---------------------------------------------------------*/
    new_fan.name        = "NZXT Kraken Fan";
    new_fan.speed_min   = 25;
    new_fan.speed_max   = 100;
    new_fan.speed_cmd   = 100 * kraken->fan_speed / 2000;
    new_fan.prev_speed_cmd = 0;
    new_fan.rpm_rdg     = kraken->fan_speed;
    fans.push_back(new_fan);

    /*---------------------------------------------------------*\
    | Add pump                                                  |
    \*---------------------------------------------------------*/
    new_fan.name        = "NZXT Kraken Pump";
    new_fan.speed_min   = 50;
    new_fan.speed_max   = 100;
    new_fan.speed_cmd   = 100 * kraken->pump_speed / 2800;
    new_fan.prev_speed_cmd = 0;
    new_fan.rpm_rdg     = kraken->pump_speed;
    fans.push_back(new_fan);

    UpdateControl();
}

void FanController_NZXTKraken::UpdateControl()
{
    if(fans[0].speed_cmd != fans[0].prev_speed_cmd)
    {
        kraken->SetSpeed(NZXT_KRAKEN_CHANNEL_FAN, fans[0].speed_cmd);
        fans[0].prev_speed_cmd = fans[0].speed_cmd;
    }
    if(fans[1].speed_cmd != fans[1].prev_speed_cmd)
    {
        kraken->SetSpeed(NZXT_KRAKEN_CHANNEL_PUMP, fans[1].speed_cmd);
        fans[1].prev_speed_cmd = fans[1].speed_cmd;
    }
}

void FanController_NZXTKraken::UpdateReading()
{
    kraken->UpdateStatus();

    fans[0].rpm_rdg = kraken->fan_speed;
    fans[1].rpm_rdg = kraken->pump_speed;
}
