/*---------------------------------------------------------*\
| RGBController_LianLiUniHub.cpp                            |
|                                                           |
|   RGBController for Lian Li Uni Hub                       |
|                                                           |
|   Luca Lovisa                                 20 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string>
#include "RGBController_LianLiUniHub.h"

mode makeMode()
{
  mode Mode;

  Mode.value      = 0;
  Mode.flags      = 0;
  Mode.speed_min  = 0;
  Mode.speed_max  = 0;
  Mode.colors_min = 0;
  Mode.colors_max = 0;
  Mode.speed      = 0;
  Mode.direction  = 0;
  Mode.color_mode = 0;

  return Mode;
}

/**------------------------------------------------------------------*\
    @name Lian Li Uni Hub
    @category Cooler
    @type USB
    @save :x:
    @direct :rotating_light:
    @effects :white_check_mark:
    @detectors DetectLianLiUniHub
    @comment
\*-------------------------------------------------------------------*/

RGBController_LianLiUniHub::RGBController_LianLiUniHub(LianLiUniHubController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Lian Li Uni Hub";
    vendor      = "Lian Li";
    version     = controller->GetVersion();
    type        = DEVICE_TYPE_COOLER;
    description = "Lian Li Uni Hub";
    location    = controller->GetLocation();
    serial      = controller->GetSerial();

    initializedMode = false;

    mode StaticColor       = makeMode();
    StaticColor.name       = "Custom";
    StaticColor.value      = UNIHUB_LED_MODE_STATIC_COLOR;
    StaticColor.flags      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    StaticColor.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(StaticColor);

    mode Rainbow       = makeMode();
    Rainbow.name       = "Rainbow Wave";
    Rainbow.value      = UNIHUB_LED_MODE_RAINBOW;
    Rainbow.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Rainbow.speed_min  = 1;
    Rainbow.speed_max  = 5;
    Rainbow.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Breathing       = makeMode();
    Breathing.name       = "Breathing";
    Breathing.value      = UNIHUB_LED_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min  = 1;
    Breathing.speed_max  = 5;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode ColorCycle       = makeMode();
    ColorCycle.name       = "Color Cycle";
    ColorCycle.value      = UNIHUB_LED_MODE_COLOR_CYCLE;
    ColorCycle.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    ColorCycle.speed_min  = 1;
    ColorCycle.speed_max  = 5;
    ColorCycle.colors_min = 3;
    ColorCycle.colors_max = 3;
    ColorCycle.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ColorCycle.colors.resize(3);
    modes.push_back(ColorCycle);

    mode Runway       = makeMode();
    Runway.name       = "Runway";
    Runway.value      = UNIHUB_LED_MODE_RUNWAY;
    Runway.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Runway.speed_min  = 1;
    Runway.speed_max  = 5;
    Runway.colors_min = 2;
    Runway.colors_max = 2;
    Runway.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Runway.colors.resize(2);
    modes.push_back(Runway);

    mode RunwaySync       = makeMode();
    RunwaySync.name       = "Runway Sync";
    RunwaySync.value      = UNIHUB_LED_MODE_RUNWAY_SYNC | 0x0100;
    RunwaySync.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    RunwaySync.speed_min  = 1;
    RunwaySync.speed_max  = 5;
    RunwaySync.colors_min = 2;
    RunwaySync.colors_max = 2;
    RunwaySync.color_mode = MODE_COLORS_MODE_SPECIFIC;
    RunwaySync.colors.resize(2);
    modes.push_back(RunwaySync);

    mode Staggered       = makeMode();
    Staggered.name       = "Staggered";
    Staggered.value      = UNIHUB_LED_MODE_STAGGGERED;
    Staggered.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Staggered.speed_min  = 1;
    Staggered.speed_max  = 5;
    Staggered.colors_min = 2;
    Staggered.colors_max = 2;
    Staggered.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Staggered.colors.resize(2);
    modes.push_back(Staggered);

    mode Mixing       = makeMode();
    Mixing.name       = "Mixing";
    Mixing.value      = UNIHUB_LED_MODE_MIXING;
    Mixing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Mixing.speed_min  = 1;
    Mixing.speed_max  = 5;
    Mixing.colors_min = 2;
    Mixing.colors_max = 2;
    Mixing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Mixing.colors.resize(2);
    modes.push_back(Mixing);

    mode Meteor       = makeMode();
    Meteor.name       = "Meteor";
    Meteor.value      = UNIHUB_LED_MODE_METEOR;
    Meteor.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Meteor.speed_min  = 1;
    Meteor.speed_max  = 5;
    Meteor.colors_min = 2;
    Meteor.colors_max = 2;
    Meteor.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Meteor.colors.resize(2);
    modes.push_back(Meteor);

    mode MeteorSync       = makeMode();
    MeteorSync.name       = "Meteor Sync";
    MeteorSync.value      = UNIHUB_LED_MODE_METEOR_SYNC | 0x0100;
    MeteorSync.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    MeteorSync.speed_min  = 1;
    MeteorSync.speed_max  = 5;
    MeteorSync.colors_min = 2;
    MeteorSync.colors_max = 2;
    MeteorSync.color_mode = MODE_COLORS_MODE_SPECIFIC;
    MeteorSync.colors.resize(2);
    modes.push_back(MeteorSync);

    mode Firework       = makeMode();
    Firework.name       = "Firework";
    Firework.value      = UNIHUB_LED_MODE_FIREWORK;
    Firework.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Firework.speed_min  = 1;
    Firework.speed_max  = 5;
    Firework.colors_min = 2;
    Firework.colors_max = 2;
    Firework.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Firework.colors.resize(2);
    modes.push_back(Firework);

    mode Stack       = makeMode();
    Stack.name       = "Stack";
    Stack.value      = UNIHUB_LED_MODE_STACK;
    Stack.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Stack.speed_min  = 1;
    Stack.speed_max  = 5;
    Stack.colors_min = 1;
    Stack.colors_max = 1;
    Stack.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Stack.colors.resize(1);
    modes.push_back(Stack);

    mode StackMultiColor       = makeMode();
    StackMultiColor.name       = "Stack Multi Color";
    StackMultiColor.value      = UNIHUB_LED_MODE_STACK_MULTI_COLOR;
    StackMultiColor.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    StackMultiColor.speed_min  = 1;
    StackMultiColor.speed_max  = 5;
    StackMultiColor.color_mode = MODE_COLORS_NONE;
    modes.push_back(StackMultiColor);

    mode Neon       = makeMode();
    Neon.name       = "Neon";
    Neon.value      = UNIHUB_LED_MODE_NEON;
    Neon.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Neon.speed_min  = 1;
    Neon.speed_max  = 5;
    Neon.color_mode = MODE_COLORS_NONE;
    modes.push_back(Neon);

    mode Rgbh       = makeMode();
    Rgbh.name       = "RGB Header";
    Rgbh.value      = UNIHUB_LED_MODE_STATIC_COLOR | 0x0200;
    Rgbh.flags      = 0;
    Rgbh.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rgbh);

    RGBController_LianLiUniHub::SetupZones();
}

RGBController_LianLiUniHub::~RGBController_LianLiUniHub()
{
    Shutdown();

    delete controller;
}

void RGBController_LianLiUniHub::SetupZones()
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
    zones.resize(UNIHUB_CHANNEL_COUNT);

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
        zones[channel_idx].leds_max   = UNIHUB_CHANLED_COUNT;

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

void RGBController_LianLiUniHub::DeviceResizeZone(int zone, int new_size)
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

void RGBController_LianLiUniHub::DeviceUpdateLEDs()
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

void RGBController_LianLiUniHub::DeviceUpdateZoneLEDs(int zone)
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

void RGBController_LianLiUniHub::DeviceUpdateSingleLED(int led)
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

void RGBController_LianLiUniHub::DeviceUpdateMode()
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
            controller->SetLedSpeed(channel, UNIHUB_LED_SPEED_000);
        }

        if(modes[active_mode].flags & MODE_FLAG_HAS_DIRECTION_LR)
        {
            controller->SetLedDirection(channel, convertLedDirection(modes[active_mode].direction));
        }
        else
        {
            controller->SetLedDirection(channel, UNIHUB_LED_DIRECTION_LTR);
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

uint8_t RGBController_LianLiUniHub::convertAnyFanCount(uint8_t count)
{
    switch (count)
    {
        case 0:
            return UNIHUB_ANY_FAN_COUNT_000;

        case 1:
            return UNIHUB_ANY_FAN_COUNT_001;

        case 2:
            return UNIHUB_ANY_FAN_COUNT_002;

        case 3:
            return UNIHUB_ANY_FAN_COUNT_003;

        case 4:
            return UNIHUB_ANY_FAN_COUNT_004;

        default:
            return UNIHUB_ANY_FAN_COUNT_001;
    }
}

uint8_t RGBController_LianLiUniHub::convertLedSpeed(uint8_t speed)
{
    switch (speed)
    {
        case 1:
            return UNIHUB_LED_SPEED_000;

        case 2:
            return UNIHUB_LED_SPEED_025;

        case 3:
            return UNIHUB_LED_SPEED_050;

        case 4:
            return UNIHUB_LED_SPEED_075;

        case 5:
            return UNIHUB_LED_SPEED_100;

        default:
            return UNIHUB_LED_SPEED_050;
    }
}

uint8_t RGBController_LianLiUniHub::convertLedDirection(uint8_t direction)
{
    switch (direction)
    {
        case 0:
            return UNIHUB_LED_DIRECTION_LTR;

        case 1:
            return UNIHUB_LED_DIRECTION_RTL;

        default:
            return UNIHUB_LED_DIRECTION_LTR;
    }
}

uint8_t RGBController_LianLiUniHub::convertLedCountToFanCount(uint8_t count)
{
    /*-------------------------------------------------*\
    | Converts 0 to 0, 1-16 to 1, 17-32 to 2, 33-48 to  |
    | 3  and 49-64+ to 4                                |
    \*-------------------------------------------------*/
    if (count == 0x00)
    {
        return 0x00;
    }
    if (count >= 0x40)
    {
        count = 0x40;
    }

    return((count -1) / 16 + 1);
}
