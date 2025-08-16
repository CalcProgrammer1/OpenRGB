/*---------------------------------------------------------*\
| RGBController_SinowealthGMOW.cpp                          |
|                                                           |
|   RGBController for Glorious Model O Wireless             |
|                                                           |
|   Matt Silva (thesilvanator)                     May 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

/**------------------------------------------------------------------*\
    @name Sinowealth Glorious Model O Wireless
    @type USB
    @save :white_check_mark:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectSinowealthMouse
    @comment
\*-------------------------------------------------------------------*/

#include "RGBController_SinowealthGMOW.h"

RGBController_GMOW::RGBController_GMOW(SinowealthGMOWController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetName();
    type                            = DEVICE_TYPE_MOUSE;
    description                     = "Sinowealth Device";
    location                        = controller->GetLocation();
    serial                          = controller->GetSerialString();
    version                         = controller->GetFirmwareVersion();

    mode Off;
    Off.name                        = "Off";
    Off.flags                       = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode                  = MODE_COLORS_NONE;
    Off.value                       = GMOW_MODE_OFF;
    modes.push_back(Off);

    mode RainbowWave;
    RainbowWave.name                = "Rainbow Wave";
    RainbowWave.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    RainbowWave.speed_min           = GMOW_SPEED1_MIN;
    RainbowWave.speed               = GMOW_SPEED1_MID;
    RainbowWave.speed_max           = GMOW_SPEED1_MAX;
    RainbowWave.direction           = MODE_DIRECTION_UP;
    RainbowWave.color_mode          = MODE_COLORS_NONE;
    RainbowWave.brightness_min      = GMOW_BRIGHTNESS_MIN;
    RainbowWave.brightness          = GMOW_BRIGHTNESS_MID;
    RainbowWave.brightness_max      = GMOW_BRIGHTNESS_MAX;
    RainbowWave.value               = GMOW_MODE_RAINBOW_WAVE;
    modes.push_back(RainbowWave);

    mode SpectrumCycle;
    SpectrumCycle.name              = "Spectrum Cycle";
    SpectrumCycle.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumCycle.speed_min         = GMOW_SPEED1_MIN;
    SpectrumCycle.speed             = GMOW_SPEED1_MID;
    SpectrumCycle.speed_max         = GMOW_SPEED1_MAX;
    SpectrumCycle.color_mode        = MODE_COLORS_NONE;
    SpectrumCycle.brightness_min    = GMOW_BRIGHTNESS_MIN;
    SpectrumCycle.brightness        = GMOW_BRIGHTNESS_MID;
    SpectrumCycle.brightness_max    = GMOW_BRIGHTNESS_MAX;
    SpectrumCycle.value             = GMOW_MODE_SPECTRUM_CYCLE;
    modes.push_back(SpectrumCycle);

    mode CustomBreathing;
    CustomBreathing.name            = "Custom Breathing";
    CustomBreathing.flags           = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    CustomBreathing.speed_min       = GMOW_SPEED1_MIN;
    CustomBreathing.speed           = GMOW_SPEED1_MID;
    CustomBreathing.speed_max       = GMOW_SPEED1_MAX;
    CustomBreathing.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    CustomBreathing.colors_min      = 2;
    CustomBreathing.colors_max      = 7;
    CustomBreathing.brightness_min  = GMOW_BRIGHTNESS_MIN;
    CustomBreathing.brightness      = GMOW_BRIGHTNESS_MID;
    CustomBreathing.brightness_max  = GMOW_BRIGHTNESS_MAX;
    CustomBreathing.value           = GMOW_MODE_CUSTOM_BREATHING;
    CustomBreathing.colors.resize(7);
    modes.push_back(CustomBreathing);

    mode Static;
    Static.name                     = "Static";
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.brightness_min           = GMOW_BRIGHTNESS_MIN;
    Static.brightness               = GMOW_BRIGHTNESS_MID;
    Static.brightness_max           = GMOW_BRIGHTNESS_MAX;
    Static.value                    = GMOW_MODE_STATIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.speed_min             = GMOW_SPEED1_MIN;
    Breathing.speed                 = GMOW_SPEED1_MID;
    Breathing.speed_max             = GMOW_SPEED1_MAX;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.brightness_min        = GMOW_BRIGHTNESS_MIN;
    Breathing.brightness            = GMOW_BRIGHTNESS_MID;
    Breathing.brightness_max        = GMOW_BRIGHTNESS_MAX;
    Breathing.value                 = GMOW_MODE_BREATHING;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Tail;
    Tail.name                       = "Tail";
    Tail.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Tail.speed_min                  = GMOW_SPEED1_MIN;
    Tail.speed                      = GMOW_SPEED1_MID;
    Tail.speed_max                  = GMOW_SPEED1_MAX;
    Tail.color_mode                 = MODE_COLORS_NONE;
    Tail.brightness_min             = GMOW_BRIGHTNESS_MIN;
    Tail.brightness                 = GMOW_BRIGHTNESS_MID;
    Tail.brightness_max             = GMOW_BRIGHTNESS_MAX;
    Tail.value                      = GMOW_MODE_TAIL;
    modes.push_back(Tail);

    mode Rave;
    Rave.name                       = "Rave";
    Rave.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Rave.speed_min                  = GMOW_SPEED2_MIN;
    Rave.speed                      = GMOW_SPEED2_MID;
    Rave.speed_max                  = GMOW_SPEED2_MAX;
    Rave.color_mode                 = MODE_COLORS_MODE_SPECIFIC;
    Rave.colors_min                 = 1;
    Rave.colors_max                 = 2;
    Rave.brightness_min             = GMOW_BRIGHTNESS_MIN;
    Rave.brightness                 = GMOW_BRIGHTNESS_MID;
    Rave.brightness_max             = GMOW_BRIGHTNESS_MAX;
    Rave.value                      = GMOW_MODE_RAVE;
    Rave.colors.resize(2);
    modes.push_back(Rave);

    mode Wave;
    Wave.name                       = "Wave";
    Wave.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.speed_min                  = GMOW_SPEED2_MIN;
    Wave.speed                      = GMOW_SPEED2_MID;
    Wave.speed_max                  = GMOW_SPEED2_MAX;
    Wave.color_mode                 = MODE_COLORS_NONE;
    Wave.brightness_min             = GMOW_BRIGHTNESS_MIN;
    Wave.brightness                 = GMOW_BRIGHTNESS_MID;
    Wave.brightness_max             = GMOW_BRIGHTNESS_MAX;
    Wave.value                      = GMOW_MODE_WAVE;
    modes.push_back(Wave);

    SetupZones();
}

RGBController_GMOW::~RGBController_GMOW()
{
    delete controller;
}

void RGBController_GMOW::SetupZones()
{

}

void RGBController_GMOW::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_GMOW::DeviceUpdateLEDs()
{

}

void RGBController_GMOW::UpdateZoneLEDs(int /*zone*/)
{

}

void RGBController_GMOW::UpdateSingleLED(int /*led*/)
{

}

void RGBController_GMOW::DeviceUpdateMode()
{
    mode curr = modes[active_mode];
    controller->SetMode(active_mode, curr.speed,curr.brightness, curr.brightness, curr.colors.data(), (unsigned char)curr.colors.size());
}
