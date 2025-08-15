/*---------------------------------------------------------*\
| RGBController_ZETEdgeAirPro.cpp                           |
|                                                           |
|   RGBController for ZET Edge Air Pro                      |
|                                                           |
|   Morgan Guimard (morg)                       29 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_ZETEdgeAirPro.h"

/**------------------------------------------------------------------*\
    @name ZET Edge Air Pro
    @category Mouse
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectZETGAMINGEdgeAirProControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_ZETEdgeAirPro::RGBController_ZETEdgeAirPro(ZETEdgeAirProController* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetNameString();
    vendor                              = "ZET Gaming";
    type                                = DEVICE_TYPE_MOUSE;
    description                         = "ZET Edge Air Pro Device";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Static;
    Static.name                         = "Static";
    Static.value                        = STATIC_MODE_VALUE;
    Static.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode                   = MODE_COLORS_PER_LED;
    Static.brightness_min               = ZET_EDGE_AIR_PRO_BRIGHTNESS_MIN;
    Static.brightness_max               = ZET_EDGE_AIR_PRO_BRIGHTNESS_MAX;
    Static.brightness                   = ZET_EDGE_AIR_PRO_BRIGHTNESS_MAX;
    modes.push_back(Static);

    mode OFF;
    OFF.name                            = "Off";
    OFF.value                           = OFF_MODE_VALUE;
    OFF.flags                           = MODE_FLAG_AUTOMATIC_SAVE;
    OFF.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(OFF);

    mode Rainbow;
    Rainbow.name                        = "Rainbow";
    Rainbow.value                       = RAINBOW_MODE_VALUE;
    Rainbow.flags                       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.color_mode                  = MODE_COLORS_NONE;
    Rainbow.brightness_min              = ZET_EDGE_AIR_PRO_BRIGHTNESS_MIN;
    Rainbow.brightness_max              = ZET_EDGE_AIR_PRO_BRIGHTNESS_MAX;
    Rainbow.brightness                  = ZET_EDGE_AIR_PRO_BRIGHTNESS_MAX;
    Rainbow.speed_min                   = ZET_EDGE_AIR_PRO_SPEED_MIN;
    Rainbow.speed_max                   = ZET_EDGE_AIR_PRO_SPEED_MAX;
    Rainbow.speed                       = ZET_EDGE_AIR_PRO_SPEED_MIN;
    modes.push_back(Rainbow);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = BREATHING_MODE_VALUE;
    Breathing.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode                = MODE_COLORS_PER_LED;
    Breathing.brightness_min            = ZET_EDGE_AIR_PRO_BRIGHTNESS_MIN;
    Breathing.brightness_max            = ZET_EDGE_AIR_PRO_BRIGHTNESS_MAX;
    Breathing.brightness                = ZET_EDGE_AIR_PRO_BRIGHTNESS_MAX;
    Breathing.speed_min                 = ZET_EDGE_AIR_PRO_SPEED_MIN;
    Breathing.speed_max                 = ZET_EDGE_AIR_PRO_SPEED_MAX;
    Breathing.speed                     = ZET_EDGE_AIR_PRO_SPEED_MIN;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_ZETEdgeAirPro::~RGBController_ZETEdgeAirPro()
{
    delete controller;
}

void RGBController_ZETEdgeAirPro::SetupZones()
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

    led new_led;
    new_led.name        = "LED 1";
    leds[0] = new_led;

    SetupColors();
}

void RGBController_ZETEdgeAirPro::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ZETEdgeAirPro::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_ZETEdgeAirPro::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateMode();
}

void RGBController_ZETEdgeAirPro::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateMode();
}

void RGBController_ZETEdgeAirPro::DeviceUpdateMode()
{
    RGBColor color;

    if(modes[active_mode].flags & MODE_FLAG_HAS_PER_LED_COLOR)
    {
        color = colors[0];
    }
    else
    {
        color = ToRGBColor(0,0,0);
    }

    controller->SetMode(color, modes[active_mode].brightness, modes[active_mode].speed, modes[active_mode].value);
}
