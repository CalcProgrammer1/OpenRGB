/*---------------------------------------------------------*\
| RGBController_ClevoLightbar.cpp                           |
|                                                           |
|   Generic RGB Interface for Clevo laptop lightbar         |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_ClevoLightbar.h"

/**------------------------------------------------------------------*\
    @name CLEVO Lightbar
    @category LEDStrip
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectClevoLightbarControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_ClevoLightbar::RGBController_ClevoLightbar(ClevoLightbarController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "CLEVO Lightbar";
    vendor      = "CLEVO Computers";
    type        = DEVICE_TYPE_LEDSTRIP;
    description = "CLEVO Laptop Lightbar";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = CLEVO_LIGHTBAR_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode = MODE_COLORS_PER_LED;
    Direct.brightness_min = CLEVO_LIGHTBAR_BRIGHTNESS_MIN;
    Direct.brightness_max = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    Direct.brightness     = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    mode Off;
    Off.name       = "Off";
    Off.value      = CLEVO_LIGHTBAR_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_ClevoLightbar::~RGBController_ClevoLightbar()
{
    delete controller;
}

void RGBController_ClevoLightbar::SetupZones()
{
    zone lightbar_zone;
    lightbar_zone.name       = "Lightbar";
    lightbar_zone.type       = ZONE_TYPE_SINGLE;
    lightbar_zone.leds_min   = 1;
    lightbar_zone.leds_max   = 1;
    lightbar_zone.leds_count = 1;
    zones.push_back(lightbar_zone);

    led lightbar_led;
    lightbar_led.name = "Lightbar";
    leds.push_back(lightbar_led);

    SetupColors();
}

void RGBController_ClevoLightbar::DeviceUpdateLEDs()
{
    unsigned char red   = RGBGetRValue(colors[0]);
    unsigned char green = RGBGetGValue(colors[0]);
    unsigned char blue  = RGBGetBValue(colors[0]);

    controller->SetColor(red, green, blue);
    controller->SetBrightness(modes[active_mode].brightness);
}

void RGBController_ClevoLightbar::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ClevoLightbar::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ClevoLightbar::DeviceUpdateMode()
{
    if(modes[active_mode].value == CLEVO_LIGHTBAR_MODE_OFF)
    {
        controller->TurnOff();
    }
}
