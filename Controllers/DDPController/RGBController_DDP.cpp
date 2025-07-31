/*---------------------------------------------------------*\
| RGBController_DDP.cpp                                     |
|                                                           |
|   RGBController for DDP devices                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_DDP.h"

/**------------------------------------------------------------------*\
    @name DDP Devices
    @category LEDStrip
    @type Network
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectDDPControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_DDP::RGBController_DDP(std::vector<DDPDevice> device_list)
{
    devices = device_list;
    name = "DDP Device Group";
    type = DEVICE_TYPE_LEDSTRIP;
    description = "Distributed Display Protocol Device";
    location = "DDP: ";

    if(devices.size() == 1)
        name = devices[0].name;
    else if(!devices[0].ip.empty())
        name += " (" + devices[0].ip + ")";

    if(!devices[0].ip.empty())
        location += devices[0].ip + ":" + std::to_string(devices[0].port);

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    Direct.brightness_min = 0;
    Direct.brightness_max = 100;
    Direct.brightness     = 100;
    modes.push_back(Direct);

    controller = new DDPController(devices);
    SetupZones();
}

RGBController_DDP::~RGBController_DDP()
{
    delete controller;
}

void RGBController_DDP::SetupZones()
{
    for(unsigned int zone_idx = 0; zone_idx < devices.size(); zone_idx++)
    {
        zone led_zone;
        led_zone.name           = devices[zone_idx].name;
        led_zone.type           = ZONE_TYPE_LINEAR;
        led_zone.leds_min       = devices[zone_idx].num_leds;
        led_zone.leds_max       = devices[zone_idx].num_leds;
        led_zone.leds_count     = devices[zone_idx].num_leds;
        led_zone.matrix_map     = NULL;
        zones.push_back(led_zone);
    }

    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name = zones[zone_idx].name + " LED " + std::to_string(led_idx + 1);
            new_led.value = 0;
            leds.push_back(new_led);
        }
    }
    SetupColors();
}

void RGBController_DDP::ResizeZone(int /*zone*/, int /*new_size*/)
{
}

void RGBController_DDP::DeviceUpdateLEDs()
{
    std::vector<unsigned int> brightness_adjusted_colors;
    brightness_adjusted_colors.reserve(colors.size());
    float brightness_scale = (float)modes[active_mode].brightness / 100.0f;

    for(unsigned int color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        unsigned int color = colors[color_idx];
        unsigned char r = color & 0xFF;
        unsigned char g = (color >> 8) & 0xFF;
        unsigned char b = (color >> 16) & 0xFF;
        r = (unsigned char)(r * brightness_scale);
        g = (unsigned char)(g * brightness_scale);
        b = (unsigned char)(b * brightness_scale);
        unsigned int adjusted_color = r | (g << 8) | (b << 16);
        brightness_adjusted_colors.push_back(adjusted_color);
    }

    controller->UpdateLEDs(brightness_adjusted_colors);
}

void RGBController_DDP::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_DDP::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_DDP::DeviceUpdateMode()
{
}

void RGBController_DDP::SetKeepaliveTime(unsigned int time_ms)
{
    if(controller != nullptr)
    {
        controller->SetKeepaliveTime(time_ms);
    }
}
