/*---------------------------------------------------------*\
| RGBController_AsusAuraCore.cpp                            |
|                                                           |
|   RGBController for ASUS ROG Aura Core                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                13 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_AsusAuraCore.h"

/**------------------------------------------------------------------*\
    @name Asus AURA Core
    @category Keyboard,LEDStrip
    @type USB
    @save :x:
    @direct :rotating_light:
    @effects :white_check_mark:
    @detectors DetectAsusAuraCoreControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AuraCore::RGBController_AuraCore(AuraCoreController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "ASUS Aura Core Device";
    vendor      = "ASUS";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();
    description = "ASUS Aura Core Device";
    type        = DEVICE_TYPE_UNKNOWN;

    if(controller->aura_device.aura_type == AURA_CORE_DEVICE_KEYBOARD)
    {
        SetupKeyboard();
    }
    else if(controller->aura_device.aura_type == AURA_CORE_DEVICE_GA15DH)
    {
        SetupGA15DH();
    }

    SetupZones();
}

RGBController_AuraCore::~RGBController_AuraCore()
{
    Shutdown();

    delete controller;
}

void RGBController_AuraCore::SetupKeyboard()
{
    name                    = "ASUS Aura Keyboard";
    vendor                  = "ASUS";
    type                    = DEVICE_TYPE_KEYBOARD;
    description             = "ASUS Aura Core Device";

    mode Static;
    Static.name             = "Static";
    Static.value            = AURA_CORE_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = AURA_CORE_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name         = "Color Cycle";
    ColorCycle.value        = AURA_CORE_MODE_SPECTRUM_CYCLE;
    ColorCycle.flags        = 0;
    ColorCycle.color_mode   = MODE_COLORS_NONE;
    modes.push_back(ColorCycle);
}

void RGBController_AuraCore::SetupGA15DH()
{
    name                    = "ASUS Aura GA15DH";
    vendor                  = "ASUS";
    type                    = DEVICE_TYPE_LEDSTRIP;
    description             = "ASUS Aura Core Device";

    mode Static;
    Static.name             = "Static";
    Static.value            = AURA_CORE_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min       = 1;
    Static.colors_max       = 1;
    Static.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = AURA_CORE_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.speed_min     = AURA_CORE_SPEED_SLOW;
    Breathing.speed_max     = AURA_CORE_SPEED_FAST;
    Breathing.speed         = AURA_CORE_SPEED_NORMAL;
    Breathing.colors_min    = 1;
    Breathing.colors_max    = 2;
    Breathing.color_mode    = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(2);
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name         = "Color Cycle";
    ColorCycle.value        = AURA_CORE_MODE_SPECTRUM_CYCLE;
    ColorCycle.flags        = MODE_FLAG_HAS_SPEED;
    ColorCycle.speed_min    = AURA_CORE_SPEED_SLOW;
    ColorCycle.speed_max    = AURA_CORE_SPEED_FAST;
    ColorCycle.speed        = AURA_CORE_SPEED_NORMAL;
    ColorCycle.color_mode   = MODE_COLORS_NONE;
    modes.push_back(ColorCycle);

    mode Rainbow;
    Rainbow.name            = "Rainbow";
    Rainbow.value           = AURA_CORE_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Rainbow.speed_min       = AURA_CORE_SPEED_SLOW;
    Rainbow.speed_max       = AURA_CORE_SPEED_FAST;
    Rainbow.speed           = AURA_CORE_SPEED_NORMAL;
    Rainbow.direction       = MODE_DIRECTION_RIGHT;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Strobe;
    Strobe.name             = "Strobe";
    Strobe.value            = AURA_CORE_MODE_STROBE;
    Strobe.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Strobe.colors_min       = 1;
    Strobe.colors_max       = 1;
    Strobe.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Strobe.colors.resize(1);
    modes.push_back(Strobe);

    mode Comet;
    Comet.name              = "Comet";
    Comet.value             = AURA_CORE_MODE_COMET;
    Comet.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Comet.colors_min        = 1;
    Comet.colors_max        = 1;
    Comet.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Comet.colors.resize(1);
    modes.push_back(Comet);

    mode Flash;
    Flash.name              = "Flash & Dash";
    Flash.value             = AURA_CORE_MODE_FLASHNDASH;
    Flash.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Flash.colors_min        = 1;
    Flash.colors_max        = 1;
    Flash.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Flash.colors.resize(1);
    modes.push_back(Flash);

    mode Irradiation;
    Irradiation.name        = "Irradiation";
    Irradiation.value       = AURA_CORE_MODE_IRRADIATION;
    Irradiation.flags       = 0;
    Irradiation.color_mode  = MODE_COLORS_NONE;
    modes.push_back(Irradiation);

    mode Direct;
    Static.name             = "Direct";
    Static.value            = AURA_CORE_MODE_DIRECT;
    Static.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Static);
}

void RGBController_AuraCore::SetupZones()
{
    zone auraZone;

    if(controller->aura_device.aura_type == AURA_CORE_DEVICE_KEYBOARD)
    {
        auraZone.name       = "Keyboard";
        auraZone.type       = ZONE_TYPE_SINGLE;
        auraZone.leds_min   = 4;
        auraZone.leds_max   = 4;
        auraZone.leds_count = 4;
    }
    else if(controller->aura_device.aura_type == AURA_CORE_DEVICE_GA15DH)
    {
        auraZone.name       = "GA15DH";
        auraZone.type       = ZONE_TYPE_LINEAR;
        auraZone.leds_min   = 20;
        auraZone.leds_max   = 20;
        auraZone.leds_count = 20;
    }
    else
    {
        auraZone.leds_count = 0;
    }

    zones.push_back(auraZone);

    for(unsigned int led_idx = 0; led_idx < auraZone.leds_count; led_idx++)
    {
        led KeyLED;
        KeyLED.name = auraZone.name + " ";
        KeyLED.name.append(std::to_string(led_idx + 1));
        leds.push_back(KeyLED);
    }

    SetupColors();
}

void RGBController_AuraCore::DeviceUpdateLEDs()
{
    DeviceUpdateZoneLEDs(0);
}

void RGBController_AuraCore::DeviceUpdateZoneLEDs(int /*zone*/)
{
    if(modes[active_mode].value == AURA_CORE_MODE_DIRECT)
    {
        std::vector<AuraColor>  aura_colors;
        std::vector<RGBColor>&  color_set = colors;

        if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            color_set = modes[active_mode].colors;
        }

        for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
        {
            AuraColor new_color;

            new_color.red   = RGBGetRValue(color_set[led_idx]);
            new_color.green = RGBGetGValue(color_set[led_idx]);
            new_color.blue  = RGBGetBValue(color_set[led_idx]);

            aura_colors.push_back(new_color);
        }

        controller->UpdateDirect(aura_colors);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
        {
            DeviceUpdateSingleLED(led_idx);
        }
    }
    else
    {
        DeviceUpdateSingleLED(0);
    }
}

void RGBController_AuraCore::DeviceUpdateSingleLED(int led)
{
    unsigned char speed = 0xFF;
    unsigned char red   = 0;
    unsigned char green = 0;
    unsigned char blue  = 0;
    unsigned char dir   = 0;
    mode& curr_mode     = modes[active_mode];

    if(curr_mode.color_mode == MODE_COLORS_PER_LED)
    {
        red     = RGBGetRValue(colors[led]);
        green   = RGBGetGValue(colors[led]);
        blue    = RGBGetBValue(colors[led]);
    }
    else if(curr_mode.color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        red     = RGBGetRValue(curr_mode.colors[led]);
        green   = RGBGetGValue(curr_mode.colors[led]);
        blue    = RGBGetBValue(curr_mode.colors[led]);
    }

    if(curr_mode.flags & MODE_FLAG_HAS_SPEED)
    {
        speed   = curr_mode.speed;
    }

    if(curr_mode.flags & MODE_FLAG_HAS_DIRECTION_LR)
    {
        if(curr_mode.direction == MODE_DIRECTION_RIGHT)
        {
            dir = 1;
        }
    }

    controller->SendUpdate
            (
            led,
            curr_mode.value,
            speed,
            dir,
            red,
            green,
            blue
            );

    controller->SendSet();
    controller->SendApply();
}

void RGBController_AuraCore::DeviceUpdateMode()
{
    if(modes[active_mode].value == AURA_CORE_MODE_DIRECT)
    {
        controller->InitDirectMode();
    }
    else
    {
        DeviceUpdateLEDs();
    }
}
