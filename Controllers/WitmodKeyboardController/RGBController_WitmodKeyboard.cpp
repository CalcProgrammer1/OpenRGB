/*---------------------------------------------------------*\
| RGBController_WitmodKeyboard.cpp                           |
|                                                           |
|   RGBController for Witmod keyboards                      |
|                                                           |
|   vlack                                       03 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_WitmodKeyboard.h"

/**------------------------------------------------------------------*\
    @name Witmod Keyboard
    @category Keyboard
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectWitmodKeyboard
    @comment Witmod platform.  The GK8110 (87 key) and GK8120 (104 key)
    boards are resold under several brands, including Dark Project KD87A
    and KD104A and the Red Square Keyrox TKL Classic.  The sizes share
    the same USB IDs and are told apart by the model reported in the
    device info string.
\*-------------------------------------------------------------------*/

typedef struct
{
    std::string name;
    int value;
    int flags;
} witmod_effect;

/*--------------------*\
|  GK8110 (87 keys)    |
\*--------------------*/
layout_values gk8110_offset_values =
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

/*--------------------*\
|  GK8120 (104 keys)   |
\*--------------------*/
layout_values gk8120_offset_values =
{
    {
        /* ESC          F1    F2    F3    F4    F5    F6    F7    F8    F9   F10   F11   F12   PRSC  SCLK  PSBK */
             7,         13,   16,   19,   22,   28,   31,   34,   37,   40,   43,   46,   49,   52,   55,   58,
        /* BKTK    1     2     3     4     5     6     7     8     9     0     -     =   BSPC  INS   HOME  PGUP  NLCK  NUM/  NUM*  NUM- */
            83,   86,   89,   92,   95,   98,  101,  104,  107,  110,  113,  116,  119,  135,  138,  141,  144,  147,  150,  153,  156,
        /* TAB     Q     W     E     R     T     Y     U     I     O     P     [     ]     \   DEL   END   PGDN  NUM7  NUM8  NUM9  NUM+ */
           159,  162,  165,  168,  171,  174,  177,  180,  183,  186,  199,  202,  205,  211,  214,  217,  220,  223,  226,  229,  232,
        /* CPLK    A     S     D     F     G     H     J     K     L     ;     "     #   ENTR                    NUM4  NUM5  NUM6 */
           235,  241,  244,  247,  250,  263,  266,  269,  272,  275,  278,  281,  284,  287,                    299,  302,  305,
        /* LSFT  ISO\    Z     X     C     V     B     N     M     ,     .     /   RSFT                    ARWU  NUM1  NUM2  NUM3  NENT */
           311,  314,  327,  330,  333,  336,  339,  342,  345,  348,  351,  354,  363,                    369,  375,  378,  391,  394,
        /* LCTL  LWIN  LALT               SPC              RALT  RFNC  RMNU  RCTL              ARWL  ARWD  ARWR  NUM0  NUM. */
           397,  400,  403,              415,              427,  430,  433,  436,              442,  455,  458,  461,  467
    },
    {
        /* Add more regional layout fixes here */
    }
};

RGBController_WitmodKeyboard::RGBController_WitmodKeyboard(WitmodKeyboardController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = controller->GetVendorString();
    type                    = DEVICE_TYPE_KEYBOARD;
    description             = "Witmod Keyboard Device";
    version                 = controller->GetVersionString();
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    int BASE_EFFECT_FLAGS   = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;

    const int EFFECTS_COUNT = 14;
    witmod_effect witmod_effects[EFFECTS_COUNT] =
    {
        {
            "Static",
            WITMOD_CONST_MODE_VALUE,
            MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
        },
        {
            "Direct",
            WITMOD_CUSTOM_MODE_VALUE,
            MODE_FLAG_HAS_PER_LED_COLOR
        },
        {
            "Wave",
            WITMOD_WAVE_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_DIRECTION_HV
        },
        {
            "Breathing",
            WITMOD_FADE_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
        {
            "Radar",
            WITMOD_RADAR_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR // round animation
        },
        {
            "Star (Interactive)",
            WITMOD_STAR_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
        {
            "Line (Interactive)",
            WITMOD_LINE_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_HV
        },
        {
            "Ripple (Interactive)",
            WITMOD_RIPPLE_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
        {
            "Stars",
            WITMOD_STARS_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
        {
            "Cross (Interactive)",
            WITMOD_CROSS_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
        {
            "Horizontal bars (Interactive)",
            WITMOD_WTF_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_DIRECTION_UD
        },
        {
            "Ripple random",
            WITMOD_RIPPLE_RANDOM_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
        {
            "Running line",
            WITMOD_RUNNING_LINE_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR // round direction
        },
        {
            "Fireworks (Interactive)",
            WITMOD_FIREWORK_MODE_VALUE,
            BASE_EFFECT_FLAGS | MODE_FLAG_HAS_SPEED
        },
    };

    for(int i = 0; i < EFFECTS_COUNT; i++)
    {
        mode m;
        m.name  = witmod_effects[i].name;
        m.value = witmod_effects[i].value;
        m.flags = witmod_effects[i].flags | MODE_FLAG_HAS_BRIGHTNESS;

        if(m.flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR && m.value != WITMOD_CONST_MODE_VALUE)
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
            m.speed_min = WITMOD_SPEED_MIN;
            m.speed_max = WITMOD_SPEED_MAX;
            m.speed     = (WITMOD_SPEED_MAX - WITMOD_SPEED_MIN) / 2;
        }

        if(m.flags & MODE_FLAG_HAS_BRIGHTNESS)
        {
            m.brightness_min = WITMOD_BRIGHTNESS_MIN;
            m.brightness_max = WITMOD_BRIGHTNESS_MAX;
            m.brightness     = m.brightness_max;
        }

        modes.push_back(m);
    }

    SetupZones();
}

RGBController_WitmodKeyboard::~RGBController_WitmodKeyboard()
{
    Shutdown();

    delete controller;
}

void RGBController_WitmodKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    |  Pick the layout for the size the controller reported.    |
    |  The number pad sits in the four matrix columns to the    |
    |  right of the GK8110 keys, so the map has to be built      |
    |  wider or those keys are dropped from it.                 |
    \*---------------------------------------------------------*/
    KEYBOARD_SIZE           keyboard_size   = controller->GetKeyboardSize();
    const layout_values*    offset_values   = &gk8110_offset_values;
    unsigned char           matrix_width    = GK8110_WIDTH;

    if(keyboard_size == KEYBOARD_SIZE_FULL)
    {
        offset_values   = &gk8120_offset_values;
        matrix_width    = GK8120_WIDTH;
    }

    KeyboardLayoutManager new_kb(KEYBOARD_LAYOUT_ANSI_QWERTY, keyboard_size, *offset_values);

    /*---------------------------------------------------------*\
    |  Create the keyboard zone usiung Keyboard Layout Manager  |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name                   = ZONE_EN_KEYBOARD;
    new_zone.type                   = ZONE_TYPE_MATRIX;
    new_zone.leds_count             = new_kb.GetKeyCount();
    new_zone.leds_min               = new_zone.leds_count;
    new_zone.leds_max               = new_zone.leds_count;
    new_zone.matrix_map             = new_kb.GetKeyMap(KEYBOARD_MAP_FILL_TYPE_COUNT, WITMOD_MATRIX_HEIGHT, matrix_width);

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

void RGBController_WitmodKeyboard::DeviceUpdateLEDs()
{
    controller->SetLEDsData(colors, leds);
}

void RGBController_WitmodKeyboard::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WitmodKeyboard::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WitmodKeyboard::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode]);
}
