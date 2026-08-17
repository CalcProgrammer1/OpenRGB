/*---------------------------------------------------------*\
| RGBController_GameSir.cpp                                 |
|                                                           |
|   GameSir RGB Device                                      |
|                                                           |
|   Added by OpenRGB Community                  08 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_GameSir.h"

RGBController_GameSir::RGBController_GameSir(GameSirController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "GameSir Nova 2 Lite";
    vendor      = "GameSir";
    type        = DEVICE_TYPE_GAMEPAD;
    description = "GameSir RGB Device";
    location    = controller->GetLocation();
    serial      = controller->GetSerialString();

    mode Static;
    Static.name       = "Static";
    Static.value      = 0xFFFF;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    SetupZones();
}

RGBController_GameSir::~RGBController_GameSir()
{
    delete controller;
}

void RGBController_GameSir::SetupZones()
{
    zone new_zone;
    new_zone.name       = "Controller";
    new_zone.type       = ZONE_TYPE_SINGLE;
    new_zone.leds_min   = 1;
    new_zone.leds_max   = 1;
    new_zone.leds_count = 1;
    zones.push_back(new_zone);

    led new_led;
    new_led.name        = "Main LED";
    leds.push_back(new_led);

    SetupColors();
}

void RGBController_GameSir::DeviceUpdateLEDs()
{
    controller->SetColor(RGBGetRValue(colors[0]), RGBGetGValue(colors[0]), RGBGetBValue(colors[0]));
}

void RGBController_GameSir::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GameSir::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GameSir::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
