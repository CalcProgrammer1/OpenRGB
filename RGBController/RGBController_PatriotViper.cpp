/*-----------------------------------------*\
|  RGBController_PatriotViper.cpp           |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Patriot Viper RGB interface              |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/1/2020    |
\*-----------------------------------------*/

#include "RGBController_PatriotViper.h"

int RGBController_PatriotViper::GetMode()
{
    return(0);
}

void RGBController_PatriotViper::SetMode(int mode)
{
    if(mode == 0)
    {
        viper->SetDirect();
    }
    else
    {
        viper->SetMode(mode - 1);
    }
}

void RGBController_PatriotViper::SetCustomMode()
{
    viper->SetDirect();
}

void RGBController_PatriotViper::SetAllLEDs(RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    if(viper->direct == true)
    {
        viper->SetAllColors(red, grn, blu);
    }
    else
    {
        viper->SetEffectColor(red, grn, blu);
    }
    
}

void RGBController_PatriotViper::SetAllZoneLEDs(int zone, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    if(viper->direct == true)
    {
        for (std::size_t x = 0; x < zones[zone].map.size(); x++)
        {
            for (std::size_t y = 0; y < zones[zone].map[x].size(); y++)
            {
                viper->SetLEDColor(zones[zone].map[x][y], red, grn, blu);
            }
        }
    }
    else
    {
        for (std::size_t x = 0; x < zones[zone].map.size(); x++)
        {
            for (std::size_t y = 0; y < zones[zone].map[x].size(); y++)
            {
                viper->SetLEDEffectColor(zones[zone].map[x][y], red, grn, blu);
            }
        }
    }
}

void RGBController_PatriotViper::SetLED(int led, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    if(viper->direct == true)
    {
        viper->SetLEDColor(led, red, grn, blu);
    }
    else
    {
        viper->SetLEDEffectColor(led, red, grn, blu);
    }
}

void RGBController_PatriotViper::UpdateLEDs()
{
    for(int led = 0; led < 5; led++)
    {
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);
        viper->SetLEDColor(led, red, grn, blu);
    }
}

RGBController_PatriotViper::RGBController_PatriotViper(PatriotViperController* viper_ptr)
{
    viper = viper_ptr;

    name = viper->GetDeviceName();
    location = viper->GetDeviceLocation();
    
    type = DEVICE_TYPE_DRAM;

    mode viper_modes[10];

    viper_modes[0].name = "Direct";
    viper_modes[1].name = "Dark";
    viper_modes[2].name = "Breathing";
    viper_modes[3].name = "Viper";
    viper_modes[4].name = "Heartbeat";
    viper_modes[5].name = "Marquee";
    viper_modes[6].name = "Raindrop";
    viper_modes[7].name = "Aurora";
    viper_modes[8].name = "Unknown";
    viper_modes[9].name = "Neon";

    for (int i = 0; i < 9; i++)
    {
        modes.push_back(viper_modes[i]);
    }

    unsigned int led_idx = 0;

    for(unsigned int slot = 0; slot < viper->GetSlotCount(); slot++)
    {
        zone* new_zone = new zone;

        char slot_idx_str[3];
        sprintf(slot_idx_str, "%d", slot);
        new_zone->name = "Patriot Viper RGB Slot ";
        new_zone->name.append(slot_idx_str);

        new_zone->type = ZONE_TYPE_LINEAR;

        std::vector<int> *new_zone_map = new std::vector<int>();

        for(int led_slot_idx = 0; led_slot_idx < 5; led_slot_idx++)
        {
            colors.push_back(0x00000000);

            led* new_led = new led();

            char led_idx_str[3];
            sprintf(led_idx_str, "%d", led_slot_idx);
            new_led->name = "Patriot Viper RGB Slot ";
            new_led->name.append(slot_idx_str);
            new_led->name.append(", LED ");
            new_led->name.append(led_idx_str);

            leds.push_back(*new_led);

            new_zone_map->push_back(led_idx);
            led_idx++;
        }

        new_zone->map.push_back(*new_zone_map);
        zones.push_back(*new_zone);
    }
}
