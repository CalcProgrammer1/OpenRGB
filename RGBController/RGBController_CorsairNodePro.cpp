/*-----------------------------------------*\
|  RGBController_CorsairNodePro.cpp         |
|                                           |
|  Generic RGB Interface for Corsair        |
|  Lighting Node Pro                        |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/12/2020   |
\*-----------------------------------------*/

#include "RGBController_CorsairNodePro.h"


RGBController_CorsairNodePro::RGBController_CorsairNodePro(CorsairNodeProController* corsair_ptr)
{
    corsair = corsair_ptr;

    name = "Corsair Lighting Node Pro";

    type = DEVICE_TYPE_LEDSTRIP;
    
    mode led_mode;
    led_mode.name = "Custom";
    modes.push_back(led_mode);

    /*-------------------------------------------------*\
    | Set size of colors array                          |
    \*-------------------------------------------------*/
    unsigned int led_count = 0;
    for (unsigned int channel_idx = 0; channel_idx < CORSAIR_NODE_PRO_NUM_CHANNELS; channel_idx++)
    {
        led_count += corsair->channel_leds[channel_idx];
    }
    colors.resize(led_count);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    unsigned int led_idx = 0;
    for (unsigned int channel_idx = 0; channel_idx < CORSAIR_NODE_PRO_NUM_CHANNELS; channel_idx++)
    {
        if(corsair->channel_leds[channel_idx] > 0)
        {
            zone* new_zone = new zone;

            char ch_idx_string[2];
            sprintf(ch_idx_string, "%d", channel_idx + 1);

            new_zone->name = "Corsair Channel ";
            new_zone->name.append(ch_idx_string);
            new_zone->type = ZONE_TYPE_LINEAR;

            std::vector<int> *new_zone_map = new std::vector<int>();

            for (unsigned int led_ch_idx = 0; led_ch_idx < corsair->channel_leds[channel_idx]; led_ch_idx++)
            {
                char led_idx_string[3];
                sprintf(led_idx_string, "%d", led_ch_idx + 1);

                led new_led;
                new_led.name = "Corsair Channel ";
                new_led.name.append(ch_idx_string);
                new_led.name.append(", LED ");
                new_led.name.append(led_idx_string);

                leds.push_back(new_led);
                leds_channel.push_back(channel_idx);

                new_zone_map->push_back(led_idx);
                led_idx++;
            }

            new_zone->map.push_back(*new_zone_map);
            zones.push_back(*new_zone);
            zones_channel.push_back(channel_idx);
        }
    }
}

int RGBController_CorsairNodePro::GetMode()
{
    return 0;
}

void RGBController_CorsairNodePro::SetMode(int /*mode*/)
{

}

void RGBController_CorsairNodePro::SetCustomMode()
{

}

void RGBController_CorsairNodePro::UpdateLEDs()
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
            corsair->SetChannelLEDs(channel, channel_colors);
        }
    }
}

void RGBController_CorsairNodePro::UpdateZoneLEDs(int zone)
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
        corsair->SetChannelLEDs(channel, channel_colors);
    }
}

void RGBController_CorsairNodePro::UpdateSingleLED(int led)
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
        corsair->SetChannelLEDs(channel, channel_colors);
    }
}
