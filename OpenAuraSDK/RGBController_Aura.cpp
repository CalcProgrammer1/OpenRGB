/*-----------------------------------------*\
|  RGBController_Aura.cpp                   |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Asus Aura driver                         |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/13/2019   |
\*-----------------------------------------*/

#include "RGBController_Aura.h"

int RGBController_Aura::GetMode()
{
    if (aura->AuraRegisterRead(AURA_REG_DIRECT))
    {
        return(0);
    }
    else
    {
        return(aura->AuraRegisterRead(AURA_REG_MODE) + 1);
    }
}

void RGBController_Aura::SetMode(int mode)
{
    if (mode == 0)
    {
        aura->SetDirect(true);
    }
    else
    {
        aura->SetDirect(false);
        aura->SetMode(mode - 1);
    }
}

void RGBController_Aura::SetAllLEDs(unsigned char red, unsigned char green, unsigned char blue)
{
    if (GetMode() == 0)
    {
        aura->SetAllColorsDirect(red, green, blue);
    }
    else
    {
        aura->SetAllColorsEffect(red, green, blue);
    }
}

RGBController_Aura::RGBController_Aura(AuraController * aura_ptr)
{
    aura = aura_ptr;

    name = aura->GetDeviceName();

    mode aura_modes[AURA_NUMBER_MODES + 1];

    aura_modes[0].name = "Direct";
    aura_modes[1].name = "Off";
    aura_modes[2].name = "Static";
    aura_modes[3].name = "Breathing";
    aura_modes[4].name = "Flashing";
    aura_modes[5].name = "Spectrum Cycle";
    aura_modes[6].name = "Rainbow";
    aura_modes[7].name = "Spectrum Cycle Breathing";
    aura_modes[8].name = "Chase Fade";
    aura_modes[9].name = "Spectrum Cycle Chase Fade";
    aura_modes[10].name = "Chase";
    aura_modes[11].name = "Spectrum Cycle Chase";
    aura_modes[12].name = "Spectrum Cycle Wave";
    aura_modes[13].name = "Chase Rainbow Pulse";
    aura_modes[14].name = "Random Flicker";

    for (int i = 0; i < (AURA_NUMBER_MODES + 1); i++)
    {
        modes.push_back(aura_modes[i]);
    }

    for (int i = 0; i < aura->GetLEDCount(); i++)
    {
        led* new_led = new led();

        new_led->name = "Aura LED";

        leds.push_back(*new_led);
    }

    zone new_zone;

    new_zone.name = "Aura Zone";
    new_zone.type = ZONE_TYPE_LINEAR;

    std::vector<int> zone_row;

    for (int i = 0; i < aura->GetLEDCount(); i++)
    {
        zone_row.push_back(i);
    }

    new_zone.map.push_back(zone_row);

    zones.push_back(new_zone);
}