/*---------------------------------------------------------*\
| RGBController_MintakaKeyboard.cpp                         |
|                                                           |
|   Driver for VSG Mintaka Devices keyboard lighting        |
|   Based on KeychronKeyboardController                     |
|                                                           |
|   Federico Scodelaro (pudymody)               08 Oct 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/
#include <chrono>
#include <thread>

#include "KeyboardLayoutManager.h"
#include "LogManager.h"
#include "RGBControllerKeyNames.h"
#include "RGBController_MintakaKeyboard.h"
/*---------------------------------------------------------------------*\
|  VSG Keyboard Mintaka Layout                                          |
\*---------------------------------------------------------------------*/
/*
 *  Putting enter on row 2:
 *      - entr doesnt light up
 *      - last four from last row doesnt light up (RALT RMNU  RCTL  RFNC)
 *      - third row is shifted by one: h lights g, a lights caps lock
 *      - fifth row is shifted by one: win lights left ctrl, right alt turns space
 *
 *  Putting enter on row 3:
 *      - caps lock doesnt light up
 *      - last four from last row doesnt light up (RALT RMNU  RCTL  RFNC)
 *      - fifth row is shifted by one: win lights left ctrl, right alt turns space
 *
 *  In both cases, the first key of the shifted ones lights nothing.
 *
 *  In both cases, esc key  is off, but this is beacause line 411 of KeyboardLayoutManager.cpp.
 *      SwapKey(keyboard_zone_fn_row[0]);
 *  If i remove that line everything works ok regarding to the key at (0,0)
 *  Whatever led i put in that position (0,0) wont turn on
 */
layout_values mintaka_offset_values =
{
    {
        /* ESC     1     2     3     4     5     6     7     8     9     0     '     ¿   BSPC */
            19,   20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,   103,
        /* TAB     Q     W     E     R     T     Y     U     I     O     P     ´     +   ENTR */
            37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,   48,   49,    85,
        /* CPLK    A     S     D     F     G     H     J     K     L     Ñ     {     }        */
            55,   56,   57,   58,   59,   60,   61,   62,   63,   64,   65,   66,  108,
        /* LSFT    <     Z     X     C     V     B     N     M     ,     .     -   RSFT       */
            73,  109,   74,   75,   76,   77,   78,   79,   80,   81,   82,   83,   84,
        /* LCTL  LWIN  LALT               SPC              RALT RMNU  RCTL  RFNC              */
            91,   92,   93,                94,               95,  96,   97,   98,
    },
    {
        /* Add more regional layout fixes here */
    }
};

typedef struct
{
    std::string name;
    int value;
    int flags;
} mintaka_effect;

/**------------------------------------------------------------------*\
    @name Mintaka Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectMintakaKeyboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_MintakaKeyboard::RGBController_MintakaKeyboard(MintakaKeyboardController* controller_ptr)
{
    controller                          = controller_ptr;
    name                                = "Mintaka Keyboard";
    vendor                              = "VSG";
    type                                = DEVICE_TYPE_KEYBOARD;
    description                         = name;
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();
    version                             = controller->GetFirmwareVersion();

    mode Custom;
    Custom.name                         = "Custom";
    Custom.value                        = CUSTOM_MODE_VALUE;
    Custom.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Custom.color_mode                   = MODE_COLORS_PER_LED;
    Custom.brightness_min               = MINTAKA_MIN_BRIGHTNESS;
    Custom.brightness_max               = MINTAKA_MAX_BRIGHTNESS;
    Custom.brightness                   = MINTAKA_MAX_BRIGHTNESS;
    modes.push_back(Custom);

    mintaka_effect mintaka_effects[20] =
    {
        {
            "Static",
            STATIC_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Keystroke light up",
            KEYSTROKE_LIGHT_UP_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Keystroke dim",
            KEYSTROKE_DIM_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Sparkle",
            SPARKLE_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Rain",
            RAIN_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Random colors",
            RANDOM_COLORS_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Breathing",
            BREATHING_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Spectrum cycle",
            SPECTRUM_CYCLE_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Ring gradient",
            RING_GRADIENT_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Vertical gradient",
            VERTICAL_GRADIENT_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Horizontal gradient / Rainbow wave",
            HORIZONTAL_GRADIENT_WAVE_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Around edges",
            AROUND_EDGES_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Keystroke horizontal lines",
            KEYSTROKE_HORIZONTAL_LINES_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Keystroke tilted lines",
            KEYSTROKE_TITLED_LINES_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Keystroke ripples",
            KEYSTROKE_RIPPLES_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Sequence",
            SEQUENCE_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Wave line",
            WAVE_LINE_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Tilted lines",
            TILTED_LINES_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Back and forth",
            BACK_AND_FORTH_MODE_VALUE,
            MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_AUTOMATIC_SAVE
        },
        {
            "Off",
            LIGHTS_OFF_MODE_VALUE,
            MODE_FLAG_AUTOMATIC_SAVE
        }
    };

    for(const mintaka_effect& effect : mintaka_effects)
    {
        mode m;
        m.name = effect.name;
        m.value = effect.value;
        m.flags = effect.flags;

        if(m.flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
        {
            m.color_mode = MODE_COLORS_MODE_SPECIFIC;
            m.colors_min = 1;
            m.colors_max = 1;
            m.colors.resize(1);
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
            m.speed_min = MINTAKA_MIN_SPEED;
            m.speed_max = MINTAKA_MAX_SPEED;
            m.speed = m.speed_min;
        }

        if(m.flags & MODE_FLAG_HAS_BRIGHTNESS)
        {
            m.brightness_min = MINTAKA_MIN_BRIGHTNESS;
            m.brightness_max = MINTAKA_MAX_BRIGHTNESS;
            m.brightness = m.brightness_max;
        }

        modes.push_back(m);
    }

    SetupZones();
}

RGBController_MintakaKeyboard::~RGBController_MintakaKeyboard()
{
    delete controller;
}

void RGBController_MintakaKeyboard::SetupZones()
{

    /*---------------------------------------------------------*\
    | Create the keyboard zone usiung Keyboard Layout Manager   |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name               = ZONE_EN_KEYBOARD;
    new_zone.type               = ZONE_TYPE_MATRIX;

    KeyboardLayoutManager new_kb(KEYBOARD_LAYOUT_ISO_QWERTY, KEYBOARD_SIZE_SIXTY, mintaka_offset_values);

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

    unsigned int* sequence_positions    = new unsigned int[new_map->height * new_map->width];
    new_kb.GetKeyMap(sequence_positions, KEYBOARD_MAP_FILL_TYPE_VALUE, new_map->height, new_map->width);
    controller->SetLedSequencePositions(sequence_positions);

    /*---------------------------------------------------------*\
    | Create LEDs for the Matrix zone                           |
    |   Place keys in the layout to populate the matrix         |
    \*---------------------------------------------------------*/
    for(size_t led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
    {
        led new_led;

        new_led.name                = new_kb.GetKeyNameAt(led_idx);
        new_led.value               = new_kb.GetKeyValueAt(led_idx);
        leds.push_back(new_led);
    }

    zones.push_back(new_zone);

    SetupColors();
}

void RGBController_MintakaKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_MintakaKeyboard::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
}

void RGBController_MintakaKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetMode(modes, active_mode, colors);
}

void RGBController_MintakaKeyboard::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_MintakaKeyboard::DeviceUpdateMode()
{
    UpdateZoneLEDs(0);
}
