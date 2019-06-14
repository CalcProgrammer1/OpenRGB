/*-----------------------------------------*\
|  RGBController_Corsair.cpp                |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Corsair Vengeance RGB driver             |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/13/2019   |
\*-----------------------------------------*/

#include "RGBController_Corsair.h"

int RGBController_Corsair::GetMode()
{
    return(CORSAIR_VENGEANCE_RGB_MODE_SINGLE);
}

void RGBController_Corsair::SetMode(int mode)
{
    corsair->SetMode(mode);
}

void RGBController_Corsair::SetAllLEDs(unsigned char red, unsigned char green, unsigned char blue)
{
    corsair->SetAllColorsDirect(red, green, blue);
}

RGBController_Corsair::RGBController_Corsair(CorsairController* corsair_ptr)
{
    corsair = corsair_ptr;

    name = corsair->GetDeviceName();

    mode corsair_modes[CORSAIR_NUMBER_MODES];

    corsair_modes[0].name = "Static";
    corsair_modes[1].name = "Fade";
    corsair_modes[2].name = "Pulse";

    for (int i = 0; i < CORSAIR_NUMBER_MODES; i++)
    {
        modes.push_back(corsair_modes[i]);
    }

    for (int i = 0; i < corsair->GetLEDCount(); i++)
    {
        led* new_led = new led();

        new_led->name = "Corsair LED";

        leds.push_back(*new_led);
    }

    zone new_zone;

    new_zone.name = "Corsair Zone";
    new_zone.type = ZONE_TYPE_SINGLE;

    std::vector<int> zone_row;

    for (int i = 0; i < corsair->GetLEDCount(); i++)
    {
        zone_row.push_back(i);
    }

    new_zone.map.push_back(zone_row);

    zones.push_back(new_zone);
}