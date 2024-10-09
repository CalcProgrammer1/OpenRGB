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

#include "RGBController_MintakaKeyboard.h"
#include "RGBControllerKeyNames.h"

#define NA      0xFFFFFFFF

typedef struct
{
    const unsigned int                      width;                  /* matrix width         */
    const unsigned int                      height;                 /* matrix height        */
    std::vector<std::vector<unsigned int>>  matrix_map;             /* matrix map           */
    std::vector<std::string>                led_names;              /* led names            */
    std::vector<unsigned int>               led_sequence_positions; /* position in buffers  */
} mintaka;

/*-----------------------------------------*\
|  The one that is showed in the original   |
|  issue                                    |
\*-----------------------------------------*/
static mintaka default_mintaka =
{
    14,
    5,
    {
        {  0,  5,   10, 15, 19, 23, 27, 32, 36, 40, 44, 49, 54, 59},
        {  1,  6,  11, 16, 20, 24, 28, 33, 37, 41, 45, 50, 55, 60},
        {  2,  7,  12, 17, 21, 25, 29, 34, 38, 42, 46, 51, 56, NA},
        {  3,  8, 13, 18, 22, 26, 30, 35, 39, 43, 47, 52, 57, NA},
        {  4,  9,  14, NA, NA, NA, 31, NA, NA, NA, 48, 53, 58, 61}
    },
    {
        KEY_EN_ESCAPE,              //0
        KEY_EN_TAB,                 //1
        KEY_EN_CAPS_LOCK,           //2
        KEY_EN_LEFT_SHIFT,          //3
        KEY_EN_LEFT_CONTROL,        //4

        KEY_EN_1,                   //5
        KEY_EN_Q,                   //6
        KEY_EN_A,                   //7
        KEY_NORD_ANGLE_BRACKET,     //8
        KEY_EN_LEFT_WINDOWS,        //9

        KEY_EN_2,                   //10
        KEY_EN_W,                   //11
        KEY_EN_S,                   //12
        KEY_EN_Z,                   //13
        KEY_EN_LEFT_ALT,            //14

        KEY_EN_3,                   //15
        KEY_EN_E,                   //16
        KEY_EN_D,                   //17
        KEY_EN_X,                   //18

        KEY_EN_4,                   //19
        KEY_EN_R,                   //20
        KEY_EN_F,                   //21
        KEY_EN_C,                   //22

        KEY_EN_5,                   //23
        KEY_EN_T,                   //24
        KEY_EN_G,                   //25
        KEY_EN_V,                   //26

        KEY_EN_6,                   //27
        KEY_EN_Y,                   //28
        KEY_EN_H,                   //29
        KEY_EN_B,                   //30
        KEY_EN_SPACE,               //31

        KEY_EN_7,                   //32
        KEY_EN_U,                   //33
        KEY_EN_J,                   //34
        KEY_EN_N,                   //35

        KEY_EN_8,                   //36
        KEY_EN_I,                   //37
        KEY_EN_K,                   //38
        KEY_EN_M,                   //39

        KEY_EN_9,                   //40
        KEY_EN_O,                   //41
        KEY_EN_L,                   //42
        KEY_EN_COMMA,               //43

        KEY_EN_0,                   //44
        KEY_EN_P,                   //45
        KEY_ES_ENIE,           //46
        KEY_EN_PERIOD,              //47
        KEY_EN_RIGHT_ALT,           //48

        KEY_EN_QUOTE,               //49
        KEY_ES_TILDE,        //50
        KEY_EN_LEFT_BRACKET,               //51
        KEY_NORD_HYPHEN,       //52
        KEY_EN_MENU,          //53

        KEY_ES_OPEN_QUESTION_MARK,              //54
        KEY_EN_PLUS,       //55
        KEY_EN_RIGHT_BRACKET,      //56
        KEY_EN_RIGHT_SHIFT,         //57
        KEY_EN_RIGHT_CONTROL,       //58

        KEY_EN_BACKSPACE,           //59

        KEY_EN_ISO_ENTER,           //60
        KEY_EN_RIGHT_FUNCTION,      //61
    },
    {
        19, // KEY_EN_ESCAPE,              //0
        37, // KEY_EN_TAB,                 //1
        55, // KEY_EN_CAPS_LOCK,           //2
        73, // KEY_EN_LEFT_SHIFT,          //3
        91, // KEY_EN_LEFT_CONTROL,        //4

        20, // KEY_EN_1,                   //5
        38, // KEY_EN_Q,                   //6
        56, // KEY_EN_A,                   //7
        109, // KEY_NORD_ANGLE_BRACKET,     //8
        92, // KEY_EN_LEFT_WINDOWS,        //9

        21, // KEY_EN_2,                   //10
        39, // KEY_EN_W,                   //11
        57, // KEY_EN_S,                   //12
        74, // KEY_EN_Z,                   //13
        93, // KEY_EN_LEFT_ALT,            //14

        22, // KEY_EN_3,                   //15
        40, // KEY_EN_E,                   //16
        58, // KEY_EN_D,                   //17
        75, // KEY_EN_X,                   //18

        23, // KEY_EN_4,                   //19
        41, // KEY_EN_R,                   //20
        59, // KEY_EN_F,                   //21
        76, // KEY_EN_C,                   //22

        24, // KEY_EN_5,                   //23
        42, // KEY_EN_T,                   //24
        60, // KEY_EN_G,                   //25
        77, // KEY_EN_V,                   //26

        25, // KEY_EN_6,                   //27
        43, // KEY_EN_Y,                   //28
        61, // KEY_EN_H,                   //29
        78, // KEY_EN_B,                   //30
        94, // KEY_EN_SPACE,               //31

        26, // KEY_EN_7,                   //32
        44, // KEY_EN_U,                   //33
        62, // KEY_EN_J,                   //34
        79, // KEY_EN_N,                   //35

        27, // KEY_EN_8,                   //36
        45, // KEY_EN_I,                   //37
        63, // KEY_EN_K,                   //38
        80, // KEY_EN_M,                   //39

        28, // KEY_EN_9,                   //40
        46, // KEY_EN_O,                   //41
        64, // KEY_EN_L,                   //42
        81, // KEY_EN_COMMA,               //43

        29, // KEY_EN_0,                   //44
        47, // KEY_EN_P,                   //45
        65, // KEY_ES_ENIE,           //46
        82, // KEY_EN_PERIOD,              //47
        95, // KEY_EN_RIGHT_ALT,           //48

        30, // KEY_EN_QUOTE,               //49
        48, // KEY_ES_TILDE,        //50
        66, // KEY_EN_LEFT_BRACKET,               //51
        83, // KEY_NORD_HYPHEN,       //52
        96, // KEY_EN_MENU,          //53

        31, // KEY_ES_OPEN_QUESTION_MARK,              //54
        49, // KEY_EN_PLUS,       //55
        108, // KEY_EN_RIGHT_BRACKET,      //56
        84, // KEY_EN_RIGHT_SHIFT,         //57
        97, // KEY_EN_RIGHT_CONTROL,       //58

        103, // KEY_EN_BACKSPACE,           //59

        85, // KEY_EN_ISO_ENTER,           //60
        98, // KEY_EN_RIGHT_FUNCTION,      //61
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
    /*-----------------------------------------*\
    |  TODO: add logical switch here when we    |
    |        will have to add different layouts |
    \*-----------------------------------------*/
    mintaka keyboard = default_mintaka;

    controller->SetLedSequencePositions(keyboard.led_sequence_positions);

    /*-----------------------------------------*\
    |  Create the zone                          |
    \*-----------------------------------------*/
    unsigned int zone_size = 0;

    zone keyboard_zone;
    keyboard_zone.name               = ZONE_EN_KEYBOARD;
    keyboard_zone.type               = ZONE_TYPE_MATRIX;

    keyboard_zone.matrix_map         = new matrix_map_type;
    keyboard_zone.matrix_map->height = keyboard.height;
    keyboard_zone.matrix_map->width  = keyboard.width;

    keyboard_zone.matrix_map->map    = new unsigned int[keyboard.height * keyboard.width];

    for(unsigned int w = 0; w < keyboard.width; w++)
    {
        for(unsigned int h = 0; h < keyboard.height; h++)
        {
            unsigned int key = keyboard.matrix_map[h][w];
            keyboard_zone.matrix_map->map[h * keyboard.width + w] = key;

            if(key != NA)
            {
                led new_led;
                new_led.name = keyboard.led_names[key];
                leds.push_back(new_led);
                zone_size++;
            }
        }
    }

    keyboard_zone.leds_min           = zone_size;
    keyboard_zone.leds_max           = zone_size;
    keyboard_zone.leds_count         = zone_size;

    zones.push_back(keyboard_zone);

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
