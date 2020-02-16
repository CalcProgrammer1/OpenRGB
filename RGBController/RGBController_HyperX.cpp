/*-----------------------------------------*\
|  RGBController_HyperX.cpp                 |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  HyperX Predator RGB interface            |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/29/2019   |
\*-----------------------------------------*/

#include "RGBController_HyperX.h"


void RGBController_HyperX::UpdateLEDs()
{
    if(hyperx->GetMode() == HYPERX_MODE_DIRECT)
    {
        for (std::size_t zone = 0; zone < zones.size(); zone++ )
        {
            for (std::size_t x = 0; x < zones[zone].map.size(); x++)
            {
                for (std::size_t y = 0; y < zones[zone].map[x].size(); y++)
                {
                    int           led   = zones[zone].map[x][y];
                    RGBColor      color = colors[led];
                    unsigned char red = RGBGetRValue(color);
                    unsigned char grn = RGBGetGValue(color);
                    unsigned char blu = RGBGetBValue(color);
                    hyperx->SetLEDColor(led, red, grn, blu);
                }
            }
        }
    }
    else
    {
        unsigned char red = RGBGetRValue(colors[0]);
        unsigned char grn = RGBGetGValue(colors[0]);
        unsigned char blu = RGBGetBValue(colors[0]);
        hyperx->SetEffectColor(red, grn, blu);
    }
}

void RGBController_HyperX::UpdateZoneLEDs(int zone)
{
    if(hyperx->GetMode() == HYPERX_MODE_DIRECT)
    {
        for (std::size_t x = 0; x < zones[zone].map.size(); x++)
        {
            for (std::size_t y = 0; y < zones[zone].map[x].size(); y++)
            {
                int           led   = zones[zone].map[x][y];
                RGBColor      color = colors[led];
                unsigned char red = RGBGetRValue(color);
                unsigned char grn = RGBGetGValue(color);
                unsigned char blu = RGBGetBValue(color);
                hyperx->SetLEDColor(led, red, grn, blu);
            }
        }
    }
    else
    {
        unsigned char red = RGBGetRValue(colors[0]);
        unsigned char grn = RGBGetGValue(colors[0]);
        unsigned char blu = RGBGetBValue(colors[0]);
        hyperx->SetEffectColor(red, grn, blu);
    }
}

void RGBController_HyperX::UpdateSingleLED(int led)
{
    RGBColor      color = colors[led];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    if(hyperx->GetMode() == HYPERX_MODE_DIRECT)
    {
        hyperx->SetLEDColor(led, red, grn, blu);
    }
    else
    {
        hyperx->SetEffectColor(red, grn, blu);
    }
}

RGBController_HyperX::RGBController_HyperX(HyperXController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name = hyperx->GetDeviceName();
    location = hyperx->GetDeviceLocation();
    
    type = DEVICE_TYPE_DRAM;

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = HYPERX_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = HYPERX_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Rainbow;
    Rainbow.name       = "Rainbow";
    Rainbow.value      = HYPERX_MODE_RAINBOW;
    Rainbow.flags      = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min  = HYPERX_SPEED_RAINBOW_SLOW;
    Rainbow.speed_max  = HYPERX_SPEED_RAINBOW_FAST;
    Rainbow.speed      = HYPERX_SPEED_RAINBOW_NORMAL;
    Rainbow.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Comet;
    Comet.name       = "Comet";
    Comet.value      = HYPERX_MODE_COMET;
    Comet.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Comet.speed_min  = HYPERX_SPEED_COMET_SLOW;
    Comet.speed_max  = HYPERX_SPEED_COMET_FAST;
    Comet.colors_min = 1;
    Comet.colors_max = 1;
    Comet.speed      = HYPERX_SPEED_COMET_NORMAL;
    Comet.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Comet.colors.resize(1);
    modes.push_back(Comet);

    mode Heartbeat;
    Heartbeat.name       = "Heartbeat";
    Heartbeat.value      = HYPERX_MODE_HEARTBEAT;
    Heartbeat.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Heartbeat.speed_min  = HYPERX_SPEED_COMET_SLOW;
    Heartbeat.speed_max  = HYPERX_SPEED_COMET_FAST;
    Heartbeat.colors_min = 1;
    Heartbeat.colors_max = 1;
    Heartbeat.speed      = HYPERX_SPEED_COMET_NORMAL;
    Heartbeat.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Heartbeat.colors.resize(1);
    modes.push_back(Heartbeat);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Spectrum Cycle";
    SpectrumCycle.value      = HYPERX_MODE_CYCLE;
    SpectrumCycle.flags      = MODE_FLAG_HAS_SPEED;
    SpectrumCycle.speed_min  = HYPERX_SPEED_CYCLE_SLOW;
    SpectrumCycle.speed_max  = HYPERX_SPEED_CYCLE_FAST;
    SpectrumCycle.speed      = HYPERX_SPEED_CYCLE_NORMAL;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);
    
    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = HYPERX_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Breathing.speed_min  = HYPERX_SPEED_BREATHING_SLOW;
    Breathing.speed_max  = HYPERX_SPEED_BREATHING_FAST;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.speed      = HYPERX_SPEED_BREATHING_NORMAL;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Bounce;
    Bounce.name       = "Bounce";
    Bounce.value      = HYPERX_MODE_BOUNCE;
    Bounce.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Bounce.speed_min  = HYPERX_SPEED_BOUNCE_SLOW;
    Bounce.speed_max  = HYPERX_SPEED_BOUNCE_FAST;
    Bounce.colors_min = 1;
    Bounce.colors_max = 1;
    Bounce.speed      = HYPERX_SPEED_BOUNCE_NORMAL;
    Bounce.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Bounce.colors.resize(1);
    modes.push_back(Bounce);

    mode Blink;
    Blink.name       = "Blink";
    Blink.value      = HYPERX_MODE_BLINK;
    Blink.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Blink.speed_min  = HYPERX_SPEED_BLINK_SLOW;
    Blink.speed_max  = HYPERX_SPEED_BLINK_FAST;
    Blink.colors_min = 1;
    Blink.colors_max = 1;
    Blink.speed      = HYPERX_SPEED_BLINK_NORMAL;
    Blink.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Blink.colors.resize(1);
    modes.push_back(Blink);

    colors.resize(hyperx->GetLEDCount());

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

void RGBController_HyperX::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_HyperX::UpdateMode()
{
    bool random = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);

    hyperx->SetMode(modes[active_mode].value, random, modes[active_mode].speed);

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        unsigned char red = RGBGetRValue(modes[active_mode].colors[0]);
        unsigned char grn = RGBGetGValue(modes[active_mode].colors[0]);
        unsigned char blu = RGBGetBValue(modes[active_mode].colors[0]);

        hyperx->SetEffectColor(red, grn, blu);
    }
}

