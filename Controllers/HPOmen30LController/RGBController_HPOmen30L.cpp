/*---------------------------------------------------------*\
| RGBController_HPOmen30L.cpp                               |
|                                                           |
|   RGBController for HP Omen 30L                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_HPOmen30L.h"

/**------------------------------------------------------------------*\
    @name HP Omen 30L
    @category Motherboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectHPOmen30LController
    @comment
\*-------------------------------------------------------------------*/

RGBController_HPOmen30L::RGBController_HPOmen30L(HPOmen30LController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "HP Omen 30L";
    vendor      = "HP";
    type        = DEVICE_TYPE_MOTHERBOARD;
    description = "HP Omen 30L Device";
    location    = controller->GetLocationString();

    mode Direct;
    Direct.name           = "Direct";
    Direct.value          = HP_OMEN_30L_DIRECT;
    Direct.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode     = MODE_COLORS_PER_LED;
    Direct.brightness_min = 0;
    Direct.brightness_max = 100;
    Direct.brightness     = 100;
    modes.push_back(Direct);

    mode Static;
    Static.name           = "Static";
    Static.value          = HP_OMEN_30L_STATIC;
    Static.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode     = MODE_COLORS_PER_LED;
    Static.brightness_min = 0;
    Static.brightness_max = 100;
    Static.brightness     = 100;
    modes.push_back(Static);

    mode Off;
    Off.name       = "Off";
    Off.value      = HP_OMEN_30L_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Breathing;
    Breathing.name           = "Breathing";
    Breathing.value          = HP_OMEN_30L_BREATHING;
    Breathing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.speed_min      = HP_OMEN_30L_SPEED_SLOW;
    Breathing.speed_max      = HP_OMEN_30L_SPEED_FAST;
    Breathing.speed          = HP_OMEN_30L_SPEED_MED;
    Breathing.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min     = 1;
    Breathing.colors_max     = 6;
    Breathing.colors.resize(4);
    Breathing.brightness_min = 0;
    Breathing.brightness_max = 100;
    Breathing.brightness     = 100;
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name           = "Color Cycle";
    ColorCycle.value          = HP_OMEN_30L_COLOR_CYCLE;
    ColorCycle.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    ColorCycle.speed_min      = HP_OMEN_30L_SPEED_SLOW;
    ColorCycle.speed_max      = HP_OMEN_30L_SPEED_FAST;
    ColorCycle.speed          = HP_OMEN_30L_SPEED_MED;
    ColorCycle.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    ColorCycle.colors_min     = 1;
    ColorCycle.colors_max     = 6;
    ColorCycle.colors.resize(4);
    ColorCycle.brightness_min = 0;
    ColorCycle.brightness_max = 100;
    ColorCycle.brightness     = 100;
    modes.push_back(ColorCycle);

    mode Blinking;
    Blinking.name           = "Blinking";
    Blinking.value          = HP_OMEN_30L_BLINKING;
    Blinking.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Blinking.speed_min      = HP_OMEN_30L_SPEED_SLOW;
    Blinking.speed_max      = HP_OMEN_30L_SPEED_FAST;
    Blinking.speed          = HP_OMEN_30L_SPEED_MED;
    Blinking.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Blinking.colors_min     = 1;
    Blinking.colors_max     = 6;
    Blinking.colors.resize(4);
    Blinking.brightness_min = 0;
    Blinking.brightness_max = 100;
    Blinking.brightness     = 100;
    modes.push_back(Blinking);

    mode Wave;
    Wave.name           = "Wave";
    Wave.value          = HP_OMEN_30L_WAVE;
    Wave.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Wave.speed_min      = HP_OMEN_30L_SPEED_SLOW;
    Wave.speed_max      = HP_OMEN_30L_SPEED_FAST;
    Wave.speed          = HP_OMEN_30L_SPEED_MED;
    Wave.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Wave.colors_min     = 6;
    Wave.colors_max     = 6;
    Wave.colors.resize(6);
    Wave.brightness_min = 0;
    Wave.brightness_max = 100;
    Wave.brightness     = 100;
    modes.push_back(Wave);

    mode Radial;
    Radial.name           = "Radial";
    Radial.value          = HP_OMEN_30L_RADIAL;
    Radial.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Radial.speed_min      = HP_OMEN_30L_SPEED_SLOW;
    Radial.speed_max      = HP_OMEN_30L_SPEED_FAST;
    Radial.speed          = HP_OMEN_30L_SPEED_MED;
    Radial.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Radial.colors_min     = 1;
    Radial.colors_max     = 6;
    Radial.colors.resize(4);
    Radial.brightness_min = 0;
    Radial.brightness_max = 100;
    Radial.brightness     = 100;
    modes.push_back(Radial);

    SetupZones();
}

RGBController_HPOmen30L::~RGBController_HPOmen30L()
{
    delete controller;
}

void RGBController_HPOmen30L::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone logo_zone;
    logo_zone.name          = "Omen Logo";
    logo_zone.type          = ZONE_TYPE_SINGLE;
    logo_zone.leds_min      = 1;
    logo_zone.leds_max      = 1;
    logo_zone.leds_count    = 1;
    logo_zone.matrix_map    = NULL;
    zones.push_back(logo_zone);

    zone light_bar;
    light_bar.name          = "Light Bar";
    light_bar.type          = ZONE_TYPE_SINGLE;
    light_bar.leds_min      = 1;
    light_bar.leds_max      = 1;
    light_bar.leds_count    = 1;
    light_bar.matrix_map    = NULL;
    zones.push_back(light_bar);

    zone ring_zone;
    ring_zone.name          = "Front Fan";
    ring_zone.type          = ZONE_TYPE_SINGLE;
    ring_zone.leds_min      = 1;
    ring_zone.leds_max      = 1;
    ring_zone.leds_count    = 1;
    ring_zone.matrix_map    = NULL;
    zones.push_back(ring_zone);

    zone cpu_zone;
    cpu_zone.name           = "CPU Cooler";
    cpu_zone.type           = ZONE_TYPE_SINGLE;
    cpu_zone.leds_min       = 1;
    cpu_zone.leds_max       = 1;
    cpu_zone.leds_count     = 1;
    cpu_zone.matrix_map     = NULL;
    zones.push_back(cpu_zone);

    zone bot_fan;
    bot_fan.name           = "Front Bottom Fan";
    bot_fan.type           = ZONE_TYPE_SINGLE;
    bot_fan.leds_min       = 1;
    bot_fan.leds_max       = 1;
    bot_fan.leds_count     = 1;
    bot_fan.matrix_map     = NULL;
    zones.push_back(bot_fan);

    zone mid_fan;
    mid_fan.name           = "Front Middle Fan";
    mid_fan.type           = ZONE_TYPE_SINGLE;
    mid_fan.leds_min       = 1;
    mid_fan.leds_max       = 1;
    mid_fan.leds_count     = 1;
    mid_fan.matrix_map     = NULL;
    zones.push_back(mid_fan);

    zone top_fan;
    top_fan.name           = "Front Top Fan";
    top_fan.type           = ZONE_TYPE_SINGLE;
    top_fan.leds_min       = 1;
    top_fan.leds_max       = 1;
    top_fan.leds_count     = 1;
    top_fan.matrix_map     = NULL;
    zones.push_back(top_fan);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    led logo_led;
    logo_led.name           = "Logo LED";
    leds.push_back(logo_led);

    led bar_led;
    bar_led.name            = "Bar LED";
    leds.push_back(bar_led);

    led fan_led;
    fan_led.name            = "Fan LED";
    leds.push_back(fan_led);

    led cpu_led;
    cpu_led.name            = "CPU LED";
    leds.push_back(cpu_led);

    led bot_fan_led;
    bot_fan_led.name        = "Bottom Fan LED";
    leds.push_back(bot_fan_led);

    led mid_fan_led;
    bot_fan_led.name        = "Middle Fan LED";
    leds.push_back(bot_fan_led);

    led top_fan_led;
    bot_fan_led.name        = "Top Fan LED";
    leds.push_back(bot_fan_led);

    SetupColors();
}

void RGBController_HPOmen30L::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HPOmen30L::DeviceUpdateLEDs()
{
    for(unsigned int i = 0; i < zones.size(); i++)
    {
        if(modes[active_mode].value == HP_OMEN_30L_STATIC ||
           modes[active_mode].value == HP_OMEN_30L_DIRECT ||
           modes[active_mode].value == HP_OMEN_30L_OFF)
        {
            controller->SetZoneColor(i, colors);
        }
        else
        {
            controller->SetZoneColor(i, modes[active_mode].colors);
        }
    }
}

void RGBController_HPOmen30L::UpdateZoneLEDs(int zone)
{
    controller->SetZoneColor(zone,colors);
}

void RGBController_HPOmen30L::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_HPOmen30L::DeviceUpdateMode()
{
    for(unsigned int i = 0; i < zones.size(); i++)
    {
        controller->SetZoneMode(i, modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness);
    }

    DeviceUpdateLEDs();
}
