/*-----------------------------------------*\
|  RGBController_NZXTHue2.cpp               |
|                                           |
|  Generic RGB Interface for NZXT Hue 2     |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/29/2019  |
\*-----------------------------------------*/

#include "RGBController_NZXTHue2.h"


RGBController_NZXTHue2::RGBController_NZXTHue2(NZXTHue2Controller* hue2_ptr)
{
    hue2 = hue2_ptr;

    name        = "NZXT Hue 2";
    vendor      = "NZXT";
    type        = DEVICE_TYPE_LEDSTRIP;
    description = "NZXT Hue 2 Device";
    version     = hue2->GetFirmwareVersion();
    location    = hue2->GetLocation();
    serial      = hue2->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = HUE_2_MODE_FIXED;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Fading;
    Fading.name       = "Fading";
    Fading.value      = HUE_2_MODE_FADING;
    Fading.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Fading.speed_min  = HUE_2_SPEED_SLOWEST;
    Fading.speed_max  = HUE_2_SPEED_FASTEST;
    Fading.colors_min = 1;
    Fading.colors_max = 8;
    Fading.speed      = HUE_2_SPEED_NORMAL;
    Fading.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Fading.colors.resize(1);
    modes.push_back(Fading);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Spectrum Cycle";
    SpectrumCycle.value      = HUE_2_MODE_SPECTRUM;
    SpectrumCycle.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    SpectrumCycle.speed_min  = HUE_2_SPEED_SLOWEST;
    SpectrumCycle.speed_max  = HUE_2_SPEED_FASTEST;
    SpectrumCycle.speed      = HUE_2_SPEED_NORMAL;
    SpectrumCycle.direction  = MODE_DIRECTION_RIGHT;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Marquee;
    Marquee.name       = "Marquee";
    Marquee.value      = HUE_2_MODE_MARQUEE;
    Marquee.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Marquee.speed_min  = HUE_2_SPEED_SLOWEST;
    Marquee.speed_max  = HUE_2_SPEED_FASTEST;
    Marquee.colors_min = 1;
    Marquee.colors_max = 1;
    Marquee.speed      = HUE_2_SPEED_NORMAL;
    Marquee.direction  = MODE_DIRECTION_RIGHT;
    Marquee.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Marquee.colors.resize(1);
    modes.push_back(Marquee);

    mode CoverMarquee;
    CoverMarquee.name       = "Cover Marquee";
    CoverMarquee.value      = HUE_2_MODE_COVER_MARQUEE;
    CoverMarquee.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    CoverMarquee.speed_min  = HUE_2_SPEED_SLOWEST;
    CoverMarquee.speed_max  = HUE_2_SPEED_FASTEST;
    CoverMarquee.colors_min = 1;
    CoverMarquee.colors_max = 8;
    CoverMarquee.speed      = HUE_2_SPEED_NORMAL;
    CoverMarquee.direction  = MODE_DIRECTION_RIGHT;
    CoverMarquee.color_mode = MODE_COLORS_MODE_SPECIFIC;
    CoverMarquee.colors.resize(1);
    modes.push_back(CoverMarquee);

    mode Alternating;
    Alternating.name       = "Alternating";
    Alternating.value      = HUE_2_MODE_ALTERNATING;
    Alternating.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Alternating.speed_min  = HUE_2_SPEED_SLOWEST;
    Alternating.speed_max  = HUE_2_SPEED_FASTEST;
    Alternating.colors_min = 1;
    Alternating.colors_max = 2;
    Alternating.speed      = HUE_2_SPEED_NORMAL;
    Alternating.direction  = MODE_DIRECTION_RIGHT;
    Alternating.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Alternating.colors.resize(1);
    modes.push_back(Alternating);

    mode Pulsing;
    Pulsing.name       = "Pulsing";
    Pulsing.value      = HUE_2_MODE_PULSING;
    Pulsing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Pulsing.speed_min  = HUE_2_SPEED_SLOWEST;
    Pulsing.speed_max  = HUE_2_SPEED_FASTEST;
    Pulsing.colors_min = 1;
    Pulsing.colors_max = 8;
    Pulsing.speed      = HUE_2_SPEED_NORMAL;
    Pulsing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Pulsing.colors.resize(1);
    modes.push_back(Pulsing);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = HUE_2_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min  = HUE_2_SPEED_SLOWEST;
    Breathing.speed_max  = HUE_2_SPEED_FASTEST;
    Breathing.colors_min = 1;
    Breathing.colors_max = 8;
    Breathing.speed      = HUE_2_SPEED_NORMAL;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_NZXTHue2::~RGBController_NZXTHue2()
{
    delete hue2;
}

void RGBController_NZXTHue2::SetupZones()
{
    /*-------------------------------------------------*\
    | Set up zones                                      |
    \*-------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < hue2->GetNumRGBChannels(); zone_idx++)
    {
        zone* new_zone = new zone;

        new_zone->name          = "Hue 2 Channel ";
        new_zone->name.append(std::to_string(zone_idx + 1));
        new_zone->type          = ZONE_TYPE_LINEAR;
        new_zone->leds_min      = 0;
        new_zone->leds_max      = 40;
        new_zone->leds_count    = hue2->channel_leds[zone_idx];
        new_zone->matrix_map    = NULL;

        zones.push_back(*new_zone);
    }

    /*-------------------------------------------------*\
    | Set up LEDs                                       |
    \*-------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name = "Hue 2 Channel ";
            new_led.name.append(std::to_string(zone_idx + 1));
            new_led.name.append(", LED ");
            new_led.name.append(std::to_string(led_idx + 1));
            new_led.value = zone_idx;

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_NZXTHue2::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_NZXTHue2::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        hue2->SetChannelLEDs(zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}

void RGBController_NZXTHue2::UpdateZoneLEDs(int zone)
{
    hue2->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
}

void RGBController_NZXTHue2::UpdateSingleLED(int led)
{
    unsigned int zone_idx = leds[led].value;

    hue2->SetChannelLEDs(zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
}

void RGBController_NZXTHue2::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_NZXTHue2::DeviceUpdateMode()
{
    if(modes[active_mode].value == 0xFFFF)
    {
        DeviceUpdateLEDs();
    }
    else
    {
        for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
        {
            RGBColor*   colors      = NULL;
            bool        direction   = false;

            if(modes[active_mode].direction == MODE_DIRECTION_LEFT)
            {
                direction = true;
            }

            if(modes[active_mode].colors.size() > 0)
            {
                colors = &modes[active_mode].colors[0];
            }

            hue2->SetChannelEffect
                    (
                    zone_idx,
                    modes[active_mode].value,
                    modes[active_mode].speed,
                    direction,
                    colors,
                    modes[active_mode].colors.size()
                    );
        }
    }
}
