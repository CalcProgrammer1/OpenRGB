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

    name = "Asus Aura Addressable";
    type = DEVICE_TYPE_MOTHERBOARD;

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

    /*-------------------------------------------------*\
    | Set size of colors array                          |
    \*-------------------------------------------------*/
    unsigned int led_count = 0;
    for (unsigned int channel_idx = 0; channel_idx < 1; channel_idx++)
    {
        led_count += aura->channel_leds[channel_idx];
    }
    colors.resize(led_count);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    unsigned int led_idx = 0;
    for (unsigned int channel_idx = 0; channel_idx < 1; channel_idx++)
    {
        if(aura->channel_leds[channel_idx] > 0)
        {
            zone* new_zone = new zone;

            char ch_idx_string[2];
            sprintf(ch_idx_string, "%d", channel_idx + 1);

            new_zone->name = "Channel ";
            new_zone->name.append(ch_idx_string);
            new_zone->type = ZONE_TYPE_LINEAR;

            std::vector<int> *new_zone_map = new std::vector<int>();

            for (unsigned int led_ch_idx = 0; led_ch_idx < aura->channel_leds[channel_idx]; led_ch_idx++)
            {
                char led_idx_string[3];
                sprintf(led_idx_string, "%d", led_ch_idx + 1);

                led new_led;
                new_led.name = "Channel ";
                new_led.name.append(ch_idx_string);
                new_led.name.append(", LED ");
                new_led.name.append(led_idx_string);

                leds.push_back(new_led);
                leds_channel.push_back(channel_idx + 1);

                new_zone_map->push_back(led_idx);
                led_idx++;
            }

            new_zone->map.push_back(*new_zone_map);
            zones.push_back(*new_zone);
            zones_channel.push_back(channel_idx + 1);
        }
    }
}

RGBController_AuraAddressable::~RGBController_AuraAddressable()
{

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