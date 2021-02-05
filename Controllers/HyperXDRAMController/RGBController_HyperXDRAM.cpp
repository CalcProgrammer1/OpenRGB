/*-----------------------------------------*\
|  RGBController_HyperXDRAM.cpp             |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  HyperX Predator and Fury RGB interface   |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/29/2019   |
\*-----------------------------------------*/

#include "RGBController_HyperXDRAM.h"


RGBController_HyperXDRAM::RGBController_HyperXDRAM(HyperXDRAMController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name        = "HyperX DRAM";
    vendor      = "HyperX";
    type        = DEVICE_TYPE_DRAM;
    description = "HyperX DRAM Device";
    location    = hyperx->GetDeviceLocation();

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

    SetupZones();
}

RGBController_HyperXDRAM::~RGBController_HyperXDRAM()
{
    delete hyperx;
}

void RGBController_HyperXDRAM::SetupZones()
{
    for(unsigned int slot = 0; slot < hyperx->GetSlotCount(); slot++)
    {
        zone* new_zone = new zone;

        new_zone->name          = "HyperX Slot ";
        new_zone->name.append(std::to_string(slot + 1));
        new_zone->type          = ZONE_TYPE_LINEAR;
        new_zone->leds_min      = 5;
        new_zone->leds_max      = 5;
        new_zone->leds_count    = 5;
        new_zone->matrix_map    = NULL;

        zones.push_back(*new_zone);
    }

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(std::size_t led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led* new_led = new led();

            new_led->name = "HyperX Slot ";
            new_led->name.append(std::to_string(zone_idx + 1));
            new_led->name.append(", LED ");
            new_led->name.append(std::to_string(led_idx + 1));

            new_led->value = leds.size();

            leds.push_back(*new_led);
        }
    }

    SetupColors();
}

void RGBController_HyperXDRAM::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXDRAM::DeviceUpdateLEDs()
{
    if(hyperx->GetMode() == HYPERX_MODE_DIRECT)
    {
        for (std::size_t led_idx = 0; led_idx < colors.size(); led_idx++ )
        {
            RGBColor      color = colors[led_idx];
            unsigned char red = RGBGetRValue(color);
            unsigned char grn = RGBGetGValue(color);
            unsigned char blu = RGBGetBValue(color);
            hyperx->SetLEDColor(led_idx, red, grn, blu);
        }
        hyperx->SendApply();
    }
    else
    {
        unsigned char red = RGBGetRValue(colors[0]);
        unsigned char grn = RGBGetGValue(colors[0]);
        unsigned char blu = RGBGetBValue(colors[0]);
        hyperx->SetEffectColor(red, grn, blu);
    }
}

void RGBController_HyperXDRAM::UpdateZoneLEDs(int zone)
{
    if(hyperx->GetMode() == HYPERX_MODE_DIRECT)
    {
        for (std::size_t led_idx = 0; led_idx < zones[zone].leds_count; led_idx++ )
        {
            unsigned int  led   = zones[zone].leds[led_idx].value;
            RGBColor      color = colors[led];
            unsigned char red = RGBGetRValue(color);
            unsigned char grn = RGBGetGValue(color);
            unsigned char blu = RGBGetBValue(color);
            hyperx->SetLEDColor(led, red, grn, blu);
        }
        hyperx->SendApply();
    }
    else
    {
        unsigned char red = RGBGetRValue(colors[0]);
        unsigned char grn = RGBGetGValue(colors[0]);
        unsigned char blu = RGBGetBValue(colors[0]);
        hyperx->SetEffectColor(red, grn, blu);
    }
}

void RGBController_HyperXDRAM::UpdateSingleLED(int led)
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
    hyperx->SendApply();
}

void RGBController_HyperXDRAM::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_HyperXDRAM::DeviceUpdateMode()
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

