/*---------------------------------------------------------*\
| RGBController_CorsairHydro2.cpp                           |
|                                                           |
|   RGBController for Corsair H100i v2                      |
|                                                           |
|   Tim Demand (tim.dmd)                        10 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_CorsairHydro2.h"

/**------------------------------------------------------------------*\
    @name Corsair Hydro Series H100i v2 AIO
    @category Cooler
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairHydro2Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_CorsairHydro2::RGBController_CorsairHydro2(CorsairHydro2Controller* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Corsair H100i v2";
    vendor      = "Corsair";
    description = "Corsair H100i v2";
    type        = DEVICE_TYPE_COOLER;
    location    = controller->GetLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_CorsairHydro2::~RGBController_CorsairHydro2()
{
    delete controller;
}

void RGBController_CorsairHydro2::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Pump Zone";
    new_zone.type       = ZONE_TYPE_SINGLE;
    new_zone.leds_min   = 1;
    new_zone.leds_max   = 1;
    new_zone.leds_count = 1;
    new_zone.matrix_map = NULL;
    zones.push_back(new_zone);

    led new_led;

    new_led.name = "Pump LED";
    leds.push_back(new_led);

    SetupColors();
}

void RGBController_CorsairHydro2::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairHydro2::DeviceUpdateLEDs()
{
    controller->SetLED(colors);
}

void RGBController_CorsairHydro2::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetLED(colors);
}

void RGBController_CorsairHydro2::UpdateSingleLED(int /*led*/)
{
    controller->SetLED(colors);
}

void RGBController_CorsairHydro2::DeviceUpdateMode()
{

}
