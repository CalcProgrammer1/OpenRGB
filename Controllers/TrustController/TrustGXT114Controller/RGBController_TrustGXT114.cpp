/*---------------------------------------------------------*\
| RGBController_TrustGXT114.cpp                             |
|                                                           |
|   RGBController for Trust GXT 114                         |
|                                                           |
|   Morgan Guimard (morg)                       24 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_TrustGXT114.h"

/**------------------------------------------------------------------*\
    @name Trust GXT 114
    @category Mouse
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectTrustGXT114Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_TrustGXT114::RGBController_TrustGXT114(TrustGXT114Controller* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetNameString();
    vendor                              = "Trust";
    type                                = DEVICE_TYPE_MOUSE;
    description                         = "Trust GXT 114 Device";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Static;
    Static.name                         = "Static";
    Static.value                        = STATIC_MODE_VALUE;
    Static.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min                   = 1;
    Static.colors_max                   = 1;
    Static.brightness_min               = TRUST_GXT_114_BRIGHTNESS_MIN;
    Static.brightness_max               = TRUST_GXT_114_BRIGHTNESS_MAX;
    Static.brightness                   = TRUST_GXT_114_BRIGHTNESS_MAX;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = BREATHING_MODE_VALUE;
    Breathing.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min                = 1;
    Breathing.colors_max                = 1;
    Breathing.speed_min                 = TRUST_GXT_114_SPEED_MIN;
    Breathing.speed_max                 = TRUST_GXT_114_SPEED_MAX;
    Breathing.speed                     = TRUST_GXT_114_SPEED_MIN;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Blink;
    Blink.name                          = "Blink";
    Blink.value                         = BLINK_MODE_VALUE;
    Blink.flags                         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Blink.color_mode                    = MODE_COLORS_MODE_SPECIFIC;
    Blink.colors_min                    = 1;
    Blink.colors_max                    = 1;
    Blink.speed_min                     = TRUST_GXT_114_SPEED_MIN;
    Blink.speed_max                     = TRUST_GXT_114_SPEED_MAX;
    Blink.speed                         = TRUST_GXT_114_SPEED_MIN;
    Blink.colors.resize(1);
    modes.push_back(Blink);

    SetupZones();
}

RGBController_TrustGXT114::~RGBController_TrustGXT114()
{
    delete controller;
}

void RGBController_TrustGXT114::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Mouse";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = TRUST_GXT_114_NUMBER_OF_LEDS;
    new_zone.leds_max   = TRUST_GXT_114_NUMBER_OF_LEDS;
    new_zone.leds_count = TRUST_GXT_114_NUMBER_OF_LEDS;
    new_zone.matrix_map = nullptr;

    zones.emplace_back(new_zone);

    leds.resize(new_zone.leds_count);

    for(unsigned int i = 0; i < TRUST_GXT_114_NUMBER_OF_LEDS; i++)
    {
        leds[i].name = "LED " + std::to_string(i);
    }

    SetupColors();
}

void RGBController_TrustGXT114::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_TrustGXT114::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_TrustGXT114::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateMode();
}

void RGBController_TrustGXT114::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateMode();
}

void RGBController_TrustGXT114::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode].colors[0], modes[active_mode].brightness, modes[active_mode].speed, modes[active_mode].value);
}
