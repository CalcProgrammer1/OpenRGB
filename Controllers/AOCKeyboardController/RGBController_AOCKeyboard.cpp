/*---------------------------------------------------------*\
| RGBController_AOCKeyboard.cpp                             |
|                                                           |
|   RGBController for AOC keyboard                          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                10 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AOCKeyboard.h"
#include "KeyboardLayoutManager.h"

/**--------------------------------------------------------------------*\
    @name AOC Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAOCKeyboardControllers
    @comment
\*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*\
|  AOC Keyboard KLM Layout                                              |
\*---------------------------------------------------------------------*/
layout_values aoc_keyboard_offset_values =
{
    {
        /* ESC          F1    F2    F3    F4    F5    F6    F7    F8    F9   F10   F11   F12   PRSC  SCLK  PSBK                         */
            90,         92,   77,   63,   79,   94,   81,   96,   82,   83,   98,   40,   55,   85,  100,  104,
        /* BKTK    1     2     3     4     5     6     7     8     9     0     -     =   BSPC  INS   HOME  PGUP  NLCK   NP/   NP*  NP-  */
            75,   76,   91,   62,   48,   64,   50,   65,   66,   67,   97,   68,   84,   70,   59,   74,   89,    58,   73,   88,  103,
        /* TAB     Q     W     E     R     T     Y     U     I     O     P     [     ]     \   DEL   END   PGDN   NP7   NP8   NP9   NP+ */
            60,   61,   47,   78,   33,   49,   35,   80,   51,   52,   53,   69,   99,   25,   44,   29,   14,    43,   28,   13,  102,
        /* CPLK    A     S     D     F     G     H     J     K     L     ;     "     #   ENTR                     NP4   NP5   NP6       */
            45,   46,   32,   93,   18,   34,   20,   95,   36,   37,   38,   54,    0,   10,                      57,   72,   87,
        /* LSFT    \     Z     X     C     V     B     N     M     ,     .     /   RSFT             ARWU          NP1   NP2   NP3  NPEN */
            30,    0,   31,   17,    2,    3,   19,    5,    6,   21,   22,   23,   39,               11,          42,   27,   12,  101,
        /* LCTL  LWIN  LALT               SPC              RALT  RFNC  RMNU  RCTL              ARWL ARWD   ARWR   NP0         NP.       */
            15,    0,    1,                4,                7,    8,   24,    9,               26,   41,   56,    71,         86
    },
    {
        /* Add more regional layout fixes here */
    }
};


RGBController_AOCKeyboard::RGBController_AOCKeyboard(AOCKeyboardController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "AOC";
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = "AOC Keyboard Device";
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    mode Static;
    Static.name                     = "Static";
    Static.value                    = AOC_KEYBOARD_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness_min           = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Static.brightness_max           = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Static.brightness               = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode SpectrumCycle;
    SpectrumCycle.name              = "Spectrum Cycle";
    SpectrumCycle.value             = AOC_KEYBOARD_MODE_STATIC;
    SpectrumCycle.flags             = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    SpectrumCycle.color_mode        = MODE_COLORS_RANDOM;
    SpectrumCycle.brightness_min    = AOC_KEYBOARD_BRIGHTNESS_OFF;
    SpectrumCycle.brightness_max    = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    SpectrumCycle.brightness        = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    SpectrumCycle.speed_min         = AOC_KEYBOARD_SPEED_SLOW;
    SpectrumCycle.speed_max         = AOC_KEYBOARD_SPEED_FAST;
    SpectrumCycle.speed             = AOC_KEYBOARD_SPEED_MEDIUM;
    modes.push_back(SpectrumCycle);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = AOC_KEYBOARD_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.brightness_min        = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Breathing.brightness_max        = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Breathing.brightness            = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Breathing.speed_min             = AOC_KEYBOARD_SPEED_SLOW;
    Breathing.speed_max             = AOC_KEYBOARD_SPEED_FAST;
    Breathing.speed                 = AOC_KEYBOARD_SPEED_MEDIUM;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode React;
    React.name                      = "React";
    React.value                     = AOC_KEYBOARD_MODE_REACT;
    React.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    React.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    React.brightness_min            = AOC_KEYBOARD_BRIGHTNESS_OFF;
    React.brightness_max            = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    React.brightness                = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    React.speed_min                 = AOC_KEYBOARD_SPEED_SLOW;
    React.speed_max                 = AOC_KEYBOARD_SPEED_FAST;
    React.speed                     = AOC_KEYBOARD_SPEED_MEDIUM;
    React.colors_min                = 1;
    React.colors_max                = 1;
    React.colors.resize(1);
    modes.push_back(React);

    mode Ripple;
    Ripple.name                     = "Ripple";
    Ripple.value                    = AOC_KEYBOARD_MODE_RIPPLE;
    Ripple.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Ripple.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Ripple.brightness_min           = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Ripple.brightness_max           = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Ripple.brightness               = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Ripple.speed_min                = AOC_KEYBOARD_SPEED_SLOW;
    Ripple.speed_max                = AOC_KEYBOARD_SPEED_FAST;
    Ripple.speed                    = AOC_KEYBOARD_SPEED_MEDIUM;
    Ripple.colors_min               = 1;
    Ripple.colors_max               = 1;
    Ripple.colors.resize(1);
    modes.push_back(Ripple);

    mode Radar;
    Radar.name                      = "Radar";
    Radar.value                     = AOC_KEYBOARD_MODE_RADAR;
    Radar.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Radar.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Radar.brightness_min            = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Radar.brightness_max            = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Radar.brightness                = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Radar.speed_min                 = AOC_KEYBOARD_SPEED_SLOW;
    Radar.speed_max                 = AOC_KEYBOARD_SPEED_FAST;
    Radar.speed                     = AOC_KEYBOARD_SPEED_MEDIUM;
    Radar.colors_min                = 1;
    Radar.colors_max                = 1;
    Radar.colors.resize(1);
    modes.push_back(Radar);

    mode Fireworks;
    Fireworks.name                  = "Fireworks";
    Fireworks.value                 = AOC_KEYBOARD_MODE_FIREWORKS;
    Fireworks.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Fireworks.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Fireworks.brightness_min        = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Fireworks.brightness_max        = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Fireworks.brightness            = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Fireworks.speed_min             = AOC_KEYBOARD_SPEED_SLOW;
    Fireworks.speed_max             = AOC_KEYBOARD_SPEED_FAST;
    Fireworks.speed                 = AOC_KEYBOARD_SPEED_MEDIUM;
    Fireworks.colors_min            = 1;
    Fireworks.colors_max            = 1;
    Fireworks.colors.resize(1);
    modes.push_back(Fireworks);

    mode Blink;
    Blink.name                      = "Flashing";
    Blink.value                     = AOC_KEYBOARD_MODE_BLINK;
    Blink.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Blink.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Blink.brightness_min            = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Blink.brightness_max            = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Blink.brightness                = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Blink.speed_min                 = AOC_KEYBOARD_SPEED_SLOW;
    Blink.speed_max                 = AOC_KEYBOARD_SPEED_FAST;
    Blink.speed                     = AOC_KEYBOARD_SPEED_MEDIUM;
    Blink.colors_min                = 1;
    Blink.colors_max                = 1;
    Blink.colors.resize(1);
    modes.push_back(Blink);

    mode Wave;
    Wave.name                       = "Wave";
    Wave.value                      = AOC_KEYBOARD_MODE_WAVE;
    Wave.flags                      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.color_mode                 = MODE_COLORS_MODE_SPECIFIC;
    Wave.brightness_min             = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Wave.brightness_max             = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Wave.brightness                 = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Wave.speed_min                  = AOC_KEYBOARD_SPEED_SLOW;
    Wave.speed_max                  = AOC_KEYBOARD_SPEED_FAST;
    Wave.speed                      = AOC_KEYBOARD_SPEED_MEDIUM;
    Wave.colors_min                 = 1;
    Wave.colors_max                 = 1;
    Wave.colors.resize(1);
    modes.push_back(Wave);

    mode RainbowWave;
    RainbowWave.name                = "Rainbow Wave";
    RainbowWave.value               = AOC_KEYBOARD_MODE_WAVE;
    RainbowWave.flags               = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowWave.color_mode          = MODE_COLORS_RANDOM;
    RainbowWave.brightness_min      = AOC_KEYBOARD_BRIGHTNESS_OFF;
    RainbowWave.brightness_max      = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    RainbowWave.brightness          = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    RainbowWave.speed_min           = AOC_KEYBOARD_SPEED_SLOW;
    RainbowWave.speed_max           = AOC_KEYBOARD_SPEED_FAST;
    RainbowWave.speed               = AOC_KEYBOARD_SPEED_MEDIUM;
    modes.push_back(RainbowWave);

    mode ConcentricCircles;
    ConcentricCircles.name          = "Concentric Circles";
    ConcentricCircles.value         = AOC_KEYBOARD_MODE_CONCENTRIC_CIRCLES;
    ConcentricCircles.flags         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    ConcentricCircles.color_mode    = MODE_COLORS_MODE_SPECIFIC;
    ConcentricCircles.brightness_min= AOC_KEYBOARD_BRIGHTNESS_OFF;
    ConcentricCircles.brightness_max= AOC_KEYBOARD_BRIGHTNESS_HIGH;
    ConcentricCircles.brightness    = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    ConcentricCircles.speed_min     = AOC_KEYBOARD_SPEED_SLOW;
    ConcentricCircles.speed_max     = AOC_KEYBOARD_SPEED_FAST;
    ConcentricCircles.speed         = AOC_KEYBOARD_SPEED_MEDIUM;
    ConcentricCircles.colors_min    = 1;
    ConcentricCircles.colors_max    = 1;
    ConcentricCircles.colors.resize(1);
    modes.push_back(ConcentricCircles);

    mode WWave;
    WWave.name                      = "W Wave";
    WWave.value                     = AOC_KEYBOARD_MODE_W_WAVE;
    WWave.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    WWave.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    WWave.brightness_min            = AOC_KEYBOARD_BRIGHTNESS_OFF;
    WWave.brightness_max            = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    WWave.brightness                = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    WWave.speed_min                 = AOC_KEYBOARD_SPEED_SLOW;
    WWave.speed_max                 = AOC_KEYBOARD_SPEED_FAST;
    WWave.speed                     = AOC_KEYBOARD_SPEED_MEDIUM;
    WWave.colors_min                = 1;
    WWave.colors_max                = 1;
    WWave.colors.resize(1);
    modes.push_back(WWave);

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = AOC_KEYBOARD_MODE_CUSTOM;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    Direct.brightness_min           = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Direct.brightness_max           = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Direct.brightness               = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    modes.push_back(Direct);

    SetupZones();
};

RGBController_AOCKeyboard::~RGBController_AOCKeyboard()
{
    delete controller;
}

void RGBController_AOCKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Create the keyboard zone usiung Keyboard Layout Manager   |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name               = ZONE_EN_KEYBOARD;
    new_zone.type               = ZONE_TYPE_MATRIX;

    KeyboardLayoutManager new_kb(KEYBOARD_LAYOUT_ANSI_QWERTY, KEYBOARD_SIZE_FULL, aoc_keyboard_offset_values);

    matrix_map_type * new_map   = new matrix_map_type;
    new_zone.matrix_map         = new_map;
    new_zone.matrix_map->height = new_kb.GetRowCount();
    new_zone.matrix_map->width  = new_kb.GetColumnCount();

    new_zone.matrix_map->map    = new unsigned int[new_map->height * new_map->width];
    new_zone.leds_count         = new_kb.GetKeyCount();
    new_zone.leds_min           = new_zone.leds_count;
    new_zone.leds_max           = new_zone.leds_count;

    /*---------------------------------------------------------*\
    | Matrix map still uses declared zone rows and columns      |
    |   as the packet structure depends on the matrix map       |
    \*---------------------------------------------------------*/
    new_kb.GetKeyMap(new_map->map, KEYBOARD_MAP_FILL_TYPE_COUNT, new_map->height, new_map->width);

    /*---------------------------------------------------------*\
    | Create LEDs for the Matrix zone                           |
    |   Place keys in the layout to populate the matrix         |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
    {
        led new_led;

        new_led.name                = new_kb.GetKeyNameAt(led_idx);
        new_led.value               = new_kb.GetKeyValueAt(led_idx);
        leds.push_back(new_led);
    }

    zones.push_back(new_zone);

    SetupColors();
}

void RGBController_AOCKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AOCKeyboard::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == AOC_KEYBOARD_MODE_CUSTOM)
    {
        RGBColor color_buf[120];

        for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
        {
            color_buf[leds[led_idx].value] = colors[led_idx];
        }

        controller->SetCustom(&color_buf[0]);
    }
    else
    {
        DeviceUpdateMode();
    }
}

void RGBController_AOCKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCKeyboard::DeviceUpdateMode()
{
    unsigned char   aoc_direction   = AOC_KEYBOARD_DIRECTION_CLOCKWISE;
    unsigned char   aoc_random      = AOC_KEYBOARD_SINGLE_COLOR;
    RGBColor*       aoc_colors      = &colors[0];

    if(modes[active_mode].direction == MODE_DIRECTION_LEFT)
    {
        aoc_direction = AOC_KEYBOARD_DIRECTION_COUNTERCLOCKWISE;
    }

    if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
    {
        aoc_random = AOC_KEYBOARD_RANDOM;
    }

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        aoc_colors = &modes[active_mode].colors[0];
    }

    controller->SetLightingConfig(modes[active_mode].value,
                                  aoc_random,
                                  modes[active_mode].brightness,
                                  modes[active_mode].speed,
                                  aoc_direction,
                                  aoc_colors);
}
