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
#include "RGBControllerKeyNames.h"
#include "RGBController_MintakaKeyboard.h"
/*---------------------------------------------------------------------*\
|  VSG Keyboard Mintaka Layout                                          |
\*---------------------------------------------------------------------*/
layout_values mintaka_offset_values =
{
    {
        /* ESC     1     2     3     4     5     6     7     8     9     0     '     ¿   BSPC */
            19,   20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,   103,
        /* TAB     Q     W     E     R     T     Y     U     I     O     P     ´     +        */
            37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
        /* CPLK    A     S     D     F     G     H     J     K     L     Ñ     {     }   ENTR */
            55,   56,   57,   58,   59,   60,   61,   62,   63,   64,   65,   66,  108,    85,
        /* LSFT    <     Z     X     C     V     B     N     M     ,     .     -   RSFT       */
            73,  109,   74,   75,   76,   77,   78,   79,   80,   81,   82,   83,   84,
        /* LCTL  LWIN  LALT               SPC              RALT RMNU  RCTL  RFNC              */
            91,   92,   93,               94,                95,  96,   97,   98,
    },
    {
        { KEYBOARD_LAYOUT_ISO_QWERTY, {
            /*-------------------------------------------------------------------------------------------------------------------------------------*\
            | Edit Keys                                                                                                                             |
            |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
            \*-------------------------------------------------------------------------------------------------------------------------------------*/
            {   0,      1,      11,         0,          KEY_EN_QUOTE,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
            {   0,      1,      12,         0,          KEY_ES_OPEN_QUESTION_MARK,  KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },

            {   0,      2,      11,         0,          KEY_ES_TILDE,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
            {   0,      2,      12,         0,          KEY_EN_PLUS,                KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },

            {   0,      3,      10,         0,          KEY_ES_ENIE,                KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
            {   0,      3,      11,         0,          KEY_EN_LEFT_BRACKET,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
            {   0,      3,      12,         0,          KEY_EN_RIGHT_BRACKET,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },

            {   0,      4,       1,         0,          KEY_NORD_ANGLE_BRACKET,     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
            {   0,      4,      11,         0,          KEY_NORD_HYPHEN,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },

            {   0,      5,      11,         0,          KEY_EN_MENU,                KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
            {   0,      5,      12,         0,          KEY_EN_RIGHT_CONTROL,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
            {   0,      5,      13,         0,          KEY_EN_RIGHT_FUNCTION,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        }}
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

    name                                = controller->GetNameString();
    vendor                              = "VSG";
    type                                = DEVICE_TYPE_KEYBOARD;
    description                         = name;
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

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
    controller->SetLedSequencePositions(mintaka_offset_values.default_values);

    /*---------------------------------------------------------*\
    | Create LEDs for the Matrix zone                           |
    |   Place keys in the layout to populate the matrix         |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
    {
        led new_led;

        new_led.name            = new_kb.GetKeyNameAt(led_idx);
        new_led.value           = new_kb.GetKeyValueAt(led_idx);
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
