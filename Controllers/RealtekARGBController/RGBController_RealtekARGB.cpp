/*---------------------------------------------------------*\
| RGBController_RealtekARGB.cpp                             |
|                                                           |
|   RGBController for Realtek USB ARGB ICs                  |
|                                                           |
|   Jerry Fan (JerryFan0612)                    13 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_RealtekARGB.h"

/**------------------------------------------------------------------*\
    @name Realtek ARGB Device
    @category LEDStrip
    @type USB
    @save :x:
    @direct :rotating_light:
    @effects :white_check_mark:
    @detectors RealtekARGBControllerDetect
    @comment
\*-------------------------------------------------------------------*/

RGBController_RealtekARGB::RGBController_RealtekARGB(RealtekARGBController* controller_ptr)
{
    controller  = controller_ptr;
    name        = controller_ptr->get_dev_name();
    vendor      = controller_ptr->get_manu_name();
    location    = controller_ptr->get_dev_loc();
    serial      = controller_ptr->get_sn();
    version     = controller_ptr->get_fw_ver();
    description = vendor + "ARGB Device";
    type        = DEVICE_TYPE_LEDSTRIP;
    std::fill(std::begin(ready_to_reboot), std::end(ready_to_reboot), false);

    SetupModes();
    SetupZones();
}

void RGBController_RealtekARGB::SetupModes()
{
    int brightness = controller->get_argb_brightness(0) >> 8;

    mode Direct;
    Direct.name                   = "Direct";
    Direct.value                  = REALTEK_ARGB_EFF_NULL;
    Direct.flags                  = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode             = MODE_COLORS_PER_LED;
    Direct.brightness_min         = 0;
    Direct.brightness_max         = 255;
    Direct.brightness             = brightness;
    modes.push_back(Direct);

    mode Static;
    Static.name                   = "Static";
    Static.value                  = REALTEK_ARGB_EFF_ALWAYS_ON;
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
    Blink.value                   = REALTEK_ARGB_EFF_BLINK;
    Blink.flags                   = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Blink.speed_min               = REALTEK_ARGB_SPEED_MIN;
    Blink.speed_max               = REALTEK_ARGB_SPEED_MAX;
    Blink.speed                   = REALTEK_ARGB_SPEED_NORMAL;
    Blink.colors_min              = 1;
    Blink.colors_max              = 2;
    Blink.color_mode              = MODE_COLORS_MODE_SPECIFIC;
    Blink.colors.resize(2);
    Blink.brightness_min          = 0;
    Blink.brightness_max          = 255;
    Blink.brightness              = brightness;
    modes.push_back(Blink);

    mode Breathing;
    Breathing.name                = "Breathing";
    Breathing.value               = REALTEK_ARGB_EFF_BREATH;
    Breathing.flags               = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.speed_min           = REALTEK_ARGB_SPEED_MIN;
    Breathing.speed_max           = REALTEK_ARGB_SPEED_MAX;
    Breathing.speed               = REALTEK_ARGB_SPEED_NORMAL;
    Breathing.colors_min          = 1;
    Breathing.colors_max          = 2;
    Breathing.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(2);
    Breathing.brightness_min      = 0;
    Breathing.brightness_max      = 255;
    Breathing.brightness          = brightness;
    modes.push_back(Breathing);

    mode Spectrum;
    Spectrum.name                 = "Spectrum";
    Spectrum.value                = REALTEK_ARGB_EFF_SPECTRUM;
    Spectrum.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Spectrum.speed_min            = REALTEK_ARGB_SPEED_MIN;
    Spectrum.speed_max            = REALTEK_ARGB_SPEED_MAX;
    Spectrum.speed                = REALTEK_ARGB_SPEED_NORMAL;
    Spectrum.color_mode           = MODE_COLORS_NONE;
    Spectrum.brightness_min       = 0;
    Spectrum.brightness_max       = 255;
    Spectrum.brightness           = brightness;
    modes.push_back(Spectrum);

    mode Scroll;
    Scroll.name                   = "Scroll";
    Scroll.value                  = REALTEK_ARGB_EFF_SCROLL;
    Scroll.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Scroll.speed_min              = REALTEK_ARGB_SPEED_MIN;
    Scroll.speed_max              = REALTEK_ARGB_SPEED_MAX;
    Scroll.speed                  = REALTEK_ARGB_SPEED_NORMAL;
    Scroll.colors_min             = 1;
    Scroll.colors_max             = 2;
    Scroll.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Scroll.colors.resize(2);
    Scroll.brightness_min         = 0;
    Scroll.brightness_max         = 255;
    Scroll.brightness             = brightness;
    modes.push_back(Scroll);

    mode RainbowScroll;
    RainbowScroll.name            = "Rainbow Scroll";
    RainbowScroll.value           = REALTEK_ARGB_EFF_RAINBOW_SCROLL;
    RainbowScroll.flags           = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    RainbowScroll.speed_min       = REALTEK_ARGB_SPEED_MIN;
    RainbowScroll.speed_max       = REALTEK_ARGB_SPEED_MAX;
    RainbowScroll.speed           = REALTEK_ARGB_SPEED_NORMAL;
    RainbowScroll.color_mode      = MODE_COLORS_NONE;
    RainbowScroll.brightness_min  = 0;
    RainbowScroll.brightness_max  = 255;
    RainbowScroll.brightness      = brightness;
    modes.push_back(RainbowScroll);

    mode RunningWater;
    RunningWater.name             = "Running Water";
    RunningWater.value            = REALTEK_ARGB_EFF_RUNNING_WATER;
    RunningWater.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    RunningWater.speed_min        = REALTEK_ARGB_SPEED_MIN;
    RunningWater.speed_max        = REALTEK_ARGB_SPEED_MAX;
    RunningWater.speed            = REALTEK_ARGB_SPEED_NORMAL;
    RunningWater.colors_min       = 1;
    RunningWater.colors_max       = 2;
    RunningWater.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    RunningWater.colors.resize(2);
    RunningWater.brightness_min   = 0;
    RunningWater.brightness_max   = 255;
    RunningWater.brightness       = brightness;
    modes.push_back(RunningWater);

    mode Sliding;
    Sliding.name                  = "Sliding";
    Sliding.value                 = REALTEK_ARGB_EFF_SLIDING;
    Sliding.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Sliding.speed_min             = REALTEK_ARGB_SPEED_MIN;
    Sliding.speed_max             = REALTEK_ARGB_SPEED_MAX;
    Sliding.speed                 = REALTEK_ARGB_SPEED_NORMAL;
    Sliding.colors_min            = 1;
    Sliding.colors_max            = 2;
    Sliding.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Sliding.colors.resize(2);
    Sliding.brightness_min        = 0;
    Sliding.brightness_max        = 255;
    Sliding.brightness            = brightness;
    modes.push_back(Sliding);

    mode WideSliding;
    WideSliding.name              = "Wide Sliding";
    WideSliding.value             = REALTEK_ARGB_EFF_WIDE_SLIDING;
    WideSliding.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    WideSliding.speed_min         = REALTEK_ARGB_SPEED_MIN;
    WideSliding.speed_max         = REALTEK_ARGB_SPEED_MAX;
    WideSliding.speed             = REALTEK_ARGB_SPEED_NORMAL;
    WideSliding.colors_min        = 1;
    WideSliding.colors_max        = 2;
    WideSliding.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    WideSliding.colors.resize(2);
    WideSliding.brightness_min    = 0;
    WideSliding.brightness_max    = 255;
    WideSliding.brightness        = brightness;
    modes.push_back(WideSliding);

    mode RainbowSliding;
    RainbowSliding.name           = "Rainbow Sliding";
    RainbowSliding.value          = REALTEK_ARGB_EFF_RAINBOW_SLIDING;
    RainbowSliding.flags          = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowSliding.speed_min      = REALTEK_ARGB_SPEED_MIN;
    RainbowSliding.speed_max      = REALTEK_ARGB_SPEED_MAX;
    RainbowSliding.speed          = REALTEK_ARGB_SPEED_NORMAL;
    RainbowSliding.color_mode     = MODE_COLORS_NONE;
    RainbowSliding.brightness_min = 0;
    RainbowSliding.brightness_max = 255;
    RainbowSliding.brightness     = brightness;
    modes.push_back(RainbowSliding);

    mode RainbowFadeSliding;
    RainbowFadeSliding.name           = "Rainbow Fade Sliding";
    RainbowFadeSliding.value          = REALTEK_ARGB_EFF_RAINBOW_FADE_SLIDING;
    RainbowFadeSliding.flags          = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    RainbowFadeSliding.speed_min      = REALTEK_ARGB_SPEED_MIN;
    RainbowFadeSliding.speed_max      = REALTEK_ARGB_SPEED_MAX;
    RainbowFadeSliding.speed          = REALTEK_ARGB_SPEED_NORMAL;
    RainbowFadeSliding.color_mode     = MODE_COLORS_NONE;
    RainbowFadeSliding.brightness_min = 0;
    RainbowFadeSliding.brightness_max = 255;
    RainbowFadeSliding.brightness     = brightness;
    modes.push_back(RainbowFadeSliding);

    mode NewtonCradle;
    NewtonCradle.name             = "Newton Cradle";
    NewtonCradle.value            = REALTEK_ARGB_EFF_NEWTON_CRADLE;
    NewtonCradle.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    NewtonCradle.speed_min        = REALTEK_ARGB_SPEED_MIN;
    NewtonCradle.speed_max        = REALTEK_ARGB_SPEED_MAX;
    NewtonCradle.speed            = REALTEK_ARGB_SPEED_NORMAL;
    NewtonCradle.colors_min       = 1;
    NewtonCradle.colors_max       = 2;
    NewtonCradle.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    NewtonCradle.colors.resize(2);
    NewtonCradle.brightness_min   = 0;
    NewtonCradle.brightness_max   = 255;
    NewtonCradle.brightness       = brightness;
    modes.push_back(NewtonCradle);

    mode Meteor;
    Meteor.name                   = "Meteor";
    Meteor.value                  = REALTEK_ARGB_EFF_METEOR;
    Meteor.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Meteor.speed_min              = REALTEK_ARGB_SPEED_MIN;
    Meteor.speed_max              = REALTEK_ARGB_SPEED_MAX;
    Meteor.speed                  = REALTEK_ARGB_SPEED_NORMAL;
    Meteor.colors_min             = 1;
    Meteor.colors_max             = 2;
    Meteor.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Meteor.colors.resize(2);
    Meteor.brightness_min         = 0;
    Meteor.brightness_max         = 255;
    Meteor.brightness             = brightness;
    modes.push_back(Meteor);

    mode ZigZag;
    ZigZag.name                   = "ZigZag";
    ZigZag.value                  = REALTEK_ARGB_EFF_ZIGZAG;
    ZigZag.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR;
    ZigZag.speed_min              = REALTEK_ARGB_SPEED_MIN;
    ZigZag.speed_max              = REALTEK_ARGB_SPEED_MAX;
    ZigZag.speed                  = REALTEK_ARGB_SPEED_NORMAL;
    ZigZag.colors_min             = 1;
    ZigZag.colors_max             = 2;
    ZigZag.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    ZigZag.colors.resize(2);
    ZigZag.brightness_min         = 0;
    ZigZag.brightness_max         = 255;
    ZigZag.brightness             = brightness;
    modes.push_back(ZigZag);

    mode StarryNight;
    StarryNight.name              = "Starry Night";
    StarryNight.value             = REALTEK_ARGB_EFF_STARRY_NIGHT;
    StarryNight.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR;
    StarryNight.speed_min         = REALTEK_ARGB_SPEED_MIN;
    StarryNight.speed_max         = REALTEK_ARGB_SPEED_MAX;
    StarryNight.speed             = REALTEK_ARGB_SPEED_NORMAL;
    StarryNight.colors_min        = 1;
    StarryNight.colors_max        = 1;
    StarryNight.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    StarryNight.colors.resize(1);
    StarryNight.brightness_min    = 0;
    StarryNight.brightness_max    = 255;
    StarryNight.brightness        = brightness;
    modes.push_back(StarryNight);

    mode Stack;
    Stack.name                    = "Stack";
    Stack.value                   = REALTEK_ARGB_EFF_STACK;
    Stack.flags                   = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Stack.colors_min              = 1;
    Stack.colors_max              = 2;
    Stack.color_mode              = MODE_COLORS_MODE_SPECIFIC;
    Stack.colors.resize(2);
    Stack.brightness_min          = 0;
    Stack.brightness_max          = 255;
    Stack.brightness              = brightness;
    modes.push_back(Stack);
}

RGBController_RealtekARGB::~RGBController_RealtekARGB()
{
    delete controller;
}

void RGBController_RealtekARGB::SetupZones()
{
    /*-------------------------------------------------*\
    | Only set LED count on the first run               |
    \*-------------------------------------------------*/
    bool first_run      = false;
    int idx             = 0;
    int argb_num        = 0;
    int fix_grps        = 0;
    int num_fixgrp      = 0;
    int argb_num_fixgrp = 0;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();
    valid_grp.clear();
    fix_grps = controller->get_fix_grps();

    for(int grp_num = 0; grp_num < REALTEK_ARGB_NUM_ARGB_GRP; grp_num++)
    {
        if(controller->get_zone_enable(grp_num))
        {
            valid_grp.push_back(grp_num);
            if(fix_grps & (0x1 << grp_num))
            {
                num_fixgrp++;
                argb_num_fixgrp += controller->get_argb_num(grp_num);
            }
        }
    }

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    zones.resize(valid_grp.size());
    for(int grp_num : valid_grp)
    {
        argb_num = controller->get_argb_num(grp_num);
        zones[idx].name = "strip " + std::to_string(idx + 1);
        zones[idx].type = ZONE_TYPE_LINEAR;
        if(fix_grps & (0x1 << grp_num))
        {
            zones[idx].leds_count = argb_num;
            zones[idx].leds_min = argb_num;
            zones[idx].leds_max = argb_num;
        }
        else
        {
            if(first_run)
            {
                zones[idx].leds_count = 0;
            }
            else
            {
                zones[idx].leds_count = argb_num;
            }

            zones[idx].leds_min = 0;
            zones[idx].leds_max = (REALTEK_ARGB_MAX - argb_num_fixgrp) / ((int)valid_grp.size() - num_fixgrp);
        }

        for(unsigned int led_idx = 0; led_idx < zones[idx].leds_count; led_idx++)
        {
            led myled;
            myled.name = zones[idx].name + "  led_";
            myled.name.append(std::to_string(led_idx + 1));
            leds.push_back(myled);
        }
        idx++;
    }
    SetupColors();
}

void RGBController_RealtekARGB::ResizeZone(int zone, int new_size)
{
    int fix_grps = controller->get_fix_grps();
    int orig_num = controller->get_argb_num(valid_grp[zone]);
    bool need_reboot = true;

    if((size_t) zone >= zones.size())
    {
        return;
    }
    if((new_size == orig_num) && zones[zone].leds_count)
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        int total_argb_num = 0;
        std::vector<RGBColor> rtk_colors(orig_num, 0);
        for(int grp_num = 0; grp_num < REALTEK_ARGB_NUM_ARGB_GRP; grp_num++)
        {
            if(grp_num == valid_grp[zone])
            {
                total_argb_num += new_size;
            }
            else
            {
                total_argb_num += controller->get_argb_num(grp_num);
            }
        }
        if(total_argb_num > REALTEK_ARGB_MAX)
        {
            return;
        }

        controller->set_argb_direct(valid_grp[zone], rtk_colors, 0xFF);
        controller->set_argb_num(valid_grp[zone], new_size);
        ready_to_reboot[valid_grp[zone]] = true;

        for(int grp_num = 0; grp_num < REALTEK_ARGB_NUM_ARGB_GRP; grp_num++)
        {
            if(!ready_to_reboot[grp_num] && !(fix_grps & (0x1 << grp_num)))
            {
                need_reboot = false;
                break;
            }
        }
        if(need_reboot)
        {
            controller->device_reboot();
            controller->device_rescan_trigger();
        }
        SetupZones();
    }
}

void RGBController_RealtekARGB::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        if(zones[zone_idx].leds_count > 0)
        {
           DeviceUpdateZoneLEDs((int)zone_idx);
        }
    }
}

void RGBController_RealtekARGB::DeviceUpdateZoneLEDs(int zone)
{
    unsigned short brightness = 0xFF;
    mode& curr_mode = modes[active_mode];
    std::vector<RGBColor> color_buf;

    if(curr_mode.color_mode == MODE_COLORS_PER_LED &&
       curr_mode.value == REALTEK_ARGB_EFF_NULL)       //direct mode
    {
        if(curr_mode.flags & MODE_FLAG_HAS_BRIGHTNESS)
        {
            brightness = curr_mode.brightness;
        }

        color_buf.resize(zones[zone].leds_count);
        for(unsigned int i = 0; i < zones[zone].leds_count; i++)
            color_buf[i] = zones[zone].colors[i];
        controller->set_argb_direct(valid_grp[zone], color_buf, brightness);
    }
    else
    {
        DeviceUpdateSingleLED(zone);
    }
}

void RGBController_RealtekARGB::DeviceUpdateSingleLED(int zone)
{
    mode& curr_mode = modes[active_mode];
    std::vector<RGBColor> rtk_colors = curr_mode.colors;
    struct RealtekARGBControllerSetEffParam param;

    param.speed = REALTEK_ARGB_SPEED_NORMAL;
    param.brightness = 0xFF;
    param.dir = 0;
    param.random_color = 0;

    if(curr_mode.flags & MODE_FLAG_HAS_SPEED)
    {
        param.speed = curr_mode.speed;
    }
    if(curr_mode.flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        param.brightness = curr_mode.brightness;
    }
    if(curr_mode.flags & MODE_FLAG_HAS_DIRECTION_LR)
    {
        if(curr_mode.direction == MODE_DIRECTION_RIGHT)
        {
            param.dir = 1;
        }
    }
    if(curr_mode.flags & MODE_FLAG_HAS_RANDOM_COLOR)
    {
        if(curr_mode.color_mode == MODE_COLORS_RANDOM)
        {
            param.random_color = 1;
        }
    }

    if(curr_mode.color_mode == MODE_COLORS_PER_LED)
    {
        rtk_colors = colors;
    }
    else if(curr_mode.color_mode == MODE_COLORS_NONE)
    {
        rtk_colors.clear();
    }

    controller->set_argb_effect(valid_grp[zone], curr_mode.value, rtk_colors, &param);
}

void RGBController_RealtekARGB::DeviceUpdateMode()
{
    if(modes[active_mode].value != REALTEK_ARGB_EFF_NULL)
    {
        DeviceUpdateLEDs();
    }
}
