/*---------------------------------------------------------*\
| RGBController_LexipMouse.cpp                              |
|                                                           |
|   RGBController for Lexip mouse                           |
|                                                           |
|   Morgan Guimard (morg)                       21 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_LexipMouse.h"

/**------------------------------------------------------------------*\
    @name Lexip Mouse
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectLexipMouseControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_LexipMouse::RGBController_LexipMouse(LexipMouseController* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetNameString();
    vendor                              = "Lexip";
    type                                = DEVICE_TYPE_MOUSE;
    description                         = name;
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Direct;
    Direct.name                         = "Direct";
    Direct.value                        = 0x00;
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_LexipMouse::~RGBController_LexipMouse()
{
    delete controller;
}

void RGBController_LexipMouse::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Mouse";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = 1;
    new_zone.leds_max   = 1;
    new_zone.leds_count = 1;
    new_zone.matrix_map = nullptr;

    zones.emplace_back(new_zone);

    leds.resize(1);
    leds[0].name = "LED 1";

    SetupColors();
}

void RGBController_LexipMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LexipMouse::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
}

void RGBController_LexipMouse::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetDirect(colors[0]);
}

void RGBController_LexipMouse::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_LexipMouse::DeviceUpdateMode()
{
    UpdateZoneLEDs(0);
}
