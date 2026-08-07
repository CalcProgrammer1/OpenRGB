/*---------------------------------------------------------*\
| RGBController_SkyloongGK68HEPro.cpp                       |
|                                                           |
|   RGBController for Skyloong GK68HE Pro keyboard          |
|                                                           |
|   Titan                                       05 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_SkyloongGK68HEPro.h"

#define NA 0xFFFFFFFF

static unsigned int matrix_map[5][16] =
{
    { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, NA, NA},
    {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, NA, 28},
    {29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, NA, NA, 42},
    {43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, NA, NA, 55, 56},
    {57, 58, 59, 60, NA, NA, NA, NA, NA, NA, 61, 62, 63, 64, 65, 66}
};

static const unsigned char led_slots[GK68HE_PRO_LED_COUNT] =
{
     0,  6, 12, 18, 24, 30, 36, 42, 48, 54, 60, 66, 72, 78,
     1,  7, 13, 19, 25, 31, 37, 43, 49, 55, 61, 67, 73, 79, 85,
     2,  8, 14, 20, 26, 32, 38, 44, 50, 56, 62, 68, 80, 86,
     3, 15, 21, 27, 33, 39, 45, 51, 57, 63, 69, 75, 81, 87,
     4, 10, 16, 40, 58, 64, 70, 76, 82, 88
};

static const char* led_names[GK68HE_PRO_LED_COUNT] =
{
    KEY_EN_ESCAPE,          KEY_EN_1,           KEY_EN_2,           KEY_EN_3,           KEY_EN_4,
    KEY_EN_5,               KEY_EN_6,           KEY_EN_7,           KEY_EN_8,           KEY_EN_9,
    KEY_EN_0,               KEY_EN_MINUS,       KEY_EN_EQUALS,      KEY_EN_BACKSPACE,
    KEY_EN_TAB,             KEY_EN_Q,           KEY_EN_W,           KEY_EN_E,           KEY_EN_R,
    KEY_EN_T,               KEY_EN_Y,           KEY_EN_U,           KEY_EN_I,           KEY_EN_O,
    KEY_EN_P,               KEY_EN_LEFT_BRACKET,KEY_EN_RIGHT_BRACKET,KEY_EN_BACK_SLASH, KEY_EN_DELETE,
    KEY_EN_CAPS_LOCK,       KEY_EN_A,           KEY_EN_S,           KEY_EN_D,           KEY_EN_F,
    KEY_EN_G,               KEY_EN_H,           KEY_EN_J,           KEY_EN_K,           KEY_EN_L,
    KEY_EN_SEMICOLON,       KEY_EN_QUOTE,       KEY_EN_ANSI_ENTER,  KEY_EN_PAGE_UP,
    KEY_EN_LEFT_SHIFT,      KEY_EN_Z,           KEY_EN_X,           KEY_EN_C,           KEY_EN_V,
    KEY_EN_B,               KEY_EN_N,           KEY_EN_M,           KEY_EN_COMMA,       KEY_EN_PERIOD,
    KEY_EN_FORWARD_SLASH,   KEY_EN_RIGHT_SHIFT, KEY_EN_UP_ARROW,    KEY_EN_PAGE_DOWN,
    KEY_EN_LEFT_CONTROL,    KEY_EN_LEFT_WINDOWS,KEY_EN_LEFT_ALT,    KEY_EN_SPACE,       KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_FUNCTION,  KEY_EN_RIGHT_CONTROL,KEY_EN_LEFT_ARROW, KEY_EN_DOWN_ARROW,  KEY_EN_RIGHT_ARROW
};

/**------------------------------------------------------------------*\
    @name Skyloong GK68HE Pro
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRoyuanGK68HEProKeyboardControllers
    @comment
        Global-color hardware lighting support for Skyloong GK68HE Pro
        keyboards using USB ID 3151:5029.
\*-------------------------------------------------------------------*/

RGBController_SkyloongGK68HEPro::RGBController_SkyloongGK68HEPro(RoyuanKeyboardController* controller_ptr)
{
    controller                              = controller_ptr;

    name                                    = "Skyloong GK68HE Pro";
    vendor                                  = "Skyloong";
    type                                    = DEVICE_TYPE_KEYBOARD;
    description                             = "Skyloong GK68HE Pro lighting controller";
    serial                                  = controller->GetSerial();
    location                                = controller->GetLocation();

    mode Direct;
    Direct.name                             = "Direct";
    Direct.value                            = controller->GetDirectModeID();
    Direct.flags                            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Direct.brightness_min                   = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    Direct.brightness_max                   = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    Direct.colors_min                       = 1;
    Direct.colors_max                       = 1;
    Direct.brightness                       = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    Direct.color_mode                       = MODE_COLORS_MODE_SPECIFIC;
    Direct.colors.resize(1);
    modes.push_back(Direct);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_PATTERN_1_DEFAULT);

    mode Static;
    Static.name                             = "Static";
    Static.value                            = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_ALWAYS_ON;
    Static.flags                            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.brightness_min                   = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    Static.brightness_max                   = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    Static.colors_min                       = 1;
    Static.colors_max                       = 1;
    Static.brightness                       = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    Static.color_mode                       = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_USER_COLOR);

    mode Breathing;
    Breathing.name                          = "Breathing";
    Breathing.value                         = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_DYNAMIC_BREATHING;
    Breathing.flags                         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.speed_min                     = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    Breathing.speed_max                     = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    Breathing.brightness_min                = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    Breathing.brightness_max                = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    Breathing.colors_min                    = 1;
    Breathing.colors_max                    = 1;
    Breathing.speed                         = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    Breathing.brightness                    = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    Breathing.color_mode                    = MODE_COLORS_RANDOM;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode SpectrumCycle;
    SpectrumCycle.name                      = "Spectrum Cycle";
    SpectrumCycle.value                     = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_SPECTRUM_CYCLE;
    SpectrumCycle.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumCycle.speed_min                 = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    SpectrumCycle.speed_max                 = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    SpectrumCycle.brightness_min            = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    SpectrumCycle.brightness_max            = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    SpectrumCycle.colors_min                = 1;
    SpectrumCycle.colors_max                = 1;
    SpectrumCycle.speed                     = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    SpectrumCycle.brightness                = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    SpectrumCycle.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    SpectrumCycle.colors.resize(1);
    modes.push_back(SpectrumCycle);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_USER_COLOR);

    mode Wave;
    Wave.name                               = "Wave";
    Wave.value                              = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_WAVE;
    Wave.flags                              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    Wave.speed_min                          = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    Wave.speed_max                          = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    Wave.brightness_min                     = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    Wave.brightness_max                     = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    Wave.colors_min                         = 1;
    Wave.colors_max                         = 1;
    Wave.speed                              = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    Wave.brightness                         = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    Wave.direction                          = MODE_DIRECTION_RIGHT;
    Wave.color_mode                         = MODE_COLORS_RANDOM;
    Wave.colors.resize(1);
    modes.push_back(Wave);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode Ripple;
    Ripple.name                             = "Ripple";
    Ripple.value                            = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_RIPPLE;
    Ripple.flags                            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Ripple.speed_min                        = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    Ripple.speed_max                        = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    Ripple.brightness_min                   = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    Ripple.brightness_max                   = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    Ripple.colors_min                       = 1;
    Ripple.colors_max                       = 1;
    Ripple.speed                            = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    Ripple.brightness                       = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    Ripple.color_mode                       = MODE_COLORS_RANDOM;
    Ripple.colors.resize(1);
    modes.push_back(Ripple);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode Starlight;
    Starlight.name                          = "Starlight";
    Starlight.value                         = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_STARLIGHT;
    Starlight.flags                         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Starlight.speed_min                     = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    Starlight.speed_max                     = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    Starlight.brightness_min                = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    Starlight.brightness_max                = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    Starlight.colors_min                    = 1;
    Starlight.colors_max                    = 1;
    Starlight.speed                         = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    Starlight.brightness                    = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    Starlight.color_mode                    = MODE_COLORS_RANDOM;
    Starlight.colors.resize(1);
    modes.push_back(Starlight);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode Stream;
    Stream.name                             = "Stream";
    Stream.value                            = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_STREAM;
    Stream.flags                            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_DIRECTION_LR;
    Stream.speed_min                        = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    Stream.speed_max                        = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    Stream.brightness_min                   = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    Stream.brightness_max                   = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    Stream.colors_min                       = 1;
    Stream.colors_max                       = 1;
    Stream.speed                            = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    Stream.brightness                       = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    Stream.direction                        = MODE_DIRECTION_RIGHT;
    Stream.color_mode                       = MODE_COLORS_RANDOM;
    Stream.colors.resize(1);
    modes.push_back(Stream);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode Reactive;
    Reactive.name                           = "Reactive";
    Reactive.value                          = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_SHADOW;
    Reactive.flags                          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Reactive.speed_min                      = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    Reactive.speed_max                      = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    Reactive.brightness_min                 = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    Reactive.brightness_max                 = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    Reactive.colors_min                     = 1;
    Reactive.colors_max                     = 1;
    Reactive.speed                          = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    Reactive.brightness                     = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    Reactive.color_mode                     = MODE_COLORS_RANDOM;
    Reactive.colors.resize(1);
    modes.push_back(Reactive);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode MountainWave;
    MountainWave.name                       = "Mountain Wave";
    MountainWave.value                      = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_MOUNTAIN_WAVE;
    MountainWave.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    MountainWave.speed_min                  = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    MountainWave.speed_max                  = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    MountainWave.brightness_min             = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    MountainWave.brightness_max             = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    MountainWave.colors_min                 = 1;
    MountainWave.colors_max                 = 1;
    MountainWave.speed                      = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    MountainWave.brightness                 = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    MountainWave.color_mode                 = MODE_COLORS_RANDOM;
    MountainWave.colors.resize(1);
    modes.push_back(MountainWave);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode SineWave;
    SineWave.name                           = "Sine Wave";
    SineWave.value                          = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_SINE_WAVE;
    SineWave.flags                          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    SineWave.speed_min                      = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    SineWave.speed_max                      = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    SineWave.brightness_min                 = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    SineWave.brightness_max                 = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    SineWave.colors_min                     = 1;
    SineWave.colors_max                     = 1;
    SineWave.speed                          = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    SineWave.brightness                     = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    SineWave.color_mode                     = MODE_COLORS_RANDOM;
    SineWave.colors.resize(1);
    modes.push_back(SineWave);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode ColorSpring;
    ColorSpring.name                        = "Color Spring";
    ColorSpring.value                       = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_COLOR_SPRING;
    ColorSpring.flags                       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    ColorSpring.speed_min                   = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    ColorSpring.speed_max                   = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    ColorSpring.brightness_min              = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    ColorSpring.brightness_max              = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    ColorSpring.colors_min                  = 1;
    ColorSpring.colors_max                  = 1;
    ColorSpring.speed                       = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    ColorSpring.brightness                  = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    ColorSpring.color_mode                  = MODE_COLORS_RANDOM;
    ColorSpring.colors.resize(1);
    modes.push_back(ColorSpring);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode FlowerWave;
    FlowerWave.name                         = "Flower Wave";
    FlowerWave.value                        = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_FLOWER_WAVE;
    FlowerWave.flags                        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_DIRECTION_LR;
    FlowerWave.speed_min                    = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    FlowerWave.speed_max                    = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    FlowerWave.brightness_min               = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    FlowerWave.brightness_max               = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    FlowerWave.colors_min                   = 1;
    FlowerWave.colors_max                   = 1;
    FlowerWave.speed                        = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    FlowerWave.brightness                   = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    FlowerWave.direction                    = MODE_DIRECTION_RIGHT;
    FlowerWave.color_mode                   = MODE_COLORS_RANDOM;
    FlowerWave.colors.resize(1);
    modes.push_back(FlowerWave);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode CustomStaticProfile1;
    CustomStaticProfile1.name               = "Custom Static Profile 1";
    CustomStaticProfile1.value              = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_CUSTOM;
    CustomStaticProfile1.flags              = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    CustomStaticProfile1.brightness_min = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    CustomStaticProfile1.brightness_max = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    CustomStaticProfile1.brightness         = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    CustomStaticProfile1.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(CustomStaticProfile1);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_PATTERN_1_DEFAULT);

    mode CustomStaticProfile2;
    CustomStaticProfile2.name               = "Custom Static Profile 2";
    CustomStaticProfile2.value              = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_CUSTOM;
    CustomStaticProfile2.flags              = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    CustomStaticProfile2.brightness_min = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    CustomStaticProfile2.brightness_max = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    CustomStaticProfile2.brightness         = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    CustomStaticProfile2.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(CustomStaticProfile2);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_PATTERN_2);

    mode CustomStaticProfile3;
    CustomStaticProfile3.name               = "Custom Static Profile 3";
    CustomStaticProfile3.value              = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_CUSTOM;
    CustomStaticProfile3.flags              = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    CustomStaticProfile3.brightness_min = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    CustomStaticProfile3.brightness_max = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    CustomStaticProfile3.brightness         = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    CustomStaticProfile3.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(CustomStaticProfile3);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_PATTERN_3);

    mode CustomStaticProfile4;
    CustomStaticProfile4.name               = "Custom Static Profile 4";
    CustomStaticProfile4.value              = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_CUSTOM;
    CustomStaticProfile4.flags              = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    CustomStaticProfile4.brightness_min = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    CustomStaticProfile4.brightness_max = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    CustomStaticProfile4.brightness         = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    CustomStaticProfile4.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(CustomStaticProfile4);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_PATTERN_4);

    mode CustomStaticProfile5;
    CustomStaticProfile5.name               = "Custom Static Profile 5";
    CustomStaticProfile5.value              = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_CUSTOM;
    CustomStaticProfile5.flags              = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    CustomStaticProfile5.brightness_min = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    CustomStaticProfile5.brightness_max = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    CustomStaticProfile5.brightness         = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    CustomStaticProfile5.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(CustomStaticProfile5);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_PATTERN_5);

    mode KillTwoBirds;
    KillTwoBirds.name                       = "Kill Two Birds";
    KillTwoBirds.value                      = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_KILL_TWO_BIRDS;
    KillTwoBirds.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    KillTwoBirds.speed_min                  = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    KillTwoBirds.speed_max                  = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    KillTwoBirds.brightness_min             = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    KillTwoBirds.brightness_max             = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    KillTwoBirds.colors_min                 = 1;
    KillTwoBirds.colors_max                 = 1;
    KillTwoBirds.speed                      = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    KillTwoBirds.brightness                 = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    KillTwoBirds.color_mode                 = MODE_COLORS_RANDOM;
    KillTwoBirds.colors.resize(1);
    modes.push_back(KillTwoBirds);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode CircleWave;
    CircleWave.name                         = "Circle Wave";
    CircleWave.value                        = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_CIRCLE_WAVE;
    CircleWave.flags                        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_DIRECTION_LR;
    CircleWave.speed_min                    = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    CircleWave.speed_max                    = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    CircleWave.brightness_min               = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    CircleWave.brightness_max               = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    CircleWave.colors_min                   = 1;
    CircleWave.colors_max                   = 1;
    CircleWave.speed                        = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    CircleWave.brightness                   = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    CircleWave.direction                    = MODE_DIRECTION_RIGHT;
    CircleWave.color_mode                   = MODE_COLORS_RANDOM;
    CircleWave.colors.resize(1);
    modes.push_back(CircleWave);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode ColorfulCross;
    ColorfulCross.name                      = "Colorful Cross";
    ColorfulCross.value                     = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_COLORFUL_CROSS;
    ColorfulCross.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    ColorfulCross.speed_min                 = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    ColorfulCross.speed_max                 = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    ColorfulCross.brightness_min            = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    ColorfulCross.brightness_max            = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    ColorfulCross.colors_min                = 1;
    ColorfulCross.colors_max                = 1;
    ColorfulCross.speed                     = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    ColorfulCross.brightness                = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    ColorfulCross.color_mode                = MODE_COLORS_RANDOM;
    ColorfulCross.colors.resize(1);
    modes.push_back(ColorfulCross);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode SnowFall;
    SnowFall.name                           = "Snow Fall";
    SnowFall.value                          = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_SNOW_FALL;
    SnowFall.flags                          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    SnowFall.speed_min                      = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    SnowFall.speed_max                      = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    SnowFall.brightness_min                 = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    SnowFall.brightness_max                 = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    SnowFall.colors_min                     = 1;
    SnowFall.colors_max                     = 1;
    SnowFall.speed                          = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    SnowFall.brightness                     = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    SnowFall.color_mode                     = MODE_COLORS_RANDOM;
    SnowFall.colors.resize(1);
    modes.push_back(SnowFall);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode Meteor;
    Meteor.name                             = "Meteor";
    Meteor.value                            = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_METEOR;
    Meteor.flags                            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Meteor.speed_min                        = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    Meteor.speed_max                        = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    Meteor.brightness_min                   = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    Meteor.brightness_max                   = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    Meteor.colors_min                       = 1;
    Meteor.colors_max                       = 1;
    Meteor.speed                            = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    Meteor.brightness                       = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    Meteor.color_mode                       = MODE_COLORS_RANDOM;
    Meteor.colors.resize(1);
    modes.push_back(Meteor);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);

    mode TracelessSnowstep;
    TracelessSnowstep.name                  = "Traceless Snowstep";
    TracelessSnowstep.value                 = ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_TRACELESS_SNOWSTEP;
    TracelessSnowstep.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    TracelessSnowstep.speed_min             = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN;
    TracelessSnowstep.speed_max             = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX;
    TracelessSnowstep.brightness_min        = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN;
    TracelessSnowstep.brightness_max        = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX;
    TracelessSnowstep.colors_min            = 1;
    TracelessSnowstep.colors_max            = 1;
    TracelessSnowstep.speed                 = ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT;
    TracelessSnowstep.brightness            = ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT;
    TracelessSnowstep.color_mode            = MODE_COLORS_RANDOM;
    TracelessSnowstep.colors.resize(1);
    modes.push_back(TracelessSnowstep);
    mode_options.push_back(ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW);
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

    RGBColor seed_color = ToRGBColor(controller->GetRed(), controller->GetGreen(), controller->GetBlue());

    for(unsigned int mode_index = 0; mode_index < modes.size(); mode_index++)
    {
        if(modes[mode_index].flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
        {
            modes[mode_index].colors[0] = seed_color;
        }
    }

    SetupZones();

    for(unsigned int led_index = 0; led_index < GK68HE_PRO_LED_COUNT; led_index++)
    {
        colors[led_index] = seed_color;

        flashed_colors[(led_index * 3) + 0] = RGBGetRValue(seed_color);
        flashed_colors[(led_index * 3) + 1] = RGBGetGValue(seed_color);
        flashed_colors[(led_index * 3) + 2] = RGBGetBValue(seed_color);
    }

    layer_flashed = true;
}

RGBController_SkyloongGK68HEPro::~RGBController_SkyloongGK68HEPro()
{
    Shutdown();

    delete controller;
}

void RGBController_SkyloongGK68HEPro::SetupZones()
{
    zone keyboard_zone;
    keyboard_zone.name              = "Keyboard";
    keyboard_zone.type              = ZONE_TYPE_MATRIX;
    keyboard_zone.leds_min          = GK68HE_PRO_LED_COUNT;
    keyboard_zone.leds_max          = GK68HE_PRO_LED_COUNT;
    keyboard_zone.leds_count        = GK68HE_PRO_LED_COUNT;
    keyboard_zone.matrix_map.Set(5, 16, (unsigned int*)&matrix_map);
    zones.push_back(keyboard_zone);

    for(unsigned int led_index = 0; led_index < GK68HE_PRO_LED_COUNT; led_index++)
    {
        led keyboard_led;
        keyboard_led.name           = led_names[led_index];
        keyboard_led.value          = led_slots[led_index];
        leds.push_back(keyboard_led);
    }

    SetupColors();
}

void RGBController_SkyloongGK68HEPro::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_CUSTOM)
    {
        if(LayerColorsChanged())
        {
            SendCustomLayer();

            for(unsigned int led_index = 0; led_index < GK68HE_PRO_LED_COUNT; led_index++)
            {
                flashed_colors[(led_index * 3) + 0] = RGBGetRValue(colors[led_index]);
                flashed_colors[(led_index * 3) + 1] = RGBGetGValue(colors[led_index]);
                flashed_colors[(led_index * 3) + 2] = RGBGetBValue(colors[led_index]);
            }

            layer_flashed = true;
        }

        return;
    }

    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SetColor(red, grn, blu);
}

void RGBController_SkyloongGK68HEPro::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SkyloongGK68HEPro::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SkyloongGK68HEPro::DeviceUpdateMode()
{
    unsigned char option = mode_options[active_mode];

    if(modes[active_mode].flags & (MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD))
    {
        unsigned int direction = modes[active_mode].direction;

        if(direction > MODE_DIRECTION_DOWN)
        {
            direction = MODE_DIRECTION_LEFT;
        }

        static const unsigned char direction_nibble[4] = { 1, 0, 3, 2 };

        if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
        {
            option = ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW;
        }
        else
        {
            option = ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_USER_COLOR;
        }

        option = option | (direction_nibble[direction] << 4);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
    {
        option = ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW;
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        option = ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_USER_COLOR;
    }

    controller->SetOption(option);
    controller->SetMode(
        modes[active_mode].value,
        modes[active_mode].speed,
        modes[active_mode].brightness
    );

    RGBColor color = colors[0];

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        color = modes[active_mode].colors[0];
    }

    controller->SetColor(RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));
}

bool RGBController_SkyloongGK68HEPro::LayerColorsChanged()
{
    if(!layer_flashed)
    {
        return true;
    }

    for(unsigned int led_index = 0; led_index < GK68HE_PRO_LED_COUNT; led_index++)
    {
        if(RGBGetRValue(colors[led_index]) != flashed_colors[(led_index * 3) + 0]
        || RGBGetGValue(colors[led_index]) != flashed_colors[(led_index * 3) + 1]
        || RGBGetBValue(colors[led_index]) != flashed_colors[(led_index * 3) + 2])
        {
            return true;
        }
    }

    return false;
}

void RGBController_SkyloongGK68HEPro::SendCustomLayer()
{
    unsigned char slot_colors[ROYUAN_SKYLOONG_GK68HE_PRO_USERPIC_SLOTS * 3] = { 0x00 };

    for(unsigned int led_index = 0; led_index < GK68HE_PRO_LED_COUNT; led_index++)
    {
        unsigned int slot = led_slots[led_index];

        slot_colors[(slot * 3) + 0] = RGBGetRValue(colors[led_index]);
        slot_colors[(slot * 3) + 1] = RGBGetGValue(colors[led_index]);
        slot_colors[(slot * 3) + 2] = RGBGetBValue(colors[led_index]);
    }

    controller->SetUserPic(slot_colors, mode_options[active_mode] >> 4);
    controller->SetOption(mode_options[active_mode]);
    controller->SetMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness);
}
