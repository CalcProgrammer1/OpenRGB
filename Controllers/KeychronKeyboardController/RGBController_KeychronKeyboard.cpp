/*---------------------------------------------------------*\
| RGBController_KeychronKeyboard.cpp                        |
|                                                           |
|   RGBController for Keychron keyboard                     |
|                                                           |
|   Morgan Guimard (morg)                       20 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBControllerKeyNames.h"
#include "RGBController_KeychronKeyboard.h"

#define NA      0xFFFFFFFF

typedef struct
{
    const unsigned int                      width;                  /* matrix width         */
    const unsigned int                      height;                 /* matrix height        */
    std::vector<std::vector<unsigned int>>  matrix_map;             /* matrix map           */
    std::vector<std::string>                led_names;              /* led names            */
    std::vector<unsigned int>               led_sequence_positions; /* position in buffers  */
} keychron;

/*-----------------------------------------*\
|  The one that is showed in the original   |
|  issue                                    |
\*-----------------------------------------*/
static keychron default_keychron =
{
    17,
    5,
    {
        {  0,  5,   7, 12, 16, 20, 24, 29, 33, 37, 41, 46, 51, 54, NA, 61, 66},
        {  1,  NA,  8, 13, 17, 21, 25, 30, 34, 38, 42, 47, 52, 55, 59, 62, 67},
        {  2,  NA,  9, 14, 18, 22, 26, 31, 35, 39, 43, 48, 53, 56, NA, 63, 68},
        {  3,  NA, 10, 15, 19, 23, 27, 32, 36, 40, 44, 49, NA, 57, NA, 64, 69},
        {  4,  6,  11, NA, NA, NA, 28, NA, NA, NA, 45, 50, NA, 58, 60, 65, 70}
    },
    {
        KEY_EN_ESCAPE,              //0
        KEY_EN_TAB,                 //1
        KEY_EN_CAPS_LOCK,           //2
        KEY_EN_LEFT_SHIFT,          //3
        KEY_EN_LEFT_CONTROL,        //4

        KEY_EN_1,                   //5
        KEY_EN_LEFT_WINDOWS,        //6

        KEY_EN_2,                   //7
        KEY_EN_Q,                   //8
        KEY_EN_A,                   //9
        KEY_EN_Z,                   //10
        KEY_EN_LEFT_ALT,            //11

        KEY_EN_3,                   //12
        KEY_EN_W,                   //13
        KEY_EN_S,                   //14
        KEY_EN_X,                   //15

        KEY_EN_4,                   //16
        KEY_EN_E,                   //17
        KEY_EN_D,                   //18
        KEY_EN_C,                   //19

        KEY_EN_5,                   //20
        KEY_EN_R,                   //21
        KEY_EN_F,                   //22
        KEY_EN_V,                   //23

        KEY_EN_6,                   //24
        KEY_EN_T,                   //25
        KEY_EN_G,                   //26
        KEY_EN_B,                   //27
        KEY_EN_SPACE,               //28

        KEY_EN_7,                   //29
        KEY_EN_Y,                   //30
        KEY_EN_H,                   //31
        KEY_EN_N,                   //32

        KEY_EN_8,                   //33
        KEY_EN_U,                   //34
        KEY_EN_J,                   //35
        KEY_EN_M,                   //36

        KEY_EN_9,                   //37
        KEY_EN_I,                   //38
        KEY_EN_K,                   //39
        KEY_EN_COMMA,               //40

        KEY_EN_0,                   //41
        KEY_EN_O,                   //42
        KEY_EN_L,                   //43
        KEY_EN_PERIOD,              //44
        KEY_EN_RIGHT_ALT,           //45

        KEY_EN_MINUS,               //46
        KEY_EN_P,                   //47
        KEY_EN_SEMICOLON,           //48
        KEY_EN_FORWARD_SLASH,       //49
        KEY_EN_RIGHT_FUNCTION,      //50


        KEY_EN_EQUALS,              //51
        KEY_EN_LEFT_BRACKET,        //52
        KEY_EN_QUOTE,               //53

        KEY_EN_BACKSPACE,           //54
        KEY_EN_RIGHT_BRACKET,       //55
        KEY_EN_ISO_ENTER,           //56
        KEY_EN_RIGHT_SHIFT,         //57
        KEY_EN_RIGHT_CONTROL,       //58

        KEY_EN_ISO_BACK_SLASH,      //59
        KEY_EN_LEFT_ARROW,          //60

        KEY_EN_INSERT,              //61
        KEY_EN_DELETE,              //62
        KEY_EN_PAUSE_BREAK,         //63
        KEY_EN_UP_ARROW,            //64
        KEY_EN_DOWN_ARROW,          //65

        KEY_EN_HOME,                //66
        KEY_EN_END,                 //67
        KEY_EN_PAGE_UP,             //68
        KEY_EN_PAGE_DOWN,           //69
        KEY_EN_RIGHT_ARROW          //70
    },
    {
        0, //KEY_EN_ESCAPE,              //0
        1, //KEY_EN_TAB,                 //1
        2, //KEY_EN_CAPS_LOCK,           //2
        3, //KEY_EN_LEFT_SHIFT,          //3
        4, //KEY_EN_LEFT_CONTROL,        //4
        5, //KEY_EN_1,                   //5
        6, //KEY_EN_LEFT_WINDOWS,        //6
        7, //KEY_EN_2,                   //7
        8, //KEY_EN_Q,                   //8
        9, //KEY_EN_A,                   //9
       10, //KEY_EN_Z,                   //10
       11, //KEY_EN_LEFT_ALT,            //11
       12, //KEY_EN_3,                   //12
       13, //KEY_EN_W,                   //13
       14, //KEY_EN_S,                   //14
       15, //KEY_EN_X,                   //15
       16, //KEY_EN_4,                   //16
       17, //KEY_EN_E,                   //17
       18, //KEY_EN_D,                   //18
       19, //KEY_EN_C,                   //19
       20, //KEY_EN_5,                   //20
       21, //KEY_EN_R,                   //21
       22, //KEY_EN_F,                   //22
       23, //KEY_EN_V,                   //23
       24, //KEY_EN_6,                   //24
       25, //KEY_EN_T,                   //25
       26, //KEY_EN_G,                   //26
       27, //KEY_EN_B,                   //27
       28, //KEY_EN_SPACE,               //28
       29, //KEY_EN_7,                   //29
       30, //KEY_EN_Y,                   //30
       31, //KEY_EN_H,                   //31
       32, //KEY_EN_N,                   //32
       33, //KEY_EN_8,                   //33
       34, //KEY_EN_U,                   //34
       35, //KEY_EN_J,                   //35
       36, //KEY_EN_M,                   //36
       37, //KEY_EN_9,                   //37
       38, //KEY_EN_I,                   //38
       39, //KEY_EN_K,                   //39
       40, //KEY_EN_COMMA,               //40
       41, //KEY_EN_0,                   //41
       42, //KEY_EN_O,                   //42
       43, //KEY_EN_L,                   //43
       44, //KEY_EN_PERIOD,              //44
       45, //KEY_EN_RIGHT_ALT,           //45
       46, //KEY_EN_MINUS,               //46
       47, //KEY_EN_P,                   //47
       48, //KEY_EN_SEMICOLON,           //48
       49, //KEY_EN_FORWARD_SLASH,       //49
       50, //KEY_EN_RIGHT_FUNCTION,      //50
       51, //KEY_EN_EQUALS,              //51
       52, //KEY_EN_LEFT_BRACKET,        //52
       53, //KEY_EN_QUOTE,               //53
       54, //KEY_EN_BACKSPACE,           //54
       55, //KEY_EN_RIGHT_BRACKET,       //55
       56, //KEY_EN_ISO_ENTER,           //56
       57, //KEY_EN_RIGHT_SHIFT,         //57
       58, //KEY_EN_RIGHT_CONTROL,       //58
       59, //KEY_EN_ISO_BACK_SLASH,      //59
       60, //KEY_EN_LEFT_ARROW,          //60
       61, //KEY_EN_INSERT,              //61
       62, //KEY_EN_DELETE,              //62
       63, //KEY_EN_PAUSE_BREAK,         //63
       64, //KEY_EN_UP_ARROW,            //64
       65, //KEY_EN_DOWN_ARROW,          //65
       66, //KEY_EN_HOME,                //66
       67, //KEY_EN_END,                 //67
       68, //KEY_EN_PAGE_UP,             //68
       69, //KEY_EN_PAGE_DOWN,           //69
       70  //KEY_EN_RIGHT_ARROW          //70
    }
};

/*-----------------------------------------*\
|  K3 V2 VERSION                            |
\*-----------------------------------------*/
static keychron k3_keychron =
{
    16,
    6,
    {
        {  0,  6,  9, 15, 20, 25, 30, 36, 41, 46, 51, 57, 63, 69, 73, 78},
        {  1,  7, 10, 16, 21, 26, 31, 37, 42, 47, 52, 58, 64, 70, NA, 79},
        {  2, NA, 11, 17, 22, 27, 32, 38, 43, 48, 53, 59, 65, 71, 74, 80},
        {  3, NA, 12, 18, 23, 28, 33, 39, 44, 49, 54, 60, 66, NA, 75, 81},
        {  4, NA, 13, 19, 24, 29, 34, 40, 45, 50, 55, 61, 67, NA, 76, 82},
        {  5,  8, 14, NA, NA, NA, 35, NA, NA, NA, 56, 62, 68, 72, 77, 83}
    },
    {
        KEY_EN_ESCAPE,              //0
        KEY_EN_BACK_TICK,           //1
        KEY_EN_TAB,                 //2
        KEY_EN_CAPS_LOCK,           //3
        KEY_EN_LEFT_SHIFT,          //4
        KEY_EN_LEFT_CONTROL,        //5

        KEY_EN_F1,                  //6
        KEY_EN_1,                   //7
        KEY_EN_LEFT_WINDOWS,        //8

        KEY_EN_F2,                  //9
        KEY_EN_2,                   //10
        KEY_EN_Q,                   //11
        KEY_EN_A,                   //12
        KEY_EN_Z,                   //13
        KEY_EN_LEFT_ALT,            //14

        KEY_EN_F3,                  //15
        KEY_EN_3,                   //16
        KEY_EN_W,                   //17
        KEY_EN_S,                   //18
        KEY_EN_X,                   //19

        KEY_EN_F4,                  //20
        KEY_EN_4,                   //21
        KEY_EN_E,                   //22
        KEY_EN_D,                   //23
        KEY_EN_C,                   //24

        KEY_EN_F5,                  //25
        KEY_EN_5,                   //26
        KEY_EN_R,                   //27
        KEY_EN_F,                   //28
        KEY_EN_V,                   //29

        KEY_EN_F6,                  //30
        KEY_EN_6,                   //31
        KEY_EN_T,                   //32
        KEY_EN_G,                   //33
        KEY_EN_B,                   //34
        KEY_EN_SPACE,               //35

        KEY_EN_F7,                  //36
        KEY_EN_7,                   //37
        KEY_EN_Y,                   //38
        KEY_EN_H,                   //39
        KEY_EN_N,                   //40

        KEY_EN_F8,                  //41
        KEY_EN_8,                   //42
        KEY_EN_U,                   //43
        KEY_EN_J,                   //44
        KEY_EN_M,                   //45

        KEY_EN_F9,                  //46
        KEY_EN_9,                   //47
        KEY_EN_I,                   //48
        KEY_EN_K,                   //49
        KEY_EN_COMMA,               //50

        KEY_EN_F10,                 //51
        KEY_EN_0,                   //52
        KEY_EN_O,                   //53
        KEY_EN_L,                   //54
        KEY_EN_PERIOD,              //55
        KEY_EN_RIGHT_ALT,           //56

        KEY_EN_F11,                 //57
        KEY_EN_MINUS,               //58
        KEY_EN_P,                   //59
        KEY_EN_SEMICOLON,           //60
        KEY_EN_FORWARD_SLASH,       //61
        KEY_EN_RIGHT_FUNCTION,      //62

        KEY_EN_F12,                 //63
        KEY_EN_NUMPAD_PLUS,         //64
        KEY_EN_LEFT_BRACKET,        //65
        KEY_EN_QUOTE,               //66
        KEY_EN_LEFT_SHIFT,          //67
        KEY_EN_RIGHT_CONTROL,       //68

        KEY_EN_PRINT_SCREEN,        //69
        KEY_EN_BACKSPACE,           //70
        KEY_EN_RIGHT_BRACKET,       //71
        KEY_EN_LEFT_ARROW,          //72

        KEY_EN_DELETE,              //73
        KEY_EN_ANSI_BACK_SLASH,     //74
        KEY_EN_ISO_ENTER,           //75
        KEY_EN_UP_ARROW,            //76
        KEY_EN_DOWN_ARROW,          //77

        "Key: Light",               //78
        KEY_EN_PAGE_UP,             //79
        KEY_EN_PAGE_DOWN,           //80
        KEY_EN_HOME,                //81
        KEY_EN_END,                 //82
        KEY_EN_RIGHT_ARROW          //83
    },
    {
        143,
        18,
        34,
        50,
        66,
        81,
        2,
        19,
        82,
        3,
        20,
        35,
        51,
        67,
        83,
        4,
        21,
        36,
        52,
        68,
        5,
        22,
        37,
        53,
        69,
        6,
        23,
        38,
        54,
        70,
        7,
        24,
        39,
        55,
        71,
        84,
        8,
        25,
        40,
        56,
        72,
        9,
        26,
        41,
        57,
        73,
        10,
        27,
        42,
        58,
        74,
        11,
        28,
        43,
        59,
        75,
        85,
        12,
        29,
        44,
        60,
        76,
        86,
        13,
        30,
        45,
        61,
        77,
        87,
        14,
        31,
        46,
        88,
        15,
        47,
        63,
        78,
        89,
        16,
        32,
        48,
        64,
        79,
        90
    }
};

typedef struct
{
    std::string name;
    int value;
    int flags;
} keychron_effect;

/**------------------------------------------------------------------*\
    @name Keychron Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectKeychronKeyboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_KeychronKeyboard::RGBController_KeychronKeyboard(KeychronKeyboardController* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetNameString();
    vendor                              = "Keychron";
    type                                = DEVICE_TYPE_KEYBOARD;
    description                         = name;
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Custom;
    Custom.name                         = "Custom";
    Custom.value                        = CUSTOM_MODE_VALUE;
    Custom.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Custom.color_mode                   = MODE_COLORS_PER_LED;
    Custom.brightness_min               = KEYCHRON_MIN_BRIGHTNESS;
    Custom.brightness_max               = KEYCHRON_MAX_BRIGHTNESS;
    Custom.brightness                   = KEYCHRON_MAX_BRIGHTNESS;
    modes.push_back(Custom);

    keychron_effect keychron_effects[20] =
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

    for(const keychron_effect& effect : keychron_effects)
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
            m.speed_min = KEYCHRON_MIN_SPEED;
            m.speed_max = KEYCHRON_MAX_SPEED;
            m.speed = m.speed_min;
        }

        if(m.flags & MODE_FLAG_HAS_BRIGHTNESS)
        {
            m.brightness_min = KEYCHRON_MIN_BRIGHTNESS;
            m.brightness_max = KEYCHRON_MAX_BRIGHTNESS;
            m.brightness = m.brightness_max;
        }

        modes.push_back(m);
    }

    SetupZones();
}

RGBController_KeychronKeyboard::~RGBController_KeychronKeyboard()
{
    delete controller;
}

void RGBController_KeychronKeyboard::SetupZones()
{
    /*-----------------------------------------*\
    |  TODO: add logical switch here when we    |
    |        will have to add different layouts |
    \*-----------------------------------------*/
    keychron keyboard = k3_keychron;

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

void RGBController_KeychronKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_KeychronKeyboard::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
}

void RGBController_KeychronKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetMode(modes, active_mode, colors);
}

void RGBController_KeychronKeyboard::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_KeychronKeyboard::DeviceUpdateMode()
{
    UpdateZoneLEDs(0);
}
