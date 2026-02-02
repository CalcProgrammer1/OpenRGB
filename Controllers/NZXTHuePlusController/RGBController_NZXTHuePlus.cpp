/*---------------------------------------------------------*\
| RGBController_NZXTHuePlus.cpp                             |
|                                                           |
|   RGBController for NZXT Hue Plus                         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      20 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_NZXTHuePlus.h"

/**------------------------------------------------------------------*\
    @name NZXT Hue+
    @category LEDStrip
    @type Serial
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectNZXTHuePlusControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_HuePlus::RGBController_HuePlus(HuePlusController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "NZXT Hue+";
    vendor      = "NZXT";
    type        = DEVICE_TYPE_LEDSTRIP;
    description = "NZXT Hue+ Device";
    location    = controller->GetLocation();

    SetupZones();
    SetupModes();
}

RGBController_HuePlus::~RGBController_HuePlus()
{
    Shutdown();

    delete controller;
}

void RGBController_HuePlus::SetupModes()
{
    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = HUE_PLUS_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_REQUIRES_ENTIRE_DEVICE;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Fading;
    Fading.name             = "Fading";
    Fading.value            = HUE_PLUS_MODE_FADING;
    Fading.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Fading.speed_min        = HUE_PLUS_SPEED_SLOWEST;
    Fading.speed_max        = HUE_PLUS_SPEED_FASTEST;
    Fading.colors_min       = 1;
    Fading.colors_max       = 8;
    Fading.speed            = HUE_PLUS_SPEED_NORMAL;
    Fading.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Fading.colors.resize(2);
    modes.push_back(Fading);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Fading);
    }

    mode RainbowWave;
    RainbowWave.name        = "Rainbow Wave";
    RainbowWave.value       = HUE_PLUS_MODE_SPECTRUM;
    RainbowWave.flags       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowWave.speed_min   = HUE_PLUS_SPEED_SLOWEST;
    RainbowWave.speed_max   = HUE_PLUS_SPEED_FASTEST;
    RainbowWave.speed       = HUE_PLUS_SPEED_NORMAL;
    RainbowWave.direction   = MODE_DIRECTION_RIGHT;
    RainbowWave.color_mode  = MODE_COLORS_NONE;
    modes.push_back(RainbowWave);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(RainbowWave);
    }

    mode Marquee;
    Marquee.name            = "Marquee";
    Marquee.value           = HUE_PLUS_MODE_MARQUEE;
    Marquee.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Marquee.speed_min       = HUE_PLUS_SPEED_SLOWEST;
    Marquee.speed_max       = HUE_PLUS_SPEED_FASTEST;
    Marquee.colors_min      = 1;
    Marquee.colors_max      = 1;
    Marquee.speed           = HUE_PLUS_SPEED_NORMAL;
    Marquee.direction       = MODE_DIRECTION_RIGHT;
    Marquee.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    Marquee.colors.resize(1);
    modes.push_back(Marquee);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Marquee);
    }

    mode CoverMarquee;
    CoverMarquee.name       = "Cover Marquee";
    CoverMarquee.value      = HUE_PLUS_MODE_COVER_MARQUEE;
    CoverMarquee.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    CoverMarquee.speed_min  = HUE_PLUS_SPEED_SLOWEST;
    CoverMarquee.speed_max  = HUE_PLUS_SPEED_FASTEST;
    CoverMarquee.colors_min = 1;
    CoverMarquee.colors_max = 8;
    CoverMarquee.speed      = HUE_PLUS_SPEED_NORMAL;
    CoverMarquee.direction  = MODE_DIRECTION_RIGHT;
    CoverMarquee.color_mode = MODE_COLORS_MODE_SPECIFIC;
    CoverMarquee.colors.resize(2);
    modes.push_back(CoverMarquee);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(CoverMarquee);
    }

    mode Alternating;
    Alternating.name        = "Alternating";
    Alternating.value       = HUE_PLUS_MODE_ALTERNATING;
    Alternating.flags       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Alternating.speed_min   = HUE_PLUS_SPEED_SLOWEST;
    Alternating.speed_max   = HUE_PLUS_SPEED_FASTEST;
    Alternating.colors_min  = 1;
    Alternating.colors_max  = 2;
    Alternating.speed       = HUE_PLUS_SPEED_NORMAL;
    Alternating.direction   = MODE_DIRECTION_RIGHT;
    Alternating.color_mode  = MODE_COLORS_MODE_SPECIFIC;
    Alternating.colors.resize(2);
    modes.push_back(Alternating);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Alternating);
    }

    mode Pulsing;
    Pulsing.name            = "Pulsing";
    Pulsing.value           = HUE_PLUS_MODE_PULSING;
    Pulsing.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Pulsing.speed_min       = HUE_PLUS_SPEED_SLOWEST;
    Pulsing.speed_max       = HUE_PLUS_SPEED_FASTEST;
    Pulsing.colors_min      = 1;
    Pulsing.colors_max      = 8;
    Pulsing.speed           = HUE_PLUS_SPEED_NORMAL;
    Pulsing.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    Pulsing.colors.resize(2);
    modes.push_back(Pulsing);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Pulsing);
    }

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = HUE_PLUS_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min     = HUE_PLUS_SPEED_SLOWEST;
    Breathing.speed_max     = HUE_PLUS_SPEED_FASTEST;
    Breathing.colors_min    = 1;
    Breathing.colors_max    = 8;
    Breathing.speed         = HUE_PLUS_SPEED_NORMAL;
    Breathing.color_mode    = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(2);
    modes.push_back(Breathing);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Breathing);
    }

    mode Alert;
    Alert.name              = "Alert";
    Alert.value             = HUE_PLUS_MODE_ALERT;
    Alert.flags             = 0;
    Alert.color_mode        = MODE_COLORS_NONE;
    modes.push_back(Alert);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Alert);
    }

    mode Candlelight;
    Candlelight.name        = "Candlelight";
    Candlelight.value       = HUE_PLUS_MODE_CANDLELIGHT;
    Candlelight.flags       = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Candlelight.colors_min  = 1;
    Candlelight.colors_max  = 1;
    Candlelight.color_mode  = MODE_COLORS_MODE_SPECIFIC;
    Candlelight.colors.resize(1);
    modes.push_back(Candlelight);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Candlelight);
    }

    mode Wings;
    Wings.name              = "Wings";
    Wings.value             = HUE_PLUS_MODE_WINGS;
    Wings.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Wings.speed_min         = HUE_PLUS_SPEED_SLOWEST;
    Wings.speed_max         = HUE_PLUS_SPEED_FASTEST;
    Wings.colors_min        = 1;
    Wings.colors_max        = 1;
    Wings.speed             = HUE_PLUS_SPEED_NORMAL;
    Wings.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Wings.colors.resize(1);
    modes.push_back(Wings);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Wings);
    }

    mode Wave;
    Wave.name               = "Wave";
    Wave.value              = HUE_PLUS_MODE_WAVE;
    Wave.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Wave.speed_min          = HUE_PLUS_SPEED_SLOWEST;
    Wave.speed_max          = HUE_PLUS_SPEED_FASTEST;
    Wave.speed              = HUE_PLUS_SPEED_NORMAL;
    Wave.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(Wave);
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].modes.push_back(Wave);
    }
}

void RGBController_HuePlus::SetupZones()
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
    zones.resize(HUE_PLUS_NUM_CHANNELS);

    /*-------------------------------------------------*\
    | Set up zones                                      |
    \*-------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < HUE_PLUS_NUM_CHANNELS; zone_idx++)
    {
        zones[zone_idx].name            = "Hue+ Channel ";
        zones[zone_idx].name.append(std::to_string(zone_idx + 1));
        zones[zone_idx].type            = ZONE_TYPE_LINEAR;
        zones[zone_idx].leds_min        = 0;
        zones[zone_idx].leds_max        = 40;

        if(first_run)
        {
            zones[zone_idx].leds_count  = controller->channel_leds[zone_idx];
        }
    }

    /*-------------------------------------------------*\
    | Set up LEDs                                       |
    \*-------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name = "Hue+ Channel ";
            new_led.name.append(std::to_string(zone_idx + 1));
            new_led.name.append(", LED ");
            new_led.name.append(std::to_string(led_idx + 1));
            new_led.value = zone_idx;

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_HuePlus::DeviceResizeZone(int zone, int new_size)
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

void RGBController_HuePlus::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        DeviceUpdateZoneLEDs(zone_idx);
    }
}

void RGBController_HuePlus::DeviceUpdateZoneLEDs(int zone)
{
    if(modes[active_mode].value == HUE_PLUS_MODE_DIRECT)
    {
        controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
    }
}

void RGBController_HuePlus::DeviceUpdateSingleLED(int led)
{
    unsigned int zone_idx = leds[led].value;

    DeviceUpdateZoneLEDs(zone_idx);
}

void RGBController_HuePlus::DeviceUpdateMode()
{
    if(modes[active_mode].value == HUE_PLUS_MODE_DIRECT)
    {
        DeviceUpdateLEDs();
    }
    else
    {
        for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
        {
            DeviceUpdateZoneMode(zone_idx);
        }
    }
}

void RGBController_HuePlus::DeviceUpdateZoneMode(int zone)
{
    if(modes[active_mode].value == HUE_PLUS_MODE_DIRECT)
    {
        return;
    }
    else
    {
        RGBColor*   colors      = NULL;
        bool        direction   = false;
        mode*       mode_ptr    = NULL;

        if((zones[zone].active_mode >= 0) && (zones[zone].active_mode < (int)zones[zone].modes.size()))
        {
            mode_ptr = &zones[zone].modes[zones[zone].active_mode];
        }
        else if(active_mode < (int)modes.size())
        {
            mode_ptr = &modes[active_mode];
        }

        if(mode_ptr != NULL)
        {
            if(mode_ptr->direction == MODE_DIRECTION_LEFT)
            {
                direction = true;
            }

            if(mode_ptr->colors.size() > 0)
            {
                colors = &mode_ptr->colors[0];
            }

            controller->SetChannelEffect
                    (
                    (unsigned char)zone,
                    mode_ptr->value,
                    mode_ptr->speed,
                    direction,
                    colors,
                    (unsigned int)mode_ptr->colors.size()
                    );
        }
    }
}
