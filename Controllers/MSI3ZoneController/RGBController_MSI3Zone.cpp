/*---------------------------------------------------------*\
| RGBController_MSI3Zone.cpp                                |
|                                                           |
|   RGBController for MSI/SteelSeries 3-Zone keyboard       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_MSI3Zone.h"

/**------------------------------------------------------------------*\
    @name MSI 3 Zone Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectMSI3ZoneControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_MSI3Zone::RGBController_MSI3Zone(MSI3ZoneController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "MSI 3-Zone Keyboard";
    vendor      = "MSI";
    type        = DEVICE_TYPE_LAPTOP;
    description = "MSI 3-Zone Keyboard Device";
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

RGBController_MSI3Zone::~RGBController_MSI3Zone()
{
    delete controller;
}

void RGBController_MSI3Zone::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up Keyboard zone and Keyboard LEDs                    |
    \*---------------------------------------------------------*/
    zone keyboard_zone;
    keyboard_zone.name          = "Keyboard";
    keyboard_zone.type          = ZONE_TYPE_LINEAR;
    keyboard_zone.leds_min      = 3;
    keyboard_zone.leds_max      = 3;
    keyboard_zone.leds_count    = 3;
    keyboard_zone.matrix_map    = NULL;
    zones.push_back(keyboard_zone);

    led left_led;
    left_led.name = "Keyboard Left";
    leds.push_back(left_led);

    led mid_led;
    mid_led.name = "Keyboard Middle";
    leds.push_back(mid_led);

    led right_led;
    right_led.name = "Keyboard Right";
    leds.push_back(right_led);

    /*---------------------------------------------------------*\
    | Set up Aux zone and Aux LED                               |
    \*---------------------------------------------------------*/
    zone aux_zone;
    aux_zone.name           = "Aux";
    aux_zone.type           = ZONE_TYPE_SINGLE;
    aux_zone.leds_min       = 1;
    aux_zone.leds_max       = 1;
    aux_zone.leds_count     = 1;
    aux_zone.matrix_map     = NULL;
    zones.push_back(aux_zone);

    led aux_led;
    aux_led.name = "Aux";
    leds.push_back(aux_led);

    SetupColors();
}

void RGBController_MSI3Zone::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_MSI3Zone::DeviceUpdateLEDs()
{
    controller->SetLEDs(colors);
}

void RGBController_MSI3Zone::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetLEDs(colors);
}

void RGBController_MSI3Zone::UpdateSingleLED(int /*led*/)
{
    controller->SetLEDs(colors);
}

void RGBController_MSI3Zone::DeviceUpdateMode()
{

}
