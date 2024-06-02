/*---------------------------------------------------------*\
| RGBController_KasaSmart.cpp                               |
|                                                           |
|   RGBController for Kasa Smart bulbs                      |
|                                                           |
|   Devin Wendt (umbreon222)                    16 Feb 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_KasaSmart.h"

/**------------------------------------------------------------------*\
    @name Kasa Smart Bulbs
    @category Light
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectKasaSmartControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_KasaSmart::RGBController_KasaSmart(KasaSmartController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetManufacturer() + " " + controller->GetName();
    vendor      = controller->GetManufacturer();
    version     = controller->GetVersion();
    description = "Kasa Smart Device";
    serial      = controller->GetUniqueID();
    location    = controller->GetLocation();

    if(controller->GetKasaType() == KASA_SMART_TYPE_LIGHT)
    {
        type = DEVICE_TYPE_LIGHT;
    }
    else if(controller->GetKasaType() == KASA_SMART_TYPE_OTHER_LEDSTRIP
         || controller->GetKasaType() == KASA_SMART_TYPE_KL420)
    {
        type = DEVICE_TYPE_LEDSTRIP;
    }

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = KASA_SMART_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    if(controller->GetKasaType() == KASA_SMART_TYPE_KL420)
    {
        mode Rainbow;
        Rainbow.name       = "Rainbow";
        Rainbow.value      = KASA_SMART_MODE_RAINBOW;
        Rainbow.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
        Rainbow.color_mode = MODE_COLORS_PER_LED;
        modes.push_back(Rainbow);
    }

    mode Off;
    Off.name       = "Off";
    Off.value      = KASA_SMART_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_KasaSmart::~RGBController_KasaSmart()
{
    delete controller;
}

void RGBController_KasaSmart::SetupZones()
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

void RGBController_KasaSmart::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*-------------------------------------------*\
    | This device does not support resizing zones |
    \*-------------------------------------------*/
}

void RGBController_KasaSmart::DeviceUpdateLEDs()
{
    if(modes[active_mode].value != KASA_SMART_MODE_DIRECT)
    {
        return;
    }

    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SetColor(red, grn, blu, type);
}

void RGBController_KasaSmart::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_KasaSmart::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_KasaSmart::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case KASA_SMART_MODE_OFF:
            controller->TurnOff(type);
            break;
        case KASA_SMART_MODE_RAINBOW:
            controller->SetEffect(KASA_SMART_EFFECT_RAINBOW_COMMAND);
            break;
    }
}
