/*---------------------------------------------------------*\
| RGBController_LenovoM300.cpp                              |
|                                                           |
|   RGBController for Lenovo Legion M300 mouse              |
|                                                           |
|   Wayne Riordan                               09 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_LenovoM300.h"

/**------------------------------------------------------------------*\
    @name Lenovo Legion M300
    @category Mouse
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectLenovoLegionM300Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_LenovoM300::RGBController_LenovoM300(LenovoM300Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "Lenovo";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "Lenovo M300 Device";
    location                    = controller->GetDeviceLocation();

    mode Static;
    Static.name                 = "Static";
    Static.value                = M300_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.brightness_max       = M300_MAX_BRIGTH;
    Static.brightness_min       = M300_MIN_BRIGHT;
    Static.brightness           = M300_MAX_BRIGTH;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = M300_MODE_OFF;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = M300_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.brightness_max    = M300_MAX_BRIGTH;
    Breathing.brightness_min    = M300_MIN_BRIGHT;
    Breathing.brightness        = M300_MAX_BRIGTH;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(2);
    modes.push_back(Breathing);

    mode Spectrum;
    Spectrum.name               = "Spectrum Cycle";
    Spectrum.value              = M300_MODE_RAINBOW;
    Spectrum.flags              = MODE_FLAG_AUTOMATIC_SAVE;
    Spectrum.color_mode         = MODE_COLORS_NONE;
    modes.push_back(Spectrum);

    SetupZones();
}

RGBController_LenovoM300::~RGBController_LenovoM300()
{
    delete controller;
}

void RGBController_LenovoM300::SetupZones()
{
    zone default_zone;
    default_zone.name           = "Mouse";
    default_zone.type           = ZONE_TYPE_SINGLE;
    default_zone.leds_min       = 1;
    default_zone.leds_max       = 1;
    default_zone.leds_count     = 1;
    default_zone.matrix_map     = nullptr;
    zones.emplace_back(default_zone);

    leds.resize(1);
    leds[0].name = "LED 1";

    SetupColors();
}

void RGBController_LenovoM300::ResizeZone(int /*zone*/, int /*new_size*/)
{
    // Not Supported
}

void RGBController_LenovoM300::DeviceUpdateLEDs()
{
    const mode& active = modes[active_mode];
    controller->SetMode(active.colors, active.value, active.brightness);
}

void RGBController_LenovoM300::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LenovoM300::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LenovoM300::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
