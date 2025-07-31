/*---------------------------------------------------------*\
| RGBController_AMBX.cpp                                    |
|                                                           |
|   RGB Controller for Philips amBX Gaming lights           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AMBX.h"

/**------------------------------------------------------------------*\
    @name Philips amBX
    @category Accessory
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectAMBXControllers
    @comment The Philips amBX Gaming lights system includes left and right
    lights and a wall-washer bar with three zones.
\*-------------------------------------------------------------------*/

RGBController_AMBX::RGBController_AMBX(AMBXController* controller_ptr)
{
    controller = controller_ptr;

    name        = "Philips amBX";
    vendor      = "Philips";
    type        = DEVICE_TYPE_ACCESSORY;
    description = "Philips amBX Gaming Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_AMBX::~RGBController_AMBX()
{
    delete controller;
}

void RGBController_AMBX::SetupZones()
{
    // Set up zones
    zone side_lights_zone;
    side_lights_zone.name       = "Side Lights";
    side_lights_zone.type       = ZONE_TYPE_LINEAR;
    side_lights_zone.leds_min   = 2;
    side_lights_zone.leds_max   = 2;
    side_lights_zone.leds_count = 2;
    side_lights_zone.matrix_map = NULL;
    zones.push_back(side_lights_zone);

    zone wallwasher_zone;
    wallwasher_zone.name       = "Wallwasher";
    wallwasher_zone.type       = ZONE_TYPE_LINEAR;
    wallwasher_zone.leds_min   = 3;
    wallwasher_zone.leds_max   = 3;
    wallwasher_zone.leds_count = 3;
    wallwasher_zone.matrix_map = NULL;
    zones.push_back(wallwasher_zone);

    // Set up LEDs
    led left_light;
    left_light.name  = "Left";
    left_light.value = AMBX_LIGHT_LEFT;
    leds.push_back(left_light);

    led right_light;
    right_light.name  = "Right";
    right_light.value = AMBX_LIGHT_RIGHT;
    leds.push_back(right_light);

    led wall_left;
    wall_left.name  = "Wall Left";
    wall_left.value = AMBX_LIGHT_WALL_LEFT;
    leds.push_back(wall_left);

    led wall_center;
    wall_center.name  = "Wall Center";
    wall_center.value = AMBX_LIGHT_WALL_CENTER;
    leds.push_back(wall_center);

    led wall_right;
    wall_right.name  = "Wall Right";
    wall_right.value = AMBX_LIGHT_WALL_RIGHT;
    leds.push_back(wall_right);

    SetupColors();
}

void RGBController_AMBX::ResizeZone(int /*zone*/, int /*new_size*/)
{
    // This device does not support resizing zones
}

void RGBController_AMBX::DeviceUpdateLEDs()
{
    if(!controller->IsInitialized())
    {
        return;
    }

    unsigned int led_values[5];
    RGBColor led_colors[5];

    for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        led_values[led_idx] = leds[led_idx].value;
        led_colors[led_idx] = colors[led_idx];
    }

    controller->SetLEDColors(led_values, led_colors, static_cast<unsigned int>(leds.size()));
}

void RGBController_AMBX::UpdateZoneLEDs(int zone)
{
    if(!controller->IsInitialized())
    {
        return;
    }

    unsigned int start_idx = 0;
    unsigned int zone_size = 0;

    // Calculate start index and size
    for(unsigned int z_idx = 0; z_idx < zones.size(); z_idx++)
    {
        if(z_idx == (unsigned int)zone)
        {
            zone_size = zones[z_idx].leds_count;
            break;
        }

        start_idx += zones[z_idx].leds_count;
    }

    unsigned int led_values[5];
    RGBColor led_colors[5];

    for(unsigned int led_idx = 0; led_idx < zone_size; led_idx++)
    {
        unsigned int current_idx = start_idx + led_idx;
        led_values[led_idx] = leds[current_idx].value;
        led_colors[led_idx] = colors[current_idx];
    }

    controller->SetLEDColors(led_values, led_colors, zone_size);
}

void RGBController_AMBX::UpdateSingleLED(int led)
{
    if(!controller->IsInitialized())
    {
        return;
    }

    unsigned int led_value = leds[led].value;
    RGBColor color = colors[led];
    controller->SetLEDColor(led_value, color);
}

void RGBController_AMBX::DeviceUpdateMode()
{
    if(!controller->IsInitialized())
    {
        return;
    }

    DeviceUpdateLEDs();
}
