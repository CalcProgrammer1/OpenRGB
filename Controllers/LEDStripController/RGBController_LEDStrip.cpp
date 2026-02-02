/*---------------------------------------------------------*\
| RGBController_LEDStrip.cpp                                |
|                                                           |
|   RGBController for serial LED strips                     |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      20 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_LEDStrip.h"

/**------------------------------------------------------------------*\
    @name Serial LED Strip
    @category LEDStrip
    @type Serial
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectLEDStripControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_LEDStrip::RGBController_LEDStrip(LEDStripController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetName();
    type                = DEVICE_TYPE_LEDSTRIP;
    description         = "Serial LED Strip Device";
    location            = controller->GetLocation();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_LEDStrip::~RGBController_LEDStrip()
{
    Shutdown();

    delete controller;
}

void RGBController_LEDStrip::SetupZones()
{
    zone led_zone;
    led_zone.name       = "LED Strip";
    led_zone.type       = ZONE_TYPE_LINEAR;
    led_zone.leds_min   = controller->num_leds;
    led_zone.leds_max   = controller->num_leds;
    led_zone.leds_count = controller->num_leds;
    zones.push_back(led_zone);

    for(int led_idx = 0; led_idx < controller->num_leds; led_idx++)
    {
        led new_led;
        new_led.name    = "LED ";
        new_led.name.append(std::to_string(led_idx));

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_LEDStrip::DeviceUpdateLEDs()
{
    controller->SetLEDs(colors);
}

void RGBController_LEDStrip::DeviceUpdateZoneLEDs(int /*zone*/)
{
    controller->SetLEDs(colors);
}

void RGBController_LEDStrip::DeviceUpdateSingleLED(int /*led*/)
{
    controller->SetLEDs(colors);
}

void RGBController_LEDStrip::DeviceUpdateMode()
{

}
