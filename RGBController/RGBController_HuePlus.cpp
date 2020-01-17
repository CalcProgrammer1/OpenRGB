/*-----------------------------------------*\
|  RGBController_HuePlus.cpp                |
|                                           |
|  Generic RGB Interface for NZXT Hue+      |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/20/2019   |
\*-----------------------------------------*/

#include "RGBController_HuePlus.h"


RGBController_HuePlus::RGBController_HuePlus(HuePlusController* hueplus_ptr)
{
    hueplus = hueplus_ptr;

    name = "NZXT Hue+";

    type = DEVICE_TYPE_LEDSTRIP;

    location = hueplus->GetLocation();
    
    mode Direct;
    Direct.name  = "Direct";
    Direct.value = HUE_PLUS_MODE_FIXED;
    Direct.flags = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    modes.push_back(Direct);

// Fading mode is currently disabled because it doesn't appear to work
//    mode Fading;
//    Fading.name      = "Fading";
//    Fading.value     = HUE_PLUS_MODE_FADING;
//    Fading.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
//    Fading.speed_min = HUE_PLUS_SPEED_SLOWEST;
//    Fading.speed_max = HUE_PLUS_SPEED_FASTEST;
//    Fading.speed     = HUE_PLUS_SPEED_NORMAL;
//    modes.push_back(Fading);

    mode SpectrumCycle;
    SpectrumCycle.name      = "Spectrum Cycle";
    SpectrumCycle.value     = HUE_PLUS_MODE_SPECTRUM;
    SpectrumCycle.flags     = MODE_FLAG_HAS_SPEED;
    SpectrumCycle.speed_min = HUE_PLUS_SPEED_SLOWEST;
    SpectrumCycle.speed_max = HUE_PLUS_SPEED_FASTEST;
    SpectrumCycle.speed     = HUE_PLUS_SPEED_NORMAL;
    modes.push_back(SpectrumCycle);

    mode Marquee;
    Marquee.name      = "Marquee";
    Marquee.value     = HUE_PLUS_MODE_MARQUEE;
    Marquee.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    Marquee.speed_min = HUE_PLUS_SPEED_SLOWEST;
    Marquee.speed_max = HUE_PLUS_SPEED_FASTEST;
    Marquee.speed     = HUE_PLUS_SPEED_NORMAL;
    modes.push_back(Marquee);

    mode CoverMarquee;
    CoverMarquee.name      = "Cover Marquee";
    CoverMarquee.value     = HUE_PLUS_MODE_COVER_MARQUEE;
    CoverMarquee.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    CoverMarquee.speed_min = HUE_PLUS_SPEED_SLOWEST;
    CoverMarquee.speed_max = HUE_PLUS_SPEED_FASTEST;
    CoverMarquee.speed     = HUE_PLUS_SPEED_NORMAL;
    modes.push_back(CoverMarquee);

    mode Alternating;
    Alternating.name      = "Alternating";
    Alternating.value     = HUE_PLUS_MODE_ALTERNATING;
    Alternating.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    Alternating.speed_min = HUE_PLUS_SPEED_SLOWEST;
    Alternating.speed_max = HUE_PLUS_SPEED_FASTEST;
    Alternating.speed     = HUE_PLUS_SPEED_NORMAL;
    modes.push_back(Alternating);

    mode Pulsing;
    Pulsing.name      = "Pulsing";
    Pulsing.value     = HUE_PLUS_MODE_PULSING;
    Pulsing.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    Pulsing.speed_min = HUE_PLUS_SPEED_SLOWEST;
    Pulsing.speed_max = HUE_PLUS_SPEED_FASTEST;
    Pulsing.speed     = HUE_PLUS_SPEED_NORMAL;
    modes.push_back(Pulsing);

    mode Breathing;
    Breathing.name      = "Breathing";
    Breathing.value     = HUE_PLUS_MODE_BREATHING;
    Breathing.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    Breathing.speed_min = HUE_PLUS_SPEED_SLOWEST;
    Breathing.speed_max = HUE_PLUS_SPEED_FASTEST;
    Breathing.speed     = HUE_PLUS_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode Alert;
    Alert.name  = "Alert";
    Alert.value = HUE_PLUS_MODE_ALERT;
    Alert.flags = 0;
    modes.push_back(Alert);

    mode Candlelight;
    Candlelight.name  = "Candlelight";
    Candlelight.value = HUE_PLUS_MODE_CANDLELIGHT;
    Candlelight.flags = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    modes.push_back(Candlelight);

    mode Wings;
    Wings.name      = "Wings";
    Wings.value     = HUE_PLUS_MODE_WINGS;
    Wings.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    Wings.speed_min = HUE_PLUS_SPEED_SLOWEST;
    Wings.speed_max = HUE_PLUS_SPEED_FASTEST;
    Wings.speed     = HUE_PLUS_SPEED_NORMAL;
    modes.push_back(Wings);

// Wave mode is currently disabled because it doesn't appear to work
//    mode Wave;
//    Wave.name      = "Wave";
//    Wave.value     = HUE_PLUS_MODE_WAVE;
//    Wave.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
//    Wave.speed_min = HUE_PLUS_SPEED_SLOWEST;
//    Wave.speed_max = HUE_PLUS_SPEED_FASTEST;
//    Wave.speed     = HUE_PLUS_SPEED_NORMAL;
//    modes.push_back(Wave);

    /*-------------------------------------------------*\
    | Set size of colors array                          |
    \*-------------------------------------------------*/
    unsigned int led_count = 0;
    for (unsigned int channel_idx = 0; channel_idx < HUE_PLUS_NUM_CHANNELS; channel_idx++)
    {
        led_count += hueplus->channel_leds[channel_idx];
    }
    colors.resize(led_count);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    unsigned int led_idx = 0;
    for (int channel_idx = 0; channel_idx < HUE_PLUS_NUM_CHANNELS; channel_idx++)
    {
        if(hueplus->channel_leds[channel_idx] > 0)
        {
            zone* new_zone = new zone;

            char ch_idx_string[2];
            sprintf(ch_idx_string, "%d", channel_idx + 1);

            new_zone->name = "Hue+ Channel ";
            new_zone->name.append(ch_idx_string);
            new_zone->type = ZONE_TYPE_LINEAR;

            std::vector<int> *new_zone_map = new std::vector<int>();

            for (unsigned led_ch_idx = 0; led_ch_idx < hueplus->channel_leds[channel_idx]; led_ch_idx++)
            {
                char led_idx_string[3];
                sprintf(led_idx_string, "%d", led_ch_idx + 1);

                led new_led;
                new_led.name = "Hue+ Channel ";
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

int RGBController_HuePlus::GetMode()
{
    return 0;
}

void RGBController_HuePlus::SetMode(int mode)
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
            hueplus->SetMode(modes[mode].value, modes[mode].speed);
            hueplus->SetChannelLEDs(channel, channel_colors);
        }
    }
}

void RGBController_HuePlus::SetCustomMode()
{

}

void RGBController_HuePlus::UpdateLEDs()
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
            hueplus->SetChannelLEDs(channel, channel_colors);
        }
    }
}

void RGBController_HuePlus::UpdateZoneLEDs(int zone)
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
        hueplus->SetChannelLEDs(channel, channel_colors);
    }
}

void RGBController_HuePlus::UpdateSingleLED(int led)
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
        hueplus->SetChannelLEDs(channel, channel_colors);
    }
}
