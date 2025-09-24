/*---------------------------------------------------------*\
| RGBController_CorsairVengeance.cpp                        |
|                                                           |
|   RGBController for original single-zone Corsair          |
|   Vengeance DDR4 RGB RAM                                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                16 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_CorsairVengeance.h"

/**------------------------------------------------------------------*\
    @name Corsair Vengeance
    @category RAM
    @type SMBus
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectCorsairVengeanceControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_CorsairVengeance::RGBController_CorsairVengeance(CorsairVengeanceController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "Corsair";
    type        = DEVICE_TYPE_DRAM;
    description = "Corsair Vengeance RGB Device";
    location    = controller->GetDeviceLocation();

    mode Static;
    Static.name       = "Static";
    Static.value      = CORSAIR_VENGEANCE_RGB_MODE_SINGLE;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Fade;
    Fade.name       = "Fade";
    Fade.value      = CORSAIR_VENGEANCE_RGB_MODE_FADE;
    Fade.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Fade.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Fade);

    mode Pulse;
    Pulse.name       = "Pulse";
    Pulse.value      = CORSAIR_VENGEANCE_RGB_MODE_PULSE;
    Pulse.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Pulse.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Pulse);

    SetupZones();
}

RGBController_CorsairVengeance::~RGBController_CorsairVengeance()
{
    delete controller;
}

void RGBController_CorsairVengeance::SetupZones()
{
    /*---------------------------------------------------------*\
    | Create a single zone                                      |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name       = "Corsair Zone";
    new_zone.type       = ZONE_TYPE_SINGLE;
    new_zone.leds_min   = controller->GetLEDCount();
    new_zone.leds_max   = controller->GetLEDCount();
    new_zone.leds_count = controller->GetLEDCount();
    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led* new_led    = new led();
        new_led->name   = "Corsair LED";
        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_CorsairVengeance::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    controller->SetLEDColor(red, grn, blu);
}

void RGBController_CorsairVengeance::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairVengeance::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairVengeance::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode].value);
}
