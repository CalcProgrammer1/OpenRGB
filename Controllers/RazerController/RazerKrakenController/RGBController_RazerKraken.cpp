/*---------------------------------------------------------*\
| RGBController_RazerKraken.cpp                             |
|                                                           |
|   RGBController for Razer Kraken                          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                28 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_RazerKraken.h"
#include "RazerDevices.h"

/**------------------------------------------------------------------*\
    @name Razer Kraken
    @category Headset
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRazerKrakenControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RazerKraken::RGBController_RazerKraken(RazerKrakenController* controller_ptr)
{
    controller      = controller_ptr;

    name            = controller->GetName();
    vendor          = "Razer";
    type            = controller->GetDeviceType();
    description     = "Razer Kraken Device";
    location        = controller->GetDeviceLocation();
    version         = controller->GetFirmwareString();
    serial          = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = RAZER_KRAKEN_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name       = "Off";
    Off.value      = RAZER_KRAKEN_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name       = "Static";
    Static.value      = RAZER_KRAKEN_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = RAZER_KRAKEN_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min = 1;
    /*---------------------------------------------------------*\
    | Razer Kraken 7.1 Chroma only does single color breathing  |
    \*---------------------------------------------------------*/
    if(device_list[controller->GetDeviceIndex()]->pid == RAZER_KRAKEN_PID)
    {
        Breathing.colors_max = 1;
    }
    else
    {
        Breathing.colors_max = 3;
    }
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Spectrum Cycle";
    SpectrumCycle.value      = RAZER_KRAKEN_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags      = 0;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    SetupZones();
}

RGBController_RazerKraken::~RGBController_RazerKraken()
{
    delete controller;
}

void RGBController_RazerKraken::SetupZones()
{
    unsigned int device_index = controller->GetDeviceIndex();

    /*---------------------------------------------------------*\
    | Fill in zone information based on device table            |
    \*---------------------------------------------------------*/
    for(unsigned int zone_id = 0; zone_id < RAZER_MAX_ZONES; zone_id++)
    {
        if(device_list[device_index]->zones[zone_id] != NULL)
        {
            zone new_zone;

            new_zone.name       = device_list[device_index]->zones[zone_id]->name;
            new_zone.type       = device_list[device_index]->zones[zone_id]->type;

            new_zone.leds_count = device_list[device_index]->zones[zone_id]->rows * device_list[device_index]->zones[zone_id]->cols;
            new_zone.leds_min   = new_zone.leds_count;
            new_zone.leds_max   = new_zone.leds_count;

            if(new_zone.type == ZONE_TYPE_MATRIX)
            {
                matrix_map_type * new_map = new matrix_map_type;
                new_zone.matrix_map = new_map;

                new_map->height = device_list[device_index]->zones[zone_id]->rows;
                new_map->width  = device_list[device_index]->zones[zone_id]->cols;

                new_map->map = new unsigned int[new_map->height * new_map->width];

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
                new_zone.matrix_map = NULL;
            }

            zones.push_back(new_zone);
        }
    }

    for(unsigned int zone_id = 0; zone_id < zones.size(); zone_id++)
    {
        for (unsigned int row_id = 0; row_id < device_list[device_index]->zones[zone_id]->rows; row_id++)
        {
            for (unsigned int col_id = 0; col_id < device_list[device_index]->zones[zone_id]->cols; col_id++)
            {
                led* new_led = new led();

                new_led->name = device_list[device_index]->zones[zone_id]->name;

                if(zones[zone_id].leds_count > 1)
                {
                    new_led->name.append(" LED ");
                    new_led->name.append(std::to_string(col_id + 1));
                }

                leds.push_back(*new_led);
            }
        }
    }

    SetupColors();
}

void RGBController_RazerKraken::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RazerKraken::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SetModeCustom(red, grn, blu);
}

void RGBController_RazerKraken::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RazerKraken::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RazerKraken::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case RAZER_KRAKEN_MODE_OFF:
            controller->SetModeOff();
            break;

        case RAZER_KRAKEN_MODE_STATIC:
            if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                if(modes[active_mode].colors.size() == 1)
                {
                    unsigned char red = RGBGetRValue(modes[active_mode].colors[0]);
                    unsigned char grn = RGBGetGValue(modes[active_mode].colors[0]);
                    unsigned char blu = RGBGetBValue(modes[active_mode].colors[0]);

                    controller->SetModeStatic(red, grn, blu);
                }
            }
            break;

        case RAZER_KRAKEN_MODE_BREATHING:
            if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
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
                else if(modes[active_mode].colors.size() == 3)
                {
                    unsigned char red1 = RGBGetRValue(modes[active_mode].colors[0]);
                    unsigned char grn1 = RGBGetGValue(modes[active_mode].colors[0]);
                    unsigned char blu1 = RGBGetBValue(modes[active_mode].colors[0]);
                    unsigned char red2 = RGBGetRValue(modes[active_mode].colors[1]);
                    unsigned char grn2 = RGBGetGValue(modes[active_mode].colors[1]);
                    unsigned char blu2 = RGBGetBValue(modes[active_mode].colors[1]);
                    unsigned char red3 = RGBGetRValue(modes[active_mode].colors[2]);
                    unsigned char grn3 = RGBGetGValue(modes[active_mode].colors[2]);
                    unsigned char blu3 = RGBGetBValue(modes[active_mode].colors[2]);

                    controller->SetModeBreathingThreeColors(red1, grn1, blu1, red2, grn2, blu2, red3, grn3, blu3);
                }
            }
            break;

        case RAZER_KRAKEN_MODE_SPECTRUM_CYCLE:
            controller->SetModeSpectrumCycle();
            break;
    }
}
