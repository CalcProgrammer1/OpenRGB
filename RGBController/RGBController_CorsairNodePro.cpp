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

    mode Direct;
    Direct.name  = "Direct";
    Direct.value = 0xFFFF;
    Direct.flags = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    modes.push_back(Direct);

    mode RainbowWave;
    RainbowWave.name      = "Rainbow Wave";
    RainbowWave.value     = CORSAIR_CMDR_PRO_MODE_RAINBOW_WAVE;
    RainbowWave.flags     = MODE_FLAG_HAS_SPEED;
    RainbowWave.speed_min = CORSAIR_CMDR_PRO_SPEED_SLOW;
    RainbowWave.speed_max = CORSAIR_CMDR_PRO_SPEED_FAST;
    RainbowWave.speed     = CORSAIR_CMDR_PRO_SPEED_MEDIUM;
    modes.push_back(RainbowWave);

    mode ColorShift;
    ColorShift.name       = "Color Shift";
    ColorShift.value      = CORSAIR_CMDR_PRO_MODE_COLOR_SHIFT;
    ColorShift.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    ColorShift.speed_min  = CORSAIR_CMDR_PRO_SPEED_SLOW;
    ColorShift.speed_max  = CORSAIR_CMDR_PRO_SPEED_FAST;
    ColorShift.speed      = CORSAIR_CMDR_PRO_SPEED_MEDIUM;
    modes.push_back(ColorShift);

    mode ColorPulse;
    ColorPulse.name      = "Color Pulse";
    ColorPulse.value     = CORSAIR_CMDR_PRO_MODE_COLOR_PULSE;
    ColorPulse.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    ColorPulse.speed_min = CORSAIR_CMDR_PRO_SPEED_SLOW;
    ColorPulse.speed_max = CORSAIR_CMDR_PRO_SPEED_FAST;
    ColorPulse.speed     = CORSAIR_CMDR_PRO_SPEED_MEDIUM;
    modes.push_back(ColorPulse);

    mode ColorWave;
    ColorWave.name      = "Color Wave";
    ColorWave.value     = CORSAIR_CMDR_PRO_MODE_COLOR_WAVE;
    ColorWave.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    ColorWave.speed_min = CORSAIR_CMDR_PRO_SPEED_SLOW;
    ColorWave.speed_max = CORSAIR_CMDR_PRO_SPEED_FAST;
    ColorWave.speed     = CORSAIR_CMDR_PRO_SPEED_MEDIUM;
    modes.push_back(ColorWave);

    mode Static;
    Static.name  = "Static";
    Static.value = CORSAIR_CMDR_PRO_MODE_STATIC;
    Static.flags = MODE_FLAG_HAS_COLOR;
    modes.push_back(Static);

    mode Temperature;
    Temperature.name  = "Temperature";
    Temperature.value = CORSAIR_CMDR_PRO_MODE_TEMPERATURE;
    Temperature.flags = MODE_FLAG_HAS_COLOR;
    modes.push_back(Temperature);

    mode Visor;
    Visor.name      = "Visor";
    Visor.value     = CORSAIR_CMDR_PRO_MODE_VISOR;
    Visor.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    Visor.speed_min = CORSAIR_CMDR_PRO_SPEED_SLOW;
    Visor.speed_max = CORSAIR_CMDR_PRO_SPEED_FAST;
    Visor.speed     = CORSAIR_CMDR_PRO_SPEED_MEDIUM;
    modes.push_back(Visor);

    mode Marquee;
    Marquee.name      = "Marquee";
    Marquee.value     = CORSAIR_CMDR_PRO_MODE_MARQUEE;
    Marquee.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    Marquee.speed_min = CORSAIR_CMDR_PRO_SPEED_SLOW;
    Marquee.speed_max = CORSAIR_CMDR_PRO_SPEED_FAST;
    Marquee.speed     = CORSAIR_CMDR_PRO_SPEED_MEDIUM;
    modes.push_back(Marquee);

    mode Blink;
    Blink.name      = "Blink";
    Blink.value     = CORSAIR_CMDR_PRO_MODE_BLINK;
    Blink.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    Blink.speed_min = CORSAIR_CMDR_PRO_SPEED_SLOW;
    Blink.speed_max = CORSAIR_CMDR_PRO_SPEED_FAST;
    Blink.speed     = CORSAIR_CMDR_PRO_SPEED_MEDIUM;
    modes.push_back(Blink);

    mode Sequential;
    Sequential.name      = "Sequential";
    Sequential.value     = CORSAIR_CMDR_PRO_MODE_SEQUENTIAL;
    Sequential.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    Sequential.speed_min = CORSAIR_CMDR_PRO_SPEED_SLOW;
    Sequential.speed_max = CORSAIR_CMDR_PRO_SPEED_FAST;
    Sequential.speed     = CORSAIR_CMDR_PRO_SPEED_MEDIUM;
    modes.push_back(Sequential);

    mode Rainbow;
    Rainbow.name      = "Rainbow";
    Rainbow.value     = CORSAIR_CMDR_PRO_MODE_RAINBOW;
    Rainbow.flags     = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min = CORSAIR_CMDR_PRO_SPEED_SLOW;
    Rainbow.speed_max = CORSAIR_CMDR_PRO_SPEED_FAST;
    Rainbow.speed     = CORSAIR_CMDR_PRO_SPEED_MEDIUM;
    modes.push_back(Rainbow);
    
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

void RGBController_CorsairNodePro::SetMode(int mode)
{
    active_mode = mode;

    if(modes[active_mode].value == 0xFFFF)
    {
        UpdateLEDs();
    }
    else
    {
        for(int channel = 0; channel < CORSAIR_CMDR_PRO_NUM_CHANNELS; channel++)
        {
            corsair->SetChannelEffect(channel,
                                      modes[active_mode].value,
                                      modes[active_mode].speed,
                                      0,
                                      modes[active_mode].random,
                                      RGBGetRValue(colors[0]),
                                      RGBGetGValue(colors[0]),
                                      RGBGetBValue(colors[0]),
                                      RGBGetRValue(colors[1]),
                                      RGBGetGValue(colors[1]),
                                      RGBGetBValue(colors[1]));
        }
    }
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
