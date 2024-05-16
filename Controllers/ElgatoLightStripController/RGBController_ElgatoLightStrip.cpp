/*---------------------------------------------------------*\
| RGBController_ElgatoLightStrip.cpp                        |
|                                                           |
|   RGBController for Elgato Light Strip                    |
|                                                           |
|   Monks (@iamtherealestmonkey)                03 Nov 2021 |
|   DomePlaysHD                                 12 Mar 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_ElgatoLightStrip.h"
#include "hsv.h"

RGBController_ElgatoLightStrip::RGBController_ElgatoLightStrip(ElgatoLightStripController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetName();
    vendor              = controller->GetManufacturer();
    type                = DEVICE_TYPE_LEDSTRIP;
    version             = controller->GetVersion();
    description         = "Elgato LightStrip Device";
    serial              = controller->GetUniqueID();
    location            = controller->GetLocation();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness_min   = 0;
    Direct.brightness_max   = 100;
    Direct.brightness       = 100;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_ElgatoLightStrip::~RGBController_ElgatoLightStrip()
{
    delete controller;
}

void RGBController_ElgatoLightStrip::SetupZones()
{
    zone led_zone;
    led_zone.name       = "Lightstrip";
    led_zone.type       = ZONE_TYPE_SINGLE;
    led_zone.leds_min   = 1;
    led_zone.leds_max   = 1;
    led_zone.leds_count = 1;
    led_zone.matrix_map = NULL;
    zones.push_back(led_zone);

    led new_led;
    new_led.name = "Lightstrip";
    leds.push_back(new_led);

    SetupColors();
}

void RGBController_ElgatoLightStrip::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ElgatoLightStrip::DeviceUpdateLEDs()
{
    RGBColor rgb_color = colors[0];
    hsv_t hsv_color;
    rgb2hsv(rgb_color, &hsv_color);
    controller->SetColor(hsv_color);
    controller->SetBrightness((unsigned char)modes[(unsigned int)active_mode].brightness);
}

void RGBController_ElgatoLightStrip::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ElgatoLightStrip::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ElgatoLightStrip::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
