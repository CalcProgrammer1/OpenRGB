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

    location = hueplus->GetLEDString();
    
    mode led_mode;
    led_mode.name = "Custom";
    modes.push_back(led_mode);

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
                colors.push_back(0x00000000);

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

void RGBController_HuePlus::SetMode(int /*mode*/)
{

}

void RGBController_HuePlus::SetCustomMode()
{

}

void RGBController_HuePlus::UpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx <= zones.size(); zone_idx++)
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

    hueplus->SetChannelLEDs(channel, channel_colors);
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
    hueplus->SetChannelLEDs(channel, channel_colors);
}
