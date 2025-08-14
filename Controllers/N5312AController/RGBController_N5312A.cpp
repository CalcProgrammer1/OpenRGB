/*---------------------------------------------------------*\
| RGBController_N5312A.cpp                                  |
|                                                           |
|   RGBController for N5312A                                |
|                                                           |
|   Morgan Guimard (morg)                       02 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_N5312A.h"

/**------------------------------------------------------------------*\
    @name N5312A mouse
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectN5312AControllers
    @comment This controller should work with all mouse with this chip.
    Identified devices that work with this controller: ANT Esports KM540 Mouse,
    Marvo M115
\*-------------------------------------------------------------------*/

RGBController_N5312A::RGBController_N5312A(N5312AController* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetNameString();
    vendor                              = "Unknown";
    type                                = DEVICE_TYPE_MOUSE;
    description                         = "N5312A Device";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Static;
    Static.name                         = "Direct";
    Static.value                        = N5312A_DIRECT_MODE_VALUE;
    Static.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode                   = MODE_COLORS_PER_LED;
    Static.brightness_min               = N5312A_BRIGHTNESS_MIN;
    Static.brightness_max               = N5312A_BRIGHTNESS_MAX;
    Static.brightness                   = N5312A_BRIGHTNESS_MAX;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = N5312A_BREATHING_MODE_VALUE;
    Breathing.flags                     = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode                = MODE_COLORS_PER_LED;
    Breathing.brightness_min            = N5312A_BRIGHTNESS_MIN;
    Breathing.brightness_max            = N5312A_BRIGHTNESS_MAX;
    Breathing.brightness                = N5312A_BRIGHTNESS_MAX;
    Breathing.speed                     = N5312A_SPEED_MIN;
    Breathing.speed_min                 = N5312A_SPEED_MIN;
    Breathing.speed_max                 = N5312A_SPEED_MAX;
    modes.push_back(Breathing);

    mode SingleBreath;
    SingleBreath.name                   = "Single Breath";
    SingleBreath.value                  = N5312A_SINGLE_BREATH_MODE_VALUE;
    SingleBreath.flags                  = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    SingleBreath.color_mode             = MODE_COLORS_PER_LED;
    SingleBreath.brightness_min         = N5312A_BRIGHTNESS_MIN;
    SingleBreath.brightness_max         = N5312A_BRIGHTNESS_MAX;
    SingleBreath.brightness             = N5312A_BRIGHTNESS_MAX;
    SingleBreath.speed                  = N5312A_SPEED_MIN;
    SingleBreath.speed_min              = N5312A_SPEED_MIN;
    SingleBreath.speed_max              = N5312A_SPEED_MAX;
    modes.push_back(SingleBreath);

    mode Off;
    Off.name                            = "Off";
    Off.value                           = N5312A_OFF_MODE_VALUE;
    Off.flags                           = 0x00;
    Off.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_N5312A::~RGBController_N5312A()
{
    delete controller;
}

void RGBController_N5312A::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Mouse";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = N5312A_NUMBER_OF_LEDS;
    new_zone.leds_max   = N5312A_NUMBER_OF_LEDS;
    new_zone.leds_count = N5312A_NUMBER_OF_LEDS;
    new_zone.matrix_map = nullptr;

    zones.emplace_back(new_zone);

    leds.resize(new_zone.leds_count);

    for(unsigned int i = 0; i < N5312A_NUMBER_OF_LEDS; i++)
    {
        leds[i].name = "LED " + std::to_string(i + 1);
    }

    SetupColors();
}

void RGBController_N5312A::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_N5312A::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
}

void RGBController_N5312A::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetColor(colors[0]);
}

void RGBController_N5312A::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_N5312A::DeviceUpdateMode()
{
    const RGBColor& color = modes[active_mode].value == N5312A_OFF_MODE_VALUE ? 0 : colors[0];
    controller->SetMode(color, modes[active_mode].value, modes[active_mode].brightness, modes[active_mode].speed);
}
