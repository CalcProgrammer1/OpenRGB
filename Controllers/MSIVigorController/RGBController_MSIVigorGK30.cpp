/*---------------------------------------------------------*\
| RGBController_MSIVigorGK30.cpp                            |
|                                                           |
|   RGBController for MSI Vigor GK30                        |
|                                                           |
|   Morgan Guimard (morg)                       01 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_MSIVigorGK30.h"

/**------------------------------------------------------------------*\
    @name MSI Vigor GK30
    @category Keyboard
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectMSIVigorGK30Controllers
    @comment This device does only support 7 different colors
\*-------------------------------------------------------------------*/

RGBController_MSIVigorGK30::RGBController_MSIVigorGK30(MSIVigorGK30Controller* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetNameString();
    vendor                              = "MSI";
    type                                = DEVICE_TYPE_KEYBOARD;
    description                         = "MSI VigorGK30 USB Device";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Custom;
    Custom.name                         = "Custom";
    Custom.value                        = MSI_VIGOR_GK30_CUSTOM_MODE_VALUE;
    Custom.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Custom.brightness                   = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Custom.brightness_min               = MSI_VIGOR_GK30_BRIGHTNESS_MIN;
    Custom.brightness_max               = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Custom.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode Static;
    Static.name                         = "Static";
    Static.value                        = MSI_VIGOR_GK30_STATIC_MODE_VALUE;
    Static.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.brightness                   = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Static.brightness_min               = MSI_VIGOR_GK30_BRIGHTNESS_MIN;
    Static.brightness_max               = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Static.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Off;
    Off.name                            = "Off";
    Off.value                           = MSI_VIGOR_GK30_OFF_MODE_VALUE;
    Off.flags                           = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = MSI_VIGOR_GK30_BREATHING_MODE_VALUE;
    Breathing.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.brightness                = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Breathing.brightness_min            = MSI_VIGOR_GK30_BRIGHTNESS_MIN;
    Breathing.brightness_max            = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Breathing.speed                     = MSI_VIGOR_GK30_SPEED_MIN;
    Breathing.speed_min                 = MSI_VIGOR_GK30_SPEED_MIN;
    Breathing.speed_max                 = MSI_VIGOR_GK30_SPEED_MAX;
    Breathing.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Rainbow;
    Rainbow.name                        = "Rainbow";
    Rainbow.value                       = MSI_VIGOR_GK30_RAINBOW_MODE_VALUE;
    Rainbow.flags                       = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_DIRECTION_LR;
    Rainbow.color_mode                  = MODE_COLORS_NONE;
    Rainbow.brightness                  = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Rainbow.brightness_min              = MSI_VIGOR_GK30_BRIGHTNESS_MIN;
    Rainbow.brightness_max              = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Rainbow.speed                       = MSI_VIGOR_GK30_SPEED_MIN;
    Rainbow.speed_min                   = MSI_VIGOR_GK30_SPEED_MIN;
    Rainbow.speed_max                   = MSI_VIGOR_GK30_SPEED_MAX;
    Rainbow.direction                   = MODE_DIRECTION_LEFT;
    modes.push_back(Rainbow);

    mode Meteor;
    Meteor.name                         = "Meteor";
    Meteor.value                        = MSI_VIGOR_GK30_METEOR_MODE_VALUE;
    Meteor.flags                        = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_DIRECTION_LR;
    Meteor.color_mode                   = MODE_COLORS_NONE;
    Meteor.brightness                   = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Meteor.brightness_min               = MSI_VIGOR_GK30_BRIGHTNESS_MIN;
    Meteor.brightness_max               = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Meteor.speed                        = MSI_VIGOR_GK30_SPEED_MIN;
    Meteor.speed_min                    = MSI_VIGOR_GK30_SPEED_MIN;
    Meteor.speed_max                    = MSI_VIGOR_GK30_SPEED_MAX;
    Meteor.direction                    = MODE_DIRECTION_LEFT;
    modes.push_back(Meteor);

    mode Ripple;
    Ripple.name                         = "Ripple";
    Ripple.value                        = MSI_VIGOR_GK30_RIPPLE_MODE_VALUE;
    Ripple.flags                        = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Ripple.brightness                   = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Ripple.brightness_min               = MSI_VIGOR_GK30_BRIGHTNESS_MIN;
    Ripple.brightness_max               = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Ripple.speed                        = MSI_VIGOR_GK30_SPEED_MIN;
    Ripple.speed_min                    = MSI_VIGOR_GK30_SPEED_MIN;
    Ripple.speed_max                    = MSI_VIGOR_GK30_SPEED_MAX;
    Ripple.color_mode                   = MODE_COLORS_NONE;
    modes.push_back(Ripple);


    mode Dimming;
    Dimming.name                        = "Dimming";
    Dimming.value                       = MSI_VIGOR_GK30_DIMMING_MODE_VALUE;
    Dimming.flags                       = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Dimming.brightness                  = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Dimming.brightness_min              = MSI_VIGOR_GK30_BRIGHTNESS_MIN;
    Dimming.brightness_max              = MSI_VIGOR_GK30_BRIGHTNESS_MAX;
    Dimming.speed                       = MSI_VIGOR_GK30_SPEED_MIN;
    Dimming.speed_min                   = MSI_VIGOR_GK30_SPEED_MIN;
    Dimming.speed_max                   = MSI_VIGOR_GK30_SPEED_MAX;
    Dimming.color_mode                  = MODE_COLORS_MODE_SPECIFIC;
    Dimming.colors.resize(1);
    modes.push_back(Dimming);

    SetupZones();
}

RGBController_MSIVigorGK30::~RGBController_MSIVigorGK30()
{
    delete controller;
}

void RGBController_MSIVigorGK30::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Keyboard";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = MSI_VIGOR_GK30_LEDS_COUNT;
    new_zone.leds_max   = MSI_VIGOR_GK30_LEDS_COUNT;
    new_zone.leds_count = MSI_VIGOR_GK30_LEDS_COUNT;
    new_zone.matrix_map = nullptr;

    zones.emplace_back(new_zone);

    leds.resize(new_zone.leds_count);

    for(unsigned int i = 0; i < MSI_VIGOR_GK30_LEDS_COUNT; i++)
    {
        leds[i].name = "LED " + std::to_string(i);
    }

    SetupColors();
}

void RGBController_MSIVigorGK30::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_MSIVigorGK30::DeviceUpdateLEDs()
{
    UpdateSingleLED(0);
}

void RGBController_MSIVigorGK30::UpdateZoneLEDs(int /*zone*/)
{
    UpdateSingleLED(0);
}

void RGBController_MSIVigorGK30::UpdateSingleLED(int /*led*/)
{
    const mode& active = modes[active_mode];

    controller->SetMode(
                active.color_mode == MODE_COLORS_MODE_SPECIFIC ? active.colors : colors,
                active.brightness,
                active.speed,
                active.value,
                active.flags,
                active.color_mode,
                active.direction
                );
}

void RGBController_MSIVigorGK30::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
