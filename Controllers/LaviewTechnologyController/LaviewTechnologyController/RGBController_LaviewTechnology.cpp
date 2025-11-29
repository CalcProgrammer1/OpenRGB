/*---------------------------------------------------------*\
| RGBController_LaviewTechnology.cpp                        |
|                                                           |
|   RGBController for Laview Tech. mice, including Glorious |
|                                                           |
|   Kosta A (kostaarvanitis)                    27 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_LaviewTechnology.h"

/**------------------------------------------------------------------*\
    @name Laview Technology Mice
    @category Mouse
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectLaviewTechnologyMouse
    @comment
\*-------------------------------------------------------------------*/

RGBController_LaviewTechnology::RGBController_LaviewTechnology(LaviewTechnologyController* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetName();
    type                                = DEVICE_TYPE_MOUSE;
    description                         = "Glorious Device";
    vendor                              = controller->GetVendor();
    location                            = controller->GetLocation();
    serial                              = controller->GetSerialNumber();
    version                             = controller->GetFirmwareVersion();

    mode Custom;
    Custom.name                         = "Custom";
    Custom.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Custom.brightness_min               = LAVIEW_TECHNOLOGY_BRIGHTNESS_SLOW;
    Custom.brightness                   = LAVIEW_TECHNOLOGY_BRIGHTNESS_NORMAL;
    Custom.brightness_max               = LAVIEW_TECHNOLOGY_BRIGHTNESS_FAST;
    Custom.color_mode                   = MODE_COLORS_PER_LED;
    Custom.value                        = LAVIEW_TECHNOLOGY_MODE_STATIC;
    modes.push_back(Custom);

    mode Flashing;
    Flashing.name                       = "Flashing";
    Flashing.flags                      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Flashing.brightness_min             = LAVIEW_TECHNOLOGY_BRIGHTNESS_SLOW;
    Flashing.brightness                 = LAVIEW_TECHNOLOGY_BRIGHTNESS_NORMAL;
    Flashing.brightness_max             = LAVIEW_TECHNOLOGY_BRIGHTNESS_FAST;
    Flashing.speed_min                  = LAVIEW_TECHNOLOGY_SPEED_SLOW;
    Flashing.speed                      = LAVIEW_TECHNOLOGY_SPEED_NORMAL;
    Flashing.speed_max                  = LAVIEW_TECHNOLOGY_SPEED_FAST;
    Flashing.color_mode                 = MODE_COLORS_NONE;
    Flashing.value                      = LAVIEW_TECHNOLOGY_MODE_FLASHING;
    modes.push_back(Flashing);

    mode Chase;
    Chase.name                          = "Chase";
    Chase.flags                         = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Chase.brightness_min                = LAVIEW_TECHNOLOGY_BRIGHTNESS_SLOW;
    Chase.brightness                    = LAVIEW_TECHNOLOGY_BRIGHTNESS_NORMAL;
    Chase.brightness_max                = LAVIEW_TECHNOLOGY_BRIGHTNESS_FAST;
    Chase.speed_min                     = LAVIEW_TECHNOLOGY_SPEED_SLOW;
    Chase.speed                         = LAVIEW_TECHNOLOGY_SPEED_NORMAL;
    Chase.speed_max                     = LAVIEW_TECHNOLOGY_SPEED_FAST;
    Chase.color_mode                    = MODE_COLORS_NONE;
    Chase.value                         = LAVIEW_TECHNOLOGY_MODE_CHASE;
    modes.push_back(Chase);

    mode Wave;
    Wave.name                           = "Wave";
    Wave.flags                          = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Wave.brightness_min                 = LAVIEW_TECHNOLOGY_BRIGHTNESS_SLOW;
    Wave.brightness                     = LAVIEW_TECHNOLOGY_BRIGHTNESS_NORMAL;
    Wave.brightness_max                 = LAVIEW_TECHNOLOGY_BRIGHTNESS_FAST;
    Wave.speed_min                      = LAVIEW_TECHNOLOGY_SPEED_SLOW;
    Wave.speed                          = LAVIEW_TECHNOLOGY_SPEED_NORMAL;
    Wave.speed_max                      = LAVIEW_TECHNOLOGY_SPEED_FAST;
    Wave.color_mode                     = MODE_COLORS_NONE;
    Wave.value                          = LAVIEW_TECHNOLOGY_MODE_WAVE;
    modes.push_back(Wave);

    mode SpectrumCycle;
    SpectrumCycle.name                  = "Spectrum Cycle";
    SpectrumCycle.flags                 = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    SpectrumCycle.brightness_min        = LAVIEW_TECHNOLOGY_BRIGHTNESS_SLOW;
    SpectrumCycle.brightness            = LAVIEW_TECHNOLOGY_BRIGHTNESS_NORMAL;
    SpectrumCycle.brightness_max        = LAVIEW_TECHNOLOGY_BRIGHTNESS_FAST;
    SpectrumCycle.speed_min             = LAVIEW_TECHNOLOGY_SPEED_SLOW;
    SpectrumCycle.speed                 = LAVIEW_TECHNOLOGY_SPEED_NORMAL;
    SpectrumCycle.speed_max             = LAVIEW_TECHNOLOGY_SPEED_FAST;
    SpectrumCycle.color_mode            = MODE_COLORS_NONE;
    SpectrumCycle.value                 = LAVIEW_TECHNOLOGY_MODE_SPECTRUM_CYCLE;
    modes.push_back(SpectrumCycle);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.brightness_min            = LAVIEW_TECHNOLOGY_BRIGHTNESS_SLOW;
    Breathing.brightness                = LAVIEW_TECHNOLOGY_BRIGHTNESS_NORMAL;
    Breathing.brightness_max            = LAVIEW_TECHNOLOGY_BRIGHTNESS_FAST;
    Breathing.speed_min                 = LAVIEW_TECHNOLOGY_SPEED_SLOW;
    Breathing.speed                     = LAVIEW_TECHNOLOGY_SPEED_NORMAL;
    Breathing.speed_max                 = LAVIEW_TECHNOLOGY_SPEED_FAST;
    Breathing.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min                = 1;
    Breathing.colors_max                = 1;
    Breathing.colors.resize(1, ToRGBColor(255, 0, 0));
    Breathing.value                     = LAVIEW_TECHNOLOGY_MODE_BREATHING;
    modes.push_back(Breathing);

    mode SpectrumBreathing;
    SpectrumBreathing.name              = "Spectrum Breathing";
    SpectrumBreathing.flags             = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    SpectrumBreathing.brightness_min    = LAVIEW_TECHNOLOGY_BRIGHTNESS_SLOW;
    SpectrumBreathing.brightness        = LAVIEW_TECHNOLOGY_BRIGHTNESS_NORMAL;
    SpectrumBreathing.brightness_max    = LAVIEW_TECHNOLOGY_BRIGHTNESS_FAST;
    SpectrumBreathing.speed_min         = LAVIEW_TECHNOLOGY_SPEED_SLOW;
    SpectrumBreathing.speed             = LAVIEW_TECHNOLOGY_SPEED_NORMAL;
    SpectrumBreathing.speed_max         = LAVIEW_TECHNOLOGY_SPEED_FAST;
    SpectrumBreathing.color_mode        = MODE_COLORS_NONE;
    SpectrumBreathing.value             = LAVIEW_TECHNOLOGY_MODE_SPECTRUM_BREATHING;
    modes.push_back(SpectrumBreathing);

    mode RainbowWave;
    RainbowWave.name                    = "Rainbow Wave";
    RainbowWave.flags                   = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    RainbowWave.brightness_min          = LAVIEW_TECHNOLOGY_BRIGHTNESS_SLOW;
    RainbowWave.brightness              = LAVIEW_TECHNOLOGY_BRIGHTNESS_NORMAL;
    RainbowWave.brightness_max          = LAVIEW_TECHNOLOGY_BRIGHTNESS_FAST;
    RainbowWave.speed_min               = LAVIEW_TECHNOLOGY_SPEED_SLOW;
    RainbowWave.speed                   = LAVIEW_TECHNOLOGY_SPEED_NORMAL;
    RainbowWave.speed_max               = LAVIEW_TECHNOLOGY_SPEED_FAST;
    RainbowWave.color_mode              = MODE_COLORS_NONE;
    RainbowWave.value                   = LAVIEW_TECHNOLOGY_MODE_RAINBOW_WAVE;
    modes.push_back(RainbowWave);

    mode Off;
    Off.name                            = "Off";
    Off.flags                           = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode                      = MODE_COLORS_NONE;
    Off.value                           = LAVIEW_TECHNOLOGY_MODE_OFF;
    modes.push_back(Off);

    SetupZones();
}

void RGBController_LaviewTechnology::SetupZones()
{
    /*---------------------------------------------------------*\
    | Create a single zone                                      |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name       = "Mouse";
    new_zone.type       = ZONE_TYPE_SINGLE;
    new_zone.leds_min   = 1;
    new_zone.leds_max   = 1;
    new_zone.leds_count = 1;
    new_zone.matrix_map = NULL;
    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    led new_led;
    new_led.name = "LED";
    leds.push_back(new_led);

    SetupColors();
}

RGBController_LaviewTechnology::~RGBController_LaviewTechnology()
{
    delete controller;
}

void RGBController_LaviewTechnology::ResizeZone(int /*zone*/, int /*new_size*/)
{
}

void RGBController_LaviewTechnology::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_LaviewTechnology::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateMode();
}

void RGBController_LaviewTechnology::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateMode();
}

void RGBController_LaviewTechnology::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        controller->SetMode(modes[active_mode].value, modes[active_mode].brightness, modes[active_mode].speed, &colors[0]);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        controller->SetMode(modes[active_mode].value, modes[active_mode].brightness, modes[active_mode].speed, &modes[active_mode].colors[0]);
    }
    else // MODE_COLORS_NONE
    {
        controller->SetMode(modes[active_mode].value, modes[active_mode].brightness, modes[active_mode].speed, 0);
    }
}
