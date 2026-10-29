/*---------------------------------------------------------*\
| RGBController_MSIClutchGM11.cpp                           |
|                                                           |
|   RGBController for MSI Clutch GM11 Gaming Mouse          |
|                                                           |
|   Anatoly U. (TTolikoLL)                      20 SEP 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_MSIClutchGM11.h"

/**------------------------------------------------------------------*\
    @name MSI Clutch GM11
    @category Mouse
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectMSIClutchGM11Controllers
\*-------------------------------------------------------------------*/

RGBController_MSIClutchGM11::RGBController_MSIClutchGM11(MSIClutchGM11Controller* controller_ptr)
{
    controller          = controller_ptr;

    name                = "MSI Clutch GM11";
    vendor              = "MSI";
    type                = DEVICE_TYPE_MOUSE;
    description         = "MSI Clutch GM11 Gaming Mouse";
    location            = controller->GetDeviceLocation();
    serial              = controller->GetSerialString();

    /*-----------------------------------------------------*\
    | Mode: Static (Supports full 24-bit custom RGB)        |
    \*-----------------------------------------------------*/
    mode Static;
    Static.name         = "Static";
    Static.value        = MSI_CLUTCH_GM11_MODE_STATIC;
    Static.flags        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    Static.colors[0]    = ToRGBColor(255, 0, 0);
    Static.brightness_min = MSI_CLUTCH_GM11_BRIGHTNESS_MIN;
    Static.brightness_max = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    Static.brightness   = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    modes.push_back(Static);

    /*-----------------------------------------------------*\
    | Mode: Breathing                                       |
    \*-----------------------------------------------------*/
    mode Breathing;
    Breathing.name      = "Breathing";
    Breathing.value     = MSI_CLUTCH_GM11_MODE_BREATHING;
    Breathing.flags     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.colors[0] = ToRGBColor(255, 0, 0);
    Breathing.brightness_min = MSI_CLUTCH_GM11_BRIGHTNESS_MIN;
    Breathing.brightness_max = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    Breathing.brightness = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    Breathing.speed_min = MSI_CLUTCH_GM11_SPEED_MIN;
    Breathing.speed_max = MSI_CLUTCH_GM11_SPEED_MAX;
    Breathing.speed     = 2;
    modes.push_back(Breathing);

    /*-----------------------------------------------------*\
    | Mode: Wave                                            |
    \*-----------------------------------------------------*/
    mode Wave;
    Wave.name           = "Wave";
    Wave.value          = MSI_CLUTCH_GM11_MODE_WAVE;
    Wave.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.color_mode     = MODE_COLORS_NONE;
    Wave.brightness_min = MSI_CLUTCH_GM11_BRIGHTNESS_MIN;
    Wave.brightness_max = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    Wave.brightness     = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    Wave.speed_min      = MSI_CLUTCH_GM11_SPEED_MIN;
    Wave.speed_max      = MSI_CLUTCH_GM11_SPEED_MAX;
    Wave.speed          = 2;
    Wave.direction      = MODE_DIRECTION_DOWN;
    modes.push_back(Wave);

    /*-----------------------------------------------------*\
    | Mode: Radar                                           |
    \*-----------------------------------------------------*/
    mode Radar;
    Radar.name          = "Radar";
    Radar.value         = MSI_CLUTCH_GM11_MODE_RADAR;
    Radar.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_AUTOMATIC_SAVE;
    Radar.color_mode    = MODE_COLORS_NONE;
    Radar.brightness_min = MSI_CLUTCH_GM11_BRIGHTNESS_MIN;
    Radar.brightness_max = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    Radar.brightness    = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    Radar.speed_min     = MSI_CLUTCH_GM11_SPEED_MIN;
    Radar.speed_max     = MSI_CLUTCH_GM11_SPEED_MAX;
    Radar.speed         = 2;
    Radar.direction     = MODE_DIRECTION_DOWN;
    modes.push_back(Radar);

    /*-----------------------------------------------------*\
    | Mode: Horizon                                         |
    \*-----------------------------------------------------*/
    mode Horizon;
    Horizon.name        = "Horizon";
    Horizon.value       = MSI_CLUTCH_GM11_MODE_HORIZON;
    Horizon.flags       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_AUTOMATIC_SAVE;
    Horizon.color_mode  = MODE_COLORS_NONE;
    Horizon.brightness_min = MSI_CLUTCH_GM11_BRIGHTNESS_MIN;
    Horizon.brightness_max = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    Horizon.brightness  = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    Horizon.speed_min   = MSI_CLUTCH_GM11_SPEED_MIN;
    Horizon.speed_max   = MSI_CLUTCH_GM11_SPEED_MAX;
    Horizon.speed       = 2;
    Horizon.direction   = MODE_DIRECTION_DOWN;
    modes.push_back(Horizon);

    /*-----------------------------------------------------*\
    | Mode: Ripple                                          |
    \*-----------------------------------------------------*/
    mode Ripple;
    Ripple.name         = "Ripple";
    Ripple.value        = MSI_CLUTCH_GM11_MODE_RIPPLE;
    Ripple.flags        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_AUTOMATIC_SAVE;
    Ripple.color_mode   = MODE_COLORS_NONE;
    Ripple.brightness_min = MSI_CLUTCH_GM11_BRIGHTNESS_MIN;
    Ripple.brightness_max = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    Ripple.brightness   = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    Ripple.speed_min    = MSI_CLUTCH_GM11_SPEED_MIN;
    Ripple.speed_max    = MSI_CLUTCH_GM11_SPEED_MAX;
    Ripple.speed        = 2;
    Ripple.direction    = MODE_DIRECTION_DOWN;
    modes.push_back(Ripple);

    /*-----------------------------------------------------*\
    | Mode: Reactive                                        |
    \*-----------------------------------------------------*/
    mode Reactive;
    Reactive.name       = "Reactive";
    Reactive.value      = MSI_CLUTCH_GM11_MODE_REACTIVE;
    Reactive.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Reactive.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Reactive.colors.resize(1);
    Reactive.colors[0]  = ToRGBColor(255, 0, 0);
    Reactive.brightness_min = MSI_CLUTCH_GM11_BRIGHTNESS_MIN;
    Reactive.brightness_max = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    Reactive.brightness = MSI_CLUTCH_GM11_BRIGHTNESS_MAX;
    Reactive.speed_min  = MSI_CLUTCH_GM11_SPEED_MIN;
    Reactive.speed_max  = MSI_CLUTCH_GM11_SPEED_MAX;
    Reactive.speed      = 2;
    modes.push_back(Reactive);

    /*-----------------------------------------------------*\
    | Mode: Off                                             |
    \*-----------------------------------------------------*/
    mode Off;
    Off.name            = "Off";
    Off.value           = MSI_CLUTCH_GM11_MODE_OFF;
    Off.flags           = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_MSIClutchGM11::~RGBController_MSIClutchGM11()
{
    delete controller;
}

void RGBController_MSIClutchGM11::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Mouse";
    new_zone.type       = ZONE_TYPE_SINGLE;
    new_zone.leds_min   = MSI_CLUTCH_GM11_LEDS_COUNT;
    new_zone.leds_max   = MSI_CLUTCH_GM11_LEDS_COUNT;
    new_zone.leds_count = MSI_CLUTCH_GM11_LEDS_COUNT;

    zones.push_back(new_zone);

    leds.resize(new_zone.leds_count);
    leds[0].name        = "Logo & Underglow";

    SetupColors();
}

void RGBController_MSIClutchGM11::DeviceUpdateLEDs(std::vector<RGBColor>& update_colors)
{
    const mode& active = modes[active_mode];

    if(active.value == MSI_CLUTCH_GM11_MODE_OFF)
    {
        controller->SetMode(0, MSI_CLUTCH_GM11_MODE_OFF, 0, 0, 0, MODE_COLORS_NONE);
    }
    else
    {
        RGBColor current_color = colors[0];

        if(active.color_mode == MODE_COLORS_MODE_SPECIFIC && !active.colors.empty())
        {
            current_color = active.colors[0];
        }

        controller->SetMode(current_color,
                            active.value,
                            active.brightness,
                            active.speed,
                            active.direction,
                            active.color_mode);
    }
}

void RGBController_MSIClutchGM11::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs(colors);
}

void RGBController_MSIClutchGM11::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs(colors);
}

void RGBController_MSIClutchGM11::DeviceUpdateMode()
{
    DeviceUpdateLEDs(colors);
}