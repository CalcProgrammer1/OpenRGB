/*---------------------------------------------------------*\
| RGBController_LianLiUniHubSL.cpp                          |
|                                                           |
|   RGBController for Lian Li Uni Hub - SL                  |
|                                                           |
|   Muhamad Visat                               26 Jul 2025 |
|   Original work by Luca Lovisa & Oliver P                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_LianLiUniHubSL.h"

/**------------------------------------------------------------------*\
    @name Lian Li Uni Hub - SL
    @category Cooler
    @type USB
    @save :x:
    @direct :rotating_light:
    @effects :white_check_mark:
    @detectors DetectLianLiUniHubSL
    @comment
\*-------------------------------------------------------------------*/

RGBController_LianLiUniHubSL::RGBController_LianLiUniHubSL(LianLiUniHubSLController *controller, std::string name)
{
    this->controller    = controller;
    this->name          = name;
    this->vendor        = "Lian Li";
    this->description   = "Lian Li Uni Hub - SL";
    this->version       = controller->ReadVersion();
    this->serial        = controller->ReadSerial();
    this->location      = controller->GetLocation();
    this->type          = DEVICE_TYPE_COOLER;

    initialized = false;

    mode Custom;
    Custom.name             = "Custom";
    Custom.value            = UNIHUB_SL_LED_MODE_STATIC;
    Custom.flags            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Custom.brightness_min   = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    Custom.brightness_max   = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    Custom.brightness       = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    Custom.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode Rainbow;
    Rainbow.name            = "Rainbow";
    Rainbow.value           = UNIHUB_SL_LED_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Rainbow.speed_min       = UNIHUB_SL_LED_SPEED_MIN;
    Rainbow.speed_max       = UNIHUB_SL_LED_SPEED_MAX;
    Rainbow.brightness_min  = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    Rainbow.brightness_max  = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    Rainbow.speed           = UNIHUB_SL_LED_SPEED_DEFAULT;
    Rainbow.brightness      = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    Rainbow.direction       = UNIHUB_SL_LED_DIRECTION_LTR;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode RainbowMorph;
    RainbowMorph.name           = "Rainbow Morph";
    RainbowMorph.value          = UNIHUB_SL_LED_MODE_RAINBOW_MORPH;
    RainbowMorph.flags          = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowMorph.speed_min      = UNIHUB_SL_LED_SPEED_MIN;
    RainbowMorph.speed_max      = UNIHUB_SL_LED_SPEED_MAX;
    RainbowMorph.brightness_min = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    RainbowMorph.brightness_max = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    RainbowMorph.speed          = UNIHUB_SL_LED_SPEED_DEFAULT;
    RainbowMorph.brightness     = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    RainbowMorph.direction      = UNIHUB_SL_LED_DIRECTION_LTR;
    RainbowMorph.color_mode     = MODE_COLORS_NONE;
    modes.push_back(RainbowMorph);

    mode Static;
    Static.name             = "Static";
    Static.value            = UNIHUB_SL_LED_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Static.brightness_min   = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    Static.brightness_max   = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    Static.brightness       = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    Static.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = UNIHUB_SL_LED_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min         = UNIHUB_SL_LED_SPEED_MIN;
    Breathing.speed_max         = UNIHUB_SL_LED_SPEED_MAX;
    Breathing.brightness_min    = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    Breathing.brightness_max    = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    Breathing.speed             = UNIHUB_SL_LED_SPEED_DEFAULT;
    Breathing.brightness        = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name             = "Color Cycle";
    ColorCycle.value            = UNIHUB_SL_LED_MODE_COLOR_CYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    ColorCycle.speed_min        = UNIHUB_SL_LED_SPEED_MIN;
    ColorCycle.speed_max        = UNIHUB_SL_LED_SPEED_MAX;
    ColorCycle.brightness_min   = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    ColorCycle.brightness_max   = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    ColorCycle.colors_min       = 3;
    ColorCycle.colors_max       = 3;
    ColorCycle.speed            = UNIHUB_SL_LED_SPEED_DEFAULT;
    ColorCycle.brightness       = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    ColorCycle.direction        = UNIHUB_SL_LED_DIRECTION_LTR;
    ColorCycle.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(ColorCycle);

    mode Runway;
    Runway.name             = "Runway";
    Runway.value            = UNIHUB_SL_LED_MODE_RUNWAY;
    Runway.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Runway.speed_min        = UNIHUB_SL_LED_SPEED_MIN;
    Runway.speed_max        = UNIHUB_SL_LED_SPEED_MAX;
    Runway.brightness_min   = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    Runway.brightness_max   = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    Runway.colors_min       = 2;
    Runway.colors_max       = 2;
    Runway.speed            = UNIHUB_SL_LED_SPEED_DEFAULT;
    Runway.brightness       = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    Runway.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Runway);

    mode RunwayMerged;
    RunwayMerged.name           = "Runway Merged";
    RunwayMerged.value          = UNIHUB_SL_LED_MODE_RUNWAY;
    RunwayMerged.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    RunwayMerged.speed_min      = UNIHUB_SL_LED_SPEED_MIN;
    RunwayMerged.speed_max      = UNIHUB_SL_LED_SPEED_MAX;
    RunwayMerged.brightness_min = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    RunwayMerged.brightness_max = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    RunwayMerged.colors_min     = 2;
    RunwayMerged.colors_max     = 2;
    RunwayMerged.speed          = UNIHUB_SL_LED_SPEED_DEFAULT;
    RunwayMerged.brightness     = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    RunwayMerged.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(RunwayMerged);

    mode Staggered;
    Staggered.name              = "Staggered";
    Staggered.value             = UNIHUB_SL_LED_MODE_STAGGERED;
    Staggered.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Staggered.speed_min         = UNIHUB_SL_LED_SPEED_MIN;
    Staggered.speed_max         = UNIHUB_SL_LED_SPEED_MAX;
    Staggered.brightness_min    = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    Staggered.brightness_max    = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    Staggered.colors_min        = 2;
    Staggered.colors_max        = 2;
    Staggered.speed             = UNIHUB_SL_LED_SPEED_DEFAULT;
    Staggered.brightness        = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    Staggered.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Staggered);

    mode Tide;
    Tide.name           = "Tide";
    Tide.value          = UNIHUB_SL_LED_MODE_TIDE;
    Tide.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Tide.speed_min      = UNIHUB_SL_LED_SPEED_MIN;
    Tide.speed_max      = UNIHUB_SL_LED_SPEED_MAX;
    Tide.brightness_min = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    Tide.brightness_max = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    Tide.colors_min     = 2;
    Tide.colors_max     = 2;
    Tide.speed          = UNIHUB_SL_LED_SPEED_DEFAULT;
    Tide.brightness     = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    Tide.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Tide);

    mode Meteor;
    Meteor.name             = "Meteor";
    Meteor.value            = UNIHUB_SL_LED_MODE_METEOR;
    Meteor.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Meteor.speed_min        = UNIHUB_SL_LED_SPEED_MIN;
    Meteor.speed_max        = UNIHUB_SL_LED_SPEED_MAX;
    Meteor.brightness_min   = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    Meteor.brightness_max   = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    Meteor.colors_min       = 2;
    Meteor.colors_max       = 2;
    Meteor.speed            = UNIHUB_SL_LED_SPEED_DEFAULT;
    Meteor.brightness       = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    Meteor.direction        = UNIHUB_SL_LED_DIRECTION_LTR;
    Meteor.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Meteor);

    mode MeteorMerged;
    MeteorMerged.name           = "Meteor Merged";
    MeteorMerged.value          = UNIHUB_SL_LED_MODE_METEOR_MERGED;
    MeteorMerged.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    MeteorMerged.speed_min      = UNIHUB_SL_LED_SPEED_MIN;
    MeteorMerged.speed_max      = UNIHUB_SL_LED_SPEED_MAX;
    MeteorMerged.brightness_min = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    MeteorMerged.brightness_max = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    MeteorMerged.colors_min     = 2;
    MeteorMerged.colors_max     = 2;
    MeteorMerged.speed          = UNIHUB_SL_LED_SPEED_DEFAULT;
    MeteorMerged.brightness     = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    MeteorMerged.direction      = UNIHUB_SL_LED_DIRECTION_LTR;
    MeteorMerged.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(MeteorMerged);

    mode Mixing;
    Mixing.name             = "Mixing";
    Mixing.value            = UNIHUB_SL_LED_MODE_MIXING;
    Mixing.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Mixing.speed_min        = UNIHUB_SL_LED_SPEED_MIN;
    Mixing.speed_max        = UNIHUB_SL_LED_SPEED_MAX;
    Mixing.brightness_min   = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    Mixing.brightness_max   = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    Mixing.colors_min       = 2;
    Mixing.colors_max       = 2;
    Mixing.speed            = UNIHUB_SL_LED_SPEED_DEFAULT;
    Mixing.brightness       = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    Mixing.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Mixing);

    mode Stack;
    Stack.name = "Stack";
    Stack.value             = UNIHUB_SL_LED_MODE_STACK;
    Stack.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Stack.speed_min         = UNIHUB_SL_LED_SPEED_MIN;
    Stack.speed_max         = UNIHUB_SL_LED_SPEED_MAX;
    Stack.brightness_min    = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    Stack.brightness_max    = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    Stack.colors_min        = 1;
    Stack.colors_max        = 1;
    Stack.speed             = UNIHUB_SL_LED_SPEED_DEFAULT;
    Stack.brightness        = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    Stack.direction         = UNIHUB_SL_LED_DIRECTION_LTR;
    Stack.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Stack);

    mode StackMultiColor;
    StackMultiColor.name            = "Stack Multi Color";
    StackMultiColor.value           = UNIHUB_SL_LED_MODE_STACK_MULTI_COLOR;
    StackMultiColor.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    StackMultiColor.speed_min       = UNIHUB_SL_LED_SPEED_MIN;
    StackMultiColor.speed_max       = UNIHUB_SL_LED_SPEED_MAX;
    StackMultiColor.brightness_min  = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    StackMultiColor.brightness_max  = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    StackMultiColor.speed           = UNIHUB_SL_LED_SPEED_DEFAULT;
    StackMultiColor.brightness      = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    StackMultiColor.direction       = UNIHUB_SL_LED_DIRECTION_LTR;
    StackMultiColor.color_mode      = MODE_COLORS_NONE;
    modes.push_back(StackMultiColor);

    mode Neon;
    Neon.name           = "Neon";
    Neon.value          = UNIHUB_SL_LED_MODE_NEON;
    Neon.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Neon.speed_min      = UNIHUB_SL_LED_SPEED_MIN;
    Neon.speed_max      = UNIHUB_SL_LED_SPEED_MAX;
    Neon.brightness_min = UNIHUB_SL_LED_BRIGHTNESS_MIN;
    Neon.brightness_max = UNIHUB_SL_LED_BRIGHTNESS_MAX;
    Neon.speed          = UNIHUB_SL_LED_SPEED_DEFAULT;
    Neon.brightness     = UNIHUB_SL_LED_BRIGHTNESS_DEFAULT;
    Neon.color_mode     = MODE_COLORS_NONE;
    modes.push_back(Neon);

    RGBColor default_colors[] =
    {
        ToRGBColor(255, 0, 0),      // Red
        ToRGBColor(0, 255, 0),      // Green
        ToRGBColor(0, 0, 255),      // Blue
        ToRGBColor(255, 255, 255),  // White
    };

    for(size_t mode_idx = 0; mode_idx < modes.size(); mode_idx++)
    {
        mode &m = modes[mode_idx];
        m.colors.resize(m.colors_max);
        for (unsigned int color_idx = 0; color_idx < m.colors_max; color_idx++)
        {
            m.colors[color_idx] = default_colors[color_idx % sizeof(default_colors)];
        }
    }

    RGBController_LianLiUniHubSL::SetupZones();
}

RGBController_LianLiUniHubSL::~RGBController_LianLiUniHubSL()
{
    delete this->controller;
}

void RGBController_LianLiUniHubSL::SetupZones()
{
    bool first_run = zones.size() == 0;

    leds.clear();
    colors.clear();
    if(first_run)
    {
        zones.resize(UNIHUB_SL_MAX_CHANNEL);
    }

    for(size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].name = "Channel ";
        zones[zone_idx].name.append(std::to_string(zone_idx + 1));
        zones[zone_idx].type = ZONE_TYPE_LINEAR;
        zones[zone_idx].matrix_map = NULL;

        zones[zone_idx].leds_min = 0;
        zones[zone_idx].leds_max = UNIHUB_SL_MAX_FAN_PER_CHANNEL;

        if(first_run)
        {
            zones[zone_idx].leds_count = zones[zone_idx].leds_min;
        }

        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name = zones[zone_idx].name;
            new_led.name.append(", Fan ");
            new_led.name.append(std::to_string(led_idx + 1));
            new_led.value = zone_idx;

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_LianLiUniHubSL::ResizeZone(int zone, int new_size)
{
    if((size_t)zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

        SetupZones();
    }
}

void RGBController_LianLiUniHubSL::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_LianLiUniHubSL::UpdateZoneLEDs(int zone)
{
    if(!initialized)
    {
        return DeviceUpdateMode();
    }
    controller->UpdateZoneLEDs(zone, zones[zone], modes[active_mode]);
}

void RGBController_LianLiUniHubSL::UpdateSingleLED(int /* led */)
{
    DeviceUpdateMode();
}

void RGBController_LianLiUniHubSL::DeviceUpdateMode()
{
    if(active_mode == 0)
    {
        return;
    }

    controller->UpdateMode(zones, modes[active_mode]);
    initialized = true;
}

void RGBController_LianLiUniHubSL::SetCustomMode()
{
    active_mode = 0;
}
