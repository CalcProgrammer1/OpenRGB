/*---------------------------------------------------------*\
| RGBController_GigabyteAorusMouse.cpp                      |
|                                                           |
|   RGBController for Gigabyte Aorus mouse                  |
|                                                           |
|   Morgan Guimard (morg)                       29 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_GigabyteAorusMouse.h"

/**------------------------------------------------------------------*\
    @name Gigabyte Aorus mouse
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectGigabyteAorusMouseControllers
    @comment
\*-------------------------------------------------------------------*/
RGBController_GigabyteAorusMouse::RGBController_GigabyteAorusMouse(GigabyteAorusMouseController* controller_ptr)
{
    controller                          = controller_ptr;
    name                                = controller->GetNameString();
    vendor                              = "Gigabyte";
    type                                = DEVICE_TYPE_MOUSE;
    description                         = "Gigabyte Mouse Device";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();
    version                             = controller->GetFirmwareVersion();

    mode Direct;
    Direct.name                         = "Direct";
    Direct.value                        = GIGABYTE_AORUS_MOUSE_DIRECT_MODE_VALUE;
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    Direct.brightness_min               = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MIN;
    Direct.brightness_max               = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MAX;
    Direct.brightness                   = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    mode Static;
    Static.name                         = "Static";
    Static.value                        = GIGABYTE_AORUS_MOUSE_STATIC_MODE_VALUE;
    Static.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness_min               = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MIN;
    Static.brightness_max               = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MAX;
    Static.brightness                   = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MAX;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Pulse;
    Pulse.name                          = "Breathing";
    Pulse.value                         = GIGABYTE_AORUS_MOUSE_PULSE_MODE_VALUE;
    Pulse.flags                         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Pulse.color_mode                    = MODE_COLORS_MODE_SPECIFIC;
    Pulse.brightness_min                = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MIN;
    Pulse.brightness_max                = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MAX;
    Pulse.brightness                    = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MAX;
    Pulse.speed_min                     = GIGABYTE_AORUS_MOUSE_SPEED_MIN;
    Pulse.speed_max                     = GIGABYTE_AORUS_MOUSE_SPEED_MAX;
    Pulse.speed                         = GIGABYTE_AORUS_MOUSE_SPEED_MIN / 2;
    Pulse.colors.resize(1);
    modes.push_back(Pulse);

    mode ColorCycle;
    ColorCycle.name                     = "Spectrum Cycle";
    ColorCycle.value                    = GIGABYTE_AORUS_MOUSE_COLOR_CYCLE_MODE_VALUE;
    ColorCycle.flags                    = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    ColorCycle.color_mode               = MODE_COLORS_NONE;
    ColorCycle.brightness_min           = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MIN;
    ColorCycle.brightness_max           = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MAX;
    ColorCycle.brightness               = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MAX;
    ColorCycle.speed_min                = GIGABYTE_AORUS_MOUSE_SPEED_MIN;
    ColorCycle.speed_max                = GIGABYTE_AORUS_MOUSE_SPEED_MAX;
    ColorCycle.speed                    = GIGABYTE_AORUS_MOUSE_SPEED_MIN / 2;
    modes.push_back(ColorCycle);

    mode Flash;
    Flash.name                          = "Flashing";
    Flash.value                         = GIGABYTE_AORUS_MOUSE_FLASH_MODE_VALUE;
    Flash.flags                         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Flash.color_mode                    = MODE_COLORS_MODE_SPECIFIC;
    Flash.brightness_min                = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MIN;
    Flash.brightness_max                = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MAX;
    Flash.brightness                    = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MAX;
    Flash.speed_min                     = GIGABYTE_AORUS_MOUSE_SPEED_MIN;
    Flash.speed_max                     = GIGABYTE_AORUS_MOUSE_SPEED_MAX;
    Flash.speed                         = GIGABYTE_AORUS_MOUSE_SPEED_MIN / 2;
    Flash.colors.resize(1);
    modes.push_back(Flash);

    mode DoubleFlash;
    DoubleFlash.name                    = "Double Flash";
    DoubleFlash.value                   = GIGABYTE_AORUS_MOUSE_DOUBLE_FLASH_MODE_VALUE;
    DoubleFlash.flags                   = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    DoubleFlash.color_mode              = MODE_COLORS_MODE_SPECIFIC;
    DoubleFlash.brightness_min          = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MIN;
    DoubleFlash.brightness_max          = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MAX;
    DoubleFlash.brightness              = GIGABYTE_AORUS_MOUSE_BRIGHTNESS_MAX;
    DoubleFlash.speed_min               = GIGABYTE_AORUS_MOUSE_SPEED_MIN;
    DoubleFlash.speed_max               = GIGABYTE_AORUS_MOUSE_SPEED_MAX;
    DoubleFlash.speed                   = GIGABYTE_AORUS_MOUSE_SPEED_MIN / 2;
    DoubleFlash.colors.resize(1);
    modes.push_back(DoubleFlash);

    mode Off;
    Off.name                            = "Off";
    Off.value                           = GIGABYTE_AORUS_MOUSE_STATIC_MODE_VALUE;
    Off.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_GigabyteAorusMouse::~RGBController_GigabyteAorusMouse()
{
    delete controller;
}

void RGBController_GigabyteAorusMouse::SetupZones()
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

void RGBController_GigabyteAorusMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_GigabyteAorusMouse::DeviceUpdateLEDs()
{
    controller->SendDirect(colors[0]);
}

void RGBController_GigabyteAorusMouse::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GigabyteAorusMouse::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GigabyteAorusMouse::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | Brightness cannot be updated in the direct mode packet    |
    \*---------------------------------------------------------*/
    if(modes[active_mode].value == GIGABYTE_AORUS_MOUSE_DIRECT_MODE_VALUE)
    {
        controller->SetMode(colors[0], GIGABYTE_AORUS_MOUSE_STATIC_MODE_VALUE, modes[active_mode].brightness, 0);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        controller->SetMode(modes[active_mode].colors[0], modes[active_mode].value, modes[active_mode].brightness, modes[active_mode].speed);
    }
    else
    {
        controller->SetMode(0, modes[active_mode].value, modes[active_mode].brightness, modes[active_mode].speed);
    }
}
