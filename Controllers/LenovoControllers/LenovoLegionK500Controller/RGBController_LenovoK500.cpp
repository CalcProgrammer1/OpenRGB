/*---------------------------------------------------------*\
| RGBController_LenovoK500.cpp                              |
|                                                           |
|   Driver for Lenovo Legion K500 keyboard                  |
|                                                           |
|   MX1D                                        10 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_LenovoK500.h"
#include "RGBControllerKeyNames.h"

/**------------------------------------------------------------------*\
    @name Lenovo Legion K500
    @category Keyboard
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectLenovoLegionK500Controllers
    @comment
\*-------------------------------------------------------------------*/

#define NA 0xFFFFFFFF

static unsigned int matrix_map[6][23] =
{
    { 0  ,   NA, 1  , 2  , 3  , 4  ,   NA, 5  , 6  , 7  , 8  ,   NA, 9  , 10 , 11 , 12 ,   NA, 13 , 14 , 15 ,   NA,   NA,   NA, },
    { 16 , 17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 28 , 29 ,   NA, 30 , 31 , 32 ,   NA, 33 , 34 , 35 , 36 , },
    { 37 ,   NA, 38 , 39 , 40 , 41 , 42 , 43 , 44 , 45 , 46 , 47 , 48 , 49 , 50 , 51 , 52 , 53 ,   NA, 54 , 55 , 56 , 57 , },
    { 58 ,   NA, 59 , 60 , 61 , 62 , 63 , 64 , 65 , 66 , 67 , 68 , 69 , 70 ,   NA,   NA,   NA,   NA,   NA, 71 , 72 , 73 ,   NA, },
    { 74 ,   NA, 75 , 76 , 77 , 78 , 79 , 80 , 81 , 82 , 83 , 84 , 85 ,   NA,   NA,   NA, 86 ,   NA,   NA, 87 , 88 , 89 , 90 , },
    { 91 , 92 , 93 ,   NA,   NA,   NA, 94 ,   NA,   NA,   NA,   NA, 95 , 96 , 97 , 98 , 99 , 100, 101,   NA, 102,   NA, 103,   NA, },
};

static const char* led_names[] =
{
    KEY_EN_ESCAPE,
    KEY_EN_F1,
    KEY_EN_F2,
    KEY_EN_F3,
    KEY_EN_F4,
    KEY_EN_F5,
    KEY_EN_F6,
    KEY_EN_F7,
    KEY_EN_F8,
    KEY_EN_F9,
    KEY_EN_F10,
    KEY_EN_F11,
    KEY_EN_F12,
    KEY_EN_PRINT_SCREEN,
    KEY_EN_SCROLL_LOCK,
    KEY_EN_PAUSE_BREAK,
    KEY_EN_BACK_TICK,
    KEY_EN_1,
    KEY_EN_2,
    KEY_EN_3,
    KEY_EN_4,
    KEY_EN_5,
    KEY_EN_6,
    KEY_EN_7,
    KEY_EN_8,
    KEY_EN_9,
    KEY_EN_0,
    KEY_EN_MINUS,
    KEY_EN_EQUALS,
    KEY_EN_BACKSPACE,
    KEY_EN_INSERT,
    KEY_EN_HOME,
    KEY_EN_PAGE_UP,
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_TAB,
    KEY_EN_Q,
    KEY_EN_W,
    KEY_EN_E,
    KEY_EN_R,
    KEY_EN_T,
    KEY_EN_Y,
    KEY_EN_U,
    KEY_EN_I,
    KEY_EN_O,
    KEY_EN_P,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_BACK_SLASH,
    KEY_EN_DELETE,
    KEY_EN_END,
    KEY_EN_PAGE_DOWN,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_CAPS_LOCK,
    KEY_EN_A,
    KEY_EN_S,
    KEY_EN_D,
    KEY_EN_F,
    KEY_EN_G,
    KEY_EN_H,
    KEY_EN_J,
    KEY_EN_K,
    KEY_EN_L,
    KEY_EN_SEMICOLON,
    KEY_EN_QUOTE,
    KEY_EN_ANSI_ENTER,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_6,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_Z,
    KEY_EN_X,
    KEY_EN_C,
    KEY_EN_V,
    KEY_EN_B,
    KEY_EN_N,
    KEY_EN_M,
    KEY_EN_COMMA,
    KEY_EN_PERIOD,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_UP_ARROW,
    KEY_EN_NUMPAD_1,
    KEY_EN_NUMPAD_2,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_ENTER,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    KEY_EN_SPACE,
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_WINDOWS,
    KEY_EN_MENU,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW,
    KEY_EN_NUMPAD_0,
    KEY_EN_NUMPAD_PERIOD,
};

RGBController_LenovoK500::RGBController_LenovoK500(LenovoK500Controller* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Lenovo Legion K500";
    vendor      = "Lenovo";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Lenovo Legion K500 Device";
    location    = controller->GetDeviceLocation();

    mode Static;
    Static.name       = "Static";
    Static.value      = K500_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.brightness_min = K500_BRIGHTNESS_MIN;
    Static.brightness_max = K500_BRIGHTNESS_MAX;
    Static.brightness = K500_BRIGHTNESS_DEFAULT;
    Static.speed_min  = K500_SPEED_MIN;
    Static.speed_max  = K500_SPEED_MAX;
    Static.speed      = K500_SPEED_DEFAULT;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Spiral;
    Spiral.name       = "Spiral";
    Spiral.value      = K500_MODE_SPIRAL;
    Spiral.flags      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Spiral.color_mode = MODE_COLORS_NONE;
    Spiral.brightness_min = K500_BRIGHTNESS_MIN;
    Spiral.brightness_max = K500_BRIGHTNESS_MAX;
    Spiral.brightness = K500_BRIGHTNESS_DEFAULT;
    Spiral.speed_min  = K500_SPEED_MIN;
    Spiral.speed_max  = K500_SPEED_MAX;
    Spiral.speed      = K500_SPEED_DEFAULT;
    modes.push_back(Spiral);

    mode Laser;
    Laser.name       = "Laser";
    Laser.value      = K500_MODE_LASER;
    Laser.flags      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Laser.color_mode = MODE_COLORS_NONE;
    Laser.brightness_min = K500_BRIGHTNESS_MIN;
    Laser.brightness_max = K500_BRIGHTNESS_MAX;
    Laser.brightness = K500_BRIGHTNESS_DEFAULT;
    Laser.speed_min  = K500_SPEED_MIN;
    Laser.speed_max  = K500_SPEED_MAX;
    Laser.speed      = K500_SPEED_DEFAULT;
    modes.push_back(Laser);

    mode Drifting;
    Drifting.name       = "Drifting";
    Drifting.value      = K500_MODE_DRIFTING;
    Drifting.flags      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Drifting.color_mode = MODE_COLORS_NONE;
    Drifting.brightness_min = K500_BRIGHTNESS_MIN;
    Drifting.brightness_max = K500_BRIGHTNESS_MAX;
    Drifting.brightness = K500_BRIGHTNESS_DEFAULT;
    Drifting.speed_min  = K500_SPEED_MIN;
    Drifting.speed_max  = K500_SPEED_MAX;
    Drifting.speed      = K500_SPEED_DEFAULT;
    modes.push_back(Drifting);

    mode FadingTrace;
    FadingTrace.name       = "Fading Trace";
    FadingTrace.value      = K500_MODE_FADING_TRACE;
    FadingTrace.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    FadingTrace.color_mode = MODE_COLORS_MODE_SPECIFIC;
    FadingTrace.colors_min = 1;
    FadingTrace.colors_max = 1;
    FadingTrace.brightness_min = K500_BRIGHTNESS_MIN;
    FadingTrace.brightness_max = K500_BRIGHTNESS_MAX;
    FadingTrace.brightness = K500_BRIGHTNESS_DEFAULT;
    FadingTrace.speed_min  = K500_SPEED_MIN;
    FadingTrace.speed_max  = K500_SPEED_MAX;
    FadingTrace.speed      = K500_SPEED_DEFAULT;
    FadingTrace.colors.resize(1);
    modes.push_back(FadingTrace);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = K500_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.brightness_min = K500_BRIGHTNESS_MIN;
    Breathing.brightness_max = K500_BRIGHTNESS_MAX;
    Breathing.brightness = K500_BRIGHTNESS_DEFAULT;
    Breathing.speed_min  = K500_SPEED_MIN;
    Breathing.speed_max  = K500_SPEED_MAX;
    Breathing.speed      = K500_SPEED_DEFAULT;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Customize;
    Customize.name          = "Custom";
    Customize.value         = K500_MODE_CUSTOMIZE;
    Customize.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Customize.color_mode    = MODE_COLORS_PER_LED;
    Customize.brightness_min = K500_BRIGHTNESS_MIN;
    Customize.brightness_max = K500_BRIGHTNESS_MAX;
    Customize.brightness = K500_BRIGHTNESS_DEFAULT;
    Customize.speed_min  = K500_SPEED_MIN;
    Customize.speed_max  = K500_SPEED_MAX;
    Customize.speed      = K500_SPEED_DEFAULT;
    Customize.colors.resize(104);
    modes.push_back(Customize);

    mode BreathingRainbow;
    BreathingRainbow.name       = "Breathing (Rainbow)";
    BreathingRainbow.value      = K500_MODE_BREATHING_RAINBOW;
    BreathingRainbow.flags      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    BreathingRainbow.color_mode = MODE_COLORS_NONE;
    BreathingRainbow.brightness_min = K500_BRIGHTNESS_MIN;
    BreathingRainbow.brightness_max = K500_BRIGHTNESS_MAX;
    BreathingRainbow.brightness = K500_BRIGHTNESS_DEFAULT;
    BreathingRainbow.speed_min  = K500_SPEED_MIN;
    BreathingRainbow.speed_max  = K500_SPEED_MAX;
    BreathingRainbow.speed      = K500_SPEED_DEFAULT;
    modes.push_back(BreathingRainbow);

    mode Starlight;
    Starlight.name       = "Starlight";
    Starlight.value      = K500_MODE_STARLIGHT;
    Starlight.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Starlight.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Starlight.colors_min = 1;
    Starlight.colors_max = 1;
    Starlight.brightness_min = K500_BRIGHTNESS_MIN;
    Starlight.brightness_max = K500_BRIGHTNESS_MAX;
    Starlight.brightness = K500_BRIGHTNESS_DEFAULT;
    Starlight.speed_min  = K500_SPEED_MIN;
    Starlight.speed_max  = K500_SPEED_MAX;
    Starlight.speed      = K500_SPEED_DEFAULT;
    Starlight.colors.resize(1);
    modes.push_back(Starlight);

    mode StarlightDual;
    StarlightDual.name       = "Starlight (Dual Colors)";
    StarlightDual.value      = K500_MODE_STARLIGHT_DUAL;
    StarlightDual.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    StarlightDual.color_mode = MODE_COLORS_MODE_SPECIFIC;
    StarlightDual.colors_min = 2;
    StarlightDual.colors_max = 2;
    StarlightDual.brightness_min = K500_BRIGHTNESS_MIN;
    StarlightDual.brightness_max = K500_BRIGHTNESS_MAX;
    StarlightDual.brightness = K500_BRIGHTNESS_DEFAULT;
    StarlightDual.speed_min  = K500_SPEED_MIN;
    StarlightDual.speed_max  = K500_SPEED_MAX;
    StarlightDual.speed      = K500_SPEED_DEFAULT;
    StarlightDual.colors.resize(2);
    modes.push_back(StarlightDual);

    mode Surfing;
    Surfing.name       = "Surfing";
    Surfing.value      = K500_MODE_SURFING;
    Surfing.flags      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Surfing.color_mode = MODE_COLORS_NONE;
    Surfing.brightness_min = K500_BRIGHTNESS_MIN;
    Surfing.brightness_max = K500_BRIGHTNESS_MAX;
    Surfing.brightness = K500_BRIGHTNESS_DEFAULT;
    Surfing.speed_min  = K500_SPEED_MIN;
    Surfing.speed_max  = K500_SPEED_MAX;
    Surfing.speed      = K500_SPEED_DEFAULT;
    modes.push_back(Surfing);

    SetupZones();
}

RGBController_LenovoK500::~RGBController_LenovoK500()
{
    Shutdown();
    delete controller;
}

void RGBController_LenovoK500::SetupZones()
{
    zone new_zone;
    new_zone.name       = "Keyboard";
    new_zone.type       = ZONE_TYPE_MATRIX;
    new_zone.leds_min   = 104;
    new_zone.leds_max   = 104;
    new_zone.leds_count = 104;
    new_zone.matrix_map.Set(6, 23, (unsigned int*)&matrix_map);
    zones.push_back(new_zone);

    for(int i = 0; i < 104; i++)
    {
        led new_led;
        new_led.name  = led_names[i];
        new_led.value = i;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_LenovoK500::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /* This device does not support resizing zones */
}

void RGBController_LenovoK500::DeviceUpdateLEDs()
{
    if (modes[active_mode].value == K500_MODE_CUSTOMIZE)
    {
        unsigned int custom_color = 0;
        for(size_t i = 0; i < colors.size() && i < 104; i++)
        {
            if(colors[i] != 0 && (last_colors.size() <= i || colors[i] != last_colors[i]))
            {
                custom_color = colors[i];
                break;
            }
        }
        if(custom_color == 0)
        {
            for(size_t i = 0; i < colors.size() && i < 104; i++)
            {
                if(colors[i] != 0)
                {
                    custom_color = colors[i];
                    break;
                }
            }
        }
        
        for(size_t i = 0; i < colors.size() && i < 104; i++)
        {
            if(colors[i] != 0)
            {
                colors[i] = custom_color;
            }
        }
        
        last_colors = colors;
        
        controller->SetMode(modes[active_mode].value, colors, modes[active_mode].brightness, modes[active_mode].speed);
    }
    else if (active_mode == 0)
    {
        std::vector<RGBColor> mode_colors = { colors[0] };
        controller->SetMode(K500_MODE_STATIC, mode_colors, modes[active_mode].brightness, modes[active_mode].speed);
    }
}

void RGBController_LenovoK500::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LenovoK500::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LenovoK500::DeviceUpdateMode()
{
    if (modes[active_mode].value == K500_MODE_CUSTOMIZE)
    {
        controller->SetMode(modes[active_mode].value, colors, modes[active_mode].brightness, modes[active_mode].speed);
    }
    else
    {
        controller->SetMode(modes[active_mode].value, modes[active_mode].colors, modes[active_mode].brightness, modes[active_mode].speed);
    }
}
