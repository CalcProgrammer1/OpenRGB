/*---------------------------------------------------------*\
| RGBController_CorsairDominatorPlatinum.cpp                |
|                                                           |
|   RGBController for Corsair Dominator Platinum RAM        |
|                                                           |
|   Erik Gilling (konkers)                      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_CorsairDominatorPlatinum.h"

/**------------------------------------------------------------------*\
    @name Corsair Dominator Platinum
    @category RAM
    @type SMBus
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairDominatorPlatinumControllers
    @comment
        The Corsair Dominator controller chip can be found on several
        Corsair memory sticks which have different LED counts. This can be controlled
        by editing the Part Number in OpenRGB.json with values in the below table.

        | Part Number | LED Count |
        | :---------: | --------: |
        | CMG         |  6        |
        | CMH         | 10        |
        | CMN         | 10        |
        | CMT         | 12        |
\*-------------------------------------------------------------------*/

RGBController_CorsairDominatorPlatinum::RGBController_CorsairDominatorPlatinum(CorsairDominatorPlatinumController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "Corsair";
    type        = DEVICE_TYPE_DRAM;
    description = "Corsair RAM RGB Device";
    location    = controller->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.speed_min  = 0;
    Direct.speed_max  = 0;
    Direct.speed      = 0;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    active_mode = 0;
}

RGBController_CorsairDominatorPlatinum::~RGBController_CorsairDominatorPlatinum()
{
    Shutdown();

    delete controller;
}

void RGBController_CorsairDominatorPlatinum::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name       = "Corsair RAM Zone";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = controller->GetLEDCount();
    new_zone.leds_max   = controller->GetLEDCount();
    new_zone.leds_count = controller->GetLEDCount();
    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name   = "Corsair RAM LED ";
        new_led.name.append(std::to_string(led_idx));
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_CorsairDominatorPlatinum::DeviceUpdateLEDs()
{
    for(unsigned int led = 0; led < (unsigned int)colors.size(); led++)
    {
        RGBColor color    = colors[led];
        unsigned char red = RGBGetRValue(color);
        unsigned char grn = RGBGetGValue(color);
        unsigned char blu = RGBGetBValue(color);

        controller->SetLEDColor(led, red, grn, blu);
    }

    controller->ApplyColors();
}

void RGBController_CorsairDominatorPlatinum::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairDominatorPlatinum::DeviceUpdateSingleLED(int led)
{
    RGBColor color    = colors[led];
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    controller->SetLEDColor(led, red, grn, blu);
    controller->ApplyColors();
}

void RGBController_CorsairDominatorPlatinum::DeviceUpdateMode()
{

}
