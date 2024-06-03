/*---------------------------------------------------------*\
| RGBController_PatriotViperSteel.cpp                       |
|                                                           |
|   RGBController for Patriot Viper Steel RAM               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_PatriotViperSteel.h"

/**------------------------------------------------------------------*\
    @name Patriot Viper Steel
    @category RAM
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectPatriotViperSteelControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PatriotViperSteel::RGBController_PatriotViperSteel(PatriotViperSteelController *viper_ptr)
{
    controller  = viper_ptr;

    name        = controller->GetDeviceName();
    vendor      = "Patriot";
    type        = DEVICE_TYPE_DRAM;
    description = "Patriot Viper Steel Device";
    location    = controller->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.speed_min  = 0;
    Direct.speed_max  = 0;
    Direct.color_mode = MODE_COLORS_PER_LED;
    Direct.speed      = 0;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_PatriotViperSteel::~RGBController_PatriotViperSteel()
{
    delete controller;
}

void RGBController_PatriotViperSteel::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone *new_zone       = new zone;
    new_zone->name       = "Patriot Viper Steel RGB";
    new_zone->type       = ZONE_TYPE_LINEAR;
    new_zone->leds_min   = 5;
    new_zone->leds_max   = 5;
    new_zone->leds_count = 5;
    new_zone->matrix_map = NULL;
    zones.push_back(*new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led *new_led = new led();

        new_led->name = "Patriot Viper RGB LED ";
        new_led->name.append(std::to_string(led_idx + 1));
        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_PatriotViperSteel::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PatriotViperSteel::DeviceUpdateLEDs()
{
    for(int led = 0; led < 5; led++)
    {
        RGBColor color    = colors[led];
        unsigned char red = RGBGetRValue(color);
        unsigned char grn = RGBGetGValue(color);
        unsigned char blu = RGBGetBValue(color);

        controller->SetLEDColor(led, red, grn, blu);
    }
}

void RGBController_PatriotViperSteel::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PatriotViperSteel::UpdateSingleLED(int led)
{
    RGBColor color    = colors[led];
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    controller->SetLEDColor(led, red, grn, blu);
}

void RGBController_PatriotViperSteel::DeviceUpdateMode()
{
}
