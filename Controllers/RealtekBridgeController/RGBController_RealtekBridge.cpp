/*---------------------------------------------------------*\
| RGBController_RealtekBridge.cpp                           |
|                                                           |
|   Controller for Realtek USB to SSD Bridge ICs            |
|                                                           |
|   Jerry Fan (JerryFan0612)                    13 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_RealtekBridge.h"

/**------------------------------------------------------------------*\
    @name Realtek Bridge Device
    @category Storage
    @type USB
    @save :x:
    @direct :rotating_light:
    @effects :white_check_mark:
    @detectors RealtekBridgeControllerDetect
    @comment
\*-------------------------------------------------------------------*/

RGBController_RealtekBridge::RGBController_RealtekBridge(RealtekBridgeController* controller_ptr)
{
    controller  = controller_ptr;
    name        = controller_ptr->get_product_name();
    vendor      = controller_ptr->get_manu_name();
    location    = controller_ptr->get_dev_loc();
    serial      = controller_ptr->get_sn();
    version     = controller_ptr->get_fw_ver();
    description = vendor + "Storage Device";
    type        = DEVICE_TYPE_STORAGE;

    SetupModes();
    SetupZones();
}

RGBController_RealtekBridge::~RGBController_RealtekBridge()
{
    delete controller;
}

void RGBController_RealtekBridge::SetupModes()
{
    int brightness  = controller->get_argb_brightness() >> 8;

    mode Direct;
    Direct.name                   = "Direct";
    Direct.value                  = REALTEK_BRIDGE_LED_EFF_NONE;
    Direct.flags                  = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode             = MODE_COLORS_PER_LED;
    Direct.brightness_min         = 0;
    Direct.brightness_max         = 255;
    Direct.brightness             = brightness;
    modes.push_back(Direct);

    mode Static;
    Static.name                   = "Static";
    Static.value                  = REALTEK_BRIDGE_LED_EFF_ALWAYS;
    Static.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.colors_min             = 1;
    Static.colors_max             = 1;
    Static.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    Static.brightness_min         = 0;
    Static.brightness_max         = 255;
    Static.brightness             = brightness;
    modes.push_back(Static);

    mode Blink;
    Blink.name                    = "Blink";
    Blink.value                   = REALTEK_BRIDGE_LED_EFF_BLINK;
    Blink.flags                   = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Blink.speed_min               = REALTEK_BRIDGE_SPEED_MIN;
    Blink.speed_max               = REALTEK_BRIDGE_SPEED_MAX;
    Blink.speed                   = REALTEK_BRIDGE_SPEED_NORMAL;
    Blink.colors_min              = 1;
    Blink.colors_max              = 1;
    Blink.color_mode              = MODE_COLORS_MODE_SPECIFIC;
    Blink.colors.resize(1);
    Blink.brightness_min          = 0;
    Blink.brightness_max          = 255;
    Blink.brightness              = brightness;
    modes.push_back(Blink);

    mode Breathing;
    Breathing.name                = "Breathing";
    Breathing.value               = REALTEK_BRIDGE_LED_EFF_BREATHE;
    Breathing.flags               = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.speed_min           = REALTEK_BRIDGE_SPEED_MIN;
    Breathing.speed_max           = REALTEK_BRIDGE_SPEED_MAX;
    Breathing.speed               = REALTEK_BRIDGE_SPEED_NORMAL;
    Breathing.colors_min          = 1;
    Breathing.colors_max          = 1;
    Breathing.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.brightness_min      = 0;
    Breathing.brightness_max      = 255;
    Breathing.brightness          = brightness;
    modes.push_back(Breathing);

    mode Spectrum;
    Spectrum.name                 = "Spectrum";
    Spectrum.value                = REALTEK_BRIDGE_LED_EFF_SPECTRUM;
    Spectrum.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Spectrum.speed_min            = REALTEK_BRIDGE_SPEED_MIN;
    Spectrum.speed_max            = REALTEK_BRIDGE_SPEED_MAX;
    Spectrum.speed                = REALTEK_BRIDGE_SPEED_NORMAL;
    Spectrum.color_mode           = MODE_COLORS_NONE;
    Spectrum.brightness_min       = 0;
    Spectrum.brightness_max       = 255;
    Spectrum.brightness           = brightness;
    modes.push_back(Spectrum);

    mode Scroll;
    Scroll.name                   = "Scroll";
    Scroll.value                  = REALTEK_BRIDGE_LED_EFF_SCROLL;
    Scroll.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Scroll.speed_min              = REALTEK_BRIDGE_SPEED_MIN;
    Scroll.speed_max              = REALTEK_BRIDGE_SPEED_MAX;
    Scroll.speed                  = REALTEK_BRIDGE_SPEED_NORMAL;
    Scroll.colors_min             = 1;
    Scroll.colors_max             = 1;
    Scroll.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Scroll.colors.resize(1);
    Scroll.brightness_min         = 0;
    Scroll.brightness_max         = 255;
    Scroll.brightness             = brightness;
    modes.push_back(Scroll);

    mode RainbowScroll;
    RainbowScroll.name            = "Rainbow Scroll";
    RainbowScroll.value           = REALTEK_BRIDGE_LED_EFF_RAINBOW_SCROLL;
    RainbowScroll.flags           = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    RainbowScroll.speed_min       = REALTEK_BRIDGE_SPEED_MIN;
    RainbowScroll.speed_max       = REALTEK_BRIDGE_SPEED_MAX;
    RainbowScroll.speed           = REALTEK_BRIDGE_SPEED_NORMAL;
    RainbowScroll.color_mode      = MODE_COLORS_NONE;
    RainbowScroll.brightness_min  = 0;
    RainbowScroll.brightness_max  = 255;
    RainbowScroll.brightness      = brightness;
    modes.push_back(RainbowScroll);

    mode RunningWater;
    RunningWater.name             = "Running Water";
    RunningWater.value            = REALTEK_BRIDGE_LED_EFF_RUNNING_WATER;
    RunningWater.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    RunningWater.speed_min        = REALTEK_BRIDGE_SPEED_MIN;
    RunningWater.speed_max        = REALTEK_BRIDGE_SPEED_MAX;
    RunningWater.speed            = REALTEK_BRIDGE_SPEED_NORMAL;
    RunningWater.colors_min       = 1;
    RunningWater.colors_max       = 1;
    RunningWater.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    RunningWater.colors.resize(1);
    RunningWater.brightness_min   = 0;
    RunningWater.brightness_max   = 255;
    RunningWater.brightness       = brightness;
    modes.push_back(RunningWater);

    mode Sliding;
    Sliding.name                   = "Sliding";
    Sliding.value                  = REALTEK_BRIDGE_LED_EFF_SLIDING;
    Sliding.flags                  = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Sliding.speed_min              = REALTEK_BRIDGE_SPEED_MIN;
    Sliding.speed_max              = REALTEK_BRIDGE_SPEED_MAX;
    Sliding.speed                  = REALTEK_BRIDGE_SPEED_NORMAL;
    Sliding.color_mode             = MODE_COLORS_NONE;
    Sliding.brightness_min         = 0;
    Sliding.brightness_max         = 255;
    Sliding.brightness             = brightness;
    modes.push_back(Sliding);

    mode NewtonCradle;
    NewtonCradle.name             = "Newton Cradle";
    NewtonCradle.value            = REALTEK_BRIDGE_LED_EFF_NEWTON_CRADLE;
    NewtonCradle.flags            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    NewtonCradle.speed_min        = REALTEK_BRIDGE_SPEED_MIN;
    NewtonCradle.speed_max        = REALTEK_BRIDGE_SPEED_MAX;
    NewtonCradle.speed            = REALTEK_BRIDGE_SPEED_NORMAL;
    NewtonCradle.color_mode       = MODE_COLORS_NONE;
    NewtonCradle.brightness_min   = 0;
    NewtonCradle.brightness_max   = 255;
    NewtonCradle.brightness       = brightness;
    modes.push_back(NewtonCradle);

    mode Meteor;
    Meteor.name                   = "Meteor";
    Meteor.value                  = REALTEK_BRIDGE_LED_EFF_METEOR;
    Meteor.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Meteor.speed_min              = REALTEK_BRIDGE_SPEED_MIN;
    Meteor.speed_max              = REALTEK_BRIDGE_SPEED_MAX;
    Meteor.speed                  = REALTEK_BRIDGE_SPEED_NORMAL;
    Meteor.colors_min             = 1;
    Meteor.colors_max             = 1;
    Meteor.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Meteor.colors.resize(1);
    Meteor.brightness_min         = 0;
    Meteor.brightness_max         = 255;
    Meteor.brightness             = brightness;
    modes.push_back(Meteor);
}

void RGBController_RealtekBridge::SetupZones()
{
    zone argb_zone;

    argb_zone.name       = "strip";
    argb_zone.type       = ZONE_TYPE_LINEAR;
    argb_zone.leds_min   = controller->get_argb_num();
    argb_zone.leds_max   = controller->get_argb_num();
    argb_zone.leds_count = controller->get_argb_num();

    zones.push_back(argb_zone);

    for(unsigned int led_idx = 0; led_idx < argb_zone.leds_count; led_idx++)
    {
        led         StripLED;
        StripLED.name = "led ";
        StripLED.name.append(std::to_string(led_idx + 1));
        leds.push_back(StripLED);
    }

    SetupColors();
}

void RGBController_RealtekBridge::DeviceUpdateLEDs()
{
    DeviceUpdateZoneLEDs(0);
}

void RGBController_RealtekBridge::DeviceUpdateZoneLEDs(int /*zone*/)
{
    unsigned short brightness = 0xFF;
    mode&          curr_mode  = modes[active_mode];

    if(curr_mode.color_mode == MODE_COLORS_PER_LED &&
       curr_mode.value == REALTEK_BRIDGE_LED_EFF_NONE)       //direct mode
    {
        if(curr_mode.flags & MODE_FLAG_HAS_BRIGHTNESS)
        {
            brightness = curr_mode.brightness;
        }
        controller->set_argb_direct(colors, brightness);
    }
    else
    {
        DeviceUpdateSingleLED(0);
    }
}

void RGBController_RealtekBridge::DeviceUpdateSingleLED(int /*led*/)
{
    unsigned char         speed       = REALTEK_BRIDGE_SPEED_NORMAL;
    unsigned short        brightness  = 0xFF;
    mode&                 curr_mode   = modes[active_mode];
    std::vector<RGBColor> rtk_colors  = curr_mode.colors;

    if(curr_mode.flags & MODE_FLAG_HAS_SPEED)
    {
        speed = curr_mode.speed;
    }
    if(curr_mode.flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        brightness = curr_mode.brightness;
    }

    if(curr_mode.color_mode == MODE_COLORS_PER_LED)
    {
        rtk_colors = colors;
    }
    else if(curr_mode.color_mode == MODE_COLORS_NONE)
    {
        rtk_colors.clear();
    }

    controller->set_argb_effect(curr_mode.value, rtk_colors, speed, brightness);
}

void RGBController_RealtekBridge::DeviceUpdateMode()
{
    if(modes[active_mode].value != REALTEK_BRIDGE_LED_EFF_NONE)
    {
        DeviceUpdateLEDs();
    }
}
