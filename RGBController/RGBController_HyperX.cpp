/*-----------------------------------------*\
|  RGBController_HyperX.cpp                 |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  HyperX Predator RGB interface            |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/29/2019   |
\*-----------------------------------------*/

#include "RGBController_HyperX.h"

int RGBController_HyperX::GetMode()
{
    return(0);
}

void RGBController_HyperX::SetMode(int mode)
{
    hyperx->SetMode(mode);
}

void RGBController_HyperX::SetCustomMode()
{
    hyperx->SetMode(HYPERX_MODE_DIRECT);
}

void RGBController_HyperX::SetAllLEDs(RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    if(hyperx->GetMode() == HYPERX_MODE_DIRECT)
    {
        hyperx->SetAllColors(red, grn, blu);
    }
    else
    {
        hyperx->SetEffectColor(red, grn, blu);
    }
    
}

void RGBController_HyperX::SetAllZoneLEDs(int zone, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    if(hyperx->GetMode() == HYPERX_MODE_DIRECT)
    {
        for (std::size_t x = 0; x < zones[zone].map.size(); x++)
        {
            for (std::size_t y = 0; y < zones[zone].map[x].size(); y++)
            {
                hyperx->SetLEDColor(zones[zone].map[x][y], red, grn, blu);
            }
        }
    }
    else
    {
        hyperx->SetEffectColor(red, grn, blu);
    }
}

void RGBController_HyperX::SetLED(int led, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    if(hyperx->GetMode() == HYPERX_MODE_DIRECT)
    {
        hyperx->SetLEDColor(led, red, grn, blu);
    }
    else
    {
        hyperx->SetEffectColor(red, grn, blu);
    }
}

void RGBController_HyperX::UpdateLEDs()
{
    for (std::size_t zone = 0; zone < zones.size(); zone++ )
    {
        for (std::size_t x = 0; x < zones[zone].map.size(); x++)
        {
            for (std::size_t y = 0; y < zones[zone].map[x].size(); y++)
            {
                unsigned char red = RGBGetRValue(colors[zones[zone].map[x][y]]);
                unsigned char grn = RGBGetGValue(colors[zones[zone].map[x][y]]);
                unsigned char blu = RGBGetBValue(colors[zones[zone].map[x][y]]);
                hyperx->SetLEDColor(zones[zone].map[x][y], red, grn, blu);
            }
        }
    }
}

RGBController_HyperX::RGBController_HyperX(HyperXController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name = hyperx->GetDeviceName();
    location = hyperx->GetDeviceLocation();
    
    type = DEVICE_TYPE_DRAM;

    mode hyperx_modes[HYPERX_NUMBER_MODES];

    hyperx_modes[0].name = "Direct";
    hyperx_modes[1].name = "Static";
    hyperx_modes[2].name = "Rainbow";
    hyperx_modes[3].name = "Comet";
    hyperx_modes[4].name = "Heartbeat";
    hyperx_modes[5].name = "Spectrum Cycle";
    hyperx_modes[6].name = "Breathing";
    hyperx_modes[7].name = "Bounce";
    hyperx_modes[8].name = "Blink";
    
    for (int i = 0; i < HYPERX_NUMBER_MODES; i++)
    {
        modes.push_back(hyperx_modes[i]);
    }

    unsigned int led_idx = 0;

    for(unsigned int slot = 0; slot < hyperx->GetSlotCount(); slot++)
    {
        zone* new_zone = new zone;

        char slot_idx_str[3];
        sprintf(slot_idx_str, "%d", slot);
        new_zone->name = "HyperX Slot ";
        new_zone->name.append(slot_idx_str);

        new_zone->type = ZONE_TYPE_LINEAR;

        std::vector<int> *new_zone_map = new std::vector<int>();

        for(int led_slot_idx = 0; led_slot_idx < 5; led_slot_idx++)
        {
            colors.push_back(0x00000000);

            led* new_led = new led();

            char led_idx_str[3];
            sprintf(led_idx_str, "%d", led_slot_idx);
            new_led->name = "HyperX Slot ";
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
