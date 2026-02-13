/*---------------------------------------------------------*\
| RGBController_ClevoLightbar.cpp                           |
|                                                           |
|   Generic RGB Interface for Clevo laptop lightbar         |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_ClevoLightbar.h"

/**------------------------------------------------------------------*\
    @name CLEVO Lightbar
    @category LEDStrip
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectClevoLightbarControllers
    @comment Experimental effects based on ITE 8291 protocol
\*-------------------------------------------------------------------*/

RGBController_ClevoLightbar::RGBController_ClevoLightbar(ClevoLightbarController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "CLEVO Lightbar";
    vendor      = "CLEVO Computers";
    type        = DEVICE_TYPE_LEDSTRIP;
    description = "CLEVO Laptop Lightbar";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();
    version     = controller->GetFirmwareVersion();

    mode Direct;
    Direct.name           = "Direct";
    Direct.value          = CLEVO_LIGHTBAR_MODE_DIRECT;
    Direct.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode     = MODE_COLORS_PER_LED;
    Direct.brightness_min = CLEVO_LIGHTBAR_BRIGHTNESS_MIN;
    Direct.brightness_max = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    Direct.brightness     = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name           = "Breathing";
    Breathing.value          = CLEVO_LIGHTBAR_MODE_BREATHING;
    Breathing.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode     = MODE_COLORS_PER_LED;
    Breathing.brightness_min = CLEVO_LIGHTBAR_BRIGHTNESS_MIN;
    Breathing.brightness_max = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    Breathing.brightness     = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    Breathing.speed_min      = CLEVO_LIGHTBAR_SPEED_MIN;
    Breathing.speed_max      = CLEVO_LIGHTBAR_SPEED_MAX;
    Breathing.speed          = CLEVO_LIGHTBAR_SPEED_DEFAULT;
    modes.push_back(Breathing);

    mode Wave;
    Wave.name           = "Wave";
    Wave.value          = CLEVO_LIGHTBAR_MODE_WAVE;
    Wave.flags          = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Wave.color_mode     = MODE_COLORS_NONE;
    Wave.brightness_min = CLEVO_LIGHTBAR_BRIGHTNESS_MIN;
    Wave.brightness_max = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    Wave.brightness     = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    Wave.speed_min      = CLEVO_LIGHTBAR_SPEED_MIN;
    Wave.speed_max      = CLEVO_LIGHTBAR_SPEED_MAX;
    Wave.speed          = CLEVO_LIGHTBAR_SPEED_DEFAULT;
    modes.push_back(Wave);

    mode Bounce;
    Bounce.name           = "Bounce";
    Bounce.value          = CLEVO_LIGHTBAR_MODE_BOUNCE;
    Bounce.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Bounce.color_mode     = MODE_COLORS_PER_LED;
    Bounce.brightness_min = CLEVO_LIGHTBAR_BRIGHTNESS_MIN;
    Bounce.brightness_max = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    Bounce.brightness     = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    Bounce.speed_min      = CLEVO_LIGHTBAR_SPEED_MIN;
    Bounce.speed_max      = CLEVO_LIGHTBAR_SPEED_MAX;
    Bounce.speed          = CLEVO_LIGHTBAR_SPEED_DEFAULT;
    modes.push_back(Bounce);

    mode Marquee;
    Marquee.name           = "Marquee";
    Marquee.value          = CLEVO_LIGHTBAR_MODE_MARQUEE;
    Marquee.flags          = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Marquee.color_mode     = MODE_COLORS_NONE;
    Marquee.brightness_min = CLEVO_LIGHTBAR_BRIGHTNESS_MIN;
    Marquee.brightness_max = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    Marquee.brightness     = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    Marquee.speed_min      = CLEVO_LIGHTBAR_SPEED_MIN;
    Marquee.speed_max      = CLEVO_LIGHTBAR_SPEED_MAX;
    Marquee.speed          = CLEVO_LIGHTBAR_SPEED_DEFAULT;
    modes.push_back(Marquee);

    mode Scan;
    Scan.name           = "Scan";
    Scan.value          = CLEVO_LIGHTBAR_MODE_SCAN;
    Scan.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Scan.color_mode     = MODE_COLORS_PER_LED;
    Scan.brightness_min = CLEVO_LIGHTBAR_BRIGHTNESS_MIN;
    Scan.brightness_max = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    Scan.brightness     = CLEVO_LIGHTBAR_BRIGHTNESS_MAX;
    Scan.speed_min      = CLEVO_LIGHTBAR_SPEED_MIN;
    Scan.speed_max      = CLEVO_LIGHTBAR_SPEED_MAX;
    Scan.speed          = CLEVO_LIGHTBAR_SPEED_DEFAULT;
    modes.push_back(Scan);

    mode Off;
    Off.name       = "Off";
    Off.value      = CLEVO_LIGHTBAR_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_ClevoLightbar::~RGBController_ClevoLightbar()
{
    delete controller;
}

void RGBController_ClevoLightbar::SetupZones()
{
    zone lightbar_zone;
    lightbar_zone.name       = "Lightbar";
    lightbar_zone.type       = ZONE_TYPE_SINGLE;
    lightbar_zone.leds_min   = 1;
    lightbar_zone.leds_max   = 1;
    lightbar_zone.leds_count = 1;
    lightbar_zone.matrix_map = NULL;
    zones.push_back(lightbar_zone);

    led lightbar_led;
    lightbar_led.name = "Lightbar";
    leds.push_back(lightbar_led);

    SetupColors();
}

void RGBController_ClevoLightbar::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ClevoLightbar::DeviceUpdateLEDs()
{
    unsigned char red   = RGBGetRValue(colors[0]);
    unsigned char green = RGBGetGValue(colors[0]);
    unsigned char blue  = RGBGetBValue(colors[0]);

    controller->SetColor(red, green, blue);

    /*---------------------------------------------------------*\
    | Re-apply current mode to maintain effect state            |
    \*---------------------------------------------------------*/
    unsigned char brightness = modes[active_mode].brightness;
    unsigned char speed      = modes[active_mode].speed;
    unsigned char mode_value = modes[active_mode].value;

    controller->SetMode(mode_value, brightness, speed);
}

void RGBController_ClevoLightbar::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ClevoLightbar::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ClevoLightbar::DeviceUpdateMode()
{
    if(modes[active_mode].value == CLEVO_LIGHTBAR_MODE_OFF)
    {
        controller->TurnOff();
    }
    else
    {
        unsigned char brightness = modes[active_mode].brightness;
        unsigned char speed      = modes[active_mode].speed;
        unsigned char mode_value = modes[active_mode].value;

        /*---------------------------------------------------------*\
        | Set color first for modes that use it                     |
        \*---------------------------------------------------------*/
        if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
        {
            unsigned char red   = RGBGetRValue(colors[0]);
            unsigned char green = RGBGetGValue(colors[0]);
            unsigned char blue  = RGBGetBValue(colors[0]);
            controller->SetColor(red, green, blue);
        }

        controller->SetMode(mode_value, brightness, speed);
    }
}
