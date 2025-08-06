/*---------------------------------------------------------*\
| RGBController_CorsairLightingNode.cpp                     |
|                                                           |
|   RGBController for Corsair Lighting Node devices         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_CorsairLightingNode.h"

/**------------------------------------------------------------------*\
    @name Corsair Lighting Node
    @category LEDStrip
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCorsairLightingNodeControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_CorsairLightingNode::RGBController_CorsairLightingNode(CorsairLightingNodeController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetNameString();
    vendor      = "Corsair";
    description = "Corsair Lighting Node Device";
    type        = DEVICE_TYPE_LEDSTRIP;
    version     = controller->GetFirmwareString();
    location    = controller->GetLocationString();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name           = "Direct";
    Direct.value          = 0xFFFF;
    Direct.flags          = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode RainbowWave;
    RainbowWave.name           = "Rainbow Wave";
    RainbowWave.value          = CORSAIR_LIGHTING_NODE_MODE_RAINBOW_WAVE;
    RainbowWave.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    RainbowWave.speed_min      = CORSAIR_LIGHTING_NODE_SPEED_SLOW;
    RainbowWave.speed_max      = CORSAIR_LIGHTING_NODE_SPEED_FAST;
    RainbowWave.speed          = CORSAIR_LIGHTING_NODE_SPEED_MEDIUM;
    RainbowWave.direction      = MODE_DIRECTION_RIGHT;
    RainbowWave.color_mode     = MODE_COLORS_NONE;
    RainbowWave.brightness_min = 0;
    RainbowWave.brightness_max = 100;
    RainbowWave.brightness     = 100;
    modes.push_back(RainbowWave);

    mode ColorShift;
    ColorShift.name           = "Color Shift";
    ColorShift.value          = CORSAIR_LIGHTING_NODE_MODE_COLOR_SHIFT;
    ColorShift.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    ColorShift.colors_min     = 2;
    ColorShift.colors_max     = 2;
    ColorShift.speed_min      = CORSAIR_LIGHTING_NODE_SPEED_SLOW;
    ColorShift.speed_max      = CORSAIR_LIGHTING_NODE_SPEED_FAST;
    ColorShift.speed          = CORSAIR_LIGHTING_NODE_SPEED_MEDIUM;
    ColorShift.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    ColorShift.colors.resize(2);
    ColorShift.brightness_min = 0;
    ColorShift.brightness_max = 100;
    ColorShift.brightness     = 100;
    modes.push_back(ColorShift);

    mode ColorPulse;
    ColorPulse.name           = "Color Pulse";
    ColorPulse.value          = CORSAIR_LIGHTING_NODE_MODE_COLOR_PULSE;
    ColorPulse.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    ColorPulse.colors_min     = 2;
    ColorPulse.colors_max     = 2;
    ColorPulse.speed_min      = CORSAIR_LIGHTING_NODE_SPEED_SLOW;
    ColorPulse.speed_max      = CORSAIR_LIGHTING_NODE_SPEED_FAST;
    ColorPulse.speed          = CORSAIR_LIGHTING_NODE_SPEED_MEDIUM;
    ColorPulse.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    ColorPulse.colors.resize(2);
    ColorPulse.brightness_min = 0;
    ColorPulse.brightness_max = 100;
    ColorPulse.brightness     = 100;
    modes.push_back(ColorPulse);

    mode ColorWave;
    ColorWave.name           = "Color Wave";
    ColorWave.value          = CORSAIR_LIGHTING_NODE_MODE_COLOR_WAVE;
    ColorWave.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    ColorWave.colors_min     = 2;
    ColorWave.colors_max     = 2;
    ColorWave.speed_min      = CORSAIR_LIGHTING_NODE_SPEED_SLOW;
    ColorWave.speed_max      = CORSAIR_LIGHTING_NODE_SPEED_FAST;
    ColorWave.speed          = CORSAIR_LIGHTING_NODE_SPEED_MEDIUM;
    ColorWave.direction      = MODE_DIRECTION_RIGHT;
    ColorWave.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    ColorWave.colors.resize(2);
    ColorWave.brightness_min = 0;
    ColorWave.brightness_max = 100;
    ColorWave.brightness     = 100;
    modes.push_back(ColorWave);

    mode Static;
    Static.name           = "Static";
    Static.value          = CORSAIR_LIGHTING_NODE_MODE_STATIC;
    Static.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.colors_min     = 1;
    Static.colors_max     = 1;
    Static.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    Static.brightness_min = 0;
    Static.brightness_max = 100;
    Static.brightness     = 100;
    modes.push_back(Static);

    mode Temperature;
    Temperature.name           = "Temperature";
    Temperature.value          = CORSAIR_LIGHTING_NODE_MODE_TEMPERATURE;
    Temperature.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Temperature.colors_min     = 3;
    Temperature.colors_max     = 3;
    Temperature.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Temperature.colors.resize(3);
    Temperature.brightness_min = 0;
    Temperature.brightness_max = 100;
    Temperature.brightness     = 100;
    modes.push_back(Temperature);

    mode Visor;
    Visor.name           = "Visor";
    Visor.value          = CORSAIR_LIGHTING_NODE_MODE_VISOR;
    Visor.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Visor.colors_min     = 2;
    Visor.colors_max     = 2;
    Visor.speed_min      = CORSAIR_LIGHTING_NODE_SPEED_SLOW;
    Visor.speed_max      = CORSAIR_LIGHTING_NODE_SPEED_FAST;
    Visor.speed          = CORSAIR_LIGHTING_NODE_SPEED_MEDIUM;
    Visor.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Visor.colors.resize(2);
    Visor.brightness_min = 0;
    Visor.brightness_max = 100;
    Visor.brightness     = 100;
    modes.push_back(Visor);

    mode Marquee;
    Marquee.name           = "Marquee";
    Marquee.value          = CORSAIR_LIGHTING_NODE_MODE_MARQUEE;
    Marquee.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Marquee.colors_min     = 1;
    Marquee.colors_max     = 1;
    Marquee.speed_min      = CORSAIR_LIGHTING_NODE_SPEED_SLOW;
    Marquee.speed_max      = CORSAIR_LIGHTING_NODE_SPEED_FAST;
    Marquee.speed          = CORSAIR_LIGHTING_NODE_SPEED_MEDIUM;
    Marquee.direction      = MODE_DIRECTION_RIGHT;
    Marquee.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Marquee.colors.resize(1);
    Marquee.brightness_min = 0;
    Marquee.brightness_max = 100;
    Marquee.brightness     = 100;
    modes.push_back(Marquee);

    mode Blink;
    Blink.name           = "Blink";
    Blink.value          = CORSAIR_LIGHTING_NODE_MODE_BLINK;
    Blink.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Blink.colors_min     = 2;
    Blink.colors_max     = 2;
    Blink.speed_min      = CORSAIR_LIGHTING_NODE_SPEED_SLOW;
    Blink.speed_max      = CORSAIR_LIGHTING_NODE_SPEED_FAST;
    Blink.speed          = CORSAIR_LIGHTING_NODE_SPEED_MEDIUM;
    Blink.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Blink.colors.resize(2);
    Blink.brightness_min = 0;
    Blink.brightness_max = 100;
    Blink.brightness     = 100;
    modes.push_back(Blink);

    mode Sequential;
    Sequential.name           = "Sequential";
    Sequential.value          = CORSAIR_LIGHTING_NODE_MODE_SEQUENTIAL;
    Sequential.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Sequential.colors_min     = 1;
    Sequential.colors_max     = 1;
    Sequential.speed_min      = CORSAIR_LIGHTING_NODE_SPEED_SLOW;
    Sequential.speed_max      = CORSAIR_LIGHTING_NODE_SPEED_FAST;
    Sequential.speed          = CORSAIR_LIGHTING_NODE_SPEED_MEDIUM;
    Sequential.direction      = MODE_DIRECTION_RIGHT;
    Sequential.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Sequential.colors.resize(1);
    Sequential.brightness_min = 0;
    Sequential.brightness_max = 100;
    Sequential.brightness     = 100;
    modes.push_back(Sequential);

    mode Rainbow;
    Rainbow.name           = "Rainbow";
    Rainbow.value          = CORSAIR_LIGHTING_NODE_MODE_RAINBOW;
    Rainbow.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.speed_min      = CORSAIR_LIGHTING_NODE_SPEED_SLOW;
    Rainbow.speed_max      = CORSAIR_LIGHTING_NODE_SPEED_FAST;
    Rainbow.speed          = CORSAIR_LIGHTING_NODE_SPEED_MEDIUM;
    Rainbow.color_mode     = MODE_COLORS_NONE;
    Rainbow.brightness_min = 0;
    Rainbow.brightness_max = 100;
    Rainbow.brightness     = 100;
    modes.push_back(Rainbow);

    SetupZones();
}

RGBController_CorsairLightingNode::~RGBController_CorsairLightingNode()
{
    delete controller;
}

void RGBController_CorsairLightingNode::SetupZones()
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
    zones.resize(CORSAIR_LIGHTING_NODE_NUM_CHANNELS);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    for (unsigned int channel_idx = 0; channel_idx < CORSAIR_LIGHTING_NODE_NUM_CHANNELS; channel_idx++)
    {
        char ch_idx_string[2];
        snprintf(ch_idx_string, 2, "%d", channel_idx + 1);

        zones[channel_idx].name     = "Corsair Channel ";
        zones[channel_idx].name.append(ch_idx_string);
        zones[channel_idx].type     = ZONE_TYPE_LINEAR;

        /*-------------------------------------------------*\
        | I did some experimenting and determined that the  |
        | maximum number of LEDs the Corsair Commander Pro  |
        | can support is 200.                               |
        \*-------------------------------------------------*/
        zones[channel_idx].leds_min = 0;
        zones[channel_idx].leds_max = 204;

        if(first_run)
        {
            zones[channel_idx].leds_count = 0;
        }

        zones[channel_idx].matrix_map = NULL;

        for (unsigned int led_ch_idx = 0; led_ch_idx < zones[channel_idx].leds_count; led_ch_idx++)
        {
            char led_idx_string[4];
            snprintf(led_idx_string, 4, "%d", led_ch_idx + 1);

            led new_led;
            new_led.name = "Corsair Channel ";
            new_led.name.append(ch_idx_string);
            new_led.name.append(", LED ");
            new_led.name.append(led_idx_string);

            leds.push_back(new_led);
            leds_channel.push_back(channel_idx);
        }
    }

    SetupColors();
}

void RGBController_CorsairLightingNode::ResizeZone(int zone, int new_size)
{
    if((size_t) zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

        SetupZones();
    }
}

void RGBController_CorsairLightingNode::DeviceUpdateLEDs()
{
    for(unsigned char zone_idx = 0; zone_idx < (unsigned char)zones.size(); zone_idx++)
    {
        controller->SetChannelLEDs(zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}

void RGBController_CorsairLightingNode::UpdateZoneLEDs(int zone)
{
    controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
}

void RGBController_CorsairLightingNode::UpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];

    controller->SetChannelLEDs(channel, zones[channel].colors, zones[channel].leds_count);
}

void RGBController_CorsairLightingNode::DeviceUpdateMode()
{
    if(modes[active_mode].value == 0xFFFF)
    {
        DeviceUpdateLEDs();
    }
    else
    {
        for(int channel = 0; channel < CORSAIR_LIGHTING_NODE_NUM_CHANNELS; channel++)
        {
            unsigned int direction = 0;
            bool random = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);

            if(modes[active_mode].direction == MODE_DIRECTION_RIGHT)
            {
                direction = 1;
            }

            unsigned char mode_colors[9];

            if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                for(std::size_t i = 0; i < modes[active_mode].colors.size(); i++)
                {
                    mode_colors[(3 * i) + 0] = RGBGetRValue(modes[active_mode].colors[i]);
                    mode_colors[(3 * i) + 1] = RGBGetGValue(modes[active_mode].colors[i]);
                    mode_colors[(3 * i) + 2] = RGBGetBValue(modes[active_mode].colors[i]);
                }
            }

            controller->SetChannelEffect(channel,
                                         zones[channel].leds_count,
                                         modes[active_mode].value,
                                         modes[active_mode].speed,
                                         direction,
                                         random,
                                         mode_colors[0],
                                         mode_colors[1],
                                         mode_colors[2],
                                         mode_colors[3],
                                         mode_colors[4],
                                         mode_colors[5],
                                         mode_colors[6],
                                         mode_colors[7],
                                         mode_colors[8]);
        }
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        controller->SetBrightness(modes[active_mode].brightness);
    }
    else
    {
        controller->SetBrightness(100);
    }
}
