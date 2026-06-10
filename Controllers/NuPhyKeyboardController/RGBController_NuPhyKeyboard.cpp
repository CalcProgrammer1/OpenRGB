/*---------------------------------------------------------*\
| RGBController_NuPhyKeyboard.cpp                           |
|                                                           |
|   RGBController for NuPhy QMK/VIA RGB Matrix keyboards    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "hsv.h"
#include "RGBController_NuPhyKeyboard.h"

typedef struct
{
    unsigned char value;
    const char*   name;
    bool          has_color;
    bool          has_speed;
} nuphy_mode;

static const nuphy_mode nuphy_modes[] =
{
    { 0,  "Off",                       false, false },
    { 1,  "Static",                    true,  false },
    { 2,  "Alphas Mods",               true,  true  },
    { 3,  "Gradient Up Down",          true,  true  },
    { 4,  "Gradient Left Right",       true,  true  },
    { 5,  "Breathing",                 true,  true  },
    { 6,  "Band Saturation",           true,  true  },
    { 7,  "Band Brightness",           true,  true  },
    { 8,  "Band Pinwheel Saturation",  true,  true  },
    { 9,  "Band Pinwheel Brightness",  true,  true  },
    { 10, "Band Spiral Saturation",    true,  true  },
    { 11, "Band Spiral Brightness",    true,  true  },
    { 12, "Cycle All",                 false, true  },
    { 13, "Cycle Left Right",          false, true  },
    { 14, "Cycle Up Down",             false, true  },
    { 15, "Rainbow Chevron",           false, true  },
    { 16, "Cycle Out In",              false, true  },
    { 17, "Cycle Out In Dual",         false, true  },
    { 18, "Cycle Pinwheel",            false, true  },
    { 19, "Cycle Spiral",              false, true  },
    { 20, "Dual Beacon",               false, true  },
    { 21, "Rainbow Beacon",            false, true  },
    { 22, "Rainbow Pinwheels",         false, true  },
    { 23, "Raindrops",                 false, true  },
    { 24, "Jellybean Raindrops",       false, true  },
    { 25, "Hue Breathing",             false, true  },
    { 26, "Hue Pendulum",              false, true  },
    { 27, "Hue Wave",                  false, true  },
    { 28, "Typing Heatmap",            false, true  },
    { 29, "Digital Rain",              false, true  },
    { 30, "Solid Reactive Simple",     true,  true  },
    { 31, "Solid Reactive",            true,  true  },
    { 32, "Solid Reactive Wide",       true,  true  },
    { 33, "Solid Reactive Multiwide",  true,  true  },
    { 34, "Solid Reactive Cross",      true,  true  },
    { 35, "Solid Reactive Multicross", true,  true  },
    { 36, "Solid Reactive Nexus",      true,  true  },
    { 37, "Solid Reactive Multinexus", true,  true  },
    { 38, "Splash",                    false, true  },
    { 39, "Multisplash",               false, true  },
    { 40, "Solid Splash",              true,  true  },
    { 41, "Solid Multisplash",         true,  true  },
};

/**------------------------------------------------------------------*\
    @name NuPhy Air V2
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectNuPhyKeyboardControllers
    @comment Supports the Air60 V2, Air75 V2, and Air96 V2 using their
        stock QMK/VIA firmware. VIA does not expose direct per-key color
        control, so OpenRGB controls whole-keyboard effects and colors.
\*-------------------------------------------------------------------*/

RGBController_NuPhyKeyboard::RGBController_NuPhyKeyboard(NuPhyKeyboardController* controller_ptr)
{
    controller  = controller_ptr;
    name        = controller->GetName();
    description = "NuPhy QMK/VIA RGB Matrix Keyboard";
    vendor      = "NuPhy";
    location    = controller->GetLocation();
    serial      = controller->GetSerial();
    version     = controller->GetVersion();
    type        = DEVICE_TYPE_KEYBOARD;

    unsigned char brightness = controller->GetBrightness();
    unsigned char speed      = controller->GetSpeed();
    unsigned char hue        = 0;
    unsigned char saturation = 0;
    controller->GetColor(&hue, &saturation);

    /*-----------------------------------------------------*\
    | QMK speed zero advances animations so slowly that     |
    | effects such as Breathing appear stopped.             |
    \*-----------------------------------------------------*/
    if(speed == 0)
    {
        speed = 128;
    }

    for(const nuphy_mode& effect : nuphy_modes)
    {
        mode new_mode;
        new_mode.name           = effect.name;
        new_mode.value          = effect.value;
        new_mode.flags          = MODE_FLAG_HAS_BRIGHTNESS;
        new_mode.brightness_min = 0;
        new_mode.brightness_max = 255;
        new_mode.brightness     = brightness;

        if(effect.has_speed)
        {
            new_mode.flags    |= MODE_FLAG_HAS_SPEED;
            new_mode.speed_min = 0;
            new_mode.speed_max = 255;
            new_mode.speed     = speed;
        }

        if(effect.has_color)
        {
            hsv_t hsv_color;
            hsv_color.hue        = (unsigned int)((float)hue * (360.0f / 256.0f));
            hsv_color.saturation = saturation;
            hsv_color.value      = 255;

            new_mode.flags      |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
            new_mode.color_mode  = MODE_COLORS_MODE_SPECIFIC;
            new_mode.colors_min  = 1;
            new_mode.colors_max  = 1;
            new_mode.colors.push_back(hsv2rgb(&hsv_color));
        }

        modes.push_back(new_mode);
    }

    active_mode = controller->GetEffect();
    if(active_mode >= (int)modes.size())
    {
        active_mode = 0;
    }

    SetupZones();
}

RGBController_NuPhyKeyboard::~RGBController_NuPhyKeyboard()
{
    delete controller;
}

void RGBController_NuPhyKeyboard::SetupZones()
{
    zone keyboard;
    keyboard.name       = "Keyboard";
    keyboard.type       = ZONE_TYPE_SINGLE;
    keyboard.leds_min   = 1;
    keyboard.leds_max   = 1;
    keyboard.leds_count = 1;
    zones.push_back(keyboard);

    led keyboard_led;
    keyboard_led.name = "Keyboard";
    leds.push_back(keyboard_led);
    SetupColors();
}

void RGBController_NuPhyKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
}

void RGBController_NuPhyKeyboard::DeviceUpdateLEDs()
{
    if(!colors.empty())
    {
        hsv_t hsv_color;
        rgb2hsv(colors[0], &hsv_color);
        controller->SetColor((unsigned char)((float)hsv_color.hue * (256.0f / 360.0f)), hsv_color.saturation);
    }
}

void RGBController_NuPhyKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_NuPhyKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_NuPhyKeyboard::DeviceUpdateMode()
{
    controller->SetEffect((unsigned char)modes[active_mode].value);
    controller->SetBrightness((unsigned char)modes[active_mode].brightness);
    controller->SetSpeed((unsigned char)modes[active_mode].speed);

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        hsv_t hsv_color;
        rgb2hsv(modes[active_mode].colors[0], &hsv_color);
        controller->SetColor((unsigned char)((float)hsv_color.hue * (256.0f / 360.0f)), hsv_color.saturation);
    }
}

void RGBController_NuPhyKeyboard::DeviceSaveMode()
{
    controller->Save();
}
