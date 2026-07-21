/*---------------------------------------------------------*\
| RGBController_AkkoKeyboard.cpp                            |
|                                                           |
|   RGBController for Akko Multi-modes keyboards            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_AkkoKeyboard.h"

/**------------------------------------------------------------------*\
    @name Akko Multi-modes Keyboard-B
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectAkkoKeyboardControllers
    @comment
        Global-color hardware lighting support for Akko/RongYuan keyboards
        using USB ID 3151:4003 in wired mode.
\*-------------------------------------------------------------------*/

RGBController_AkkoKeyboard::RGBController_AkkoKeyboard(AkkoKeyboardController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "Akko";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Akko Multi-modes keyboard lighting controller";
    serial      = controller->GetSerial();
    location    = controller->GetLocation();

    mode off;
    off.name                = "Off";
    off.value               = AKKO_EFFECT_OFF;
    off.flags               = MODE_FLAG_AUTOMATIC_SAVE;
    off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(off);

    mode static_mode;
    static_mode.name                = "Static";
    static_mode.value               = AKKO_EFFECT_STATIC;
    static_mode.flags               = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    static_mode.color_mode          = MODE_COLORS_PER_LED;
    static_mode.brightness_min      = AKKO_BRIGHTNESS_MIN;
    static_mode.brightness_max      = AKKO_BRIGHTNESS_MAX;
    static_mode.brightness          = AKKO_BRIGHTNESS_DEFAULT;
    modes.push_back(static_mode);

    mode breathing;
    breathing.name                  = "Breathing";
    breathing.value                 = AKKO_EFFECT_BREATHING;
    breathing.flags                 = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    breathing.color_mode            = MODE_COLORS_PER_LED;
    breathing.brightness_min        = AKKO_BRIGHTNESS_MIN;
    breathing.brightness_max        = AKKO_BRIGHTNESS_MAX;
    breathing.brightness            = AKKO_BRIGHTNESS_DEFAULT;
    breathing.speed_min             = AKKO_SPEED_MIN;
    breathing.speed_max             = AKKO_SPEED_MAX;
    breathing.speed                 = AKKO_SPEED_DEFAULT;
    modes.push_back(breathing);

    mode wave;
    wave.name                       = "Wave";
    wave.value                      = AKKO_EFFECT_WAVE;
    wave.flags                      = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    wave.color_mode                 = MODE_COLORS_PER_LED;
    wave.brightness_min             = AKKO_BRIGHTNESS_MIN;
    wave.brightness_max             = AKKO_BRIGHTNESS_MAX;
    wave.brightness                 = AKKO_BRIGHTNESS_DEFAULT;
    wave.speed_min                  = AKKO_SPEED_MIN;
    wave.speed_max                  = AKKO_SPEED_MAX;
    wave.speed                      = AKKO_SPEED_DEFAULT;
    modes.push_back(wave);

    mode ripple;
    ripple.name                     = "Ripple";
    ripple.value                    = AKKO_EFFECT_RIPPLE;
    ripple.flags                    = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    ripple.color_mode               = MODE_COLORS_PER_LED;
    ripple.brightness_min           = AKKO_BRIGHTNESS_MIN;
    ripple.brightness_max           = AKKO_BRIGHTNESS_MAX;
    ripple.brightness               = AKKO_BRIGHTNESS_DEFAULT;
    ripple.speed_min                = AKKO_SPEED_MIN;
    ripple.speed_max                = AKKO_SPEED_MAX;
    ripple.speed                    = AKKO_SPEED_DEFAULT;
    modes.push_back(ripple);

    mode reactive;
    reactive.name                   = "Reactive";
    reactive.value                  = AKKO_EFFECT_REACTIVE;
    reactive.flags                  = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    reactive.color_mode             = MODE_COLORS_PER_LED;
    reactive.brightness_min         = AKKO_BRIGHTNESS_MIN;
    reactive.brightness_max         = AKKO_BRIGHTNESS_MAX;
    reactive.brightness             = AKKO_BRIGHTNESS_DEFAULT;
    reactive.speed_min              = AKKO_SPEED_MIN;
    reactive.speed_max              = AKKO_SPEED_MAX;
    reactive.speed                  = AKKO_SPEED_DEFAULT;
    modes.push_back(reactive);

    for(unsigned int mode_index = 0; mode_index < modes.size(); mode_index++)
    {
        if(modes[mode_index].value == controller->GetMode())
        {
            active_mode = mode_index;
            break;
        }
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        modes[active_mode].brightness = controller->GetBrightness();
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
    {
        modes[active_mode].speed = controller->GetSpeed();
    }

    SetupZones();
    colors[0] = ToRGBColor(controller->GetRed(), controller->GetGreen(), controller->GetBlue());
}

RGBController_AkkoKeyboard::~RGBController_AkkoKeyboard()
{
    delete controller;
}

void RGBController_AkkoKeyboard::SetupZones()
{
    zone keyboard_zone;
    keyboard_zone.name          = "Keyboard";
    keyboard_zone.type          = ZONE_TYPE_SINGLE;
    keyboard_zone.leds_min      = 1;
    keyboard_zone.leds_max      = 1;
    keyboard_zone.leds_count    = 1;
    keyboard_zone.matrix_map    = NULL;
    zones.push_back(keyboard_zone);

    led keyboard_led;
    keyboard_led.name           = "Keyboard LEDs";
    keyboard_led.value          = 0x00;
    leds.push_back(keyboard_led);

    SetupColors();
}

void RGBController_AkkoKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
}

void RGBController_AkkoKeyboard::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SetColor(red, grn, blu);
}

void RGBController_AkkoKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AkkoKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AkkoKeyboard::DeviceUpdateMode()
{
    controller->SetMode(
        modes[active_mode].value,
        modes[active_mode].speed,
        modes[active_mode].brightness
    );

    DeviceUpdateLEDs();
}
