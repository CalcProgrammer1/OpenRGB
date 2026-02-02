/*---------------------------------------------------------*\
| RGBController_Espurna.cpp                                 |
|                                                           |
|   RGBController for Espurna                               |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      11 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_Espurna.h"

/**------------------------------------------------------------------*\
    @name Espurna
    @category Light
    @type TCP
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectEspurnaControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_Espurna::RGBController_Espurna(EspurnaController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Espurna";
    type        = DEVICE_TYPE_LIGHT;
    description = "Espurna Device";
    location    = controller->GetLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_Espurna::~RGBController_Espurna()
{
    Shutdown();

    delete controller;
}

void RGBController_Espurna::SetupZones()
{
    zone led_zone;
    led_zone.name       = "RGB Light";
    led_zone.type       = ZONE_TYPE_SINGLE;
    led_zone.leds_min   = 1;
    led_zone.leds_max   = 1;
    led_zone.leds_count = 1;
    zones.push_back(led_zone);

    led new_led;
    new_led.name = "RGB Light";

    leds.push_back(new_led);

    SetupColors();
}

void RGBController_Espurna::DeviceUpdateLEDs()
{
    controller->SetLEDs(colors);
}

void RGBController_Espurna::DeviceUpdateZoneLEDs(int /*zone*/)
{
    controller->SetLEDs(colors);
}

void RGBController_Espurna::DeviceUpdateSingleLED(int /*led*/)
{
    controller->SetLEDs(colors);
}

void RGBController_Espurna::DeviceUpdateMode()
{

}
