/*---------------------------------------------------------*\
| RGBController_LIFX.cpp                                    |
|                                                           |
|   RGBController for LIFX                                  |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      05 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_LIFX.h"

/**------------------------------------------------------------------*\
    @name LIFX Globes
    @category Light
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectLIFXControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_LIFX::RGBController_LIFX(LIFXController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetManufacturer() + " " + controller->GetName();
    vendor      = controller->GetManufacturer();
    type        = DEVICE_TYPE_LIGHT;
    version     = controller->GetVersion();
    description = "LIFX Device";
    serial      = controller->GetUniqueID();
    location    = controller->GetLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_LIFX::~RGBController_LIFX()
{
    delete controller;
}

void RGBController_LIFX::SetupZones()
{
    zone led_zone;

    unsigned int zone_count = controller->GetZoneCount();

    /*---------------------------------------------------------*\
    | If there is only one zone, set up a single LED            |
    \*---------------------------------------------------------*/
    if(zone_count <= 1)
    {
        led_zone.name       = "RGB Light";
        led_zone.type       = ZONE_TYPE_SINGLE;
        led_zone.leds_min   = 1;
        led_zone.leds_max   = 1;
        led_zone.leds_count = 1;
        led_zone.matrix_map = NULL;
        zones.push_back(led_zone);

        led new_led;
        new_led.name = "RGB Light";

        leds.push_back(new_led);
    }
    else
    {
        /*---------------------------------------------------------*\
        | Set up multiple LEDs                                      |
        \*---------------------------------------------------------*/
        led_zone.name = "RGB Light Strip";
        led_zone.type = ZONE_TYPE_LINEAR;
        led_zone.leds_min = 1;
        led_zone.leds_max = zone_count;
        led_zone.leds_count = zone_count;
        led_zone.matrix_map = NULL;

        zones.push_back(led_zone);

        for(size_t zone_idx = 0; zone_idx < zone_count; zone_idx++)
        {

            /*---------------------------------------------------------*\
            | Set up LEDs                                               |
            \*---------------------------------------------------------*/
            led new_led;

            new_led.name = "LED " + std::to_string(zone_idx);

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_LIFX::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LIFX::DeviceUpdateLEDs()
{
    controller->SetColors(colors);
}

void RGBController_LIFX::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LIFX::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LIFX::DeviceUpdateMode()
{

}
