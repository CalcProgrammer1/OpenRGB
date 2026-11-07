/*---------------------------------------------------------*\
| RGBController_CorsairVoid.cpp                             |
|                                                           |
|   RGBController for Corsair Void Pro headsets             |
|                                                           |
|   Derek Ross                                  15 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_CorsairVoid.h"

/**------------------------------------------------------------------*\
    @name Corsair Void Pro
    @category Headset
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairVoidControllers
    @comment The headset's mic mute LED stays off while OpenRGB
    controls the lighting; it returns when OpenRGB exits.
\*-------------------------------------------------------------------*/

RGBController_CorsairVoid::RGBController_CorsairVoid(CorsairVoidController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "Corsair";
    type                    = DEVICE_TYPE_HEADSET;
    description             = "Corsair Void Pro Headset Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0x00;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;

    modes.push_back(Direct);

    SetupZones();
}

RGBController_CorsairVoid::~RGBController_CorsairVoid()
{
    Shutdown();

    delete controller;
}

void RGBController_CorsairVoid::SetupZones()
{
    zone left_earcup;
    left_earcup.name        = "Left Earcup";
    left_earcup.type        = ZONE_TYPE_SINGLE;
    left_earcup.leds_min    = 1;
    left_earcup.leds_max    = 1;
    left_earcup.leds_count  = 1;
    zones.push_back(left_earcup);

    led left_led;
    left_led.name           = "Left Earcup";
    leds.push_back(left_led);

    zone right_earcup;
    right_earcup.name       = "Right Earcup";
    right_earcup.type       = ZONE_TYPE_SINGLE;
    right_earcup.leds_min   = 1;
    right_earcup.leds_max   = 1;
    right_earcup.leds_count = 1;
    zones.push_back(right_earcup);

    led right_led;
    right_led.name          = "Right Earcup";
    leds.push_back(right_led);

    SetupColors();
}

void RGBController_CorsairVoid::DeviceUpdateLEDs()
{
    controller->SetColors(colors[0], colors[1]);
}

void RGBController_CorsairVoid::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairVoid::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairVoid::DeviceUpdateMode()
{

}
