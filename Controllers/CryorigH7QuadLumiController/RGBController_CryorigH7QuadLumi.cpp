/*---------------------------------------------------------*\
| RGBController_CryorigH7QuadLumi.cpp                       |
|                                                           |
|   RGBController for Cryorig H7 Quad Lumi                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_CryorigH7QuadLumi.h"

/**------------------------------------------------------------------*\
    @name Cryorig H7 Quad Lumi
    @category Cooler
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCryorigH7QuadLumi
    @comment
\*-------------------------------------------------------------------*/

RGBController_CryorigH7QuadLumi::RGBController_CryorigH7QuadLumi(CryorigH7QuadLumiController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetName();
    vendor      = "CRYORIG";
    type        = DEVICE_TYPE_COOLER;
    description = "CRYORIG H7 Quad Lumi Device";
    version     = controller->GetFirmwareVersion();
    location    = controller->GetLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = CRYORIG_H7_QUAD_LUMI_MODE_FIXED;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Fading;
    Fading.name       = "Fading";
    Fading.value      = CRYORIG_H7_QUAD_LUMI_MODE_FADING;
    Fading.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Fading.speed_min  = CRYORIG_H7_QUAD_LUMI_SPEED_SLOWEST;
    Fading.speed_max  = CRYORIG_H7_QUAD_LUMI_SPEED_FASTEST;
    Fading.colors_min = 1;
    Fading.colors_max = 8;
    Fading.speed      = CRYORIG_H7_QUAD_LUMI_SPEED_NORMAL;
    Fading.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Fading.colors.resize(2);
    modes.push_back(Fading);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Rainbow Wave";
    SpectrumCycle.value      = CRYORIG_H7_QUAD_LUMI_MODE_SPECTRUM;
    SpectrumCycle.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    SpectrumCycle.speed_min  = CRYORIG_H7_QUAD_LUMI_SPEED_SLOWEST;
    SpectrumCycle.speed_max  = CRYORIG_H7_QUAD_LUMI_SPEED_FASTEST;
    SpectrumCycle.speed      = CRYORIG_H7_QUAD_LUMI_SPEED_NORMAL;
    SpectrumCycle.direction  = MODE_DIRECTION_RIGHT;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Marquee;
    Marquee.name             = "Marquee";
    Marquee.value            = CRYORIG_H7_QUAD_LUMI_MODE_MARQUEE;
    Marquee.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Marquee.speed_min        = CRYORIG_H7_QUAD_LUMI_SPEED_SLOWEST;
    Marquee.speed_max        = CRYORIG_H7_QUAD_LUMI_SPEED_FASTEST;
    Marquee.colors_min       = 1;
    Marquee.colors_max       = 1;
    Marquee.speed            = CRYORIG_H7_QUAD_LUMI_SPEED_NORMAL;
    Marquee.direction        = MODE_DIRECTION_RIGHT;
    Marquee.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Marquee.colors.resize(1);
    modes.push_back(Marquee);

    mode CoverMarquee;
    CoverMarquee.name        = "Cover Marquee";
    CoverMarquee.value       = CRYORIG_H7_QUAD_LUMI_MODE_COVER_MARQUEE;
    CoverMarquee.flags       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    CoverMarquee.speed_min   = CRYORIG_H7_QUAD_LUMI_SPEED_SLOWEST;
    CoverMarquee.speed_max   = CRYORIG_H7_QUAD_LUMI_SPEED_FASTEST;
    CoverMarquee.colors_min  = 1;
    CoverMarquee.colors_max  = 8;
    CoverMarquee.speed       = CRYORIG_H7_QUAD_LUMI_SPEED_NORMAL;
    CoverMarquee.direction   = MODE_DIRECTION_RIGHT;
    CoverMarquee.color_mode  = MODE_COLORS_MODE_SPECIFIC;
    CoverMarquee.colors.resize(2);
    modes.push_back(CoverMarquee);

    mode Alternating;
    Alternating.name         = "Alternating";
    Alternating.value        = CRYORIG_H7_QUAD_LUMI_MODE_ALTERNATING;
    Alternating.flags        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Alternating.speed_min    = CRYORIG_H7_QUAD_LUMI_SPEED_SLOWEST;
    Alternating.speed_max    = CRYORIG_H7_QUAD_LUMI_SPEED_FASTEST;
    Alternating.colors_min   = 1;
    Alternating.colors_max   = 2;
    Alternating.speed        = CRYORIG_H7_QUAD_LUMI_SPEED_NORMAL;
    Alternating.direction    = MODE_DIRECTION_RIGHT;
    Alternating.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    Alternating.colors.resize(2);
    modes.push_back(Alternating);

    mode Pulsing;
    Pulsing.name             = "Pulsing";
    Pulsing.value            = CRYORIG_H7_QUAD_LUMI_MODE_PULSING;
    Pulsing.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Pulsing.speed_min        = CRYORIG_H7_QUAD_LUMI_SPEED_SLOWEST;
    Pulsing.speed_max        = CRYORIG_H7_QUAD_LUMI_SPEED_FASTEST;
    Pulsing.colors_min       = 1;
    Pulsing.colors_max       = 8;
    Pulsing.speed            = CRYORIG_H7_QUAD_LUMI_SPEED_NORMAL;
    Pulsing.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Pulsing.colors.resize(2) ;
    modes.push_back(Pulsing);

    mode Breathing;
    Breathing.name           = "Breathing";
    Breathing.value          = CRYORIG_H7_QUAD_LUMI_MODE_BREATHING;
    Breathing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min      = CRYORIG_H7_QUAD_LUMI_SPEED_SLOWEST;
    Breathing.speed_max      = CRYORIG_H7_QUAD_LUMI_SPEED_FASTEST;
    Breathing.colors_min     = 1;
    Breathing.colors_max     = 8;
    Breathing.speed          = CRYORIG_H7_QUAD_LUMI_SPEED_NORMAL;
    Breathing.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(2);
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_CryorigH7QuadLumi::~RGBController_CryorigH7QuadLumi()
{
    delete controller;
}

void RGBController_CryorigH7QuadLumi::SetupZones()
{
    const char* zone_names[] = { "Logo", "Underglow" };

    /*-------------------------------------------------*\
    | Set up zones                                      |
    \*-------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < 2; zone_idx++)
    {
        zone* new_zone = new zone;

        new_zone->name          = zone_names[zone_idx];
        new_zone->type          = ZONE_TYPE_LINEAR;
        new_zone->leds_min      = 5;
        new_zone->leds_max      = 5;
        new_zone->leds_count    = 5;
        new_zone->matrix_map    = NULL;

        zones.push_back(*new_zone);
    }

    /*-------------------------------------------------*\
    | Set up LEDs                                       |
    \*-------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name = zone_names[zone_idx];
            new_led.name.append(", LED ");
            new_led.name.append(std::to_string(led_idx + 1));
            new_led.value = zone_idx;

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_CryorigH7QuadLumi::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_CryorigH7QuadLumi::DeviceUpdateLEDs()
{
    for(unsigned char zone_idx = 0; zone_idx < (unsigned char)zones.size(); zone_idx++)
    {
        controller->SetChannelLEDs(zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}

void RGBController_CryorigH7QuadLumi::UpdateZoneLEDs(int zone)
{
    controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
}

void RGBController_CryorigH7QuadLumi::UpdateSingleLED(int led)
{
    unsigned int zone_idx = leds[led].value;

    controller->SetChannelLEDs(zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
}

void RGBController_CryorigH7QuadLumi::DeviceUpdateMode()
{
    if(modes[active_mode].value == CRYORIG_H7_QUAD_LUMI_MODE_FIXED)
    {
        DeviceUpdateLEDs();
    }
    else
    {
        for(unsigned char zone_idx = 0; zone_idx < (unsigned char)zones.size(); zone_idx++)
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

            controller->SetChannelEffect
                    (
                    zone_idx,
                    modes[active_mode].value,
                    modes[active_mode].speed,
                    direction,
                    colors,
                    (unsigned int)modes[active_mode].colors.size()
                    );
        }
    }
}
