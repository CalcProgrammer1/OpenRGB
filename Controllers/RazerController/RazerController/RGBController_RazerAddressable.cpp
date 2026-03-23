/*---------------------------------------------------------*\
| RGBController_RazerAddressable.cpp                        |
|                                                           |
|   RGBController for Razer ARGB Controller                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string.h>
#include "RGBController_RazerAddressable.h"
#include "RazerDevices.h"

/**------------------------------------------------------------------*\
    @name Razer ARGB
    @category LEDStrip
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRazerARGBControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RazerAddressable::RGBController_RazerAddressable(RazerController* controller_ptr)
{
    controller      = controller_ptr;

    name            = controller->GetName();
    vendor          = "Razer";
    type            = controller->GetDeviceType();
    description     = "Razer Addressable Device";
    location        = controller->GetDeviceLocation();
    version         = controller->GetFirmwareString();
    serial          = controller->GetSerialString();

    mode Direct;
    Direct.name           = "Direct";
    Direct.value          = RAZER_ADDRESSABLE_MODE_DIRECT;
    Direct.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode     = MODE_COLORS_PER_LED;
    Direct.brightness_min = 0;
    Direct.brightness_max = 255;
    Direct.brightness     = 255;
    modes.push_back(Direct);

    mode Off;
    Off.name       = "Off";
    Off.value      = RAZER_ADDRESSABLE_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name           = "Static";
    Static.value          = RAZER_ADDRESSABLE_MODE_STATIC;
    Static.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min     = 1;
    Static.colors_max     = 1;
    Static.colors.resize(1);
    Static.brightness_min = 0;
    Static.brightness_max = 255;
    Static.brightness     = 255;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name           = "Breathing";
    Breathing.value          = RAZER_ADDRESSABLE_MODE_BREATHING;
    Breathing.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min     = 1;
    Breathing.colors_max     = 2;
    Breathing.colors.resize(1);
    Breathing.brightness_min = 0;
    Breathing.brightness_max = 255;
    Breathing.brightness     = 255;
    modes.push_back(Breathing);

    mode SpectrumCycle;
    SpectrumCycle.name           = "Spectrum Cycle";
    SpectrumCycle.value          = RAZER_ADDRESSABLE_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags          = MODE_FLAG_HAS_BRIGHTNESS;
    SpectrumCycle.color_mode     = MODE_COLORS_NONE;
    SpectrumCycle.brightness_min = 0;
    SpectrumCycle.brightness_max = 255;
    SpectrumCycle.brightness     = 255;
    modes.push_back(SpectrumCycle);

    if(controller->SupportsWave())
    {
        mode Wave;
        Wave.name           = "Wave";
        Wave.value          = RAZER_ADDRESSABLE_MODE_WAVE;
        Wave.flags          = MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_BRIGHTNESS;
        Wave.direction      = MODE_DIRECTION_RIGHT;
        Wave.color_mode     = MODE_COLORS_NONE;
        Wave.brightness_min = 0;
        Wave.brightness_max = 255;
        Wave.brightness     = 255;
        modes.push_back(Wave);
    }

    if(controller->SupportsReactive())
    {
        mode Reactive;
        Reactive.name           = "Reactive";
        Reactive.value          = RAZER_ADDRESSABLE_MODE_REACTIVE;
        Reactive.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Reactive.color_mode     = MODE_COLORS_MODE_SPECIFIC;
        Reactive.colors_min     = 1;
        Reactive.colors_max     = 1;
        Reactive.colors.resize(1);
        Reactive.brightness_min = 0;
        Reactive.brightness_max = 255;
        Reactive.brightness     = 255;
        modes.push_back(Reactive);
    }

    SetupZones();
}

RGBController_RazerAddressable::~RGBController_RazerAddressable()
{
    Shutdown();

    delete controller;
}

void RGBController_RazerAddressable::SetupZones()
{
    unsigned int device_index = controller->GetDeviceIndex();

    /*-----------------------------------------------------*\
    | Only set LED count on the first run                   |
    \*-----------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-----------------------------------------------------*\
    | Clear any existing color/LED configuration            |
    \*-----------------------------------------------------*/
    leds.clear();
    colors.clear();

    /*-----------------------------------------------------*\
    | Count number of zones to resize zones vector          |
    \*-----------------------------------------------------*/
    unsigned int num_zones;

    for(num_zones = 0; num_zones < RAZER_MAX_ZONES; num_zones++)
    {
        if(!device_list[device_index]->zones[num_zones])
        {
            break;
        }
    }

    zones.resize(num_zones);

    /*-----------------------------------------------------*\
    | Set up zones                                          |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].leds_min                    = 0;
        zones[zone_idx].leds_max                    = device_list[device_index]->zones[zone_idx]->rows * device_list[device_index]->zones[zone_idx]->cols;;

        if(first_run)
        {
            zones[zone_idx].flags                   = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
        {
            zones[zone_idx].name                    = device_list[device_index]->zones[zone_idx]->name;;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
        {
            zones[zone_idx].leds_count              = 0;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
        {
            zones[zone_idx].type                    = device_list[device_index]->zones[zone_idx]->type;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
        {
            if(device_list[device_index]->zones[zone_idx]->type == ZONE_TYPE_MATRIX)
            {
                zones[zone_idx].matrix_map.height = device_list[device_index]->zones[zone_idx]->rows;
                zones[zone_idx].matrix_map.width  = device_list[device_index]->zones[zone_idx]->cols;
                zones[zone_idx].matrix_map.map.resize(zones[zone_idx].matrix_map.height * zones[zone_idx].matrix_map.width);

                for(unsigned int y = 0; y < zones[zone_idx].matrix_map.height; y++)
                {
                    for(unsigned int x = 0; x < zones[zone_idx].matrix_map.width; x++)
                    {
                        zones[zone_idx].matrix_map.map[(y * zones[zone_idx].matrix_map.width) + x] = (y * zones[zone_idx].matrix_map.width) + x;
                    }
                }
            }
            else
            {
                zones[zone_idx].matrix_map.width        = 0;
                zones[zone_idx].matrix_map.height       = 0;
                zones[zone_idx].matrix_map.map.resize(0);
            }
        }

        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name = zones[zone_idx].name + + ", LED " + std::to_string(led_idx + 1);

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_RazerAddressable::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        controller->SetAddressableZoneSizes(zones[0].leds_count,
                                            zones[1].leds_count,
                                            zones[2].leds_count,
                                            zones[3].leds_count,
                                            zones[4].leds_count,
                                            zones[5].leds_count);

        SetupZones();
    }
}

void RGBController_RazerAddressable::DeviceUpdateLEDs()
{
    RGBColor    colors_buf[80 * 6];

    for(unsigned int zone_id = 0; zone_id < zones.size(); zone_id++)
    {
        memcpy(&colors_buf[(80 * zone_id)], zones[zone_id].colors, sizeof(RGBColor) * zones[zone_id].leds_count);
    }

    controller->SetLEDs(&colors_buf[0]);
}

void RGBController_RazerAddressable::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RazerAddressable::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RazerAddressable::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case RAZER_ADDRESSABLE_MODE_DIRECT:
            /*---------------------------------------------------------*\
            | Controller does not preserve the LEDs for direct mode.    |
            | We have to restore them.                                  |
            \*---------------------------------------------------------*/
            DeviceUpdateLEDs();
            break;
        case RAZER_ADDRESSABLE_MODE_OFF:
            controller->SetModeOff();
            break;

        case RAZER_ADDRESSABLE_MODE_STATIC:
            if(modes[active_mode].colors.size() == 1)
            {
                unsigned char red = RGBGetRValue(modes[active_mode].colors[0]);
                unsigned char grn = RGBGetGValue(modes[active_mode].colors[0]);
                unsigned char blu = RGBGetBValue(modes[active_mode].colors[0]);

                controller->SetModeStatic(red, grn, blu);
            }
            break;

        case RAZER_ADDRESSABLE_MODE_BREATHING:
            if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
            {
                controller->SetModeBreathingRandom();
            }
            else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                if(modes[active_mode].colors.size() == 1)
                {
                    unsigned char red = RGBGetRValue(modes[active_mode].colors[0]);
                    unsigned char grn = RGBGetGValue(modes[active_mode].colors[0]);
                    unsigned char blu = RGBGetBValue(modes[active_mode].colors[0]);

                    controller->SetModeBreathingOneColor(red, grn, blu);
                }
                else if(modes[active_mode].colors.size() == 2)
                {
                    unsigned char red1 = RGBGetRValue(modes[active_mode].colors[0]);
                    unsigned char grn1 = RGBGetGValue(modes[active_mode].colors[0]);
                    unsigned char blu1 = RGBGetBValue(modes[active_mode].colors[0]);
                    unsigned char red2 = RGBGetRValue(modes[active_mode].colors[1]);
                    unsigned char grn2 = RGBGetGValue(modes[active_mode].colors[1]);
                    unsigned char blu2 = RGBGetBValue(modes[active_mode].colors[1]);

                    controller->SetModeBreathingTwoColors(red1, grn1, blu1, red2, grn2, blu2);
                }
            }
            break;

        case RAZER_ADDRESSABLE_MODE_SPECTRUM_CYCLE:
            controller->SetModeSpectrumCycle();
            break;

        case RAZER_ADDRESSABLE_MODE_WAVE:
            switch(modes[active_mode].direction)
            {
                case MODE_DIRECTION_LEFT:
                    controller->SetModeWave(2);
                    break;

                default:
                    controller->SetModeWave(1);
                    break;
            }
            break;
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        controller->SetBrightness(modes[active_mode].brightness);
    }
    else
    {
        controller->SetBrightness(255);
    }
}
