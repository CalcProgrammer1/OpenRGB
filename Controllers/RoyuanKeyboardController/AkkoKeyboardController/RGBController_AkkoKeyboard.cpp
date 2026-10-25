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
    @name Akko 3068B Plus / PC75B Plus
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectRoyuanAkkoKeyboardControllers
    @comment
        Global-color hardware lighting support for Akko/RongYuan keyboards
        using USB ID 3151:4003 in wired mode, including the Akko PC75B
        Plus. The PC75B Plus can lock up when written to too quickly, so
        writes for this ID are rate limited.
\*-------------------------------------------------------------------*/

RGBController_AkkoKeyboard::RGBController_AkkoKeyboard(RoyuanKeyboardController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Akko 3068B Plus / PC75B Plus";
    vendor      = "Akko";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Akko 3068B Plus / PC75B Plus lighting controller";
    serial      = controller->GetSerial();
    location    = controller->GetLocation();

    mode off;
    off.name                = "Off";
    off.value               = ROYUAN_AKKO_EFFECT_OFF;
    off.flags               = MODE_FLAG_AUTOMATIC_SAVE;
    off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(off);

    mode static_mode;
    static_mode.name                = "Static";
    static_mode.value               = ROYUAN_AKKO_EFFECT_STATIC;
    static_mode.flags               = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    static_mode.color_mode          = MODE_COLORS_PER_LED;
    static_mode.brightness_min      = ROYUAN_AKKO_BRIGHTNESS_MIN;
    static_mode.brightness_max      = ROYUAN_AKKO_BRIGHTNESS_MAX;
    static_mode.brightness          = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    modes.push_back(static_mode);

    mode breathing;
    breathing.name                  = "Breathing";
    breathing.value                 = ROYUAN_AKKO_EFFECT_BREATHING;
    breathing.flags                 = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    breathing.color_mode            = MODE_COLORS_PER_LED;
    breathing.brightness_min        = ROYUAN_AKKO_BRIGHTNESS_MIN;
    breathing.brightness_max        = ROYUAN_AKKO_BRIGHTNESS_MAX;
    breathing.brightness            = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    breathing.speed_min             = ROYUAN_AKKO_SPEED_MIN;
    breathing.speed_max             = ROYUAN_AKKO_SPEED_MAX;
    breathing.speed                 = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(breathing);

    mode wave;
    wave.name                       = "Wave";
    wave.value                      = ROYUAN_AKKO_EFFECT_WAVE;
    wave.flags                      = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    wave.color_mode                 = MODE_COLORS_PER_LED;
    wave.direction                  = MODE_DIRECTION_RIGHT;
    wave.brightness_min             = ROYUAN_AKKO_BRIGHTNESS_MIN;
    wave.brightness_max             = ROYUAN_AKKO_BRIGHTNESS_MAX;
    wave.brightness                 = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    wave.speed_min                  = ROYUAN_AKKO_SPEED_MIN;
    wave.speed_max                  = ROYUAN_AKKO_SPEED_MAX;
    wave.speed                      = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(wave);

    mode ripple;
    ripple.name                     = "Ripple";
    ripple.value                    = ROYUAN_AKKO_EFFECT_RIPPLE;
    ripple.flags                    = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ripple.color_mode               = MODE_COLORS_PER_LED;
    ripple.brightness_min           = ROYUAN_AKKO_BRIGHTNESS_MIN;
    ripple.brightness_max           = ROYUAN_AKKO_BRIGHTNESS_MAX;
    ripple.brightness               = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    ripple.speed_min                = ROYUAN_AKKO_SPEED_MIN;
    ripple.speed_max                = ROYUAN_AKKO_SPEED_MAX;
    ripple.speed                    = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(ripple);

    mode reactive;
    reactive.name                   = "Reactive";
    reactive.value                  = ROYUAN_AKKO_EFFECT_REACTIVE;
    reactive.flags                  = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    reactive.color_mode             = MODE_COLORS_PER_LED;
    reactive.brightness_min         = ROYUAN_AKKO_BRIGHTNESS_MIN;
    reactive.brightness_max         = ROYUAN_AKKO_BRIGHTNESS_MAX;
    reactive.brightness             = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    reactive.speed_min              = ROYUAN_AKKO_SPEED_MIN;
    reactive.speed_max              = ROYUAN_AKKO_SPEED_MAX;
    reactive.speed                  = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(reactive);

    mode neon;
    neon.name                       = "Neon";
    neon.value                      = ROYUAN_AKKO_EFFECT_NEON;
    neon.flags                      = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    neon.color_mode                 = MODE_COLORS_NONE;
    neon.brightness_min             = ROYUAN_AKKO_BRIGHTNESS_MIN;
    neon.brightness_max             = ROYUAN_AKKO_BRIGHTNESS_MAX;
    neon.brightness                 = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    neon.speed_min                  = ROYUAN_AKKO_SPEED_MIN;
    neon.speed_max                  = ROYUAN_AKKO_SPEED_MAX;
    neon.speed                      = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(neon);

    mode raindrop;
    raindrop.name                   = "Raindrop";
    raindrop.value                  = ROYUAN_AKKO_EFFECT_RAINDROP;
    raindrop.flags                  = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    raindrop.color_mode             = MODE_COLORS_PER_LED;
    raindrop.brightness_min         = ROYUAN_AKKO_BRIGHTNESS_MIN;
    raindrop.brightness_max         = ROYUAN_AKKO_BRIGHTNESS_MAX;
    raindrop.brightness             = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    raindrop.speed_min              = ROYUAN_AKKO_SPEED_MIN;
    raindrop.speed_max              = ROYUAN_AKKO_SPEED_MAX;
    raindrop.speed                  = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(raindrop);

    mode snake;
    snake.name                      = "Snake";
    snake.value                     = ROYUAN_AKKO_EFFECT_SNAKE;
    snake.flags                     = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    snake.color_mode                = MODE_COLORS_PER_LED;
    snake.brightness_min            = ROYUAN_AKKO_BRIGHTNESS_MIN;
    snake.brightness_max            = ROYUAN_AKKO_BRIGHTNESS_MAX;
    snake.brightness                = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    snake.speed_min                 = ROYUAN_AKKO_SPEED_MIN;
    snake.speed_max                 = ROYUAN_AKKO_SPEED_MAX;
    snake.speed                     = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(snake);

    mode converge;
    converge.name                   = "Converge";
    converge.value                  = ROYUAN_AKKO_EFFECT_CONVERGE;
    converge.flags                  = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    converge.color_mode             = MODE_COLORS_PER_LED;
    converge.brightness_min         = ROYUAN_AKKO_BRIGHTNESS_MIN;
    converge.brightness_max         = ROYUAN_AKKO_BRIGHTNESS_MAX;
    converge.brightness             = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    converge.speed_min              = ROYUAN_AKKO_SPEED_MIN;
    converge.speed_max              = ROYUAN_AKKO_SPEED_MAX;
    converge.speed                  = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(converge);

    mode sine_wave;
    sine_wave.name                  = "Sine Wave";
    sine_wave.value                 = ROYUAN_AKKO_EFFECT_SINE_WAVE;
    sine_wave.flags                 = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    sine_wave.color_mode            = MODE_COLORS_PER_LED;
    sine_wave.brightness_min        = ROYUAN_AKKO_BRIGHTNESS_MIN;
    sine_wave.brightness_max        = ROYUAN_AKKO_BRIGHTNESS_MAX;
    sine_wave.brightness            = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    sine_wave.speed_min             = ROYUAN_AKKO_SPEED_MIN;
    sine_wave.speed_max             = ROYUAN_AKKO_SPEED_MAX;
    sine_wave.speed                 = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(sine_wave);

    mode kaleidoscope;
    kaleidoscope.name               = "Kaleidoscope";
    kaleidoscope.value              = ROYUAN_AKKO_EFFECT_KALEIDOSCOPE;
    kaleidoscope.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    kaleidoscope.color_mode         = MODE_COLORS_PER_LED;
    kaleidoscope.brightness_min     = ROYUAN_AKKO_BRIGHTNESS_MIN;
    kaleidoscope.brightness_max     = ROYUAN_AKKO_BRIGHTNESS_MAX;
    kaleidoscope.brightness         = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    kaleidoscope.speed_min          = ROYUAN_AKKO_SPEED_MIN;
    kaleidoscope.speed_max          = ROYUAN_AKKO_SPEED_MAX;
    kaleidoscope.speed              = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(kaleidoscope);

    mode line_wave;
    line_wave.name                  = "Line Wave";
    line_wave.value                 = ROYUAN_AKKO_EFFECT_LINE_WAVE;
    line_wave.flags                 = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    line_wave.color_mode            = MODE_COLORS_PER_LED;
    line_wave.direction             = MODE_DIRECTION_RIGHT;
    line_wave.brightness_min        = ROYUAN_AKKO_BRIGHTNESS_MIN;
    line_wave.brightness_max        = ROYUAN_AKKO_BRIGHTNESS_MAX;
    line_wave.brightness            = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    line_wave.speed_min             = ROYUAN_AKKO_SPEED_MIN;
    line_wave.speed_max             = ROYUAN_AKKO_SPEED_MAX;
    line_wave.speed                 = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(line_wave);

    mode laser;
    laser.name                      = "Laser";
    laser.value                     = ROYUAN_AKKO_EFFECT_LASER;
    laser.flags                     = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    laser.color_mode                = MODE_COLORS_PER_LED;
    laser.brightness_min            = ROYUAN_AKKO_BRIGHTNESS_MIN;
    laser.brightness_max            = ROYUAN_AKKO_BRIGHTNESS_MAX;
    laser.brightness                = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    laser.speed_min                 = ROYUAN_AKKO_SPEED_MIN;
    laser.speed_max                 = ROYUAN_AKKO_SPEED_MAX;
    laser.speed                     = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(laser);

    mode circle_wave;
    circle_wave.name                = "Circle Wave";
    circle_wave.value               = ROYUAN_AKKO_EFFECT_CIRCLE_WAVE;
    circle_wave.flags               = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    circle_wave.color_mode          = MODE_COLORS_PER_LED;
    circle_wave.brightness_min      = ROYUAN_AKKO_BRIGHTNESS_MIN;
    circle_wave.brightness_max      = ROYUAN_AKKO_BRIGHTNESS_MAX;
    circle_wave.brightness          = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    circle_wave.speed_min           = ROYUAN_AKKO_SPEED_MIN;
    circle_wave.speed_max           = ROYUAN_AKKO_SPEED_MAX;
    circle_wave.speed               = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(circle_wave);

    mode dazzle;
    dazzle.name                     = "Dazzle";
    dazzle.value                    = ROYUAN_AKKO_EFFECT_DAZZLE;
    dazzle.flags                    = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    dazzle.color_mode               = MODE_COLORS_PER_LED;
    dazzle.brightness_min           = ROYUAN_AKKO_BRIGHTNESS_MIN;
    dazzle.brightness_max           = ROYUAN_AKKO_BRIGHTNESS_MAX;
    dazzle.brightness               = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    dazzle.speed_min                = ROYUAN_AKKO_SPEED_MIN;
    dazzle.speed_max                = ROYUAN_AKKO_SPEED_MAX;
    dazzle.speed                    = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(dazzle);

    mode rain_down;
    rain_down.name                  = "Rain Down";
    rain_down.value                 = ROYUAN_AKKO_EFFECT_RAIN_DOWN;
    rain_down.flags                 = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    rain_down.color_mode            = MODE_COLORS_PER_LED;
    rain_down.brightness_min        = ROYUAN_AKKO_BRIGHTNESS_MIN;
    rain_down.brightness_max        = ROYUAN_AKKO_BRIGHTNESS_MAX;
    rain_down.brightness            = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    rain_down.speed_min             = ROYUAN_AKKO_SPEED_MIN;
    rain_down.speed_max             = ROYUAN_AKKO_SPEED_MAX;
    rain_down.speed                 = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(rain_down);

    mode meteor;
    meteor.name                     = "Meteor";
    meteor.value                    = ROYUAN_AKKO_EFFECT_METEOR;
    meteor.flags                    = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    meteor.color_mode               = MODE_COLORS_PER_LED;
    meteor.brightness_min           = ROYUAN_AKKO_BRIGHTNESS_MIN;
    meteor.brightness_max           = ROYUAN_AKKO_BRIGHTNESS_MAX;
    meteor.brightness               = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    meteor.speed_min                = ROYUAN_AKKO_SPEED_MIN;
    meteor.speed_max                = ROYUAN_AKKO_SPEED_MAX;
    meteor.speed                    = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(meteor);

    mode reactive_off;
    reactive_off.name               = "Reactive Off";
    reactive_off.value              = ROYUAN_AKKO_EFFECT_REACTIVE_OFF;
    reactive_off.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    reactive_off.color_mode         = MODE_COLORS_PER_LED;
    reactive_off.brightness_min     = ROYUAN_AKKO_BRIGHTNESS_MIN;
    reactive_off.brightness_max     = ROYUAN_AKKO_BRIGHTNESS_MAX;
    reactive_off.brightness         = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    reactive_off.speed_min          = ROYUAN_AKKO_SPEED_MIN;
    reactive_off.speed_max          = ROYUAN_AKKO_SPEED_MAX;
    reactive_off.speed              = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(reactive_off);

    mode train;
    train.name                      = "Train";
    train.value                     = ROYUAN_AKKO_EFFECT_TRAIN;
    train.flags                     = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    train.color_mode                = MODE_COLORS_PER_LED;
    train.brightness_min            = ROYUAN_AKKO_BRIGHTNESS_MIN;
    train.brightness_max            = ROYUAN_AKKO_BRIGHTNESS_MAX;
    train.brightness                = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    train.speed_min                 = ROYUAN_AKKO_SPEED_MIN;
    train.speed_max                 = ROYUAN_AKKO_SPEED_MAX;
    train.speed                     = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(train);

    mode fireworks;
    fireworks.name                  = "Fireworks";
    fireworks.value                 = ROYUAN_AKKO_EFFECT_FIREWORKS;
    fireworks.flags                 = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    fireworks.color_mode            = MODE_COLORS_PER_LED;
    fireworks.direction             = MODE_DIRECTION_RIGHT;
    fireworks.brightness_min        = ROYUAN_AKKO_BRIGHTNESS_MIN;
    fireworks.brightness_max        = ROYUAN_AKKO_BRIGHTNESS_MAX;
    fireworks.brightness            = ROYUAN_AKKO_BRIGHTNESS_DEFAULT;
    fireworks.speed_min             = ROYUAN_AKKO_SPEED_MIN;
    fireworks.speed_max             = ROYUAN_AKKO_SPEED_MAX;
    fireworks.speed                 = ROYUAN_AKKO_SPEED_DEFAULT;
    modes.push_back(fireworks);

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

    if((modes[active_mode].flags & MODE_FLAG_HAS_RANDOM_COLOR) && ((controller->GetOption() & 0x0F) == ROYUAN_AKKO_COLOR_RANDOM))
    {
        modes[active_mode].color_mode = MODE_COLORS_RANDOM;
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_DIRECTION_LR)
    {
        switch(controller->GetOption() >> 4)
        {
            case ROYUAN_AKKO_DIRECTION_LEFT:
                modes[active_mode].direction = MODE_DIRECTION_LEFT;
                break;

            case ROYUAN_AKKO_DIRECTION_DOWN:
                modes[active_mode].direction = MODE_DIRECTION_DOWN;
                break;

            case ROYUAN_AKKO_DIRECTION_UP:
                modes[active_mode].direction = MODE_DIRECTION_UP;
                break;
        }
    }

    SetupZones();
    colors[0] = ToRGBColor(controller->GetRed(), controller->GetGreen(), controller->GetBlue());
}

RGBController_AkkoKeyboard::~RGBController_AkkoKeyboard()
{
    Shutdown();

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
    keyboard_zone.matrix_map.Reset();
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
    unsigned char opt = ROYUAN_AKKO_COLOR_RANDOM;

    /*-----------------------------------------------------*\
    | Off and Neon have no colour, the vendor software      |
    | sends them as random with the colour zeroed           |
    \*-----------------------------------------------------*/
    if(!(modes[active_mode].flags & MODE_FLAG_HAS_PER_LED_COLOR))
    {
        red = 0x00;
        grn = 0x00;
        blu = 0x00;
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        opt = ROYUAN_AKKO_COLOR_SINGLE;
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_DIRECTION_LR)
    {
        switch(modes[active_mode].direction)
        {
            case MODE_DIRECTION_LEFT:
                opt |= ROYUAN_AKKO_DIRECTION_LEFT << 4;
                break;

            case MODE_DIRECTION_DOWN:
                opt |= ROYUAN_AKKO_DIRECTION_DOWN << 4;
                break;

            case MODE_DIRECTION_UP:
                opt |= ROYUAN_AKKO_DIRECTION_UP << 4;
                break;
        }
    }

    controller->SetOption(opt);
    controller->SetColor(red, grn, blu);
}

void RGBController_AkkoKeyboard::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AkkoKeyboard::DeviceUpdateSingleLED(int /*led*/)
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
