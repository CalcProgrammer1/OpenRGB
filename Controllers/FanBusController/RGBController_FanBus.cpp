/*---------------------------------------------------------*\
| RGBController_FanBus.cpp                                  |
|                                                           |
|   RGBController for FanBus devices                        |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_FanBus.h"

/**------------------------------------------------------------------*\
    @name FanBus
    @category Cooler
    @type Serial
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectFanBusControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_FanBus::RGBController_FanBus(FanBusController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "FanBus Device";
    type        = DEVICE_TYPE_COOLER;
    description = "FanBus Device";
    location    = controller->GetLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_FanBus::~RGBController_FanBus()
{
    delete controller;
}

void RGBController_FanBus::SetupZones()
{
    zone led_zone;
    led_zone.name       = "Fan LEDs";
    led_zone.type       = ZONE_TYPE_SINGLE;
    led_zone.leds_min   = 4;
    led_zone.leds_max   = 4;
    led_zone.leds_count = 4;
    led_zone.matrix_map = NULL;
    zones.push_back(led_zone);

    for(unsigned int led_idx = 0; led_idx < led_zone.leds_count; led_idx++)
    {
        led new_led;
        new_led.name = "LED ";
        new_led.name.append(std::to_string(led_idx));

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_FanBus::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_FanBus::DeviceUpdateLEDs()
{
    controller->SetLEDs(colors);
}

void RGBController_FanBus::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetLEDs(colors);
}

void RGBController_FanBus::UpdateSingleLED(int /*led*/)
{
    controller->SetLEDs(colors);
}

void RGBController_FanBus::DeviceUpdateMode()
{

}
