/*-----------------------------------------*\
|  RGBController_ThermaltakeRiing.cpp       |
|                                           |
|  Generic RGB Interface for Thermaltake    |
|  Riing controller                         |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/9/2020    |
\*-----------------------------------------*/

#include "RGBController_ThermaltakeRiing.h"


RGBController_ThermaltakeRiing::RGBController_ThermaltakeRiing(ThermaltakeRiingController* riing_ptr)
{
    riing = riing_ptr;

    name = "Thermaltake Riing";

    type = DEVICE_TYPE_COOLER;

    mode Flow;
    Flow.name       = "Flow";
    Flow.value      = THERMALTAKE_MODE_FLOW;
    Flow.flags      = MODE_FLAG_HAS_SPEED;
    Flow.speed_min  = THERMALTAKE_SPEED_SLOW;
    Flow.speed_max  = THERMALTAKE_SPEED_EXTREME;
    Flow.speed      = THERMALTAKE_SPEED_NORMAL;
    Flow.color_mode = MODE_COLORS_NONE;
    modes.push_back(Flow);

    mode Spectrum;
    Spectrum.name       = "Spectrum";
    Spectrum.value      = THERMALTAKE_MODE_SPECTRUM;
    Spectrum.flags      = MODE_FLAG_HAS_SPEED;
    Spectrum.speed_min  = THERMALTAKE_SPEED_SLOW;
    Spectrum.speed_max  = THERMALTAKE_SPEED_EXTREME;
    Spectrum.speed      = THERMALTAKE_SPEED_NORMAL;
    Spectrum.color_mode = MODE_COLORS_NONE;
    modes.push_back(Spectrum);

    mode Ripple;
    Ripple.name       = "Ripple";
    Ripple.value      = THERMALTAKE_MODE_RIPPLE;
    Ripple.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Ripple.speed_min  = THERMALTAKE_SPEED_SLOW;
    Ripple.speed_max  = THERMALTAKE_SPEED_EXTREME;
    Ripple.speed      = THERMALTAKE_SPEED_NORMAL;
    Ripple.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Ripple);

    mode Blink;
    Blink.name       = "Blink";
    Blink.value      = THERMALTAKE_MODE_BLINK;
    Blink.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Blink.speed_min  = THERMALTAKE_SPEED_SLOW;
    Blink.speed_max  = THERMALTAKE_SPEED_EXTREME;
    Blink.speed      = THERMALTAKE_SPEED_NORMAL;
    Blink.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Blink);

    mode Pulse;
    Pulse.name       = "Pulse";
    Pulse.value      = THERMALTAKE_MODE_PULSE;
    Pulse.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Pulse.speed_min  = THERMALTAKE_SPEED_SLOW;
    Pulse.speed_max  = THERMALTAKE_SPEED_EXTREME;
    Pulse.speed      = THERMALTAKE_SPEED_NORMAL;
    Pulse.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Pulse);

    mode Wave;
    Wave.name       = "Wave";
    Wave.value      = THERMALTAKE_MODE_WAVE;
    Wave.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Wave.speed_min  = THERMALTAKE_SPEED_SLOW;
    Wave.speed_max  = THERMALTAKE_SPEED_EXTREME;
    Wave.speed      = THERMALTAKE_SPEED_NORMAL;
    Wave.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Wave);

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = THERMALTAKE_MODE_PER_LED;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.speed_min  = 0;
    Direct.speed_max  = 0;
    Direct.speed      = 0;
    Direct.color_mode = MODE_FLAG_HAS_PER_LED_COLOR;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = THERMALTAKE_MODE_FULL;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.speed_min  = 0;
    Static.speed_max  = 0;
    Static.speed      = 0;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    /*-------------------------------------------------*\
    | Set size of colors array                          |
    \*-------------------------------------------------*/
    unsigned int led_count = 0;
    for (unsigned int channel_idx = 0; channel_idx < 5; channel_idx++)
    {
        led_count += riing->channel_leds[channel_idx];
    }
    colors.resize(led_count);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    unsigned int led_idx = 0;
    for (unsigned int channel_idx = 0; channel_idx < 5; channel_idx++)
    {
        if(riing->channel_leds[channel_idx] > 0)
        {
            zone* new_zone = new zone;

            char ch_idx_string[2];
            sprintf(ch_idx_string, "%d", channel_idx + 1);

            new_zone->name = "Riing Channel ";
            new_zone->name.append(ch_idx_string);
            new_zone->type = ZONE_TYPE_LINEAR;

            std::vector<int> *new_zone_map = new std::vector<int>();

            for (unsigned int led_ch_idx = 0; led_ch_idx < riing->channel_leds[channel_idx]; led_ch_idx++)
            {
                char led_idx_string[3];
                sprintf(led_idx_string, "%d", led_ch_idx + 1);

                led new_led;
                new_led.name = "Riing Channel ";
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

void RGBController_ThermaltakeRiing::UpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        unsigned int channel = zones_channel[zone_idx];

        std::vector<RGBColor> channel_colors;

        for(std::size_t color = 0; color < colors.size(); color++)
        {
            if(leds_channel[color] == channel)
            {
                channel_colors.push_back(colors[color]);
            }
        }

        if(channel_colors.size() > 0)
        {
            riing->SetChannelLEDs(channel, channel_colors);
        }
    }
}

void RGBController_ThermaltakeRiing::UpdateZoneLEDs(int zone)
{
    unsigned int channel = zones_channel[zone];

    std::vector<RGBColor> channel_colors;

    for(std::size_t color = 0; color < colors.size(); color++)
    {
        if(leds_channel[color] == channel)
        {
            channel_colors.push_back(colors[color]);
        }
    }

    if(channel_colors.size() > 0)
    {
        riing->SetChannelLEDs(channel, channel_colors);
    }
}

void RGBController_ThermaltakeRiing::UpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];

    std::vector<RGBColor> channel_colors;

    for(std::size_t color = 0; color < colors.size(); color++)
    {
        if(leds_channel[color] == channel)
        {
            channel_colors.push_back(colors[color]);
        }
    }

    if(channel_colors.size() > 0)
    {
        riing->SetChannelLEDs(channel, channel_colors);
    }
}

void RGBController_ThermaltakeRiing::SetCustomMode()
{
    active_mode = 6;
}

void RGBController_ThermaltakeRiing::UpdateMode()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        unsigned int channel = zones_channel[zone_idx];

        std::vector<RGBColor> channel_colors;

        for(std::size_t color = 0; color < colors.size(); color++)
        {
            if(leds_channel[color] == channel)
            {
                channel_colors.push_back(colors[color]);
            }
        }

        if(channel_colors.size() > 0)
        {
            riing->SetMode(modes[active_mode].value, modes[active_mode].speed);
            riing->SetChannelLEDs(channel, channel_colors);
        }
    }
}
