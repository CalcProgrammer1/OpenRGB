/*---------------------------------------------------------*\
| RGBController_AirgooLustrousCommander.cpp                 |
|                                                           |
|   RGBController for Airgoo Lustrous Commander             |
|                                                           |
|   Zacahry Guinn                               07 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AirgooLustrousCommander.h"

RGBController_AirgooLustrousCommander::RGBController_AirgooLustrousCommander(AirgooLustrousCommanderController* controller_ptr)
{
    controller  = controller_ptr;

    vendor      = "Airgoo";
    description = "Lustrous Commander";
    type        = DEVICE_TYPE_LEDSTRIP;
    location    = controller->GetLocationString();

    mode Static;
    Static.name       = "Static";
    Static.value      = 1;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    // mode Shift;
    // Shift.name       = "Shift";
    // Shift.value      = 1;
    // Shift.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    // Shift.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    // Shift.colors_min           = 1;
    // Shift.colors_max           = 16;
    // Shift.colors.resize(Shift.colors_max);
    // Shift.speed_min            = 0;
    // Shift.speed_max            = 255;
    // Shift.speed                = 05;
    // Shift.brightness_min       = 100;
    // Shift.brightness_max       = 0;
    // Shift.brightness           = 0;
    // modes.push_back(Shift);

    // mode Pulse;
    // Pulse.name       = "Pulse";
    // Pulse.value      = 2;
    // Pulse.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    // Pulse.color_mode = MODE_COLORS_MODE_SPECIFIC;
    // modes.push_back(Pulse);

    // mode Gradual;
    // Gradual.name       = "Gradual";
    // Gradual.value      = 3;
    // Gradual.flags      = MODE_FLAstd::cout << "@@@@@@@Init Contoller@@@@@@@" << std::endl;ORS_MODE_SPECIFIC;
    // modes.push_back(Gradual);

    // mode StarLight;
    // StarLight.name       = "StarLight";
    // StarLight.value      = 4;
    // StarLight.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    // StarLight.color_mode = MODE_COLORS_MODE_SPECIFIC;
    // modes.push_back(StarLight);

    mode Rainbow;
    Rainbow.name       = "Rainbow";
    Rainbow.value      = 2;
    Rainbow.flags     = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min            = 1;
    Rainbow.speed_max            = 255;
    Rainbow.speed                = 50;
    Rainbow.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    // mode ColorWave;
    // ColorWave.name       = "Color Wave";
    // ColorWave.value      = 6;
    // ColorWave.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    // ColorWave.color_mode = MODE_COLORS_MODE_SPECIFIC;
    // modes.push_back(ColorWave);

    // mode ColorPulse;
    // ColorPulse.name       = "Color Pulse";
    // ColorPulse.value      = 7;
    // ColorPulse.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    // ColorPulse.color_mode = MODE_COLORS_MODE_SPECIFIC;
    // modes.push_back(ColorPulse);

    // mode ColorMeteor;
    // ColorMeteor.name       = "Color Meteor";
    // ColorMeteor.value      = 8;
    // ColorMeteor.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    // ColorMeteor.color_mode = MODE_COLORS_MODE_SPECIFIC;
    // modes.push_back(ColorMeteor);

    SetupZones();
}

RGBController_AirgooLustrousCommander::~RGBController_AirgooLustrousCommander()
{
    delete controller;
}

void RGBController_AirgooLustrousCommander::SetupZones()
{
    std::atomic<bool> first_run;
    first_run = 0;

    if(zones.size() == 0)
    {
        first_run = 1;
    }

    zones.resize(3);

    for(unsigned int i = 1; i < (AIRGOO_LUSTROUS_COMMANDER_NUM_RGB_CHANNELS + 1); i++)
    {
        zones[i].name               = "RGB Port " + std::to_string(i);
        zones[i].type               = ZONE_TYPE_LINEAR;
        zones[i].leds_min           = 0;
        zones[i].leds_max           = 34;

        if(first_run)
        {
            zones[i].leds_count     = 0;
        }
    }

    leds.clear();
    colors.clear();

    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for (unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name            = zones[zone_idx].name + " LED " + std::to_string(led_idx+1);

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_AirgooLustrousCommander::ResizeZone(int zone, int new_size)
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

void RGBController_AirgooLustrousCommander::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_AirgooLustrousCommander::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AirgooLustrousCommander::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AirgooLustrousCommander::DeviceUpdateMode()
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;
    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }
    controller->UpdateDevice(modes[active_mode].value, red, grn, blu, modes[active_mode].speed);
}
