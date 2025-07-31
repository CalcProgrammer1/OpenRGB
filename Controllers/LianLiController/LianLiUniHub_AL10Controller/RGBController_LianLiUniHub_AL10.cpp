/*---------------------------------------------------------*\
| RGBController_LianLiUniHub_AL10.cpp                       |
|                                                           |
|   RGBController for Lian Li AL10 Uni Hub                  |
|                                                           |
|   Oliver P                                    05 May 2022 |
|   Credit to Luca Lovisa for original work                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include "RGBController_LianLiUniHub_AL10.h"

/**------------------------------------------------------------------*\
    @name Lian Li Uni Hub
    @type USB
    @save :x:
    @direct :rotating_light:
    @effects :white_check_mark:
    @detectors DetectLianLiUniHub
    @comment
\*-------------------------------------------------------------------*/

RGBController_LianLiUniHub_AL10::RGBController_LianLiUniHub_AL10(LianLiUniHub_AL10Controller* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Lian Li Uni Hub - AL";
    vendor      = "Lian Li";
    version     = controller->GetVersion();
    type        = DEVICE_TYPE_COOLER;
    description = "Lian Li Uni Hub - AL v1.0";
    location    = controller->GetLocation();
    serial      = controller->GetSerial();

    initializedMode = false;

    mode Custom;
    Custom.name                 = "Custom";
    Custom.value                = UNIHUB_AL10_LED_MODE_STATIC_COLOR;
    Custom.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Custom.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode RainbowWave;
    RainbowWave.name            = "Rainbow Wave";
    RainbowWave.value           = UNIHUB_AL10_LED_MODE_RAINBOW;
    RainbowWave.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowWave.speed_min       = 0;
    RainbowWave.speed_max       = 4;
    RainbowWave.brightness_min  = 0;
    RainbowWave.brightness_max  = 4;
    RainbowWave.speed           = 3;
    RainbowWave.brightness      = 3;
    RainbowWave.direction       = UNIHUB_AL10_LED_DIRECTION_LTR;
    RainbowWave.color_mode      = MODE_COLORS_NONE;
    //RainbowWave.colors[0]       = ToRGBColor(253,253,253);
    modes.push_back(RainbowWave);

    /*  Needs updated code
    mode RainbowMorph;
    RainbowMorph.name           = "Rainbow Morph";
    RainbowMorph.value          = UNIHUB_AL10_LED_MODE_RAINBOW_MORPH;
    RainbowMorph.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    RainbowMorph.speed_min      = 0;
    RainbowMorph.speed_max      = 4;
    RainbowMorph.brightness_min = 0;
    RainbowMorph.brightness_max = 4;
    RainbowMorph.speed          = 3;
    RainbowMorph.brightness     = 3;
    RainbowMorph.color_mode     = MODE_COLORS_NONE;
    modes.push_back(RainbowMorph);
    */

    mode StaticColor;
    StaticColor.name            = "Static Color";
    StaticColor.value           = UNIHUB_AL10_LED_MODE_STATIC_COLOR;
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
    Breathing.value             = UNIHUB_AL10_LED_MODE_BREATHING;
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
    Taichi.value                = UNIHUB_AL10_LED_MODE_TAICHI;
    Taichi.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Taichi.speed_min            = 0;
    Taichi.speed_max            = 4;
    Taichi.brightness_min       = 0;
    Taichi.brightness_max       = 4;
    Taichi.colors_min           = 0;
    Taichi.colors_max           = 2;
    Taichi.speed                = 3;
    Taichi.brightness           = 3;
    Taichi.direction            = UNIHUB_AL10_LED_DIRECTION_LTR;
    Taichi.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Taichi.colors.resize(2);
    modes.push_back(Taichi);

    mode ColorCycle;
    ColorCycle.name             = "ColorCycle";
    ColorCycle.value            = UNIHUB_AL10_LED_MODE_COLOR_CYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    ColorCycle.speed_min        = 0;
    ColorCycle.speed_max        = 4;
    ColorCycle.brightness_min   = 0;
    ColorCycle.brightness_max   = 4;
    ColorCycle.colors_min       = 0;
    ColorCycle.colors_max       = 4;
    ColorCycle.speed            = 3;
    ColorCycle.brightness       = 3;
    ColorCycle.direction        = UNIHUB_AL10_LED_DIRECTION_LTR;
    ColorCycle.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    ColorCycle.colors.resize(4);
    modes.push_back(ColorCycle);

   /*  Needs updated code
    mode Runway;
    Runway.name                 = "Runway";
    Runway.value                = UNIHUB_AL10_LED_MODE_RUNWAY;
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
    */

    /*  Needs updated code
    mode Meteor;
    Meteor.name                 = "Meteor";
    Meteor.value                = UNIHUB_AL10_LED_MODE_METEOR;
    Meteor.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Meteor.speed_min            = 0;
    Meteor.speed_max            = 4;
    Meteor.brightness_min       = 0;
    Meteor.brightness_max       = 4;
    Meteor.colors_min           = 0;
    Meteor.colors_max           = 4;
    Meteor.speed                = 3;
    Meteor.brightness           = 3;
    Meteor.direction            = UNIHUB_AL10_LED_DIRECTION_LTR;
    Meteor.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Meteor.colors.resize(4);
    modes.push_back(Meteor);
    */

    mode Warning;
    Warning.name                = "Warning";
    Warning.value               = UNIHUB_AL10_LED_MODE_WARNING;
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
    Voice.value                 = UNIHUB_AL10_LED_MODE_VOICE;
    Voice.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Voice.speed_min             = 0;
    Voice.speed_max             = 4;
    Voice.brightness_min        = 0;
    Voice.brightness_max        = 4;
    Voice.colors_min            = 0;
    Voice.colors_max            = 4;
    Voice.speed                 = 3;
    Voice.brightness            = 3;
    Voice.direction             = UNIHUB_AL10_LED_DIRECTION_LTR;
    Voice.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Voice.colors.resize(4);
    modes.push_back(Voice);

    mode SpinningTeacup;
    SpinningTeacup.name             = "SpinningTeacup";
    SpinningTeacup.value            = UNIHUB_AL10_LED_MODE_SPINNING_TEACUP;
    SpinningTeacup.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    SpinningTeacup.speed_min        = 0;
    SpinningTeacup.speed_max        = 4;
    SpinningTeacup.brightness_min   = 0;
    SpinningTeacup.brightness_max   = 4;
    SpinningTeacup.colors_min       = 0;
    SpinningTeacup.colors_max       = 4;
    SpinningTeacup.speed            = 3;
    SpinningTeacup.brightness       = 3;
    SpinningTeacup.direction        = UNIHUB_AL10_LED_DIRECTION_LTR;
    SpinningTeacup.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    SpinningTeacup.colors.resize(4);
    modes.push_back(SpinningTeacup);

    mode Tornado;
    Tornado.name                = "Tornado";
    Tornado.value               = UNIHUB_AL10_LED_MODE_TORNADO;
    Tornado.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Tornado.speed_min           = 0;
    Tornado.speed_max           = 4;
    Tornado.brightness_min      = 0;
    Tornado.brightness_max      = 4;
    Tornado.colors_min          = 0;
    Tornado.colors_max          = 4;
    Tornado.speed               = 3;
    Tornado.brightness          = 3;
    Tornado.direction           = UNIHUB_AL10_LED_DIRECTION_LTR;
    Tornado.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Tornado.colors.resize(4);
    modes.push_back(Tornado);

    mode Mixing;
    Mixing.name                 = "Mixing";
    Mixing.value                = UNIHUB_AL10_LED_MODE_MIXING;
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

    /*  Needs updated code
    mode Stack;
    Stack.name                  = "Stack";
    Stack.value                 = UNIHUB_AL10_LED_MODE_STACK;
    Stack.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Stack.speed_min             = 0;
    Stack.speed_max             = 4;
    Stack.brightness_min        = 0;
    Stack.brightness_max        = 4;
    Stack.colors_min            = 0;
    Stack.colors_max            = 2;
    Stack.speed                 = 3;
    Stack.brightness            = 3;
    Stack.direction             = UNIHUB_AL10_LED_DIRECTION_LTR;
    Stack.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Stack.colors.resize(2);
    modes.push_back(Stack);
    */

    mode Staggered;
    Staggered.name              = "Staggered";
    Staggered.value             = UNIHUB_AL10_LED_MODE_STAGGGERED;
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
    Tide.value                  = UNIHUB_AL10_LED_MODE_TIDE;
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
    Scan.value                  = UNIHUB_AL10_LED_MODE_SCAN;
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
    Contest.value               = UNIHUB_AL10_LED_MODE_CONTEST;
    Contest.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Contest.speed_min           = 0;
    Contest.speed_max           = 4;
    Contest.brightness_min      = 0;
    Contest.brightness_max      = 4;
    Contest.colors_min          = 0;
    Contest.colors_max          = 2;
    Contest.speed               = 3;
    Contest.brightness          = 3;
    Contest.direction           = UNIHUB_AL10_LED_DIRECTION_LTR;
    Contest.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Contest.colors.resize(3);
    modes.push_back(Contest);

    /* Motherboard header mode? Not implemented yet
    mode Rgbh       = makeModeAL();
    Rgbh.name       = "RGB Header";
    Rgbh.value      = UNIHUB_AL10_LED_MODE_STATIC_COLOR | 0x0200;
    Rgbh.flags      = 0;
    Rgbh.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rgbh);
    */

    RGBController_LianLiUniHub_AL10::SetupZones();
}

void RGBController_LianLiUniHub_AL10::SetupZones()
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
    zones.resize(UNIHUB_AL10_CHANNEL_COUNT);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    int addressableCounter = 1;
    for(unsigned int channel_idx = 0; channel_idx < zones.size(); channel_idx++)
    {
        zones[channel_idx].name       = "Channel ";
        zones[channel_idx].name.append(std::to_string(addressableCounter));

        addressableCounter++;

        zones[channel_idx].type       = ZONE_TYPE_LINEAR;

        zones[channel_idx].leds_min   = 0;
        zones[channel_idx].leds_max   = UNIHUB_AL10_CHANLED_COUNT;

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

        zones[channel_idx].matrix_map = NULL;
    }

    SetupColors();
}

void RGBController_LianLiUniHub_AL10::ResizeZone(int zone, int new_size)
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

void RGBController_LianLiUniHub_AL10::DeviceUpdateLEDs()
{
    if(!initializedMode)
    {
        DeviceUpdateMode();
    }
    for(size_t channel = 0; channel < zones.size(); channel++)
    {
        uint8_t fanCount = convertLedCountToFanCount(zones[channel].leds_count);
        controller->SetAnyFanCount(channel, convertAnyFanCount(fanCount));
        controller->SetLedColors(channel, zones[channel].colors, zones[channel].leds_count);
    }

    controller->Synchronize();
}

void RGBController_LianLiUniHub_AL10::UpdateZoneLEDs(int zone)
{
    if(!initializedMode)
    {
        DeviceUpdateMode();
    }
    unsigned int channel = zone;

    uint8_t fanCount = convertLedCountToFanCount(zones[channel].leds_count);
    controller->SetAnyFanCount(channel, convertAnyFanCount(fanCount));
    controller->SetLedColors(channel, zones[channel].colors, zones[channel].leds_count);

    controller->Synchronize();
}

void RGBController_LianLiUniHub_AL10::UpdateSingleLED(int led)
{
    if(!initializedMode)
    {
        DeviceUpdateMode();
    }
    unsigned int channel = leds[led].value;

    uint8_t fanCount = convertLedCountToFanCount(zones[channel].leds_count);
    controller->SetAnyFanCount(channel, convertAnyFanCount(fanCount));
    controller->SetLedColors(channel, zones[channel].colors, zones[channel].leds_count);

    controller->Synchronize();
}

void RGBController_LianLiUniHub_AL10::DeviceUpdateMode()
{
    initializedMode = true;

    for (size_t channel = 0; channel < zones.size(); channel++)
    {
        uint8_t fanCount = convertLedCountToFanCount(zones[channel].leds_count);
        controller->SetAnyFanCount(channel, convertAnyFanCount(fanCount));

        switch (modes[active_mode].color_mode)
        {
            case MODE_COLORS_PER_LED:
                controller->SetLedColors(channel, zones[channel].colors, zones[channel].leds_count);
                break;

            case MODE_COLORS_MODE_SPECIFIC:
                controller->SetLedColors(channel, modes[active_mode].colors.data(), modes[active_mode].colors.size());
                break;

            default:
                controller->SetLedColors(channel, nullptr, 0);
                break;
        }

        controller->SetLedMode(channel, modes[active_mode].value);

        if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
        {
            controller->SetLedSpeed(channel, convertLedSpeed(modes[active_mode].speed));
        }
        else
        {
            controller->SetLedSpeed(channel, UNIHUB_AL10_LED_SPEED_075);
        }

        if(modes[active_mode].flags & MODE_FLAG_HAS_DIRECTION_LR)
        {
            controller->SetLedDirection(channel, convertLedDirection(modes[active_mode].direction));
        }
        else
        {
            controller->SetLedDirection(channel, UNIHUB_AL10_LED_DIRECTION_LTR);
        }

        if(modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
        {
            controller->SetLedBrightness(channel, convertLedBrightness(modes[active_mode].brightness));
        }
        else
        {
            controller->SetLedBrightness(channel, UNIHUB_AL10_LED_BRIGHTNESS_100);
        }
    }

    if(modes[active_mode].value & 0x0200)
    {
        controller->EnableRgbhMode();
        controller->DisableSyncMode();
    }
    else if (modes[active_mode].value & 0x0100)
    {
        controller->DisableRgbhMode();
        controller->EnableSyncMode();
    }
    else
    {
        controller->DisableRgbhMode();
        controller->DisableSyncMode();
    }

    controller->Synchronize();
}

uint8_t RGBController_LianLiUniHub_AL10::convertAnyFanCount(uint8_t count)
{
    switch (count)
    {
        case 0:
            return UNIHUB_AL10_ANY_FAN_COUNT_000;

        case 1:
            return UNIHUB_AL10_ANY_FAN_COUNT_001;

        case 2:
            return UNIHUB_AL10_ANY_FAN_COUNT_002;

        case 3:
            return UNIHUB_AL10_ANY_FAN_COUNT_003;

        case 4:
            return UNIHUB_AL10_ANY_FAN_COUNT_004;

        default:
            return UNIHUB_AL10_ANY_FAN_COUNT_001;
    }
}

uint8_t RGBController_LianLiUniHub_AL10::convertLedSpeed(uint8_t speed)
{
    switch (speed)
    {
        case 0:
            return UNIHUB_AL10_LED_SPEED_000;

        case 1:
            return UNIHUB_AL10_LED_SPEED_025;

        case 2:
            return UNIHUB_AL10_LED_SPEED_050;

        case 3:
            return UNIHUB_AL10_LED_SPEED_075;

        case 4:
            return UNIHUB_AL10_LED_SPEED_100;

        default:
            return UNIHUB_AL10_LED_SPEED_050;
    }
}

uint8_t RGBController_LianLiUniHub_AL10::convertLedDirection(uint8_t direction)
{
    switch (direction)
    {
        case 0:
            return UNIHUB_AL10_LED_DIRECTION_LTR;

        case 1:
            return UNIHUB_AL10_LED_DIRECTION_RTL;

        default:
            return UNIHUB_AL10_LED_DIRECTION_LTR;
    }
}

uint8_t RGBController_LianLiUniHub_AL10::convertLedBrightness(uint8_t brightness)
{
    switch (brightness)
    {
        case 0:
            return UNIHUB_AL10_LED_BRIGHTNESS_000;

        case 1:
            return UNIHUB_AL10_LED_BRIGHTNESS_025;

        case 2:
            return UNIHUB_AL10_LED_BRIGHTNESS_050;

        case 3:
            return UNIHUB_AL10_LED_BRIGHTNESS_075;

        case 4:
            return UNIHUB_AL10_LED_BRIGHTNESS_100;

        default:
            return UNIHUB_AL10_LED_BRIGHTNESS_100;
    }
}
uint8_t RGBController_LianLiUniHub_AL10::convertLedCountToFanCount(uint8_t count)
{
    /*-------------------------------------------------*\
    | Converts <20 to 0, 20-39 to 1, 40-59 to 2, 60=79  |
    | to 3  and 80+ to 4                                |
    \*-------------------------------------------------*/
    // Sets lower and upper limits
    if (count == 0x00)
    {
        return 0x00;
    }
    if (count >= 0x50)
    {
        count = 0x50;
    }

    /*---------------------------------------------------------*\
    | Returns regular count if it's not in multiples of 20s     |
    | (AL120 has 20 LEDs per fan, LED strip scenario)           |
    \*---------------------------------------------------------*/
    if (count % 20)
    {
        return(count);
    }
    else
    {
        return(count / 20);
    }
}
