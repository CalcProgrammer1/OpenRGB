/*---------------------------------------------------------*\
| RGBController_Sinowealth1007.cpp                          |
|                                                           |
|   RGBController for Sinowealth mice with PID 1007         |
|                                                           |
|   Moon_darker (Vaker)                         02 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_Sinowealth1007.h"

static const char *led_names[] =
{
    "Top Left",
    "Middle Left",
    "Bottom Left",
    "Bottom Middle",
    "Bottom Right",
    "Middle Right",
    "Top Right"
};

/**------------------------------------------------------------------*\
    @name Sinowealth 1007 Mouse
    @category Mouse
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectSinowealthMouse
    @comment
\*-------------------------------------------------------------------*/

RGBController_Sinowealth1007::RGBController_Sinowealth1007(SinowealthController1007* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetName();
    vendor                      = "ZET";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "ZET Fury Pro Mouse Device";
    location                    = controller->GetLocation();
    serial                      = controller->GetSerialString();

    mode Custom;
    Custom.name                 = "Custom";
    Custom.value                = ZET_FURY_PRO_MODE_CUSTOM;
    Custom.flags                = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_PER_LED_COLOR;
    Custom.color_mode           = MODE_COLORS_PER_LED;
    Custom.speed                = ZET_FURY_PRO_SPEED_DEF;
    modes.push_back(Custom);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = ZET_FURY_PRO_MODE_OFF;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    Off.speed                   = ZET_FURY_PRO_SPEED_DEF;
    Off.brightness              = ZET_FURY_PRO_BRIGHTNESS_DEF;
    modes.push_back(Off);

    mode Rainbow;
    Rainbow.name                = "Rainbow Wave";
    Rainbow.value               = ZET_FURY_PRO_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.speed_min           = ZET_FURY_PRO_SPEED_MIN;
    Rainbow.speed_max           = ZET_FURY_PRO_SPEED_MAX;
    Rainbow.speed               = ZET_FURY_PRO_SPEED_DEF;
    Rainbow.direction           = MODE_DIRECTION_RIGHT;
    modes.push_back(Rainbow);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ZET_FURY_PRO_MODE_STATIC;
    Static.flags                = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness_min       = ZET_FURY_PRO_BRIGHTNESS_MIN;
    Static.brightness_max       = ZET_FURY_PRO_BRIGHTNESS_MAX;
    Static.brightness           = ZET_FURY_PRO_BRIGHTNESS_DEF;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(Static.colors_max);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ZET_FURY_PRO_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed_min         = ZET_FURY_PRO_SPEED_MIN;
    Breathing.speed_max         = ZET_FURY_PRO_SPEED_MAX;
    Breathing.speed             = ZET_FURY_PRO_SPEED_DEF;
    Breathing.colors_min        = 7;
    Breathing.colors_max        = 7;
    Breathing.colors.resize(Breathing.colors_max);
    modes.push_back(Breathing);

    mode Pendulum;
    Pendulum.name               = "Pendulum";
    Pendulum.value              = ZET_FURY_PRO_MODE_PENDULUM;
    Pendulum.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED;
    Pendulum.color_mode         = MODE_COLORS_NONE;
    Pendulum.speed_min          = ZET_FURY_PRO_SPEED_MIN;
    Pendulum.speed_max          = ZET_FURY_PRO_SPEED_MAX;
    Pendulum.speed              = ZET_FURY_PRO_SPEED_DEF;
    modes.push_back(Pendulum);

    mode Spectrum;
    Spectrum.name               = "Spectrum Cycle";
    Spectrum.value              = ZET_FURY_PRO_MODE_SPECTRUM;
    Spectrum.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED;
    Spectrum.color_mode         = MODE_COLORS_NONE;
    Spectrum.speed_min          = ZET_FURY_PRO_SPEED_MIN;
    Spectrum.speed_max          = ZET_FURY_PRO_SPEED_MAX;
    Spectrum.speed              = ZET_FURY_PRO_SPEED_DEF;
    modes.push_back(Spectrum);

    mode TwoColors;
    TwoColors.name              = "Two Colors"; // Should this be called "Flashing Two Colors" or smth like that maybe? Rapidly changes between 2 colors
    TwoColors.value             = ZET_FURY_PRO_MODE_TWO_COLORS;
    TwoColors.flags             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    TwoColors.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    TwoColors.colors_min        = 2;
    TwoColors.colors_max        = 2;
    TwoColors.colors.resize(TwoColors.colors_max);
    modes.push_back(TwoColors);

    mode Reactive;
    Reactive.name               = "Reactive";
    Reactive.value              = ZET_FURY_PRO_MODE_REACTIVE;
    Reactive.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED;
    Reactive.color_mode         = MODE_COLORS_RANDOM;
    Reactive.speed_min          = ZET_FURY_PRO_SPEED_MIN;
    Reactive.speed_max          = ZET_FURY_PRO_SPEED_MAX;
    Reactive.speed              = ZET_FURY_PRO_SPEED_DEF;
    Reactive.colors_min         = 7;
    Reactive.colors_max         = 7;
    Reactive.colors.resize(Reactive.colors_max);
    modes.push_back(Reactive);

    mode Flicker;
    Flicker.name                = "Flicker"; // One color fluctuates around max brightness for some time, then changes to another
    Flicker.value               = ZET_FURY_PRO_MODE_FLICKER;
    Flicker.flags               = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Flicker.color_mode          = MODE_COLORS_NONE;
    Flicker.speed_min           = ZET_FURY_PRO_SPEED_MIN;
    Flicker.speed_max           = ZET_FURY_PRO_SPEED_MAX;
    Flicker.speed               = ZET_FURY_PRO_SPEED_DEF;
    Flicker.direction           = MODE_DIRECTION_RIGHT;
    modes.push_back(Flicker);

    mode Rain;
    Rain.name                   = "Rain"; // More like bad LSD trip
    Rain.value                  = ZET_FURY_PRO_MODE_RAIN;
    Rain.flags                  = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED;
    Rain.color_mode             = MODE_COLORS_NONE;
    Rain.speed_min              = ZET_FURY_PRO_SPEED_MIN;
    Rain.speed_max              = ZET_FURY_PRO_SPEED_MAX;
    Rain.speed                  = ZET_FURY_PRO_SPEED_DEF;
    modes.push_back(Rain);

    mode Snake;
    Snake.name                  = "Snake";
    Snake.value                 = ZET_FURY_PRO_MODE_SNAKE;
    Snake.flags                 = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED;
    Snake.color_mode            = MODE_COLORS_NONE;
    Snake.speed_min             = ZET_FURY_PRO_SPEED_MIN;
    Snake.speed_max             = ZET_FURY_PRO_SPEED_MAX;
    Snake.speed                 = ZET_FURY_PRO_SPEED_DEF;
    modes.push_back(Snake);

    SetupZones();
}

RGBController_Sinowealth1007::~RGBController_Sinowealth1007()
{
    delete controller;
}

void RGBController_Sinowealth1007::SetupZones()
{
    /*---------------------------------------------------------*\
    | Create a single zone                                      |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name       = "Mouse";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = controller->GetLEDCount();
    new_zone.leds_max   = controller->GetLEDCount();
    new_zone.leds_count = controller->GetLEDCount();
    new_zone.matrix_map = NULL;
    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for (unsigned int led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
    {
        led new_led;
        new_led.name                    = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_Sinowealth1007::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Sinowealth1007::DeviceUpdateLEDs()
{
    controller->SetLEDColors(colors);
}

void RGBController_Sinowealth1007::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Sinowealth1007::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Sinowealth1007::DeviceUpdateMode()
{
    unsigned char random = (modes[active_mode].flags & MODE_FLAG_HAS_RANDOM_COLOR) ? (unsigned char)ZET_FURY_PRO_SUBMODE_SET_COLOR : 0x00;
                  random = (modes[active_mode].color_mode == MODE_COLORS_RANDOM)   ? (unsigned char)ZET_FURY_PRO_SUBMODE_RANDOM    : random;

    if (modes[active_mode].value == ZET_FURY_PRO_MODE_BREATHING)
    {
        random = ZET_FURY_PRO_SUBMODE_SET_COLOR; // An unfortunate exception that has no random option but requires this
    }

    if (!(modes[active_mode].flags & MODE_FLAG_HAS_DIRECTION_LR))
    {
        modes[active_mode].direction = MODE_DIRECTION_RIGHT; // Left and right are backwards, and we don't want to always append 0x80
    }

    controller->SetMode(modes[active_mode].value,
                       (modes[active_mode].speed ? modes[active_mode].speed : modes[active_mode].brightness),
                        modes[active_mode].direction ? ZET_FURY_PRO_DIR_RIGHT : ZET_FURY_PRO_DIR_LEFT,
                        modes[active_mode].colors,
                        random,
                       (modes[active_mode].color_mode == MODE_COLORS_PER_LED));
}
