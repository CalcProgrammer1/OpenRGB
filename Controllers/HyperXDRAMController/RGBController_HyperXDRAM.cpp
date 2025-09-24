/*---------------------------------------------------------*\
| RGBController_HyperXDRAM.cpp                              |
|                                                           |
|   RGBController for HyperX/Kingston Fury RAM              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                29 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_HyperXDRAM.h"

/**------------------------------------------------------------------*\
    @name HyperX DRAM
    @category RAM
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectHyperXDRAMControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_HyperXDRAM::RGBController_HyperXDRAM(HyperXDRAMController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "HyperX";
    type                        = DEVICE_TYPE_DRAM;
    description                 = "HyperX DRAM Device";
    location                    = controller->GetDeviceLocation();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = HYPERX_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = HYPERX_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = HYPERX_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min           = HYPERX_SPEED_RAINBOW_SLOW;
    Rainbow.speed_max           = HYPERX_SPEED_RAINBOW_FAST;
    Rainbow.speed               = HYPERX_SPEED_RAINBOW_NORMAL;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Comet;
    Comet.name                  = "Comet";
    Comet.value                 = HYPERX_MODE_COMET;
    Comet.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Comet.speed_min             = HYPERX_SPEED_COMET_SLOW;
    Comet.speed_max             = HYPERX_SPEED_COMET_FAST;
    Comet.colors_min            = 1;
    Comet.colors_max            = 1;
    Comet.speed                 = HYPERX_SPEED_COMET_NORMAL;
    Comet.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Comet.colors.resize(1);
    modes.push_back(Comet);

    mode Heartbeat;
    Heartbeat.name              = "Heartbeat";
    Heartbeat.value             = HYPERX_MODE_HEARTBEAT;
    Heartbeat.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Heartbeat.speed_min         = HYPERX_SPEED_COMET_SLOW;
    Heartbeat.speed_max         = HYPERX_SPEED_COMET_FAST;
    Heartbeat.colors_min        = 1;
    Heartbeat.colors_max        = 1;
    Heartbeat.speed             = HYPERX_SPEED_COMET_NORMAL;
    Heartbeat.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Heartbeat.colors.resize(1);
    modes.push_back(Heartbeat);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = HYPERX_MODE_CYCLE;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED;
    SpectrumCycle.speed_min     = HYPERX_SPEED_CYCLE_SLOW;
    SpectrumCycle.speed_max     = HYPERX_SPEED_CYCLE_FAST;
    SpectrumCycle.speed         = HYPERX_SPEED_CYCLE_NORMAL;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = HYPERX_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Breathing.speed_min         = HYPERX_SPEED_BREATHING_SLOW;
    Breathing.speed_max         = HYPERX_SPEED_BREATHING_FAST;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.speed             = HYPERX_SPEED_BREATHING_NORMAL;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Bounce;
    Bounce.name                 = "Bounce";
    Bounce.value                = HYPERX_MODE_BOUNCE;
    Bounce.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Bounce.speed_min            = HYPERX_SPEED_BOUNCE_SLOW;
    Bounce.speed_max            = HYPERX_SPEED_BOUNCE_FAST;
    Bounce.colors_min           = 1;
    Bounce.colors_max           = 1;
    Bounce.speed                = HYPERX_SPEED_BOUNCE_NORMAL;
    Bounce.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Bounce.colors.resize(1);
    modes.push_back(Bounce);

    mode Blink;
    Blink.name                  = "Blink";
    Blink.value                 = HYPERX_MODE_BLINK;
    Blink.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Blink.speed_min             = HYPERX_SPEED_BLINK_SLOW;
    Blink.speed_max             = HYPERX_SPEED_BLINK_FAST;
    Blink.colors_min            = 1;
    Blink.colors_max            = 1;
    Blink.speed                 = HYPERX_SPEED_BLINK_NORMAL;
    Blink.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Blink.colors.resize(1);
    modes.push_back(Blink);

    SetupZones();
}

RGBController_HyperXDRAM::~RGBController_HyperXDRAM()
{
    delete controller;
}

void RGBController_HyperXDRAM::SetupZones()
{
    for(unsigned int slot = 0; slot < controller->GetSlotCount(); slot++)
    {
        zone* new_zone          = new zone;

        new_zone->name          = "HyperX Slot ";
        new_zone->name.append(std::to_string(slot + 1));
        new_zone->type          = ZONE_TYPE_LINEAR;
        new_zone->leds_min      = 5;
        new_zone->leds_max      = 5;
        new_zone->leds_count    = 5;

        zones.push_back(*new_zone);
    }

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(std::size_t led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led* new_led        = new led();

            new_led->name       = "HyperX Slot ";
            new_led->name.append(std::to_string(zone_idx + 1));
            new_led->name.append(", LED ");
            new_led->name.append(std::to_string(led_idx + 1));

            new_led->value      = (unsigned int)leds.size();

            leds.push_back(*new_led);
        }
    }

    SetupColors();
}

void RGBController_HyperXDRAM::DeviceUpdateLEDs()
{
    if(controller->GetMode() == HYPERX_MODE_DIRECT)
    {
        for(unsigned int led_idx = 0; led_idx < (unsigned int)colors.size(); led_idx++ )
        {
            RGBColor      color = colors[led_idx];
            unsigned char red   = RGBGetRValue(color);
            unsigned char grn   = RGBGetGValue(color);
            unsigned char blu   = RGBGetBValue(color);

            controller->SetLEDColor(led_idx, red, grn, blu);
        }
        controller->SendApply();
    }
    else
    {
        unsigned char red = RGBGetRValue(colors[0]);
        unsigned char grn = RGBGetGValue(colors[0]);
        unsigned char blu = RGBGetBValue(colors[0]);

        controller->SetEffectColor(red, grn, blu);
    }
}

void RGBController_HyperXDRAM::DeviceUpdateZoneLEDs(int zone)
{
    if(controller->GetMode() == HYPERX_MODE_DIRECT)
    {
        for(std::size_t led_idx = 0; led_idx < zones[zone].leds_count; led_idx++ )
        {
            unsigned int  led   = zones[zone].leds[led_idx].value;
            RGBColor      color = colors[led];
            unsigned char red   = RGBGetRValue(color);
            unsigned char grn   = RGBGetGValue(color);
            unsigned char blu   = RGBGetBValue(color);

            controller->SetLEDColor(led, red, grn, blu);
        }
        controller->SendApply();
    }
    else
    {
        unsigned char red = RGBGetRValue(colors[0]);
        unsigned char grn = RGBGetGValue(colors[0]);
        unsigned char blu = RGBGetBValue(colors[0]);

        controller->SetEffectColor(red, grn, blu);
    }
}

void RGBController_HyperXDRAM::DeviceUpdateSingleLED(int led)
{
    RGBColor      color = colors[led];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    if(controller->GetMode() == HYPERX_MODE_DIRECT)
    {
        controller->SetLEDColor(led, red, grn, blu);
    }
    else
    {
        controller->SetEffectColor(red, grn, blu);
    }
    controller->SendApply();
}

void RGBController_HyperXDRAM::DeviceUpdateMode()
{
    bool random = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);

    controller->SetMode(modes[active_mode].value, random, modes[active_mode].speed);

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        unsigned char red = RGBGetRValue(modes[active_mode].colors[0]);
        unsigned char grn = RGBGetGValue(modes[active_mode].colors[0]);
        unsigned char blu = RGBGetBValue(modes[active_mode].colors[0]);

        controller->SetEffectColor(red, grn, blu);
    }
}

