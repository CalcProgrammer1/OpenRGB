/*---------------------------------------------------------*\
| RGBController_ElgatoKeyLight.cpp                          |
|                                                           |
|   RGBController for Elgato Key Light                      |
|                                                           |
|   Monks (@iamtherealestmonkey)                03 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_ElgatoKeyLight.h"
#include "hsv.h"

RGBController_ElgatoKeyLight::RGBController_ElgatoKeyLight(ElgatoKeyLightController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetName();
    vendor              = controller->GetManufacturer();
    type                = DEVICE_TYPE_LIGHT;
    version             = controller->GetVersion();
    description         = "Elgato KeyLight Device";
    serial              = controller->GetUniqueID();
    location            = controller->GetLocation();

    mode Static;
    Static.name         = "Static";
    Static.value        = 0;
    Static.flags        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    SetupZones();
}

RGBController_ElgatoKeyLight::~RGBController_ElgatoKeyLight()
{
    delete controller;
}

void RGBController_ElgatoKeyLight::SetupZones()
{
    zone led_zone;
    led_zone.name       = "Keylight";
    led_zone.type       = ZONE_TYPE_SINGLE;
    led_zone.leds_min   = 1;
    led_zone.leds_max   = 1;
    led_zone.leds_count = 1;
    led_zone.matrix_map = NULL;
    zones.push_back(led_zone);

    led new_led;
    new_led.name = "Keylight";

    leds.push_back(new_led);

    SetupColors();
}

void RGBController_ElgatoKeyLight::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ElgatoKeyLight::DeviceUpdateLEDs()
{
    RGBColor rgb_color = colors[0];
    hsv_t hsv_color;
    rgb2hsv(rgb_color, &hsv_color);
    controller->SetColor(hsv_color);
}

void RGBController_ElgatoKeyLight::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ElgatoKeyLight::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ElgatoKeyLight::DeviceUpdateMode()
{

}
