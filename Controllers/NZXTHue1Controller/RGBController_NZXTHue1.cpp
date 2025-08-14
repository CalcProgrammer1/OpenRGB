/*---------------------------------------------------------*\
| RGBController_NZXTHue1.cpp                                |
|                                                           |
|   RGBController for NZXT Hue 1 (Smart Device V1)          |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      16 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_NZXTHue1.h"

/**------------------------------------------------------------------*\
    @name NZXT Hue 1
    @category LEDStrip
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectNZXTHue1Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_NZXTHue1::RGBController_NZXTHue1(NZXTHue1Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetName();
    vendor                      = "NZXT";
    type                        = DEVICE_TYPE_LEDSTRIP;
    description                 = "NZXT Hue 1 Device";
    version                     = controller->GetFirmwareVersion();
    location                    = controller->GetLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = HUE_1_MODE_FIXED;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Fading;
    Fading.name                 = "Fading";
    Fading.value                = HUE_1_MODE_FADING;
    Fading.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Fading.speed_min            = HUE_1_SPEED_SLOWEST;
    Fading.speed_max            = HUE_1_SPEED_FASTEST;
    Fading.colors_min           = 1;
    Fading.colors_max           = 8;
    Fading.speed                = HUE_1_SPEED_NORMAL;
    Fading.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Fading.colors.resize(2);
    modes.push_back(Fading);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Rainbow Wave";
    SpectrumCycle.value         = HUE_1_MODE_SPECTRUM;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    SpectrumCycle.speed_min     = HUE_1_SPEED_SLOWEST;
    SpectrumCycle.speed_max     = HUE_1_SPEED_FASTEST;
    SpectrumCycle.speed         = HUE_1_SPEED_NORMAL;
    SpectrumCycle.direction     = MODE_DIRECTION_RIGHT;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Marquee;
    Marquee.name                = "Marquee";
    Marquee.value               = HUE_1_MODE_MARQUEE;
    Marquee.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Marquee.speed_min           = HUE_1_SPEED_SLOWEST;
    Marquee.speed_max           = HUE_1_SPEED_FASTEST;
    Marquee.colors_min          = 1;
    Marquee.colors_max          = 1;
    Marquee.speed               = HUE_1_SPEED_NORMAL;
    Marquee.direction           = MODE_DIRECTION_RIGHT;
    Marquee.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Marquee.colors.resize(1);
    modes.push_back(Marquee);

    mode CoverMarquee;
    CoverMarquee.name           = "Cover Marquee";
    CoverMarquee.value          = HUE_1_MODE_COVER_MARQUEE;
    CoverMarquee.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    CoverMarquee.speed_min      = HUE_1_SPEED_SLOWEST;
    CoverMarquee.speed_max      = HUE_1_SPEED_FASTEST;
    CoverMarquee.colors_min     = 1;
    CoverMarquee.colors_max     = 8;
    CoverMarquee.speed          = HUE_1_SPEED_NORMAL;
    CoverMarquee.direction      = MODE_DIRECTION_RIGHT;
    CoverMarquee.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    CoverMarquee.colors.resize(2);
    modes.push_back(CoverMarquee);

    mode Alternating;
    Alternating.name            = "Alternating";
    Alternating.value           = HUE_1_MODE_ALTERNATING;
    Alternating.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Alternating.speed_min       = HUE_1_SPEED_SLOWEST;
    Alternating.speed_max       = HUE_1_SPEED_FASTEST;
    Alternating.colors_min      = 1;
    Alternating.colors_max      = 2;
    Alternating.speed           = HUE_1_SPEED_NORMAL;
    Alternating.direction       = MODE_DIRECTION_RIGHT;
    Alternating.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    Alternating.colors.resize(2);
    modes.push_back(Alternating);

    mode Pulsing;
    Pulsing.name                = "Pulsing";
    Pulsing.value               = HUE_1_MODE_PULSING;
    Pulsing.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Pulsing.speed_min           = HUE_1_SPEED_SLOWEST;
    Pulsing.speed_max           = HUE_1_SPEED_FASTEST;
    Pulsing.colors_min          = 1;
    Pulsing.colors_max          = 8;
    Pulsing.speed               = HUE_1_SPEED_NORMAL;
    Pulsing.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Pulsing.colors.resize(2);
    modes.push_back(Pulsing);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = HUE_1_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min         = HUE_1_SPEED_SLOWEST;
    Breathing.speed_max         = HUE_1_SPEED_FASTEST;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 8;
    Breathing.speed             = HUE_1_SPEED_NORMAL;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(2);
    modes.push_back(Breathing);

    mode Alert;
    Alert.name                  = "Alert";
    Alert.value                 = HUE_1_MODE_ALERT;
    Alert.flags                 = 0;
    Alert.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Alert);

    mode Candlelight;
    Candlelight.name            = "Candlelight";
    Candlelight.value           = HUE_1_MODE_CANDLELIGHT;
    Candlelight.flags           = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Candlelight.colors_min      = 1;
    Candlelight.colors_max      = 1;
    Candlelight.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    Candlelight.colors.resize(1);
    modes.push_back(Candlelight);

    mode Wings;
    Wings.name                  = "Wings";
    Wings.value                 = HUE_1_MODE_WINGS;
    Wings.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Wings.speed_min             = HUE_1_SPEED_SLOWEST;
    Wings.speed_max             = HUE_1_SPEED_FASTEST;
    Wings.colors_min            = 1;
    Wings.colors_max            = 1;
    Wings.speed                 = HUE_1_SPEED_NORMAL;
    Wings.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Wings.colors.resize(1);
    modes.push_back(Wings);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = HUE_1_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Wave.speed_min              = HUE_1_SPEED_SLOWEST;
    Wave.speed_max              = HUE_1_SPEED_FASTEST;
    Wave.speed                  = HUE_1_SPEED_NORMAL;
    Wave.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Wave);

    SetupZones();
}

RGBController_NZXTHue1::~RGBController_NZXTHue1()
{
    delete controller;
}

void RGBController_NZXTHue1::SetupZones()
{
    /*-------------------------------------------------*\
    | Set up zone                                       |
    \*-------------------------------------------------*/
    zone* new_zone = new zone;

    new_zone->name          = "Hue 1 Channel";
    new_zone->type          = ZONE_TYPE_LINEAR;
    new_zone->leds_min      = 0;
    new_zone->leds_max      = 40;
    new_zone->leds_count    = controller->num_leds;
    new_zone->matrix_map    = NULL;

    zones.push_back(*new_zone);

    /*-------------------------------------------------*\
    | Set up LEDs                                       |
    \*-------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name = "Hue 1 Channel";
        new_led.name.append(", LED ");
        new_led.name.append(std::to_string(led_idx + 1));

        leds.push_back(new_led);
    }

    /*-------------------------------------------------*\
    | Set up Segments                                   |
    \*-------------------------------------------------*/
    unsigned int num_segments = 0;
    unsigned int segment_size = 0;
    std::string  segment_name = "";

    switch(controller->GetAccessoryType())
    {
        case HUE_1_ACCESSORY_STRIP:
            segment_size = 10;
            num_segments = zones[0].leds_count / segment_size;
            segment_name = "Hue+ Strip";
            break;

        case HUE_1_ACCESSORY_FAN:
            segment_size = 8;
            num_segments = zones[0].leds_count / segment_size;
            segment_name = "Aer RGB Fan";
            break;
    }

    if(segment_name != "")
    {
        for(unsigned int segment_idx = 0; segment_idx < num_segments; segment_idx++)
        {
            segment new_segment;
            new_segment.name = segment_name;
            new_segment.type = ZONE_TYPE_LINEAR;
            new_segment.start_idx = segment_idx * segment_size;
            new_segment.leds_count = segment_size;

            zones[0].segments.push_back(new_segment);
        }
    }

    SetupColors();
}

void RGBController_NZXTHue1::ResizeZone(int /*zone*/, int /*new_size*/)
{
}

void RGBController_NZXTHue1::DeviceUpdateLEDs()
{
    controller->SetLEDs(zones[0].colors, zones[0].leds_count);
}

void RGBController_NZXTHue1::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_NZXTHue1::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_NZXTHue1::DeviceUpdateMode()
{
    if(modes[active_mode].value == HUE_1_MODE_FIXED)
    {
        DeviceUpdateLEDs();
    }
    else
    {
        RGBColor*   colors      = NULL;
        bool        direction   = false;

        if(modes[active_mode].direction == MODE_DIRECTION_LEFT)
        {
            direction = true;
        }

        if(modes[active_mode].colors.size() > 0)
        {
            colors = &modes[active_mode].colors[0];
        }

        controller->SetEffect
                (
                modes[active_mode].value,
                modes[active_mode].speed,
                direction,
                colors,
                (unsigned int)modes[active_mode].colors.size()
                );
    }
}
