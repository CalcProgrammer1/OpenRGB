/*---------------------------------------------------------*\
| RGBController_LianLiUniHubAL.cpp                          |
|                                                           |
|   RGBController for Lian Li AL Uni Hub                    |
|                                                           |
|   Oliver P                                    26 Apr 2022 |
|   Credit to Luca Lovisa for original work                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include "RGBController_LianLiUniHubAL.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[8][35] =
    { {  NA,  NA,  10,  NA,  NA,  11,  NA,  NA,  NA,  NA,  NA,  30,  NA,  NA,  31,  NA,  NA,  NA,  NA,  NA,  50,  NA,  NA,  51,  NA,  NA,  NA,  NA,  NA,  70,  NA,  NA,  71,  NA,  NA},
      {  NA,   9,  NA,  NA,  NA,  NA,  12,  NA,  NA,  NA,  29,  NA,  NA,  NA,  NA,  32,  NA,  NA,  NA,  49,  NA,  NA,  NA,  NA,  52,  NA,  NA,  NA,  69,  NA,  NA,  NA,  NA,  72,  NA},
      {   8,  NA,  NA,   1,   2,  NA,  NA,  13,  NA,  28,  NA,  NA,  21,  22,  NA,  NA,  33,  NA,  48,  NA,  NA,  41,  42,  NA,  NA,  53,  NA,  68,  NA,  NA,  61,  62,  NA,  NA,  73},
      {  NA,  NA,   0,  NA,  NA,   3,  NA,  NA,  NA,  NA,  NA,  20,  NA,  NA,  23,  NA,  NA,  NA,  NA,  NA,  40,  NA,  NA,  43,  NA,  NA,  NA,  NA,  NA,  60,  NA,  NA,  63,  NA,  NA},
      {  NA,  NA,   7,  NA,  NA,   4,  NA,  NA,  NA,  NA,  NA,  27,  NA,  NA,  24,  NA,  NA,  NA,  NA,  NA,  47,  NA,  NA,  44,  NA,  NA,  NA,  NA,  NA,  67,  NA,  NA,  64,  NA,  NA},
      {  19,  NA,  NA,   6,   5,  NA,  NA,  14,  NA,  39,  NA,  NA,  26,  25,  NA,  NA,  34,  NA,  59,  NA,  NA,  46,  45,  NA,  NA,  54,  NA,  79,  NA,  NA,  66,  65,  NA,  NA,  74},
      {  NA,  18,  NA,  NA,  NA,  NA,  15,  NA,  NA,  NA,  38,  NA,  NA,  NA,  NA,  35,  NA,  NA,  NA,  58,  NA,  NA,  NA,  NA,  55,  NA,  NA,  NA,  78,  NA,  NA,  NA,  NA,  75,  NA},
      {  NA,  NA,  17,  NA,  NA,  16,  NA,  NA,  NA,  NA,  NA,  37,  NA,  NA,  36,  NA,  NA,  NA,  NA,  NA,  57,  NA,  NA,  56,  NA,  NA,  NA,  NA,  NA,  77,  NA,  NA,  76,  NA,  NA}
    };

/**------------------------------------------------------------------*\
    @name Lian Li Uni Hub AL
    @type USB
    @save :x:
    @direct :rotating_light:
    @effects :white_check_mark:
    @detectors DetectLianLiUniHubAL
    @comment
\*-------------------------------------------------------------------*/

RGBController_LianLiUniHubAL::RGBController_LianLiUniHubAL(LianLiUniHubALController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetName();
    vendor      = "Lian Li";
    type        = DEVICE_TYPE_COOLER;
    description = "Lian Li Uni Hub - AL";
    version     = controller->GetFirmwareVersionString();
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();


    initializedMode = false;

    mode Custom;
    Custom.name                 = "Custom";
    Custom.value                = UNIHUB_AL_LED_MODE_STATIC_COLOR;
    Custom.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Custom.brightness_min       = 0;
    Custom.brightness_max       = 50;
    Custom.brightness           = 37;
    Custom.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode RainbowWave;
    RainbowWave.name            = "Rainbow Wave";
    RainbowWave.value           = UNIHUB_AL_LED_MODE_RAINBOW;
    RainbowWave.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowWave.speed_min       = 0;
    RainbowWave.speed_max       = 4;
    RainbowWave.brightness_min  = 0;
    RainbowWave.brightness_max  = 4;
    RainbowWave.speed           = 3;
    RainbowWave.brightness      = 3;
    RainbowWave.direction       = UNIHUB_AL_LED_DIRECTION_LTR;
    RainbowWave.color_mode      = MODE_COLORS_NONE;
    modes.push_back(RainbowWave);

    mode RainbowMorph;
    RainbowMorph.name           = "Rainbow Morph";
    RainbowMorph.value          = UNIHUB_AL_LED_MODE_RAINBOW_MORPH;
    RainbowMorph.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    RainbowMorph.speed_min      = 0;
    RainbowMorph.speed_max      = 4;
    RainbowMorph.brightness_min = 0;
    RainbowMorph.brightness_max = 4;
    RainbowMorph.speed          = 3;
    RainbowMorph.brightness     = 3;
    RainbowMorph.color_mode     = MODE_COLORS_NONE;
    modes.push_back(RainbowMorph);

    mode StaticColor;
    StaticColor.name            = "Static Color";
    StaticColor.value           = UNIHUB_AL_LED_MODE_STATIC_COLOR;
    StaticColor.flags           = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    StaticColor.brightness_min  = 0;
    StaticColor.brightness_max  = 4;
    StaticColor.colors_min      = 0;
    StaticColor.colors_max      = 4;
    StaticColor.brightness      = 3;
    StaticColor.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    StaticColor.colors.resize(4);
    modes.push_back(StaticColor);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = UNIHUB_AL_LED_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.speed_min         = 0;
    Breathing.speed_max         = 4;
    Breathing.brightness_min    = 0;
    Breathing.brightness_max    = 4;
    Breathing.colors_min        = 0;
    Breathing.colors_max        = 4;
    Breathing.speed             = 3;
    Breathing.brightness        = 3;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(4);
    modes.push_back(Breathing);

    mode Taichi;
    Taichi.name                 = "Taichi";
    Taichi.value                = UNIHUB_AL_LED_MODE_TAICHI;
    Taichi.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Taichi.speed_min            = 0;
    Taichi.speed_max            = 4;
    Taichi.brightness_min       = 0;
    Taichi.brightness_max       = 4;
    Taichi.colors_min           = 0;
    Taichi.colors_max           = 2;
    Taichi.speed                = 3;
    Taichi.brightness           = 3;
    Taichi.direction            = UNIHUB_AL_LED_DIRECTION_LTR;
    Taichi.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Taichi.colors.resize(2);
    modes.push_back(Taichi);

    mode ColorCycle;
    ColorCycle.name             = "ColorCycle";
    ColorCycle.value            = UNIHUB_AL_LED_MODE_COLOR_CYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    ColorCycle.speed_min        = 0;
    ColorCycle.speed_max        = 4;
    ColorCycle.brightness_min   = 0;
    ColorCycle.brightness_max   = 4;
    ColorCycle.colors_min       = 0;
    ColorCycle.colors_max       = 4;
    ColorCycle.speed            = 3;
    ColorCycle.brightness       = 3;
    ColorCycle.direction        = UNIHUB_AL_LED_DIRECTION_LTR;
    ColorCycle.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    ColorCycle.colors.resize(4);
    modes.push_back(ColorCycle);

    mode Runway;
    Runway.name                 = "Runway";
    Runway.value                = UNIHUB_AL_LED_MODE_RUNWAY;
    Runway.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Runway.speed_min            = 0;
    Runway.speed_max            = 4;
    Runway.brightness_min       = 0;
    Runway.brightness_max       = 4;
    Runway.colors_min           = 0;
    Runway.colors_max           = 2;
    Runway.speed                = 3;
    Runway.brightness           = 3;
    Runway.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Runway.colors.resize(2);
    modes.push_back(Runway);

    mode Meteor;
    Meteor.name                 = "Meteor";
    Meteor.value                = UNIHUB_AL_LED_MODE_METEOR;
    Meteor.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Meteor.speed_min            = 0;
    Meteor.speed_max            = 4;
    Meteor.brightness_min       = 0;
    Meteor.brightness_max       = 4;
    Meteor.colors_min           = 0;
    Meteor.colors_max           = 4;
    Meteor.speed                = 3;
    Meteor.brightness           = 3;
    Meteor.direction            = UNIHUB_AL_LED_DIRECTION_LTR;
    Meteor.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Meteor.colors.resize(4);
    modes.push_back(Meteor);

    mode Warning;
    Warning.name                = "Warning";
    Warning.value               = UNIHUB_AL_LED_MODE_WARNING;
    Warning.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Warning.speed_min           = 0;
    Warning.speed_max           = 4;
    Warning.brightness_min      = 0;
    Warning.brightness_max      = 4;
    Warning.colors_min          = 0;
    Warning.colors_max          = 4;
    Warning.speed               = 3;
    Warning.brightness          = 3;
    Warning.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Warning.colors.resize(4);
    modes.push_back(Warning);

    mode Voice;
    Voice.name                  = "Voice";
    Voice.value                 = UNIHUB_AL_LED_MODE_VOICE;
    Voice.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Voice.speed_min             = 0;
    Voice.speed_max             = 4;
    Voice.brightness_min        = 0;
    Voice.brightness_max        = 4;
    Voice.colors_min            = 0;
    Voice.colors_max            = 4;
    Voice.speed                 = 3;
    Voice.brightness            = 3;
    Voice.direction             = UNIHUB_AL_LED_DIRECTION_LTR;
    Voice.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Voice.colors.resize(4);
    modes.push_back(Voice);

    mode SpinningTeacup;
    SpinningTeacup.name             = "SpinningTeacup";
    SpinningTeacup.value            = UNIHUB_AL_LED_MODE_SPINNING_TEACUP;
    SpinningTeacup.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    SpinningTeacup.speed_min        = 0;
    SpinningTeacup.speed_max        = 4;
    SpinningTeacup.brightness_min   = 0;
    SpinningTeacup.brightness_max   = 4;
    SpinningTeacup.colors_min       = 0;
    SpinningTeacup.colors_max       = 4;
    SpinningTeacup.speed            = 3;
    SpinningTeacup.brightness       = 3;
    SpinningTeacup.direction        = UNIHUB_AL_LED_DIRECTION_LTR;
    SpinningTeacup.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    SpinningTeacup.colors.resize(4);
    modes.push_back(SpinningTeacup);

    mode Tornado;
    Tornado.name                = "Tornado";
    Tornado.value               = UNIHUB_AL_LED_MODE_TORNADO;
    Tornado.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Tornado.speed_min           = 0;
    Tornado.speed_max           = 4;
    Tornado.brightness_min      = 0;
    Tornado.brightness_max      = 4;
    Tornado.colors_min          = 0;
    Tornado.colors_max          = 4;
    Tornado.speed               = 3;
    Tornado.brightness          = 3;
    Tornado.direction           = UNIHUB_AL_LED_DIRECTION_LTR;
    Tornado.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Tornado.colors.resize(4);
    modes.push_back(Tornado);

    mode Mixing;
    Mixing.name                 = "Mixing";
    Mixing.value                = UNIHUB_AL_LED_MODE_MIXING;
    Mixing.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Mixing.speed_min            = 0;
    Mixing.speed_max            = 4;
    Mixing.brightness_min       = 0;
    Mixing.brightness_max       = 4;
    Mixing.colors_min           = 0;
    Mixing.colors_max           = 2;
    Mixing.speed                = 3;
    Mixing.brightness           = 3;
    Mixing.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Mixing.colors.resize(2);
    modes.push_back(Mixing);

    mode Stack;
    Stack.name                  = "Stack";
    Stack.value                 = UNIHUB_AL_LED_MODE_STACK;
    Stack.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Stack.speed_min             = 0;
    Stack.speed_max             = 4;
    Stack.brightness_min        = 0;
    Stack.brightness_max        = 4;
    Stack.colors_min            = 0;
    Stack.colors_max            = 2;
    Stack.speed                 = 3;
    Stack.brightness            = 3;
    Stack.direction             = UNIHUB_AL_LED_DIRECTION_LTR;
    Stack.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Stack.colors.resize(2);
    modes.push_back(Stack);

    mode Staggered;
    Staggered.name              = "Staggered";
    Staggered.value             = UNIHUB_AL_LED_MODE_STAGGGERED;
    Staggered.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Staggered.speed_min         = 0;
    Staggered.speed_max         = 4;
    Staggered.brightness_min    = 0;
    Staggered.brightness_max    = 4;
    Staggered.colors_min        = 0;
    Staggered.colors_max        = 4;
    Staggered.speed             = 3;
    Staggered.brightness        = 3;
    Staggered.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Staggered.colors.resize(4);
    modes.push_back(Staggered);

    mode Tide;
    Tide.name                   = "Tide";
    Tide.value                  = UNIHUB_AL_LED_MODE_TIDE;
    Tide.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Tide.speed_min              = 0;
    Tide.speed_max              = 4;
    Tide.brightness_min         = 0;
    Tide.brightness_max         = 4;
    Tide.colors_min             = 0;
    Tide.colors_max             = 4;
    Tide.speed                  = 3;
    Tide.brightness             = 3;
    Tide.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Tide.colors.resize(4);
    modes.push_back(Tide);

    mode Scan;
    Scan.name                   = "Scan";
    Scan.value                  = UNIHUB_AL_LED_MODE_SCAN;
    Scan.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Scan.speed_min              = 0;
    Scan.speed_max              = 4;
    Scan.brightness_min         = 0;
    Scan.brightness_max         = 4;
    Scan.colors_min             = 0;
    Scan.colors_max             = 2;
    Scan.speed                  = 3;
    Scan.brightness             = 3;
    Scan.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Scan.colors.resize(2);
    modes.push_back(Scan);

    mode Contest;
    Contest.name                = "Contest";
    Contest.value               = UNIHUB_AL_LED_MODE_CONTEST;
    Contest.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Contest.speed_min           = 0;
    Contest.speed_max           = 4;
    Contest.brightness_min      = 0;
    Contest.brightness_max      = 4;
    Contest.colors_min          = 0;
    Contest.colors_max          = 2;
    Contest.speed               = 3;
    Contest.brightness          = 3;
    Contest.direction           = UNIHUB_AL_LED_DIRECTION_LTR;
    Contest.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Contest.colors.resize(3);
    modes.push_back(Contest);

    RGBController_LianLiUniHubAL::SetupZones();
}

RGBController_LianLiUniHubAL::~RGBController_LianLiUniHubAL()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_LianLiUniHubAL::SetupZones()
{
    /*-------------------------------------------------*\
    | Only set LED count on the first run               |
    \*-------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
        zones.resize(UNIHUB_AL_CHANNEL_COUNT);
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

        // Note:    Matrix types won't get loaded from the sizes.ors as the default zone type in this RGBController is ZONE_TYPE_LINEAR
        //          This will require augmentation on the ProfileManager.cpp to be able to override zone types but this is probably not wanted in general
        if (zones[channel_idx].leds_count == 60 || zones[channel_idx].leds_count == 40 || zones[channel_idx].leds_count == 20 || zones[channel_idx].leds_count == 80) // Assume they're AL120 Fans
        {
            zones[channel_idx].type                 = ZONE_TYPE_MATRIX;
            zones[channel_idx].leds_min             = 0;
            zones[channel_idx].leds_max             = UNIHUB_AL_CHAN_LED_COUNT;
            zones[channel_idx].matrix_map           = new matrix_map_type;
            zones[channel_idx].matrix_map->height   = 8;
            zones[channel_idx].matrix_map->width    = 35;
            zones[channel_idx].matrix_map->map      = (unsigned int *)&matrix_map;
        }
        else   // Treat as regular LED strip
        {
            zones[channel_idx].type                 = ZONE_TYPE_LINEAR;
            zones[channel_idx].leds_min             = 0;
            zones[channel_idx].leds_max             = UNIHUB_AL_CHAN_LED_COUNT;
        }

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

void RGBController_LianLiUniHubAL::ResizeZone(int zone, int new_size)
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

void RGBController_LianLiUniHubAL::DeviceUpdateLEDs()
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

void RGBController_LianLiUniHubAL::UpdateZoneLEDs(int zone)
{
    if(!initializedMode)
    {
        DeviceUpdateMode();
    }

    float brightness_scale = static_cast<float>(modes[active_mode].brightness)/modes[active_mode].brightness_max;

    controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count, brightness_scale);
}

void RGBController_LianLiUniHubAL::UpdateSingleLED(int /* led */)
{
    DeviceUpdateMode();

}

void RGBController_LianLiUniHubAL::DeviceUpdateMode()
{
    if(!active_mode)
    {
        return;                 // Do nothing, custom mode should go through DeviceUpdateLEDs() to avoid flooding controller
    }

    initializedMode   = true;

    int                 fan_idx = 0;
    bool                upd_both_fan_edge      = false;

    /*-----------------------------------------------------*\
    | Check modes that requires updating both arrays        |
    \*-----------------------------------------------------*/

    switch(modes[active_mode].value)
    {
        case UNIHUB_AL_LED_MODE_STATIC_COLOR:
        case UNIHUB_AL_LED_MODE_BREATHING:
        case UNIHUB_AL_LED_MODE_RUNWAY:
        case UNIHUB_AL_LED_MODE_METEOR:
            upd_both_fan_edge = true;
            break;
    }

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        if(zones[zone_idx].leds_count == 0)
        {
            return;     // Do nothing, channel isn't in use
        }
        fan_idx = ((zones[zone_idx].leds_count / 20) - 1);        // Indexes start at 0

        controller->SetChannelMode((unsigned char)zone_idx, modes[active_mode].value,modes[active_mode].colors, (unsigned int)modes[active_mode].colors.size(), (fan_idx >= 0 ? fan_idx : 0), upd_both_fan_edge, modes[active_mode].brightness, modes[active_mode].speed, modes[active_mode].direction);
    }
}
