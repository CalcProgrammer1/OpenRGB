/*---------------------------------------------------------*\
| RGBController_NZXTHue2.cpp                                |
|                                                           |
|   RGBController for NZXT Hue 2                            |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      29 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_NZXTHue2.h"

/**------------------------------------------------------------------*\
    @name NZXT Hue2
    @category LEDStrip
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectNZXTFanController,DetectNZXTFanController6Channel,DetectNZXTHue2,DetectNZXTHue2Ambient,DetectNZXTHue2Motherboard,DetectNZXTKrakenX3,DetectNZXTRGBController,DetectNZXTSmartDeviceV2,DetectNZXTKrakenElite
    @comment
\*-------------------------------------------------------------------*/

RGBController_NZXTHue2::RGBController_NZXTHue2(NZXTHue2Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetName();
    vendor                      = "NZXT";
    type                        = DEVICE_TYPE_LEDSTRIP;
    description                 = "NZXT Hue 2 Device";
    version                     = controller->GetFirmwareVersion();
    location                    = controller->GetLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 0xFFFF;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = HUE_2_MODE_FIXED;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Fading;
    Fading.name                 = "Fading";
    Fading.value                = HUE_2_MODE_FADING;
    Fading.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Fading.speed_min            = HUE_2_SPEED_SLOWEST;
    Fading.speed_max            = HUE_2_SPEED_FASTEST;
    Fading.colors_min           = 1;
    Fading.colors_max           = 8;
    Fading.speed                = HUE_2_SPEED_NORMAL;
    Fading.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Fading.colors.resize(1);
    modes.push_back(Fading);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = HUE_2_MODE_SPECTRUM;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    SpectrumCycle.speed_min     = HUE_2_SPEED_SLOWEST;
    SpectrumCycle.speed_max     = HUE_2_SPEED_FASTEST;
    SpectrumCycle.speed         = HUE_2_SPEED_NORMAL;
    SpectrumCycle.direction     = MODE_DIRECTION_RIGHT;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Marquee;
    Marquee.name                = "Marquee";
    Marquee.value               = HUE_2_MODE_MARQUEE;
    Marquee.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Marquee.speed_min           = HUE_2_SPEED_SLOWEST;
    Marquee.speed_max           = HUE_2_SPEED_FASTEST;
    Marquee.colors_min          = 1;
    Marquee.colors_max          = 1;
    Marquee.speed               = HUE_2_SPEED_NORMAL;
    Marquee.direction           = MODE_DIRECTION_RIGHT;
    Marquee.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Marquee.colors.resize(1);
    modes.push_back(Marquee);

    mode CoverMarquee;
    CoverMarquee.name           = "Cover Marquee";
    CoverMarquee.value          = HUE_2_MODE_COVER_MARQUEE;
    CoverMarquee.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    CoverMarquee.speed_min      = HUE_2_SPEED_SLOWEST;
    CoverMarquee.speed_max      = HUE_2_SPEED_FASTEST;
    CoverMarquee.colors_min     = 1;
    CoverMarquee.colors_max     = 8;
    CoverMarquee.speed          = HUE_2_SPEED_NORMAL;
    CoverMarquee.direction      = MODE_DIRECTION_RIGHT;
    CoverMarquee.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    CoverMarquee.colors.resize(1);
    modes.push_back(CoverMarquee);

    mode Alternating;
    Alternating.name            = "Alternating";
    Alternating.value           = HUE_2_MODE_ALTERNATING;
    Alternating.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Alternating.speed_min       = HUE_2_SPEED_SLOWEST;
    Alternating.speed_max       = HUE_2_SPEED_FASTEST;
    Alternating.colors_min      = 1;
    Alternating.colors_max      = 2;
    Alternating.speed           = HUE_2_SPEED_NORMAL;
    Alternating.direction       = MODE_DIRECTION_RIGHT;
    Alternating.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    Alternating.colors.resize(1);
    modes.push_back(Alternating);

    mode Pulsing;
    Pulsing.name                = "Pulsing";
    Pulsing.value               = HUE_2_MODE_PULSING;
    Pulsing.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Pulsing.speed_min           = HUE_2_SPEED_SLOWEST;
    Pulsing.speed_max           = HUE_2_SPEED_FASTEST;
    Pulsing.colors_min          = 1;
    Pulsing.colors_max          = 8;
    Pulsing.speed               = HUE_2_SPEED_NORMAL;
    Pulsing.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Pulsing.colors.resize(1) ;
    modes.push_back(Pulsing);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = HUE_2_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min         = HUE_2_SPEED_SLOWEST;
    Breathing.speed_max         = HUE_2_SPEED_FASTEST;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 8;
    Breathing.speed             = HUE_2_SPEED_NORMAL;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize( 1);
    modes.push_back(Breathing);

    mode Candle;
    Candle.name                 = "Candle";
    Candle.value                = HUE_2_MODE_CANDLE;
    Candle.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Candle.speed_min            = HUE_2_SPEED_SLOWEST;
    Candle.speed_max            = HUE_2_SPEED_FASTEST;
    Candle.colors_min           = 1;
    Candle.colors_max           = 8;
    Candle.speed                = HUE_2_SPEED_NORMAL;
    Candle.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Candle.colors.resize(1) ;
    modes.push_back(Candle);

    mode StarryNight;
    StarryNight.name            = "Starry Night";
    StarryNight.value           = HUE_2_MODE_STARRY_NIGHT;
    StarryNight.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    StarryNight.speed_min       = HUE_2_SPEED_SLOWEST;
    StarryNight.speed_max       = HUE_2_SPEED_FASTEST;
    StarryNight.colors_min      = 1;
    StarryNight.colors_max      = 1;
    StarryNight.speed           = HUE_2_SPEED_NORMAL;
    StarryNight.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    StarryNight.colors.resize(1);
    modes.push_back(StarryNight);

    mode SuperRainbow;
    SuperRainbow.name           = "Super Rainbow";
    SuperRainbow.value          = HUE_2_MODE_SUPER_RAINBOW;
    SuperRainbow.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    SuperRainbow.speed_min      = HUE_2_SPEED_SLOWEST;
    SuperRainbow.speed_max      = HUE_2_SPEED_FASTEST;
    SuperRainbow.speed          = HUE_2_SPEED_NORMAL;
    SuperRainbow.direction      = MODE_DIRECTION_RIGHT;
    SuperRainbow.color_mode     = MODE_COLORS_NONE;
    modes.push_back(SuperRainbow);

    mode RainbowPulse;
    RainbowPulse.name           = "Rainbow Pulse";
    RainbowPulse.value          = HUE_2_MODE_RAINBOW_PULSE;
    RainbowPulse.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowPulse.speed_min      = HUE_2_SPEED_SLOWEST;
    RainbowPulse.speed_max      = HUE_2_SPEED_FASTEST;
    RainbowPulse.speed          = HUE_2_SPEED_NORMAL;
    RainbowPulse.direction      = MODE_DIRECTION_RIGHT;
    RainbowPulse.color_mode     = MODE_COLORS_NONE;
    modes.push_back(RainbowPulse);

    mode RainbowFlow;
    RainbowFlow.name            = "Rainbow Flow";
    RainbowFlow.value           = HUE_2_MODE_RAINBOW_FLOW;
    RainbowFlow.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowFlow.speed_min       = HUE_2_SPEED_SLOWEST;
    RainbowFlow.speed_max       = HUE_2_SPEED_FASTEST;
    RainbowFlow.speed           = HUE_2_SPEED_NORMAL;
    RainbowFlow.direction       = MODE_DIRECTION_RIGHT;
    RainbowFlow.color_mode      = MODE_COLORS_NONE;
    modes.push_back(RainbowFlow);

    SetupZones();
}

RGBController_NZXTHue2::~RGBController_NZXTHue2()
{
    delete controller;
}

void RGBController_NZXTHue2::SetupZones()
{
    /*-------------------------------------------------*\
    | Set up zones                                      |
    \*-------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < controller->GetNumRGBChannels(); zone_idx++)
    {
        zone* new_zone = new zone;

        new_zone->name          = "Hue 2 Channel ";
        new_zone->name.append(std::to_string(zone_idx + 1));
        new_zone->type          = ZONE_TYPE_LINEAR;
        new_zone->leds_min      = 0;
        new_zone->leds_max      = 40;
        new_zone->leds_count    = controller->channel_leds[zone_idx];
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
            new_led.name = "Hue 2 Channel ";
            new_led.name.append(std::to_string(zone_idx + 1));
            new_led.name.append(", LED ");
            new_led.name.append(std::to_string(led_idx + 1));
            new_led.value = zone_idx;

            leds.push_back(new_led);
        }
    }

    /*-------------------------------------------------*\
    | Set up segments                                   |
    \*-------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        unsigned int start_idx = 0;

        for(unsigned int dev_idx = 0; dev_idx < 6; dev_idx++)
        {
            std::string device_name = "";
            switch(controller->channel_dev_ids[zone_idx][dev_idx])
            {
            case 0x01: //Hue 1 strip
                device_name = "Hue 1 strip";
                break;

            case 0x02: //Aer 1 fan
                device_name = "Aer 1 fan";
                break;

            case 0x04: //Hue 2 strip (10 LEDs)
                device_name = "Hue 2 strip (10 LEDs)";
                break;

            case 0x05: //Hue 2 strip (8 LEDs)
                device_name = "Hue 2 strip (8 LEDs)";
                break;

            case 0x06: //Hue 2 strip (6 LEDs)
                device_name = "Hue 2 strip (6 LEDs)";
                break;

            case 0x08: //Hue 2 Cable Comb (14 LEDs)
                device_name = "Hue 2 Cable Comb (14 LEDs)";
                break;

            case 0x09: //Hue 2 Underglow (300mm) (15 LEDs)
                device_name = "Hue 2 Underglow (300mm) (15 LEDs)";
                break;

            case 0x0A: //Hue 2 Underglow (200mm) (10 LEDs)
                device_name = "Hue 2 Underglow (200mm) (10 LEDs)";
                break;

            case 0x0B: //Aer 2 fan (120mm)
                device_name = "Aer 2 fan (120mm)";
                break;

            case 0x0C: //Aer 2 fan (140mm)
                device_name = "Aer 2 fan (140mm)";
                break;

            case 0x10: //Kraken X3 ring
                device_name = "Kraken X3 ring";
                break;

            case 0x11: //Kraken X3 logo
                device_name = "Kraken X3 logo";
                break;

            case 0x13: //F120 RGB fan (120mm)
                device_name = "F120 fan (120mm)";
                break;

            case 0x14: //F140 RGB fan (140mm)
                device_name = "F140 fan (140mm)";
                break;

            case 0x15: //F120 RGB Duo fan (120mm)
                device_name = "F120 Duo fan (120mm)";
                break;

            case 0x16: //F140 RGB Duo fan (140mm)
                device_name = "F140 Duo fan (140mm)";
                break;

            case 0x17: //F120 RGB Core fan (120mm)
                device_name = "F120 Core fan (120mm)";
                break;

            case 0x18: //F140 RGB Core fan (140mm)
                device_name = "F140 Core fan (140mm)";
                break;

            case 0x19: //F120 RGB Core fan case version (120mm)
                device_name = "F120 Core fan case version (120mm)";
                break;

            case 0x1D: //F360 Core fan case version (360mm)
                device_name = "F360 Core fan case version (360mm)";
                break;

            case 0x1E: //Kraken Elite Ring
                device_name = "Kraken Elite Ring";
                break;

            default:
                break;
            }

            if(device_name != "")
            {
                segment new_segment;
                new_segment.name = device_name;
                new_segment.type = ZONE_TYPE_LINEAR;
                new_segment.start_idx = start_idx;
                new_segment.leds_count = controller->channel_dev_szs[zone_idx][dev_idx];

                zones[zone_idx].segments.push_back(new_segment);

                start_idx += new_segment.leds_count;
            }
        }
    }

    SetupColors();
}

void RGBController_NZXTHue2::ResizeZone(int zone, int new_size)
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

void RGBController_NZXTHue2::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        controller->SetChannelLEDs((unsigned char)zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}

void RGBController_NZXTHue2::UpdateZoneLEDs(int zone)
{
    controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
}

void RGBController_NZXTHue2::UpdateSingleLED(int led)
{
    unsigned int zone_idx = leds[led].value;

    controller->SetChannelLEDs(zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
}

void RGBController_NZXTHue2::DeviceUpdateMode()
{
    if(modes[active_mode].value == 0xFFFF)
    {
        DeviceUpdateLEDs();
    }
    else
    {
        for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
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
                    (unsigned char)zone_idx,
                    modes[active_mode].value,
                    modes[active_mode].speed,
                    direction,
                    colors,
                    (unsigned int)modes[active_mode].colors.size()
                    );
        }
    }
}
