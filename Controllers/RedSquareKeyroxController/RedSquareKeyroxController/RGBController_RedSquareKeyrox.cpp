/*---------------------------------------------------------*\
| RGBController_RedSquareKeyrox.cpp                         |
|                                                           |
|   RGBController for Red Square Keyrox                     |
|                                                           |
|   cafeed28                                    03 Nov 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_RedSquareKeyrox.h"

#define NA 0xFFFFFFFF

/*-----------------------------------*\
|  TODO: Other Keyrox boards support  |
|  (but I have only TKL)              |
\*-----------------------------------*/
typedef struct
{
    const unsigned int                      width;                  /* matrix width         */
    const unsigned int                      height;                 /* matrix height        */
    std::vector<std::vector<unsigned int>>  matrix_map;             /* matrix map           */
    std::vector<std::string>                led_names;              /* led names            */
    std::vector<unsigned int>               led_sequence_positions; /* position in buffers  */
} keyrox;

/*------------*\
|  Keyrox TKL  |
\*------------*/
static keyrox keyrox_tkl =
{
    18,
    6,
    {
        {   0,  NA,   1,   2,   3,   4,   5,   6,   7,   8,  NA,   9,  10,  11,  12,  13,  14,  15 },
        {  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  NA,  30,  31,  32 },
        {  33,  NA,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49 },
        {  50,  NA,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  NA,  62,  NA,  NA,  NA },
        {  63,  NA,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  NA,  74,  NA,  NA,  75,  NA },
        {  76,  77,  78,  NA,  NA,  NA,  79,  NA,  NA,  NA,  80,  81,  NA,  82,  83,  84,  85,  86 }
    },
    {
        // 0
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
        // 10
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
        // 20
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
        // 30
        KEY_EN_INSERT,
        KEY_EN_HOME,
        KEY_EN_PAGE_UP,
        KEY_EN_TAB,
        KEY_EN_Q,
        KEY_EN_W,
        KEY_EN_E,
        KEY_EN_R,
        KEY_EN_T,
        KEY_EN_Y,
        // 40
        KEY_EN_U,
        KEY_EN_I,
        KEY_EN_O,
        KEY_EN_P,
        KEY_EN_LEFT_BRACKET,
        KEY_EN_RIGHT_BRACKET,
        KEY_EN_ANSI_BACK_SLASH,
        KEY_EN_DELETE,
        KEY_EN_END,
        KEY_EN_PAGE_DOWN,
        // 50
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
        // 60
        KEY_EN_SEMICOLON,
        KEY_EN_QUOTE,
        KEY_EN_ANSI_ENTER,
        KEY_EN_LEFT_SHIFT,
        KEY_EN_Z,
        KEY_EN_X,
        KEY_EN_C,
        KEY_EN_V,
        KEY_EN_B,
        KEY_EN_N,
        // 70
        KEY_EN_M,
        KEY_EN_COMMA,
        KEY_EN_PERIOD,
        KEY_EN_FORWARD_SLASH,
        KEY_EN_RIGHT_SHIFT,
        KEY_EN_UP_ARROW,
        KEY_EN_LEFT_CONTROL,
        KEY_EN_LEFT_WINDOWS,
        KEY_EN_LEFT_ALT,
        KEY_EN_SPACE,
        // 80
        KEY_EN_RIGHT_ALT,
        KEY_EN_RIGHT_FUNCTION,
        KEY_EN_MENU,
        KEY_EN_RIGHT_CONTROL,
        KEY_EN_LEFT_ARROW,
        KEY_EN_DOWN_ARROW,
        KEY_EN_RIGHT_ARROW,
    },
    {
         0,        1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,       13,  14,  15,
        16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,       30,  31,  32,
        37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  70,       51,  52,  53,
        58,       59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,       50,
        75,       77,  78,  79,  80,  81,  82,  83,  84,  85,  86,       88,            89,
        94,  95,  96,                 98,                100, 101, 102, 103,      104, 105, 106
    }
};

typedef struct
{
    std::string name;
    int value;
    int flags;
} keyrox_effect;

RGBController_RedSquareKeyrox::RGBController_RedSquareKeyrox(RedSquareKeyroxController* controller_ptr)
{
    controller      = controller_ptr;

    name            = controller->GetNameString();
    vendor          = "Red Square";
    type            = DEVICE_TYPE_KEYBOARD;
    description     = "Red Square Keyrox Device";
    location        = controller->GetDeviceLocation();
    serial          = controller->GetSerialString();

    keyrox_effect keyrox_effects[13] =
    {
        {
            "Custom",
            CUSTOM_MODE_VALUE,
            MODE_FLAG_HAS_PER_LED_COLOR
        },
        {
            "Wave",
            WAVE_MODE_VALUE,
            MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD
        },
        {
            "Const",
            CONST_MODE_VALUE,
            MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS
        },
        {
            "Breathe",
            BREATHE_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED
        },
        {
            "Heartrate",
            HEARTRATE_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED
        },
        {
            "Point",
            POINT_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED
        },
        {
            "Winnower",
            WINNOWER_MODE_VALUE,
            MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD
        },
        {
            "Stars",
            STARS_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED
        },
        {
            "Spectrum",
            SPECTRUM_MODE_VALUE,
            MODE_FLAG_HAS_SPEED
        },
        {
            "Plumflower",
            PLUMFLOWER_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED
        },
        {
            "Shoot",
            SHOOT_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED
        },
        {
            "Ambilight Rotate",
            AMBILIGHT_ROTATE_MODE_VALUE,
            MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD
        },
        {
            "Ripple",
            RIPPLE_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED
        },
    };

    for(const keyrox_effect& effect : keyrox_effects)
    {
        mode m;
        m.name = effect.name;
        m.value = effect.value;
        m.flags = effect.flags | MODE_FLAG_HAS_BRIGHTNESS;

        if(m.flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
        {
            m.color_mode = MODE_COLORS_MODE_SPECIFIC;
            m.colors_min = 1;
            m.colors_max = 1;

            m.colors.resize(1);
            m.colors.at(0) = ToRGBColor(255, 255, 255);
        }
        else if(m.flags & MODE_FLAG_HAS_PER_LED_COLOR)
        {
            m.color_mode = MODE_COLORS_PER_LED;
        }
        else
        {
            m.color_mode = MODE_COLORS_NONE;
            m.colors_min = 0;
            m.colors_max = 0;
            m.colors.resize(0);
        }

        if(m.flags & MODE_FLAG_HAS_SPEED)
        {
            m.speed_min = KEYROX_SPEED_MIN;
            m.speed_max = KEYROX_SPEED_MAX;
            m.speed     = m.speed_max;
        }

        if(m.flags & MODE_FLAG_HAS_BRIGHTNESS)
        {
            m.brightness_min = KEYROX_BRIGHTNESS_MIN;
            /*------------------------------------------*\
            |  In Custom mode, Keyrox stores brightness  |
            |  in A of RGBA and range is 0x00-0xFF       |
            \*------------------------------------------*/
            m.brightness_max = (m.flags & MODE_FLAG_HAS_PER_LED_COLOR) ? 0xFF : KEYROX_BRIGHTNESS_MAX;
            m.brightness     = m.brightness_max;
        }

        modes.push_back(m);
    }

    SetupZones();
}

RGBController_RedSquareKeyrox::~RGBController_RedSquareKeyrox()
{
    delete controller;
}

void RGBController_RedSquareKeyrox::SetupZones()
{
    keyrox* keyboard;
    switch(controller->GetVariant())
    {
    case KEYROX_VARIANT_TKL:
        keyboard = &keyrox_tkl;
        break;
    }

    controller->SetLedSequencePositions(keyboard->led_sequence_positions);

    /*-----------------*\
    |  Create the zone  |
    \*-----------------*/
    unsigned int zone_size = 0;

    zone z;
    z.name               = ZONE_EN_KEYBOARD;
    z.type               = ZONE_TYPE_MATRIX;

    z.matrix_map         = new matrix_map_type;
    z.matrix_map->height = keyboard->height;
    z.matrix_map->width  = keyboard->width;

    z.matrix_map->map    = new unsigned int[keyboard->height * keyboard->width];

    for(unsigned int h = 0; h < keyboard->height; h++)
    {
        for(unsigned int w = 0; w < keyboard->width; w++)
        {
            unsigned int key = keyboard->matrix_map[h][w];
            z.matrix_map->map[h * keyboard->width + w] = key;

            if(key != NA)
            {
                led l;
                l.name = keyboard->led_names[key];
                leds.push_back(l);
                zone_size++;
            }
        }
    }

    z.leds_min   = zone_size;
    z.leds_max   = zone_size;
    z.leds_count = zone_size;

    zones.push_back(z);

    SetupColors();
}

void RGBController_RedSquareKeyrox::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RedSquareKeyrox::DeviceUpdateLEDs()
{
    controller->SetLEDsData(modes, active_mode, colors);
}

void RGBController_RedSquareKeyrox::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RedSquareKeyrox::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RedSquareKeyrox::DeviceUpdateMode()
{
    controller->SetMode(modes, active_mode);
    controller->SetModeData(modes, active_mode);
}
