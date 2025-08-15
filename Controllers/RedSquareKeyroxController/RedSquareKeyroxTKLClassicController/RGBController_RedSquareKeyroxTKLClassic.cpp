/*---------------------------------------------------------*\
| RGBController_RedSquareKeyroxTKLClassic.cpp               |
|                                                           |
|   RGBController for Red Square Keyrox TKL Classic         |
|                                                           |
|   vlack                                       03 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_RedSquareKeyroxTKLClassic.h"

/**------------------------------------------------------------------*\
    @name Keyrox
    @category Keyboard
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRedSquareKeyroxTKLClassic
    @comment Also named Dark Project KD87a
\*-------------------------------------------------------------------*/

typedef struct
{
    std::string name;
    int value;
    int flags;
} keyrox_effect;

/*--------------------*\
|  Keyrox TKL Classic  |
\*--------------------*/
layout_values keyrox_tkl_offset_values =
{
    {
        /* ESC          F1    F2    F3    F4    F5    F6    F7    F8    F9   F10   F11   F12   PRSC  SCLK  PSBK */
             7,         13,   16,   19,   22,   28,   31,   34,   37,   40,   43,   46,   49,   52,   55,   58,
        /* BKTK    1     2     3     4     5     6     7     8     9     0     -     =   BSPC  INS   HOME  PGUP */
            83,   86,   89,   92,   95,   98,  101,  104,  107,  110,  113,  116,  119,  135,  138,  141,  144,
        /* TAB     Q     W     E     R     T     Y     U     I     O     P     [     ]     \   DEL   END   PGDN */
           159,  162,  165,  168,  171,  174,  177,  180,  183,  186,  199,  202,  205,  211,  214,  217,  220,
        /* CPLK    A     S     D     F     G     H     J     K     L     ;     "     #   ENTR                   */
           235,  241,  244,  247,  250,  263,  266,  269,  272,  275,  278,  281,  284,  287,
        /* LSFT  ISO\    Z     X     C     V     B     N     M     ,     .     /   RSFT                    ARWU */
           311,  314,  327,  330,  333,  336,  339,  342,  345,  348,  351,  354,  363,                    369,
        /* LCTL  LWIN  LALT               SPC              RALT  RFNC  RMNU  RCTL              ARWR  ARWD  ARWR */
           397,  400,  403,              415,              427,  430,  433,  436,              442,  455,  458
    },
    {
        /* Add more regional layout fixes here */
    }
};

RGBController_RedSquareKeyroxTKLClassic::RGBController_RedSquareKeyroxTKLClassic(RedSquareKeyroxTKLClassicController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "Red Square";
    type                    = DEVICE_TYPE_KEYBOARD;
    description             = "Red Square Keyrox TKL Classic Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    int BASE_EFFECT_FLAGS   = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;

    const int EFFECTS_COUNT = 14;
    keyrox_effect keyrox_effects[EFFECTS_COUNT] =
    {
        {
            "Static",
            CLASSIC_CONST_MODE_VALUE,
            MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
        },
        {
            "Direct",
            CLASSIC_CUSTOM_MODE_VALUE,
            MODE_FLAG_HAS_PER_LED_COLOR
        },
        {
            "Wave",
            CLASSIC_WAVE_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_DIRECTION_HV
        },
        {
            "Breathing",
            CLASSIC_FADE_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
        {
            "Radar",
            CLASSIC_RADAR_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR // round animation
        },
        {
            "Star (Interactive)",
            CLASSIC_STAR_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
        {
            "Line (Interactive)",
            CLASSIC_LINE_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_HV
        },
        {
            "Ripple (Interactive)",
            CLASSIC_RIPPLE_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
        {
            "Stars",
            CLASSIC_STARS_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
        {
            "Cross (Interactive)",
            CLASSIC_CROSS_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
        {
            "Horizontal bars (Interactive)",
            CLASSIC_WTF_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_DIRECTION_UD
        },
        {
            "Ripple random",
            CLASSIC_RIPPLE_RANDOM_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
        {
            "Running line",
            CLASSIC_RUNNING_LINE_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR // round direction
        },
        {
            "Fireworks (Interactive)",
            CLASSIC_FIREWORK_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
    };

    for(int i = 0; i < EFFECTS_COUNT; i++)
    {
        mode m;
        m.name  = keyrox_effects[i].name;
        m.value = keyrox_effects[i].value;
        m.flags = keyrox_effects[i].flags | MODE_FLAG_HAS_BRIGHTNESS;

        if(m.flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR && m.value != CLASSIC_CONST_MODE_VALUE)
        {
            // background and foreground
            m.color_mode = MODE_COLORS_MODE_SPECIFIC;
            m.colors_min = 2;
            m.colors_max = 2;

            m.colors.resize(2);
            m.colors.at(0) = ToRGBColor(255, 255, 255);
            m.colors.at(1) = ToRGBColor(0, 0, 0);
        }
        else if(m.flags & MODE_FLAG_HAS_PER_LED_COLOR)
        {
            m.color_mode = MODE_COLORS_PER_LED;
        }
        else
        {
            // foreground only
            m.color_mode = MODE_COLORS_MODE_SPECIFIC;
            m.colors_min = 1;
            m.colors_max = 1;

            m.colors.resize(1);
            m.colors.at(0) = ToRGBColor(255, 255, 255);
        }

        if(m.flags & MODE_FLAG_HAS_SPEED)
        {
            m.speed_min = CLASSIC_KEYROX_SPEED_MIN;
            m.speed_max = CLASSIC_KEYROX_SPEED_MAX;
            m.speed     = (CLASSIC_KEYROX_SPEED_MAX - CLASSIC_KEYROX_SPEED_MIN) / 2;
        }

        if(m.flags & MODE_FLAG_HAS_BRIGHTNESS)
        {
            m.brightness_min = CLASSIC_KEYROX_BRIGHTNESS_MIN;
            m.brightness_max = CLASSIC_KEYROX_BRIGHTNESS_MAX;
            m.brightness     = m.brightness_max;
        }

        modes.push_back(m);
    }

    SetupZones();
}

RGBController_RedSquareKeyroxTKLClassic::~RGBController_RedSquareKeyroxTKLClassic()
{
    delete controller;
}

void RGBController_RedSquareKeyroxTKLClassic::SetupZones()
{
    /*---------------------------------------------------------*\
    |  Create the keyboard zone usiung Keyboard Layout Manager  |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name               = ZONE_EN_KEYBOARD;
    new_zone.type               = ZONE_TYPE_MATRIX;

    KeyboardLayoutManager new_kb(KEYBOARD_LAYOUT_ANSI_QWERTY, KEYBOARD_SIZE_TKL, keyrox_tkl_offset_values);

    matrix_map_type * new_map   = new matrix_map_type;
    new_zone.matrix_map         = new_map;
    new_zone.matrix_map->height = KEYROX_TKL_CLASSIC_HEIGHT;
    new_zone.matrix_map->width  = KEYROX_TKL_CLASSIC_WIDTH;

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
    for(size_t led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
    {
        led new_led;

        new_led.name                = new_kb.GetKeyNameAt((unsigned int)led_idx);
        new_led.value               = new_kb.GetKeyValueAt((unsigned int)led_idx);
        leds.push_back(new_led);
    }

    zones.push_back(new_zone);

    SetupColors();
}

void RGBController_RedSquareKeyroxTKLClassic::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RedSquareKeyroxTKLClassic::DeviceUpdateLEDs()
{
    controller->SetLEDsData(colors, leds);
}

void RGBController_RedSquareKeyroxTKLClassic::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RedSquareKeyroxTKLClassic::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RedSquareKeyroxTKLClassic::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode]);
}
