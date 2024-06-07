/*---------------------------------------------------------*\
| RGBController_RazerAddressable.cpp                        |
|                                                           |
|   RGBController for Razer ARGB Controller                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    delete controller;
}

void RGBController_RazerAddressable::SetupZones()
{
    unsigned int device_index = controller->GetDeviceIndex();
    unsigned int zone_count   = 0;

    /*-------------------------------------------------*\
    | Only set LED count on the first run               |
    \*-------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-------------------------------------------------*\
    | Count the number of zones for this device         |
    \*-------------------------------------------------*/
    for(unsigned int zone_id = 0; zone_id < RAZER_MAX_ZONES; zone_id++)
    {
        if(device_list[device_index]->zones[zone_id] != NULL)
        {
            zone_count++;
        }
    }

    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(zone_count);

    /*---------------------------------------------------------*\
    | Fill in zone information based on device table            |
    \*---------------------------------------------------------*/
    zone_count = 0;

    for(unsigned int zone_id = 0; zone_id < RAZER_MAX_ZONES; zone_id++)
    {
        if(device_list[device_index]->zones[zone_id] != NULL)
        {
            zones[zone_count].name          = device_list[device_index]->zones[zone_id]->name;
            zones[zone_count].type          = device_list[device_index]->zones[zone_id]->type;

            zones[zone_count].leds_min      = 0;
            zones[zone_count].leds_max      = device_list[device_index]->zones[zone_id]->rows * device_list[device_index]->zones[zone_id]->cols;

            if(first_run)
            {
                zones[zone_count].leds_count = 0;
            }

            if(zones[zone_count].type == ZONE_TYPE_MATRIX)
            {
                matrix_map_type * new_map       = new matrix_map_type;
                zones[zone_count].matrix_map    = new_map;

                new_map->height                 = device_list[device_index]->zones[zone_id]->rows;
                new_map->width                  = device_list[device_index]->zones[zone_id]->cols;

                new_map->map                    = new unsigned int[new_map->height * new_map->width];

                for(unsigned int y = 0; y < new_map->height; y++)
                {
                    for(unsigned int x = 0; x < new_map->width; x++)
                    {
                        new_map->map[(y * new_map->width) + x] = (y * new_map->width) + x;
                    }
                }
            }
            else
            {
                zones[zone_count].matrix_map = NULL;
            }

            zone_count++;
        }
    }

    for(unsigned int zone_id = 0; zone_id < zones.size(); zone_id++)
    {
        for(unsigned int led_id = 0; led_id < zones[zone_id].leds_count; led_id++)
        {
            led new_led;
            new_led.name = "Channel " + std::to_string(zone_id + 1) + ", LED " + std::to_string(led_id + 1);

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_RazerAddressable::ResizeZone(int zone, int new_size)
{
    /*---------------------------------------------------------*\
    | Only the Razer Chroma Addressable RGB Controller supports |
    | zone resizing                                             |
    \*---------------------------------------------------------*/
    if((size_t) zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

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
    /*---------------------------------------------------------*\
    | Only the Razer Chroma Addressable RGB Controller supports |
    | zone resizing                                             |
    \*---------------------------------------------------------*/
    RGBColor    colors_buf[80 * 6];

    for(unsigned int zone_id = 0; zone_id < zones.size(); zone_id++)
    {
        memcpy(&colors_buf[(80 * zone_id)], zones[zone_id].colors, sizeof(RGBColor) * zones[zone_id].leds_count);
    }

    controller->SetLEDs(&colors_buf[0]);
}

void RGBController_RazerAddressable::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RazerAddressable::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RazerAddressable::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
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
