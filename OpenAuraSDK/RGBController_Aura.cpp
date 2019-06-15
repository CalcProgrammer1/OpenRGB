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

void RGBController_Aura::SetAllLEDs(RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    if (GetMode() == 0)
    {
        aura->SetAllColorsDirect(red, grn, blu);
    }
    else
    {
        aura->SetAllColorsEffect(red, grn, blu);
    }
}

void RGBController_Aura::SetAllZoneLEDs(int zone, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    for (int x = 0; x < zones[zone].map.size(); x++)
    {
        for (int y = 0; y < zones[zone].map[x].size(); y++)
        {
            if (GetMode() == 0)
            {
                aura->SetLEDColorDirect(zones[zone].map[x][y], red, grn, blu);
            }
            else
            {
                aura->SetLEDColorEffect(zones[zone].map[x][y], red, grn, blu);
            }
        }
    }
}

void RGBController_Aura::SetLED(int led, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    if (GetMode() == 0)
    {
        aura->SetLEDColorDirect(led, red, grn, blu);
    }
    else
    {
        aura->SetLEDColorEffect(led, red, grn, blu);
    }
}

RGBController_Aura::RGBController_Aura(AuraController * aura_ptr)
{
    std::vector<unsigned char> aura_channels;

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
        aura_channels.push_back(aura->GetChannel(i));

        led* new_led = new led();

        new_led->name = aura->GetChannelName(i);

        leds.push_back(*new_led);
    }

    std::vector<unsigned char> aura_zones;

    // Search through all LEDs and create zones for each channel type
    for (int i = 0; i < aura_channels.size(); i++)
    {
        bool matched = false;

        // Search through existing zones to make sure we don't create a duplicate zone
        for (int j = 0; j < aura_zones.size(); j++)
        {
            if (aura_channels[i] == aura_zones[j])
            {
                matched = true;
            }
        }

        // If zone does not already exist, create it
        if (matched == false)
        {
            zone* new_zone = new zone();
            std::vector<int>* zone_row = new std::vector<int>();

            // Set zone name to channel name
            new_zone->name = aura->GetChannelName(i);

            // Find all LEDs with this channel type and add them to zone
            for (int j = 0; j < aura->GetLEDCount(); j++)
            {
                if (aura->GetChannel(j) == aura_channels[i])
                {
                    zone_row->push_back(j);
                }
            }

            // Aura devices can be either single or linear, never matrix
            // That means only one row is needed
            new_zone->map.push_back(*zone_row);

            // Save channel to aura_zones so we know not to create another zone with this channel
            aura_zones.push_back(aura_channels[i]);

            // Push new zone to zones vector
            zones.push_back(*new_zone);
        }
    }
}