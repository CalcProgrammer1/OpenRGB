/*-----------------------------------------*\
|  RGBController_AuraAddressable.cpp        |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  addressable controller driver            |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/18/2020   |
\*-----------------------------------------*/

#include "RGBController_AuraAddressable.h"

RGBController_AuraAddressable::RGBController_AuraAddressable(AuraAddressableController* aura_ptr)
{
    aura = aura_ptr;

    name        = "ASUS Aura Addressable";
    type        = DEVICE_TYPE_MOTHERBOARD;
    description = "ASUS Aura Addressable Device";

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name       = "Off";
    Off.value      = AURA_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name       = "Static";
    Static.value      = AURA_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = AURA_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name       = "Flashing";
    Flashing.value      = AURA_MODE_FLASHING;
    Flashing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Flashing.colors_min = 1;
    Flashing.colors_max = 1;
    Flashing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Flashing.colors.resize(1);
    modes.push_back(Flashing);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Spectrum Cycle";
    SpectrumCycle.value      = AURA_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags      = 0;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Rainbow;
    Rainbow.name       = "Rainbow";
    Rainbow.value      = AURA_MODE_RAINBOW;
    Rainbow.flags      = 0;
    Rainbow.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode ChaseFade;
    ChaseFade.name       = "Chase Fade";
    ChaseFade.value      = AURA_MODE_CHASE_FADE;
    ChaseFade.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    ChaseFade.colors_min = 1;
    ChaseFade.colors_max = 1;
    ChaseFade.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ChaseFade.colors.resize(1);
    modes.push_back(ChaseFade);

    mode Chase;
    Chase.name       = "Chase";
    Chase.value      = AURA_MODE_CHASE;
    Chase.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Chase.colors_min = 1;
    Chase.colors_max = 1;
    Chase.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Chase.colors.resize(1);
    modes.push_back(Chase);

    SetupZones();
}

RGBController_AuraAddressable::~RGBController_AuraAddressable()
{

}

void RGBController_AuraAddressable::SetupZones()
{
    /*-------------------------------------------------*\
    | Only set LED count on the first run               |
    \*-------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(AURA_ADDRESSABLE_NUM_CHANNELS);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    for (unsigned int channel_idx = 0; channel_idx < AURA_ADDRESSABLE_NUM_CHANNELS; channel_idx++)
    {
        char ch_idx_string[2];
        sprintf(ch_idx_string, "%d", channel_idx + 1);

        zones[channel_idx].name     = "Aura Channel ";
        zones[channel_idx].name.append(ch_idx_string);
        zones[channel_idx].type     = ZONE_TYPE_LINEAR;

        zones[channel_idx].leds_min   = 0;
        zones[channel_idx].leds_max   = AURA_ADDRESSABLE_MAX_LEDS;

        if(first_run)
        {
            zones[channel_idx].leds_count = 0;
        }

        for (unsigned int led_ch_idx = 0; led_ch_idx < zones[channel_idx].leds_count; led_ch_idx++)
        {
            char led_idx_string[4];
            sprintf(led_idx_string, "%d", led_ch_idx + 1);

            led new_led;
            new_led.name = "Aura Channel ";
            new_led.name.append(ch_idx_string);
            new_led.name.append(", LED ");
            new_led.name.append(led_idx_string);

            leds.push_back(new_led);
            leds_channel.push_back(channel_idx);
        }
    }

    SetupColors();
}

void RGBController_AuraAddressable::ResizeZone(int zone, int new_size)
{
    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

        SetupZones();
    }
}

void RGBController_AuraAddressable::UpdateLEDs()
{
    if(modes[active_mode].value ==  0xFFFF)
    {
        aura->SetLEDsDirect(colors);
    }
}

void RGBController_AuraAddressable::UpdateZoneLEDs(int zone)
{
    UpdateLEDs();
}

void RGBController_AuraAddressable::UpdateSingleLED(int led)
{
    UpdateLEDs();
}

void RGBController_AuraAddressable::SetCustomMode()
{

}

void RGBController_AuraAddressable::UpdateMode()
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    if(modes[active_mode].value != 0xFFFF)
    {
        aura->SetMode(modes[active_mode].value, red, grn, blu);
    }
}