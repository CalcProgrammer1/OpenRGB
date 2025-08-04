/*---------------------------------------------------------*\
| RGBController_AsusSagarisKeyboard.cpp                     |
|                                                           |
|   RGBController for ASUS Sagaris keyboard                 |
|                                                           |
|   Mola19                                      20 Aug 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <iostream>
#include <vector>
#include "RGBController_AsusSagarisKeyboard.h"

/**------------------------------------------------------------------*\
    @name Asus Sagaris Keyboard
    @category Keyboard
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectAsusSagarisKeyboard
    @comment Missing controls for modifier keys, as they have independent lighting
\*-------------------------------------------------------------------*/

RGBController_AsusSagarisKeyboard::RGBController_AsusSagarisKeyboard(AsusSagarisKeyboardController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "ASUS";
    type                        = DEVICE_TYPE_KEYBOARD;
    description                 = "ASUS Sagaris Keyboard Device";
    version                     = controller->GetVersion();
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Off;
    Off.name                    = "Off";
    Off.value                   = SAGARIS_KEYBOARD_MODE_OFF;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = SAGARIS_KEYBOARD_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Static.brightness_min       = SAGARIS_KEYBOARD_BRIGHTNESS_MIN;
    Static.brightness_max       = SAGARIS_KEYBOARD_BRIGHTNESS_MAX;
    Static.brightness           = SAGARIS_KEYBOARD_BRIGHTNESS_DEFAULT;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Spiral;
    Spiral.name                 = "Spiral";
    Spiral.value                = SAGARIS_KEYBOARD_MODE_SPRIAL;
    Spiral.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Spiral.brightness_min       = SAGARIS_KEYBOARD_BRIGHTNESS_MIN;
    Spiral.brightness_max       = SAGARIS_KEYBOARD_BRIGHTNESS_MAX;
    Spiral.brightness           = SAGARIS_KEYBOARD_BRIGHTNESS_DEFAULT;
    Spiral.speed_min            = SAGARIS_KEYBOARD_SPEED_MIN;
    Spiral.speed_max            = SAGARIS_KEYBOARD_SPEED_MAX;
    Spiral.speed                = SAGARIS_KEYBOARD_SPEED_DEFAULT;
    Spiral.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Spiral.colors_min           = 7;
    Spiral.colors_max           = 7;
    Spiral.colors.resize(7);
    modes.push_back(Spiral);

    mode Custom;
    Custom.name                 = "Custom";
    Custom.value                = SAGARIS_KEYBOARD_MODE_CUSTOM;
    Custom.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Custom.brightness_min       = SAGARIS_KEYBOARD_BRIGHTNESS_MIN;
    Custom.brightness_max       = SAGARIS_KEYBOARD_BRIGHTNESS_MAX;
    Custom.brightness           = SAGARIS_KEYBOARD_BRIGHTNESS_DEFAULT;
    Custom.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Custom.colors_min           = 7;
    Custom.colors_max           = 7;
    Custom.colors.resize(7);
    modes.push_back(Custom);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = SAGARIS_KEYBOARD_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED;
    Breathing.speed_min         = SAGARIS_KEYBOARD_BRIGHTNESS_MIN;
    Breathing.speed_max         = SAGARIS_KEYBOARD_BRIGHTNESS_MAX;
    Breathing.speed             = SAGARIS_KEYBOARD_BRIGHTNESS_DEFAULT;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min        = 7;
    Breathing.colors_max        = 7;
    Breathing.colors.resize(7);
    modes.push_back(Breathing);

    mode Reactive;
    Reactive.name               = "Reactive";
    Reactive.value              = SAGARIS_KEYBOARD_MODE_REACTIVE;
    Reactive.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Reactive.brightness_min     = SAGARIS_KEYBOARD_BRIGHTNESS_MIN;
    Reactive.brightness_max     = SAGARIS_KEYBOARD_BRIGHTNESS_MAX;
    Reactive.brightness         = SAGARIS_KEYBOARD_BRIGHTNESS_DEFAULT;
    Reactive.speed_min          = SAGARIS_KEYBOARD_SPEED_MIN;
    Reactive.speed_max          = SAGARIS_KEYBOARD_SPEED_MAX;
    Reactive.speed              = SAGARIS_KEYBOARD_SPEED_DEFAULT;
    Reactive.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Reactive.colors_min         = 1;
    Reactive.colors_max         = 1;
    Reactive.colors.resize(1);
    modes.push_back(Reactive);

    mode Starry_Night;
    Starry_Night.name           = "Starry Night";
    Starry_Night.value          = SAGARIS_KEYBOARD_MODE_STARRY_NIGHT;
    Starry_Night.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Starry_Night.brightness_min = SAGARIS_KEYBOARD_SPEED_MIN;
    Starry_Night.brightness_max = SAGARIS_KEYBOARD_SPEED_MAX;
    Starry_Night.brightness     = SAGARIS_KEYBOARD_SPEED_DEFAULT;
    Starry_Night.speed_min      = SAGARIS_KEYBOARD_BRIGHTNESS_MIN;
    Starry_Night.speed_max      = SAGARIS_KEYBOARD_BRIGHTNESS_MAX;
    Starry_Night.speed          = SAGARIS_KEYBOARD_BRIGHTNESS_DEFAULT;
    Starry_Night.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Starry_Night.colors_min     = 7;
    Starry_Night.colors_max     = 7;
    Starry_Night.colors.resize(7);
    modes.push_back(Starry_Night);

    mode Laser;
    Laser.name                  = "Laser";
    Laser.value                 = SAGARIS_KEYBOARD_MODE_LASER;
    Laser.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Laser.brightness_min        = SAGARIS_KEYBOARD_SPEED_MIN;
    Laser.brightness_max        = SAGARIS_KEYBOARD_SPEED_MAX;
    Laser.brightness            = SAGARIS_KEYBOARD_SPEED_DEFAULT;
    Laser.speed_min             = SAGARIS_KEYBOARD_BRIGHTNESS_MIN;
    Laser.speed_max             = SAGARIS_KEYBOARD_BRIGHTNESS_MAX;
    Laser.speed                 = SAGARIS_KEYBOARD_BRIGHTNESS_DEFAULT;
    Laser.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Laser.colors_min            = 7;
    Laser.colors_max            = 7;
    Laser.colors.resize(7);
    modes.push_back(Laser);

    SetupZones();

    sagaris_mode current_mode   = controller->GetMode();

    active_mode = current_mode.mode;
    last_mode   = current_mode.mode;
    modes[active_mode].brightness = current_mode.brightness;
    modes[active_mode].speed      = current_mode.speed;

    current_colors = controller->GetColors();

    for(unsigned int i = 0; i < modes[active_mode].colors.size(); i++)
    {
        modes[active_mode].colors[i] = current_colors[i];
    }
}

RGBController_AsusSagarisKeyboard::~RGBController_AsusSagarisKeyboard()
{
    delete controller;
}

void RGBController_AsusSagarisKeyboard::SetupZones()
{
    zone keyboard;
    keyboard.name               = "Keyboard";
    keyboard.type               = ZONE_TYPE_SINGLE;
    keyboard.leds_min           = 1;
    keyboard.leds_max           = 1;
    keyboard.leds_count         = 1;
    keyboard.matrix_map         = nullptr;

    SetupColors();
}

void RGBController_AsusSagarisKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AsusSagarisKeyboard::DeviceUpdateLEDs()
{

}

void RGBController_AsusSagarisKeyboard::UpdateZoneLEDs(int /*zone*/)
{

}

void RGBController_AsusSagarisKeyboard::UpdateSingleLED(int /*led*/)
{

}

void RGBController_AsusSagarisKeyboard::DeviceUpdateMode()
{
    if(last_mode != active_mode)
    {
        last_mode = active_mode;
        for(unsigned int i = 0; i < modes[active_mode].colors.size(); i++)
        {
            modes[active_mode].colors[i] = current_colors[i];
        }
    }
    else
    {
        for(unsigned int i = 0; i < modes[active_mode].colors.size(); i++)
        {
            current_colors[i] = modes[active_mode].colors[i];
        }

        for(unsigned int i = 0; i < modes[active_mode].colors.size(); i++)
        {
            /*-----------------------------------------*\
            | This device uses 4bit colorValues (0-16)  |
            \*-----------------------------------------*/
            uint8_t red     = RGBGetRValue(modes[active_mode].colors[i]) / 16;
            uint8_t green   = RGBGetGValue(modes[active_mode].colors[i]) / 16;
            uint8_t blue    = RGBGetBValue(modes[active_mode].colors[i]) / 16;

            controller->SetColor(i, red, green, blue);
        }
    }

    /*------------------------------------------------------------------------------------------------------*\
    | This device uses a global color storage for 7 colors,                                                  |
    | each time a mode is selected, it is specified which color is chosen (some use all).                    |
    | As OpenRGB doesn't support selecting the color index, color 1 is used for single color modes.          |
    \*------------------------------------------------------------------------------------------------------*/

    uint8_t colorIndex = 0;

    uint8_t mode = modes[active_mode].value;

    if(mode == SAGARIS_KEYBOARD_MODE_STARRY_NIGHT)
    {
        colorIndex = 7;
    }

    controller->SetMode(mode, modes[active_mode].brightness, modes[active_mode].speed, colorIndex);
}
