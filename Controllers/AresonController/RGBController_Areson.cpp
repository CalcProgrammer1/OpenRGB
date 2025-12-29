/*---------------------------------------------------------*\
| RGBController_Areson.cpp                                  |
|                                                           |
|   RGBController for Areson mice                           |
|                                                           |
|   Morgan Guimard (morg)                       29 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_Areson.h"

/**------------------------------------------------------------------*\
    @name Areson
    @category Mouse
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectAresonControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_Areson::RGBController_Areson(AresonController* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetNameString();
    vendor                              = "Areson";
    type                                = DEVICE_TYPE_MOUSE;
    description                         = "Areson mouse";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Static;
    Static.name                         = "Static";
    Static.value                        = STATIC_MODE_VALUE;
    Static.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode                   = MODE_COLORS_PER_LED;
    Static.brightness_min               = ARESON_BRIGHTNESS_MIN;
    Static.brightness_max               = ARESON_BRIGHTNESS_MAX;
    Static.brightness                   = ARESON_BRIGHTNESS_MAX;
    modes.push_back(Static);

    mode RainbowWave;
    RainbowWave.name                    = "Rainbow Wave";
    RainbowWave.value                   = RAINBOW_WAVE_MODE_VALUE;
    RainbowWave.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    RainbowWave.color_mode              = MODE_COLORS_NONE;
    RainbowWave.brightness_min          = ARESON_BRIGHTNESS_MIN;
    RainbowWave.brightness_max          = ARESON_BRIGHTNESS_MAX;
    RainbowWave.brightness              = ARESON_BRIGHTNESS_MAX;
    RainbowWave.speed_min               = ARESON_SPEED_MIN;
    RainbowWave.speed_max               = ARESON_SPEED_MAX;
    RainbowWave.speed                   = ARESON_SPEED_MAX;
    modes.push_back(RainbowWave);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = BREATHING_MODE_VALUE;
    Breathing.flags                     = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode                = MODE_COLORS_PER_LED;
    Breathing.brightness_min            = ARESON_BRIGHTNESS_MIN;
    Breathing.brightness_max            = ARESON_BRIGHTNESS_MAX;
    Breathing.brightness                = ARESON_BRIGHTNESS_MAX;
    Breathing.speed_min                 = ARESON_SPEED_MIN;
    Breathing.speed_max                 = ARESON_SPEED_MAX;
    Breathing.speed                     = ARESON_SPEED_MIN;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode SpectrumCycle;
    SpectrumCycle.name                  = "Spectrum Cycle";
    SpectrumCycle.value                 = SPECRTUM_CYCLE_MODE_VALUE;
    SpectrumCycle.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumCycle.color_mode            = MODE_COLORS_NONE;
    SpectrumCycle.brightness_min        = ARESON_BRIGHTNESS_MIN;
    SpectrumCycle.brightness_max        = ARESON_BRIGHTNESS_MAX;
    SpectrumCycle.brightness            = ARESON_BRIGHTNESS_MAX;
    SpectrumCycle.speed_min             = ARESON_SPEED_MIN;
    SpectrumCycle.speed_max             = ARESON_SPEED_MAX;
    SpectrumCycle.speed                 = ARESON_SPEED_MAX;
    modes.push_back(SpectrumCycle);

    mode SingleColorWave;
    SingleColorWave.name                = "Single Color Wave";
    SingleColorWave.value               = SINGLE_COLOR_WAVE_MODE_VALUE;
    SingleColorWave.flags               = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    SingleColorWave.color_mode          = MODE_COLORS_PER_LED;
    SingleColorWave.brightness_min      = ARESON_BRIGHTNESS_MIN;
    SingleColorWave.brightness_max      = ARESON_BRIGHTNESS_MAX;
    SingleColorWave.brightness          = ARESON_BRIGHTNESS_MAX;
    SingleColorWave.speed_min           = ARESON_SPEED_MIN;
    SingleColorWave.speed_max           = ARESON_SPEED_MAX;
    SingleColorWave.speed               = ARESON_SPEED_MAX;
    SingleColorWave.colors.resize(1);
    modes.push_back(SingleColorWave);

    mode ColorfulBreathing;
    ColorfulBreathing.name              = "Colorful Breathing";
    ColorfulBreathing.value             = BREATHING_COLORFUL_MODE_VALUE;
    ColorfulBreathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    ColorfulBreathing.color_mode        = MODE_COLORS_NONE;
    ColorfulBreathing.brightness_min    = ARESON_BRIGHTNESS_MIN;
    ColorfulBreathing.brightness_max    = ARESON_BRIGHTNESS_MAX;
    ColorfulBreathing.brightness        = ARESON_BRIGHTNESS_MAX;
    ColorfulBreathing.speed_min         = ARESON_SPEED_MIN;
    ColorfulBreathing.speed_max         = ARESON_SPEED_MAX;
    ColorfulBreathing.speed             = ARESON_SPEED_MAX;
    ColorfulBreathing.colors.resize(1);
    modes.push_back(ColorfulBreathing);

    mode OFF;
    OFF.name                            = "Off";
    OFF.value                           = OFF_MODE_VALUE;
    OFF.flags                           = MODE_FLAG_AUTOMATIC_SAVE;
    OFF.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(OFF);

    SetupZones();
}

RGBController_Areson::~RGBController_Areson()
{
    delete controller;
}

void RGBController_Areson::SetupZones()
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

void RGBController_Areson::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Areson::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_Areson::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateMode();
}

void RGBController_Areson::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateMode();
}

void RGBController_Areson::DeviceUpdateMode()
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
