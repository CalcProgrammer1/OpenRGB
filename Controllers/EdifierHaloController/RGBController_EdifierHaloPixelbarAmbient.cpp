/*---------------------------------------------------------*\
| RGBController_EdifierHaloPixelbarAmbient.cpp              |
|                                                           |
|   RGBController for Edifier Halo PixelBar Ambient Light   |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  20 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_EdifierHaloPixelbarAmbient.h"

/**------------------------------------------------------------------*\
    @name Edifier Halo Pixelbar
    @category Speaker
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectEdifierHaloControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_EdifierHaloPixelbarAmbient::RGBController_EdifierHaloPixelbarAmbient(EdifierHaloController* controller_ptr)
{
    controller                = controller_ptr;
    first_leds_update_ignored = false;
    first_mode_update_ignored = false;

    name                = "Edifier Halo Pixelbar - Light Effects";
    vendor              = "Edifier";
    type                = DEVICE_TYPE_SPEAKER;
    description         = "Edifier Halo PixelBar Light Effects";
    serial              = controller->GetSerial();
    location            = controller->GetLocation();

    // Setup modes
    mode breathing_mode;
    breathing_mode.name           = "Breathing";
    breathing_mode.value          = EDIFIER_HALO_AMBIENT_MODE_BREATHING;
    breathing_mode.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    breathing_mode.color_mode     = MODE_COLORS_PER_LED;
    breathing_mode.speed_min      = EDIFIER_HALO_AMBIENT_SPEED_SLOWEST;
    breathing_mode.speed_max      = EDIFIER_HALO_AMBIENT_SPEED_FASTEST;
    breathing_mode.speed          = EDIFIER_HALO_AMBIENT_SPEED_FASTEST;
    breathing_mode.brightness_min = EDIFIER_HALO_AMBIENT_BRIGHTNESS_LOWEST;
    breathing_mode.brightness_max = EDIFIER_HALO_AMBIENT_BRIGHTNESS_HIGHEST;
    breathing_mode.brightness     = EDIFIER_HALO_AMBIENT_BRIGHTNESS_HIGHEST;
    modes.push_back(breathing_mode);

    mode tide_mode;
    tide_mode.name           = "Rainbow";
    tide_mode.value          = EDIFIER_HALO_AMBIENT_MODE_TIDE;
    tide_mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    tide_mode.color_mode     = MODE_COLORS_NONE;
    tide_mode.speed_min      = EDIFIER_HALO_AMBIENT_SPEED_SLOWEST;
    tide_mode.speed_max      = EDIFIER_HALO_AMBIENT_SPEED_FASTEST;
    tide_mode.speed          = EDIFIER_HALO_AMBIENT_SPEED_FASTEST;
    tide_mode.brightness_min = EDIFIER_HALO_AMBIENT_BRIGHTNESS_LOWEST;
    tide_mode.brightness_max = EDIFIER_HALO_AMBIENT_BRIGHTNESS_HIGHEST;
    tide_mode.brightness     = EDIFIER_HALO_AMBIENT_BRIGHTNESS_HIGHEST;
    modes.push_back(tide_mode);

    mode static_mode;
    static_mode.name           = "Static";
    static_mode.value          = EDIFIER_HALO_AMBIENT_MODE_STATIC;
    static_mode.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    static_mode.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    static_mode.brightness_min = EDIFIER_HALO_AMBIENT_BRIGHTNESS_LOWEST;
    static_mode.brightness_max = EDIFIER_HALO_AMBIENT_BRIGHTNESS_HIGHEST;
    static_mode.brightness     = EDIFIER_HALO_AMBIENT_BRIGHTNESS_HIGHEST;
    modes.push_back(static_mode);

    mode ripple_mode;
    ripple_mode.name           = "Glittering and colorful";
    ripple_mode.value          = EDIFIER_HALO_AMBIENT_MODE_RIPPLE;
    ripple_mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    ripple_mode.color_mode     = MODE_COLORS_NONE;
    ripple_mode.speed_min      = EDIFIER_HALO_AMBIENT_SPEED_SLOWEST;
    ripple_mode.speed_max      = EDIFIER_HALO_AMBIENT_SPEED_FASTEST;
    ripple_mode.speed          = EDIFIER_HALO_AMBIENT_SPEED_FASTEST;
    ripple_mode.brightness_min = EDIFIER_HALO_AMBIENT_BRIGHTNESS_LOWEST;
    ripple_mode.brightness_max = EDIFIER_HALO_AMBIENT_BRIGHTNESS_HIGHEST;
    ripple_mode.brightness     = EDIFIER_HALO_AMBIENT_BRIGHTNESS_HIGHEST;
    modes.push_back(ripple_mode);

    mode flow_mode;
    flow_mode.name           = "Light-flow";
    flow_mode.value          = EDIFIER_HALO_AMBIENT_MODE_FLOW;
    flow_mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    flow_mode.color_mode     = MODE_COLORS_NONE;
    flow_mode.speed_min      = EDIFIER_HALO_AMBIENT_SPEED_SLOWEST;
    flow_mode.speed_max      = EDIFIER_HALO_AMBIENT_SPEED_FASTEST;
    flow_mode.speed          = EDIFIER_HALO_AMBIENT_SPEED_FASTEST;
    flow_mode.brightness_min = EDIFIER_HALO_AMBIENT_BRIGHTNESS_LOWEST;
    flow_mode.brightness_max = EDIFIER_HALO_AMBIENT_BRIGHTNESS_HIGHEST;
    flow_mode.brightness     = EDIFIER_HALO_AMBIENT_BRIGHTNESS_HIGHEST;
    modes.push_back(flow_mode);

    mode dynamic_mode;
    dynamic_mode.name           = "Dynamic Shadows";
    dynamic_mode.value          = EDIFIER_HALO_AMBIENT_MODE_DYNAMIC;
    dynamic_mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    dynamic_mode.color_mode     = MODE_COLORS_NONE;
    dynamic_mode.speed_min      = EDIFIER_HALO_AMBIENT_SPEED_SLOWEST;
    dynamic_mode.speed_max      = EDIFIER_HALO_AMBIENT_SPEED_FASTEST;
    dynamic_mode.speed          = EDIFIER_HALO_AMBIENT_SPEED_FASTEST;
    dynamic_mode.brightness_min = EDIFIER_HALO_AMBIENT_BRIGHTNESS_LOWEST;
    dynamic_mode.brightness_max = EDIFIER_HALO_AMBIENT_BRIGHTNESS_HIGHEST;
    dynamic_mode.brightness     = EDIFIER_HALO_AMBIENT_BRIGHTNESS_HIGHEST;
    modes.push_back(dynamic_mode);

    // Default to Static mode
    active_mode = 2;

    SetupZones();
}

RGBController_EdifierHaloPixelbarAmbient::~RGBController_EdifierHaloPixelbarAmbient()
{
}

void RGBController_EdifierHaloPixelbarAmbient::SetupZones()
{
    zone ambient_zone;
    ambient_zone.name       = "Light Effects";
    ambient_zone.type       = ZONE_TYPE_SINGLE;
    ambient_zone.leds_min   = 1;
    ambient_zone.leds_max   = 1;
    ambient_zone.leds_count = 1;
    zones.push_back(ambient_zone);

    led ambient_led;
    ambient_led.name        = "Light Effects";
    leds.push_back(ambient_led);

    SetupColors();
}

void RGBController_EdifierHaloPixelbarAmbient::DeviceUpdateLEDs()
{
    if(!first_leds_update_ignored)
    {
        first_leds_update_ignored = true;
        return;
    }

    int active_idx = GetActiveMode();
    RGBColor color = colors[0];
    unsigned char r = RGBGetRValue(color);
    unsigned char g = RGBGetGValue(color);
    unsigned char b = RGBGetBValue(color);

    unsigned char effect = modes[active_idx].value;
    unsigned char brightness = modes[active_idx].brightness;
    unsigned char speed = modes[active_idx].speed;
    
    controller->SetAmbientLightPower(true);
    controller->SetAmbientLight(effect, r, g, b, brightness, speed);
}

void RGBController_EdifierHaloPixelbarAmbient::DeviceUpdateZoneLEDs(int)
{
    DeviceUpdateLEDs();
}

void RGBController_EdifierHaloPixelbarAmbient::DeviceUpdateSingleLED(int)
{
    DeviceUpdateLEDs();
}

void RGBController_EdifierHaloPixelbarAmbient::DeviceUpdateMode()
{
    if(!first_mode_update_ignored)
    {
        first_mode_update_ignored = true;
        return;
    }

    DeviceUpdateLEDs();
}
