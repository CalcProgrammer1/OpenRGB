/*---------------------------------------------------------*\
| RGBController_TrustGXT180.cpp                             |
|                                                           |
|   RGBController for Trust GXT 180                         |
|                                                           |
|   Morgan Guimard (morg)                       24 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_TrustGXT180.h"

/**------------------------------------------------------------------*\
    @name Trust GXT 180
    @category Mouse
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectTrustGXT180Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_TrustGXT180::RGBController_TrustGXT180(TrustGXT180Controller* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetNameString();
    vendor                              = "Trust";
    type                                = DEVICE_TYPE_MOUSE;
    description                         = "Trust GXT 180 Device";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Static;
    Static.name                         = "Static";
    Static.value                        = TRUST_GXT_180_STATIC_MODE_VALUE;
    Static.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode                   = MODE_COLORS_PER_LED;
    Static.brightness_min               = TRUST_GXT_180_BRIGHTNESS_MIN;
    Static.brightness_max               = TRUST_GXT_180_BRIGHTNESS_MAX;
    Static.brightness                   = TRUST_GXT_180_BRIGHTNESS_MAX;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = TRUST_GXT_180_BREATHING_MODE_VALUE;
    Breathing.flags                     = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode                = MODE_COLORS_PER_LED;
    Breathing.speed_min                 = TRUST_GXT_180_SPEED_MIN;
    Breathing.speed_max                 = TRUST_GXT_180_SPEED_MAX;
    Breathing.speed                     = TRUST_GXT_180_SPEED_MIN;
    Breathing.brightness_min            = TRUST_GXT_180_BRIGHTNESS_MIN;
    Breathing.brightness_max            = TRUST_GXT_180_BRIGHTNESS_MAX;
    Breathing.brightness                = TRUST_GXT_180_BRIGHTNESS_MAX;
    modes.push_back(Breathing);

    mode ColorShift;
    ColorShift.name                     = "ColorShift";
    ColorShift.value                    = TRUST_GXT_180_COLORSHIFT_MODE_VALUE;
    ColorShift.flags                    = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    ColorShift.color_mode               = MODE_COLORS_RANDOM;
    ColorShift.speed_min                = TRUST_GXT_180_SPEED_MIN;
    ColorShift.speed_max                = TRUST_GXT_180_SPEED_MAX;
    ColorShift.speed                    = TRUST_GXT_180_SPEED_MIN;
    ColorShift.brightness_min           = TRUST_GXT_180_BRIGHTNESS_MIN;
    ColorShift.brightness_max           = TRUST_GXT_180_BRIGHTNESS_MAX;
    ColorShift.brightness               = TRUST_GXT_180_BRIGHTNESS_MAX;
    modes.push_back(ColorShift);

    mode Off;
    Off.name                            = "Off";
    Off.value                           = TRUST_GXT_180_OFF_MODE_VALUE;
    Off.flags                           = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_TrustGXT180::~RGBController_TrustGXT180()
{
    delete controller;
}

void RGBController_TrustGXT180::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Mouse";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = TRUST_GXT_180_NUMBER_OF_LEDS;
    new_zone.leds_max   = TRUST_GXT_180_NUMBER_OF_LEDS;
    new_zone.leds_count = TRUST_GXT_180_NUMBER_OF_LEDS;
    new_zone.matrix_map = nullptr;

    zones.emplace_back(new_zone);

    leds.resize(new_zone.leds_count);

    for(unsigned int i = 0; i < TRUST_GXT_180_NUMBER_OF_LEDS; i++)
    {
        leds[i].name = "LED " + std::to_string(i + 1);
    }

    SetupColors();
}

void RGBController_TrustGXT180::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_TrustGXT180::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_TrustGXT180::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateMode();
}

void RGBController_TrustGXT180::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateMode();
}

void RGBController_TrustGXT180::DeviceUpdateMode()
{
    controller->SetMode(colors[0], modes[active_mode].brightness, modes[active_mode].speed, modes[active_mode].value);
}
