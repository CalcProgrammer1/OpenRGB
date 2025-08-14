/*---------------------------------------------------------*\
| RGBController_NZXTKraken.cpp                              |
|                                                           |
|   RGBController for NZXT Kraken                           |
|                                                           |
|   Martin Hartl (inlart)                       04 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <iostream>
#include "RGBController_NZXTKraken.h"

/**------------------------------------------------------------------*\
    @name NZXT Kraken
    @category Cooler
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectNZXTKrakenControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_NZXTKraken::RGBController_NZXTKraken(NZXTKrakenController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetName();
    vendor                      = "NZXT";
    type                        = DEVICE_TYPE_COOLER;
    description                 = "NZXT Kraken X42/X52/X62/X72/M22";
    version                     = controller->GetFirmwareVersion();
    location                    = controller->GetLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = NZXT_KRAKEN_MODE_FIXED;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Fading;
    Fading.name                 = "Fading";
    Fading.value                = NZXT_KRAKEN_MODE_FADING;
    Fading.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Fading.speed_min            = NZXT_KRAKEN_SPEED_SLOWEST;
    Fading.speed_max            = NZXT_KRAKEN_SPEED_FASTEST;
    Fading.colors_min           = 2;
    Fading.colors_max           = 8;
    Fading.speed                = NZXT_KRAKEN_SPEED_NORMAL;
    Fading.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Fading.colors.resize(2);
    modes.push_back(Fading);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = NZXT_KRAKEN_MODE_SPECTRUM;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    SpectrumCycle.speed_min     = NZXT_KRAKEN_SPEED_SLOWEST;
    SpectrumCycle.speed_max     = NZXT_KRAKEN_SPEED_FASTEST;
    SpectrumCycle.speed         = NZXT_KRAKEN_SPEED_NORMAL;
    SpectrumCycle.direction     = MODE_DIRECTION_RIGHT;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Marquee;
    Marquee.name                = "Marquee";
    Marquee.value               = NZXT_KRAKEN_MODE_MARQUEE;
    Marquee.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Marquee.speed_min           = NZXT_KRAKEN_SPEED_SLOWEST;
    Marquee.speed_max           = NZXT_KRAKEN_SPEED_FASTEST;
    Marquee.colors_min          = 1;
    Marquee.colors_max          = 1;
    Marquee.speed               = NZXT_KRAKEN_SPEED_NORMAL;
    Marquee.direction           = MODE_DIRECTION_RIGHT;
    Marquee.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Marquee.colors.resize(1);
    modes.push_back(Marquee);

    mode CoverMarquee;
    CoverMarquee.name           = "Cover Marquee";
    CoverMarquee.value          = NZXT_KRAKEN_MODE_COVER_MARQUEE;
    CoverMarquee.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    CoverMarquee.speed_min      = NZXT_KRAKEN_SPEED_SLOWEST;
    CoverMarquee.speed_max      = NZXT_KRAKEN_SPEED_FASTEST;
    CoverMarquee.colors_min     = 1;
    CoverMarquee.colors_max     = 8;
    CoverMarquee.speed          = NZXT_KRAKEN_SPEED_NORMAL;
    CoverMarquee.direction      = MODE_DIRECTION_RIGHT;
    CoverMarquee.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    CoverMarquee.colors.resize(2);
    modes.push_back(CoverMarquee);

    mode Alternating;
    Alternating.name            = "Alternating";
    Alternating.value           = NZXT_KRAKEN_MODE_ALTERNATING;
    Alternating.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Alternating.speed_min       = NZXT_KRAKEN_SPEED_SLOWEST;
    Alternating.speed_max       = NZXT_KRAKEN_SPEED_FASTEST;
    Alternating.colors_min      = 2;
    Alternating.colors_max      = 2;
    Alternating.speed           = NZXT_KRAKEN_SPEED_NORMAL;
    Alternating.direction       = MODE_DIRECTION_RIGHT;
    Alternating.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    Alternating.colors.resize(2);
    modes.push_back(Alternating);

    mode Pulse;
    Pulse.name                  = "Pulse";
    Pulse.value                 = NZXT_KRAKEN_MODE_PULSE;
    Pulse.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Pulse.speed_min             = NZXT_KRAKEN_SPEED_SLOWEST;
    Pulse.speed_max             = NZXT_KRAKEN_SPEED_FASTEST;
    Pulse.colors_min            = 1;
    Pulse.colors_max            = 8;
    Pulse.speed                 = NZXT_KRAKEN_SPEED_NORMAL;
    Pulse.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Pulse.colors.resize(1);
    modes.push_back(Pulse);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = NZXT_KRAKEN_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min         = NZXT_KRAKEN_SPEED_SLOWEST;
    Breathing.speed_max         = NZXT_KRAKEN_SPEED_FASTEST;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 8;
    Breathing.speed             = NZXT_KRAKEN_SPEED_NORMAL;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode ThaiChi;
    ThaiChi.name                = "Thai Chi";
    ThaiChi.value               = NZXT_KRAKEN_MODE_TAI_CHI;
    ThaiChi.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    ThaiChi.speed_min           = NZXT_KRAKEN_SPEED_SLOWEST;
    ThaiChi.speed_max           = NZXT_KRAKEN_SPEED_FASTEST;
    ThaiChi.speed               = NZXT_KRAKEN_SPEED_NORMAL;
    ThaiChi.colors_min          = 2;
    ThaiChi.colors_max          = 2;
    ThaiChi.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    ThaiChi.colors.resize(2);
    modes.push_back(ThaiChi);

    mode WaterCooler;
    WaterCooler.name            = "Water Cooler";
    WaterCooler.value           = NZXT_KRAKEN_MODE_WATER_COOLER;
    WaterCooler.flags           = MODE_FLAG_HAS_SPEED;
    WaterCooler.speed_min       = NZXT_KRAKEN_SPEED_SLOWEST;
    WaterCooler.speed_max       = NZXT_KRAKEN_SPEED_FASTEST;
    WaterCooler.speed           = NZXT_KRAKEN_SPEED_NORMAL;
    WaterCooler.color_mode      = MODE_COLORS_NONE;
    modes.push_back(WaterCooler);

    mode Loading;
    Loading.name                = "Loading";
    Loading.value               = NZXT_KRAKEN_MODE_LOADING;
    Loading.flags               = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Loading.colors_min          = 1;
    Loading.colors_max          = 1;
    Loading.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Loading.colors.resize(1);
    modes.push_back(Loading);

    mode Wings;
    Wings.name                  = "Wings";
    Wings.value                 = NZXT_KRAKEN_MODE_WINGS;
    Wings.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Wings.speed_min             = NZXT_KRAKEN_SPEED_SLOWEST;
    Wings.speed_max             = NZXT_KRAKEN_SPEED_FASTEST;
    Wings.speed                 = NZXT_KRAKEN_SPEED_NORMAL;
    Wings.colors_min            = 1;
    Wings.colors_max            = 1;
    Wings.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Wings.colors.resize(1);
    modes.push_back(Wings);

    /*---------------------------------------------------------*\
    | Fixed is the default mode                                 |
    \*---------------------------------------------------------*/
    default_mode = 0;

    /*---------------------------------------------------------*\
    | Modes supported by the LOGO LED                           |
    \*---------------------------------------------------------*/
    logo_modes =
    {
        NZXT_KRAKEN_MODE_FIXED,
        NZXT_KRAKEN_MODE_FADING,
        NZXT_KRAKEN_MODE_SPECTRUM,
        NZXT_KRAKEN_MODE_BREATHING,
        NZXT_KRAKEN_MODE_PULSE
    };

    SetupZones();
}

RGBController_NZXTKraken::~RGBController_NZXTKraken()
{
    delete controller;
}

void RGBController_NZXTKraken::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone logo_zone;
    logo_zone.name          = "Logo";
    logo_zone.type          = ZONE_TYPE_SINGLE;
    logo_zone.leds_min      = 1;
    logo_zone.leds_max      = 1;
    logo_zone.leds_count    = 1;
    logo_zone.matrix_map    = NULL;
    zones.push_back(logo_zone);

    zone ring_zone;
    ring_zone.name          = "Ring";
    ring_zone.type          = ZONE_TYPE_LINEAR;
    ring_zone.leds_min      = 8;
    ring_zone.leds_max      = 8;
    ring_zone.leds_count    = 8;
    ring_zone.matrix_map    = NULL;
    zones.push_back(ring_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    led logo_led;
    logo_led.name = "Logo LED";
    leds.push_back(logo_led);

    led ring_led;
    for(int i = 1; i < 9; i++)
    {
        ring_led.name = std::string("Ring LED ") + std::to_string(i);
        leds.push_back(ring_led);
    }

    SetupColors();
}

void RGBController_NZXTKraken::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

std::vector<std::vector<RGBColor>> RGBController_NZXTKraken::GetColors(int zone, const mode& channel_mode)
{
    std::vector<std::vector<RGBColor>> result;
    int length = zone < 0 ? (int)leds.size() : (int)zones[zone].leds_count;

    if(channel_mode.color_mode == MODE_COLORS_NONE)
    {
        result.push_back(std::vector<RGBColor>());
    }
    else if(channel_mode.color_mode == MODE_COLORS_PER_LED)
    {
        if(zone < 0)
        {
            result.push_back(colors);
        }
        else
        {
            std::vector<RGBColor> led_colors;
            for(std::size_t idx = 0; idx < zones[zone].leds_count; ++idx)
            {
                led_colors.push_back(zones[zone].colors[idx]);
            }
            result.push_back(led_colors);
        }
    }
    else if(channel_mode.color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        for(std::size_t idx = 0; idx < channel_mode.colors.size(); ++idx)
        {
            result.push_back(std::vector<RGBColor>(length, channel_mode.colors[idx]));
        }
    }

    return result;
}

void RGBController_NZXTKraken::UpdateChannel(NZXTKrakenChannel_t channel, int zone, const mode& channel_mode)
{
    bool direction = false;

    if((channel_mode.flags & MODE_FLAG_HAS_DIRECTION_LR)
     &&(channel_mode.direction == MODE_DIRECTION_LEFT  ))
    {
        direction = true;
    }

    unsigned char speed = NZXT_KRAKEN_SPEED_NORMAL;
    if(channel_mode.flags & MODE_FLAG_HAS_SPEED)
    {
        speed = channel_mode.speed;
    }

    std::vector<std::vector<RGBColor>> update_colors = GetColors(zone, channel_mode);
    for(std::size_t idx = 0; idx < update_colors.size(); ++idx)
    {
        controller->UpdateEffect(
            channel,
            channel_mode.value,
            direction,
            speed,
            (int)idx,
            update_colors[idx]
        );
    }
}

void RGBController_NZXTKraken::DeviceUpdateLEDs()
{
    if(logo_modes.find(modes[active_mode].value) == logo_modes.end())
    {
        UpdateChannel(NZXT_KRAKEN_CHANNEL_LOGO, 0, modes[default_mode]);
        UpdateChannel(NZXT_KRAKEN_CHANNEL_RING, 1, modes[active_mode]);
    }
    else
    {
        UpdateChannel(NZXT_KRAKEN_CHANNEL_SYNC, -1, modes[active_mode]);
    }
}

void RGBController_NZXTKraken::UpdateZoneLEDs(int zone)
{
    NZXTKrakenChannel_t channel;
    mode channel_mode = modes[active_mode];
    if(zone == 0)
    {
        channel = NZXT_KRAKEN_CHANNEL_LOGO;
        if(logo_modes.find(modes[active_mode].value) == logo_modes.end())
        {
            channel_mode = modes[default_mode];
        }
    }
    else
    {
        channel = NZXT_KRAKEN_CHANNEL_RING;
    }
    UpdateChannel(channel, zone, channel_mode);
}

void RGBController_NZXTKraken::UpdateSingleLED(int led)
{
    int zone = (led > 0) ? 1 : 0;
    UpdateZoneLEDs(zone);
}

void RGBController_NZXTKraken::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
