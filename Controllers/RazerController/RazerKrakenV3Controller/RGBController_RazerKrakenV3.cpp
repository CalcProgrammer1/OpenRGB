/*---------------------------------------------------------*\
| RGBController_RazerKrakenV3.cpp                           |
|                                                           |
|   RGBController for Razer devices with 13-byte reports    |
|                                                           |
|   Greg Sandstrom (superstrom)                  1 Nov 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "RazerDevices.h"
#include "RGBController_RazerKrakenV3.h"

RGBController_RazerKrakenV3::RGBController_RazerKrakenV3(RazerKrakenV3Controller* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetName();
    vendor                  = "Razer";
    type                    = controller->GetDeviceType();
    description             = "Razer Device";
    location                = controller->GetDeviceLocation();
    version                 = controller->GetFirmwareString();
    serial                  = controller->GetSerialString();
    uint8_t max_brightness  = controller->GetMaxBrightness();

    // By default, the device starts as Wave/Spectrum Cycle.
    // Set Wave as first mode, so switching to Direct calls DeviceUpdateMode()

    mode Wave;
    Wave.name               = "Wave";
    Wave.value              = RAZER_KRAKEN_V3_MODE_WAVE;
    Wave.flags              = MODE_FLAG_HAS_BRIGHTNESS;
    Wave.direction          = MODE_DIRECTION_RIGHT;
    Wave.color_mode         = MODE_COLORS_NONE;
    Wave.brightness_min     = 0;
    Wave.brightness_max     = max_brightness;
    Wave.brightness         = max_brightness;
    modes.push_back(Wave);


    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = RAZER_KRAKEN_V3_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness_min   = 0;
    Direct.brightness_max   = max_brightness;
    Direct.brightness       = max_brightness;
    modes.push_back(Direct);

    // V3 X does not support this mode.
    if(device_list[controller->GetDeviceIndex()]->pid == RAZER_KRAKEN_V3_HYPERSENSE_PID)
    {
        mode Breathing;
        Breathing.name       = "Breathing";
        Breathing.value      = RAZER_KRAKEN_V3_MODE_BREATHING;
        Breathing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
        Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
        Breathing.colors_min = 1;
        Breathing.colors_max = 2;
        Breathing.colors.resize(1);
        modes.push_back(Breathing);
    }

    SetupZones();
}

RGBController_RazerKrakenV3::~RGBController_RazerKrakenV3()
{
    delete controller;
}

void RGBController_RazerKrakenV3::SetupZones()
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

void RGBController_RazerKrakenV3::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_RazerKrakenV3::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == RAZER_KRAKEN_V3_MODE_DIRECT)
    {
        controller->SetDirect(&colors[0]);
    }
}

void RGBController_RazerKrakenV3::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RazerKrakenV3::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RazerKrakenV3::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case RAZER_KRAKEN_V3_MODE_DIRECT:
             controller->SetModeDirect();
             controller->SetBrightness(modes[active_mode].brightness);
             break;

        case RAZER_KRAKEN_V3_MODE_WAVE:
            controller->SetModeWave();
            controller->SetBrightness(modes[active_mode].brightness);
            break;

        case RAZER_KRAKEN_V3_MODE_BREATHING:
            controller->SetModeBreathing(modes[active_mode].colors);
            controller->SetBrightness(modes[active_mode].brightness);
            break;
    }
}
