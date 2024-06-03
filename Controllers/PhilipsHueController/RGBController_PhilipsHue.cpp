/*---------------------------------------------------------*\
| RGBController_PhilipsHue.cpp                              |
|                                                           |
|   RGBController for Philips Hue                           |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_PhilipsHue.h"

/**------------------------------------------------------------------*\
    @name Philips Hue
    @category Light
    @type Network
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectPhilipsHueControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PhilipsHue::RGBController_PhilipsHue(PhilipsHueController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetManufacturer() + " " + controller->GetName();
    type        = DEVICE_TYPE_LIGHT;
    version     = controller->GetVersion();
    description = "Philips Hue Device";
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

void RGBController_PhilipsHue::SetupZones()
{
    zone led_zone;
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

    SetupColors();
}

void RGBController_PhilipsHue::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PhilipsHue::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SetColor(red, grn, blu);
}

void RGBController_PhilipsHue::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PhilipsHue::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PhilipsHue::DeviceUpdateMode()
{

}
