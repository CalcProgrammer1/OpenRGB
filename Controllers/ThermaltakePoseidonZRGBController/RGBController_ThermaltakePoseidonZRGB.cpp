/*---------------------------------------------------------*\
| RGBController_ThermaltakePoseidonZRGB.cpp                 |
|                                                           |
|   RGBController for Thermaltake Poseidon Z RGB            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_ThermaltakePoseidonZRGB.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   0,  NA,  8,   15,  22,  29,  NA,  37,  44,  51,  58,  NA,  65,  73,  81,  88,  94, 100, 102,  NA,  NA,  NA,  NA },
      {   1,  9,   16,  23,  30,  38,  45,  52,  59,  66,  74,  NA,  82,  89, 103,  NA,   7,  21,  36,  50,  64,  80,  93 },
      {   2,  NA,  10,  17,  24,  31,  NA,  39,  46,  53,  60,  67,  75,  83,  90,  95,  14,  28,  43,  57,  72,  87,  86 },
      {   3,  NA,  11,  18,  25,  32,  NA,  40,  47,  54,  61,  68,  76,  84,  96,  NA,  NA,  NA,  NA,  35,  99,  63,  NA },
      {   4,  NA,  26,  33,  41,  48,  NA,  55,  NA,  62,  69,  77,  85,  91, 101,  NA,  NA,  27,  NA,  42,  49,  71,  98 },
      {   5,  12,  19,  NA,  NA,  NA,  NA,  34,  NA,  NA,  NA,  NA,  70,  78,  92,  97,   6,  13,  20,  56,  NA,  79,  NA } };

static const char* zone_names[] =
{
    ZONE_EN_KEYBOARD
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const unsigned int zone_sizes[] =
{
    104
};

static const char* led_names[] =
{
    KEY_EN_ESCAPE,
    KEY_EN_BACK_TICK,
    KEY_EN_TAB,
    KEY_EN_CAPS_LOCK,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_ARROW,
    KEY_EN_INSERT,
    KEY_EN_F1,
    KEY_EN_1,
    KEY_EN_Q,
    KEY_EN_A,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_DOWN_ARROW,
    KEY_EN_DELETE,
    KEY_EN_F2,
    KEY_EN_2,
    KEY_EN_W,
    KEY_EN_S,
    KEY_EN_LEFT_ALT,
    KEY_EN_RIGHT_ARROW,
    KEY_EN_HOME,
    KEY_EN_F3,
    KEY_EN_3,
    KEY_EN_E,
    KEY_EN_D,
    KEY_EN_Z,
    KEY_EN_UP_ARROW,
    KEY_EN_END,
    KEY_EN_F4,
    KEY_EN_4,
    KEY_EN_R,
    KEY_EN_F,
    KEY_EN_X,
    KEY_EN_SPACE,
    KEY_EN_NUMPAD_4,
    KEY_EN_PAGE_UP,
    KEY_EN_F5,
    KEY_EN_5,
    KEY_EN_T,
    KEY_EN_G,
    KEY_EN_C,
    KEY_EN_NUMPAD_1,
    KEY_EN_PAGE_DOWN,
    KEY_EN_F6,
    KEY_EN_6,
    KEY_EN_Y,
    KEY_EN_H,
    KEY_EN_V,
    KEY_EN_NUMPAD_2,
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_F7,
    KEY_EN_7,
    KEY_EN_U,
    KEY_EN_J,
    KEY_EN_B,
    KEY_EN_NUMPAD_0,
    KEY_EN_NUMPAD_7,
    KEY_EN_F8,
    KEY_EN_8,
    KEY_EN_I,
    KEY_EN_K,
    KEY_EN_N,
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_F9,
    KEY_EN_9,
    KEY_EN_O,
    KEY_EN_L,
    KEY_EN_M,
    KEY_EN_RIGHT_ALT,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_8,
    KEY_EN_F10,
    KEY_EN_0,
    KEY_EN_P,
    KEY_EN_SEMICOLON,
    KEY_EN_COMMA,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_NUMPAD_PERIOD,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_F11,
    KEY_EN_MINUS,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_QUOTE,
    KEY_EN_PERIOD,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_NUMPAD_9,
    KEY_EN_F12,
    KEY_EN_EQUALS,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_MENU,
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_PRINT_SCREEN,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_ANSI_ENTER,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_NUMPAD_ENTER,
    KEY_EN_NUMPAD_5,
    KEY_EN_SCROLL_LOCK,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_PAUSE_BREAK,
    KEY_EN_BACKSPACE
};

/**------------------------------------------------------------------*\
    @name Thermaltake PoseidonZ
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectPoseidonZRGBControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PoseidonZRGB::RGBController_PoseidonZRGB(PoseidonZRGBController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Thermaltake Poseidon Z RGB";
    vendor      = "Thermaltake";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Thermaltake Poseidon Z RGB Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = POSEIDONZ_MODE_STATIC;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = POSEIDONZ_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Wave;
    Wave.name       = "Wave";
    Wave.value      = POSEIDONZ_MODE_WAVE;
    Wave.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.speed_min  = POSEIDONZ_SPEED_SLOW;
    Wave.speed_max  = POSEIDONZ_SPEED_FAST;
    Wave.color_mode = MODE_COLORS_NONE;
    Wave.speed      = POSEIDONZ_SPEED_FAST;
    Wave.direction  = MODE_DIRECTION_LEFT;
    modes.push_back(Wave);

    mode Ripple;
    Ripple.name       = "Ripple";
    Ripple.value      = POSEIDONZ_MODE_RIPPLE;
    Ripple.flags      = MODE_FLAG_AUTOMATIC_SAVE;
    Ripple.color_mode = MODE_COLORS_NONE;
    modes.push_back(Ripple);

    mode Reactive;
    Reactive.name       = "Reactive";
    Reactive.value      = POSEIDONZ_MODE_REACTIVE;
    Reactive.flags      = MODE_FLAG_AUTOMATIC_SAVE;
    Reactive.color_mode = MODE_COLORS_NONE;
    modes.push_back(Reactive);

    SetupZones();
}

RGBController_PoseidonZRGB::~RGBController_PoseidonZRGB()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_PoseidonZRGB::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        zone new_zone;
        new_zone.name               = zone_names[zone_idx];
        new_zone.type               = zone_types[zone_idx];
        new_zone.leds_min           = zone_sizes[zone_idx];
        new_zone.leds_max           = zone_sizes[zone_idx];
        new_zone.leds_count         = zone_sizes[zone_idx];
        new_zone.matrix_map         = new matrix_map_type;
        new_zone.matrix_map->height = 6;
        new_zone.matrix_map->width  = 23;
        new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
        zones.push_back(new_zone);

        total_led_count += zone_sizes[zone_idx];
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_PoseidonZRGB::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PoseidonZRGB::DeviceUpdateLEDs()
{
    if(active_mode == 0)
    {
        controller->SetLEDsDirect(colors);
    }
    else
    {
        controller->SetLEDs(colors);
    }
}

void RGBController_PoseidonZRGB::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PoseidonZRGB::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PoseidonZRGB::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode].value, modes[active_mode].direction, modes[active_mode].speed);
}
