/*---------------------------------------------------------*\
| RGBController_LianLiUniHubSLV2.cpp                        |
|                                                           |
|   RGBController for Lian Li SLV2 Uni Hub                  |
|                                                           |
|   Will Kennedy                                17 Jan 2023 |
|   Oliver P                                    26 Apr 2022 |
|   Credit to Luca Lovisa for original work.                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include "RGBController_LianLiUniHubSLV2.h"

/**------------------------------------------------------------------*\
    @name Lian Li Uni Hub SLV2
    @type USB
    @save :x:
    @direct :rotating_light:
    @effects :white_check_mark:
    @detectors DetectLianLiUniHubSLV2
    @comment
\*-------------------------------------------------------------------*/

RGBController_LianLiUniHubSLV2::RGBController_LianLiUniHubSLV2(LianLiUniHubSLV2Controller* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetName();
    vendor      = "Lian Li";
    type        = DEVICE_TYPE_COOLER;
    description = "Lian Li Uni Hub - SL V2";
    version     = controller->GetFirmwareVersionString();
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    initializedMode = false;

    mode Custom;
    Custom.name                 = "Custom";
    Custom.value                = UNIHUB_SLV2_LED_MODE_STATIC_COLOR;
    Custom.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Custom.brightness_min       = 0;
    Custom.brightness_max       = 50;
    Custom.brightness           = 50;
    Custom.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode StaticColor;
    StaticColor.name            = "Static";
    StaticColor.value           = UNIHUB_SLV2_LED_MODE_STATIC_COLOR;
    StaticColor.flags           = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    StaticColor.brightness_min  = 0;
    StaticColor.brightness_max  = 4;
    StaticColor.colors_min      = 0;
    StaticColor.colors_max      = 6;
    StaticColor.brightness      = 4;
    StaticColor.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    StaticColor.colors.resize(6);
    modes.push_back(StaticColor);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = UNIHUB_SLV2_LED_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.speed_min         = 0;
    Breathing.speed_max         = 4;
    Breathing.brightness_min    = 0;
    Breathing.brightness_max    = 4;
    Breathing.colors_min        = 0;
    Breathing.colors_max        = 6;
    Breathing.speed             = 2;
    Breathing.brightness        = 4;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(6);
    modes.push_back(Breathing);

    mode RainbowMorph;
    RainbowMorph.name           = "Spectrum Cycle";
    RainbowMorph.value          = UNIHUB_SLV2_LED_MODE_RAINBOW_MORPH;
    RainbowMorph.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    RainbowMorph.speed_min      = 0;
    RainbowMorph.speed_max      = 4;
    RainbowMorph.brightness_min = 0;
    RainbowMorph.brightness_max = 4;
    RainbowMorph.speed          = 2;
    RainbowMorph.brightness     = 4;
    RainbowMorph.color_mode     = MODE_COLORS_NONE;
    modes.push_back(RainbowMorph);

    mode RainbowWave;
    RainbowWave.name            = "Rainbow Wave";
    RainbowWave.value           = UNIHUB_SLV2_LED_MODE_RAINBOW;
    RainbowWave.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowWave.speed_min       = 0;
    RainbowWave.speed_max       = 4;
    RainbowWave.brightness_min  = 0;
    RainbowWave.brightness_max  = 4;
    RainbowWave.speed           = 2;
    RainbowWave.brightness      = 4;
    RainbowWave.direction       = UNIHUB_SLV2_LED_DIRECTION_LTR;
    RainbowWave.color_mode      = MODE_COLORS_NONE;
    modes.push_back(RainbowWave);

    mode Staggered;
    Staggered.name              = "Staggered";
    Staggered.value             = UNIHUB_SLV2_LED_MODE_STAGGERED;
    Staggered.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Staggered.speed_min         = 0;
    Staggered.speed_max         = 4;
    Staggered.brightness_min    = 0;
    Staggered.brightness_max    = 4;
    Staggered.colors_min        = 0;
    Staggered.colors_max        = 2;
    Staggered.speed             = 2;
    Staggered.brightness        = 4;
    Staggered.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Staggered.colors.resize(2);
    modes.push_back(Staggered);

    mode Tide; // TODO: Has merge
    Tide.name                   = "Tide";
    Tide.value                  = UNIHUB_SLV2_LED_MODE_TIDE;
    Tide.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Tide.speed_min              = 0;
    Tide.speed_max              = 4;
    Tide.brightness_min         = 0;
    Tide.brightness_max         = 4;
    Tide.colors_min             = 0;
    Tide.colors_max             = 2;
    Tide.speed                  = 2;
    Tide.brightness             = 4;
    Tide.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Tide.colors.resize(2);
    modes.push_back(Tide);

    mode Runway; //TODO: Has merge
    Runway.name                 = "Runway";
    Runway.value                = UNIHUB_SLV2_LED_MODE_RUNWAY;
    Runway.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Runway.speed_min            = 0;
    Runway.speed_max            = 4;
    Runway.brightness_min       = 0;
    Runway.brightness_max       = 4;
    Runway.colors_min           = 0;
    Runway.colors_max           = 2;
    Runway.speed                = 2;
    Runway.brightness           = 4;
    Runway.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Runway.colors.resize(2);
    modes.push_back(Runway);

    mode Mixing; //TODO: Has merge
    Mixing.name                 = "Mixing";
    Mixing.value                = UNIHUB_SLV2_LED_MODE_MIXING;
    Mixing.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Mixing.speed_min            = 0;
    Mixing.speed_max            = 4;
    Mixing.brightness_min       = 0;
    Mixing.brightness_max       = 4;
    Mixing.colors_min           = 0;
    Mixing.colors_max           = 2;
    Mixing.speed                = 2;
    Mixing.brightness           = 4;
    Mixing.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Mixing.colors.resize(2);
    modes.push_back(Mixing);

    mode Stack;
    Stack.name                  = "Stack";
    Stack.value                 = UNIHUB_SLV2_LED_MODE_STACK;
    Stack.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Stack.speed_min             = 0;
    Stack.speed_max             = 4;
    Stack.brightness_min        = 0;
    Stack.brightness_max        = 4;
    Stack.colors_min            = 0;
    Stack.colors_max            = 1;
    Stack.speed                 = 2;
    Stack.brightness            = 4;
    Stack.direction             = UNIHUB_SLV2_LED_DIRECTION_LTR;
    Stack.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Stack.colors.resize(1);
    modes.push_back(Stack);

    mode StackMultiColor; //TODO: Has merge
    Stack.name                  = "Stack Multi Color";
    Stack.value                 = UNIHUB_SLV2_LED_MODE_STACK_MULTI_COLOR;
    Stack.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Stack.speed_min             = 0;
    Stack.speed_max             = 4;
    Stack.brightness_min        = 0;
    Stack.brightness_max        = 4;
    Stack.speed                 = 2;
    Stack.brightness            = 4;
    Stack.direction             = UNIHUB_SLV2_LED_DIRECTION_LTR;
    Stack.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Stack);

    mode Neon;
    Neon.name                 = "Neon";
    Neon.value                = UNIHUB_SLV2_LED_MODE_NEON;
    Neon.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Neon.speed_min            = 0;
    Neon.speed_max            = 4;
    Neon.brightness_min       = 0;
    Neon.brightness_max       = 4;
    Neon.speed                = 2;
    Neon.brightness           = 4;
    Neon.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Neon);

    mode ColorCycle;
    ColorCycle.name             = "ColorCycle";
    ColorCycle.value            = UNIHUB_SLV2_LED_MODE_COLOR_CYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    ColorCycle.speed_min        = 0;
    ColorCycle.speed_max        = 4;
    ColorCycle.brightness_min   = 0;
    ColorCycle.brightness_max   = 4;
    ColorCycle.colors_min       = 0;
    ColorCycle.colors_max       = 3;
    ColorCycle.speed            = 2;
    ColorCycle.brightness       = 4;
    ColorCycle.direction        = UNIHUB_SLV2_LED_DIRECTION_LTR;
    ColorCycle.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    ColorCycle.colors.resize(3);
    modes.push_back(ColorCycle);

    mode Meteor; //TODO: Has merge
    Meteor.name                 = "Meteor";
    Meteor.value                = UNIHUB_SLV2_LED_MODE_METEOR;
    Meteor.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Meteor.speed_min            = 0;
    Meteor.speed_max            = 4;
    Meteor.brightness_min       = 0;
    Meteor.brightness_max       = 4;
    Meteor.colors_min           = 0;
    Meteor.colors_max           = 2;
    Meteor.speed                = 2;
    Meteor.brightness           = 4;
    Meteor.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Meteor.colors.resize(2);
    modes.push_back(Meteor);

    mode Voice;
    Voice.name                  = "Voice";
    Voice.value                 = UNIHUB_SLV2_LED_MODE_VOICE;
    Voice.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Voice.speed_min             = 0;
    Voice.speed_max             = 4;
    Voice.brightness_min        = 0;
    Voice.brightness_max        = 4;
    Voice.speed                 = 2;
    Voice.brightness            = 4;
    modes.push_back(Voice);

    mode Groove;
    Groove.name             = "Groove";
    Groove.value            = UNIHUB_SLV2_LED_MODE_GROOVE;
    Groove.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Groove.speed_min        = 0;
    Groove.speed_max        = 4;
    Groove.brightness_min   = 0;
    Groove.brightness_max   = 4;
    Groove.colors_min       = 0;
    Groove.colors_max       = 1;
    Groove.speed            = 2;
    Groove.brightness       = 4;
    Groove.direction        = UNIHUB_SLV2_LED_DIRECTION_LTR;
    Groove.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Groove.colors.resize(1);
    modes.push_back(Groove);

    mode Render;
    Render.name             = "Render";
    Render.value            = UNIHUB_SLV2_LED_MODE_RENDER;
    Render.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Render.speed_min        = 0;
    Render.speed_max        = 4;
    Render.brightness_min   = 0;
    Render.brightness_max   = 4;
    Render.colors_min       = 0;
    Render.colors_max       = 4;
    Render.speed            = 2;
    Render.brightness       = 4;
    Render.direction        = UNIHUB_SLV2_LED_DIRECTION_LTR;
    Render.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Render.colors.resize(4);
    modes.push_back(Render);

    mode Tunnel;
    Tunnel.name             = "Tunnel";
    Tunnel.value            = UNIHUB_SLV2_LED_MODE_TUNNEL;
    Tunnel.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Tunnel.speed_min        = 0;
    Tunnel.speed_max        = 4;
    Tunnel.brightness_min   = 0;
    Tunnel.brightness_max   = 4;
    Tunnel.colors_min       = 0;
    Tunnel.colors_max       = 4;
    Tunnel.speed            = 2;
    Tunnel.brightness       = 4;
    Tunnel.direction        = UNIHUB_SLV2_LED_DIRECTION_LTR;
    Tunnel.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Tunnel.colors.resize(4);
    modes.push_back(Tunnel);

    RGBController_LianLiUniHubSLV2::SetupZones();
}

RGBController_LianLiUniHubSLV2::~RGBController_LianLiUniHubSLV2()
{
    delete controller;
}

void RGBController_LianLiUniHubSLV2::SetupZones()
{
    /*-------------------------------------------------*\
    | Only set LED count on the first run               |
    \*-------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
        zones.resize(UNIHUB_SLV2_CHANNEL_COUNT);
    }

    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    for(unsigned int channel_idx = 0; channel_idx < zones.size(); channel_idx++)
    {
        zones[channel_idx].name                 = "Channel ";
        zones[channel_idx].name.append(std::to_string(channel_idx + 1));

        zones[channel_idx].type                 = ZONE_TYPE_LINEAR;
        zones[channel_idx].leds_min             = 0;
        zones[channel_idx].leds_max             = UNIHUB_SLV2_CHAN_LED_COUNT;

        if(first_run)
        {
            zones[channel_idx].leds_count = zones[channel_idx].leds_min;
        }

        for(unsigned int led_ch_idx = 0; led_ch_idx < zones[channel_idx].leds_count; led_ch_idx++)
        {
            led new_led;
            new_led.name = zones[channel_idx].name;
            new_led.name.append(", LED ");
            new_led.name.append(std::to_string(led_ch_idx + 1));
            new_led.value = channel_idx;

            leds.push_back(new_led);
        }

    }

    SetupColors();
}

void RGBController_LianLiUniHubSLV2::ResizeZone(int zone, int new_size)
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

void RGBController_LianLiUniHubSLV2::DeviceUpdateLEDs()
{

    if(!initializedMode)
    {
        DeviceUpdateMode();
    }

    float brightness_scale = static_cast<float>(modes[active_mode].brightness)/modes[active_mode].brightness_max;

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        controller->SetChannelLEDs((unsigned char)zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count, brightness_scale);
    }
}

void RGBController_LianLiUniHubSLV2::UpdateZoneLEDs(int zone)
{
    if(!initializedMode)
    {
        DeviceUpdateMode();
    }

    float brightness_scale = static_cast<float>(modes[active_mode].brightness)/modes[active_mode].brightness_max;

    controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count, brightness_scale);
}

void RGBController_LianLiUniHubSLV2::UpdateSingleLED(int /* led */)
{
    DeviceUpdateMode();

}

void RGBController_LianLiUniHubSLV2::DeviceUpdateMode()
{
    if(!active_mode)
    {
        return;                 // Do nothing, custom mode should go through DeviceUpdateLEDs() to avoid flooding controller
    }

    initializedMode   = true;

    int                 fan_idx = 0;

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        if(zones[zone_idx].leds_count == 0)
        {
            return;     // Do nothing, channel isn't in use
        }
        fan_idx = ((zones[zone_idx].leds_count / 16) - 1);        // Indexes start at 0

        controller->SetChannelMode((unsigned char)zone_idx,
                                   modes[active_mode],
                                   fan_idx);

    }
}
