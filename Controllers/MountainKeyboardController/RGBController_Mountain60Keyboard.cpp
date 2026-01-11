/*---------------------------------------------------------*\
| RGBController_MountainKeyboard.cpp                        |
|                                                           |
|   RGBController for Mountain keyboard                     |
|                                                           |
|   O'D.Sæzl                                       Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_Mountain60Keyboard.h"
#include "RGBControllerKeyNames.h"
#include "KeyboardLayoutManager.h"

using namespace std::chrono_literals;

/*-------------------------------*\
| TODO: Detect detached keypad    |
\*-------------------------------*/

std::vector<unsigned int> mountain60_keyboard_key_id_values =
    {
        /*  ESC    1     2     3     4     5     6     7     8     9     0     -    =   BSPC    */
        0,    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,   32,   33,   34,
        /*  TAB    Q     W     E     R     T     Y     U     I     O     P     [     ]     \    */
        42,   43,   44,   45,   46,   47,   48,   49,   50,   51,   52,   53,   54,   55,
        /* CPLK    A     S     D     F     G     H     J     K     L     ;     "         ENTR   */
        63,   64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,         76,
        /* LSFT    Z     X     C     V     B     N     M     ,     .     /   RSFT  ARWU   DEL   */
        84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   97,   94,   56,
        /* LCTL  LWIN  LALT               SPC              RALT  RFNC  FNC   ARWL  ARWD  ARWR   */
        105,  106,  107,                110,                  113,  115,  119,  120,  121,
};

layout_values mountain60_layout =
    {
        mountain60_keyboard_key_id_values,
        {
            /*------------------------------------------*\
            |   No regional layout fix for the moment    |
            \*------------------------------------------*/
        },
};

keyboard_keymap_overlay_values  mountain60_keyboard_overlay_no_numpad =
    {
        KEYBOARD_SIZE::KEYBOARD_SIZE_SIXTY,
        mountain60_layout,
        {
         /*--------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                         |
        |   Zone,   Row,    Column,     Value,      Key,                     Alt name,              OpCode,                                 |
        \*---------------------------------------------------------------------------------------------------------------------------------*/
         {   0,      4,      13,         120,        KEY_EN_DOWN_ARROW,      KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      4,      12,         119,        KEY_EN_LEFT_ARROW,      KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      4,      14,         121,        KEY_EN_RIGHT_ARROW,     KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      4,      14,         121,        KEY_EN_RIGHT_ARROW,     KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      3,      13,          99,        KEY_EN_UP_ARROW,        KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,      12,          97,        KEY_EN_RIGHT_SHIFT,     KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,       2,          85,        KEY_EN_Z,               KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,       3,          86,        KEY_EN_X,               KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,       4,          87,        KEY_EN_C,               KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,       5,          88,        KEY_EN_B,               KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,       6,          89,        KEY_EN_V,               KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,       7,          90,        KEY_EN_N,               KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,       8,          91,        KEY_EN_M,               KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,       9,          92,        KEY_EN_COMMA,           KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,      10,          93,        KEY_EN_PERIOD,          KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,      14,          56,        KEY_EN_DELETE,          KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      2,      13,          76,        KEY_EN_ANSI_ENTER,      KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,       0,          84,        KEY_EN_LEFT_SHIFT,      KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         //upper edge
         {   0,      0,       0,          126,       "Edge 1",               KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_ROW,            },
         {   0,      0,       1,          127,       "Edge 2",               KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,       2,          128,       "Edge 3",               KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,       3,          129,       "Edge 4",               KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,       4,          130,       "Edge 5",               KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,       5,          131,       "Edge 6",               KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,       6,          132,       "Edge 7",               KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,       7,          133,       "Edge 8",               KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,       8,          134,       "Edge 9",               KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,       9,          135,       "Edge 10",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,      10,          136,       "Edge 11",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,      11,          137,       "Edge 12",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,      12,          138,       "Edge 13",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,      13,          139,       "Edge 14",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,      14,          140,       "Edge 15",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,      15,          141,       "Edge 16",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         //left edge
         {   0,      0,       0,          169,       "Edge 44",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      1,       0,          168,       "Edge 43",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      2,       0,          167,       "Edge 42",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      3,       0,          166,       "Edge 41",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      4,       0,          165,       "Edge 40",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      5,       0,          164,       "Edge 39",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         //down edge
         {   0,      6,       0,            1,       KEY_EN_UNUSED,          KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_ROW,            },
         {   0,      6,       0,          163,       "Edge 38",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,       1,          162,       "Edge 37",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,       2,          161,       "Edge 36",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,       3,          160,       "Edge 35",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,       4,          159,       "Edge 34",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,       5,          158,       "Edge 33",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,       6,          157,       "Edge 32",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,       7,          156,       "Edge 31",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,       8,          155,       "Edge 30",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,       9,          154,       "Edge 29",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,      10,          153,       "Edge 28",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,      11,          152,       "Edge 27",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,      12,          151,       "Edge 26",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,      13,          150,       "Edge 25",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,      14,          149,       "Edge 24",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,      15,          148,       "Edge 23",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         //right edge
         {   0,      1,      16,          142,       "Edge 17",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      2,      16,          143,       "Edge 18",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      3,      16,          144,       "Edge 19",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      4,      16,          145,       "Edge 20",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      5,      16,          146,       "Edge 21",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,      16,          147,       "Edge 22",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,      16,          147,       "Edge 22",              KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         //numpad left edge
         {   0,      0,      17,         191,        "Numpad Edge 22",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      1,      17,         190,        "Numpad Edge 21",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      2,      17,         189,        "Numpad Edge 20",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      3,      17,         188,        "Numpad Edge 19",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      4,      17,         187,        "Numpad Edge 18",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      5,      17,         186,        "Numpad Edge 17",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         //numpad upper edge
         {   0,      0,      18,         170,        "Numpad Edge 1",        KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,      19,         171,        "Numpad Edge 2",        KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,      20,         172,        "Numpad Edge 3",        KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,      21,         173,        "Numpad Edge 4",        KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      0,      22,         174,        "Numpad Edge 5",        KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         //numpad down edge
         {   0,      6,      17,         185,        "Numpad Edge 12",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT     },
         {   0,      6,      18,         184,        "Numpad Edge 13",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,      19,         183,        "Numpad Edge 14",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,      20,         182,        "Numpad Edge 15",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,      21,         181,        "Numpad Edge 16",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         //numpad right edge
         {   0,      1,      22,         175,        "Numpad Edge 6",        KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      2,      22,         176,        "Numpad Edge 7",        KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      3,      22,         177,        "Numpad Edge 8",        KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      4,      22,         178,        "Numpad Edge 9",        KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      5,      22,         179,        "Numpad Edge 10",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,      22,         180,        "Numpad Edge 11",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT,    },
         {   0,      6,      22,         180,        "Numpad Edge 11",       KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         //numpad keys
         {   0,      1,      18,          38,        KEY_EN_NUMPAD_LOCK,     KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      1,      19,          39,        KEY_EN_NUMPAD_DIVIDE,   KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      1,      20,          40,        KEY_EN_NUMPAD_TIMES,    KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      1,      21,          41,        KEY_EN_NUMPAD_MINUS,    KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      2,      18,          59,        KEY_EN_NUMPAD_7,        KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      2,      19,          60,        KEY_EN_NUMPAD_8,        KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      2,      20,          61,        KEY_EN_NUMPAD_9,        KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      2,      21,          62,        KEY_EN_NUMPAD_PLUS,     KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,      18,          80,        KEY_EN_NUMPAD_4,        KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,      19,          81,        KEY_EN_NUMPAD_5,        KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      3,      20,          82,        KEY_EN_NUMPAD_6,        KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      4,      18,         101,        KEY_EN_NUMPAD_1,        KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      4,      19,         102,        KEY_EN_NUMPAD_2,        KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      4,      20,         103,        KEY_EN_NUMPAD_3,        KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      5,      18,         122,        KEY_EN_NUMPAD_0,        KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      5,      20,         124,        KEY_EN_NUMPAD_PERIOD,   KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         {   0,      5,      21,         125,        KEY_EN_NUMPAD_ENTER,    KEY_EN_UNUSED,          KEYBOARD_OPCODE_SWAP_ONLY,             },
         }
};

/**------------------------------------------------------------------*\
    @name Mountain Keyboard
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectMountainKeyboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_Mountain60Keyboard::RGBController_Mountain60Keyboard(Mountain60KeyboardController* controller_ptr)
{
    controller                          = controller_ptr;
    name                                = controller->GetNameString();
    vendor                              = "Mountain";
    type                                = DEVICE_TYPE_KEYBOARD;
    description                         = "Mountain Everest Keyboard 60% Device";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Direct;
    Direct.name                         = "Direct";
    Direct.value                        = MOUNTAIN60_KEYBOARD_MODE_CUSTOM;
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.brightness_min               = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MIN;
    Direct.brightness                   = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Direct.brightness_max               = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name                            = "Off";
    Off.value                           = MOUNTAIN60_KEYBOARD_MODE_OFF;
    Off.flags                           = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                         = "Static";
    Static.value                        = MOUNTAIN60_KEYBOARD_MODE_STATIC;
    Static.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.brightness_min               = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MIN;
    Static.brightness                   = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Static.brightness_max               = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Static.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min                   = 1;
    Static.colors_max                   = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode ColorWaveRainbow;
    ColorWaveRainbow.name               = "Rainbow Wave";
    ColorWaveRainbow.value              = MOUNTAIN60_KEYBOARD_MODE_COLOR_WAVE;
    ColorWaveRainbow.flags              = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_MANUAL_SAVE;
    ColorWaveRainbow.brightness_min     = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MIN;
    ColorWaveRainbow.brightness         = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    ColorWaveRainbow.brightness_max     = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    ColorWaveRainbow.speed_min          = MOUNTAIN60_KEYBOARD_SPEED_MIN;
    ColorWaveRainbow.speed              = MOUNTAIN60_KEYBOARD_SPEED_DEFAULT;
    ColorWaveRainbow.speed_max          = MOUNTAIN60_KEYBOARD_SPEED_MAX;
    ColorWaveRainbow.color_mode         = MODE_COLORS_RANDOM;

    modes.push_back(ColorWaveRainbow);

    mode ColorWave;
    ColorWave.name                      = "ColorWave";
    ColorWave.value                     = MOUNTAIN60_KEYBOARD_MODE_COLOR_WAVE;
    ColorWave.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_MANUAL_SAVE;
    ColorWave.brightness_min            = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MIN;
    ColorWave.brightness                = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    ColorWave.brightness_max            = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    ColorWave.speed_min                 = MOUNTAIN60_KEYBOARD_SPEED_MIN;
    ColorWave.speed                     = MOUNTAIN60_KEYBOARD_SPEED_DEFAULT;
    ColorWave.speed_max                 = MOUNTAIN60_KEYBOARD_SPEED_MAX;
    ColorWave.colors_min                = 1;
    ColorWave.colors_max                = 2;
    ColorWave.colors.resize(2);
    ColorWave.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(ColorWave);

    mode Tornado;
    Tornado.name                        = "Tornado";
    Tornado.value                       = MOUNTAIN60_KEYBOARD_MODE_TORNADO;
    Tornado.flags                       = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_MANUAL_SAVE;
    Tornado.brightness_min              = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MIN;
    Tornado.brightness                  = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Tornado.brightness_max              = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Tornado.speed_min                   = MOUNTAIN60_KEYBOARD_SPEED_MIN;
    Tornado.speed                       = MOUNTAIN60_KEYBOARD_SPEED_DEFAULT;
    Tornado.speed_max                   = MOUNTAIN60_KEYBOARD_SPEED_MAX;
    Tornado.colors_min                  = 1;
    Tornado.colors_max                  = 1;
    Tornado.colors.resize(1);
    Tornado.color_mode                  = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Tornado);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = MOUNTAIN60_KEYBOARD_MODE_BREATHING;
    Breathing.flags                     = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Breathing.brightness_min            = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MIN;
    Breathing.brightness                = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Breathing.brightness_max            = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Breathing.speed_min                 = MOUNTAIN60_KEYBOARD_SPEED_MIN;
    Breathing.speed                     = MOUNTAIN60_KEYBOARD_SPEED_DEFAULT;
    Breathing.speed_max                 = MOUNTAIN60_KEYBOARD_SPEED_MAX;
    Breathing.colors_min                = 1;
    Breathing.colors_max                = 2;
    Breathing.colors.resize(2);
    Breathing.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Breathing);

    mode Reactive;
    Reactive.name                       = "Reactive";
    Reactive.value                      = MOUNTAIN60_KEYBOARD_MODE_REACTIVE;
    Reactive.flags                      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Reactive.brightness_min             = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MIN;
    Reactive.brightness                 = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Reactive.brightness_max             = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Reactive.speed_min                  = MOUNTAIN60_KEYBOARD_SPEED_MIN;
    Reactive.speed                      = MOUNTAIN60_KEYBOARD_SPEED_DEFAULT;
    Reactive.speed_max                  = MOUNTAIN60_KEYBOARD_SPEED_MAX;
    Reactive.color_mode                 = MODE_COLORS_MODE_SPECIFIC;
    Reactive.colors_min                 = 1;
    Reactive.colors_max                 = 2;
    Reactive.colors.resize(2);
    modes.push_back(Reactive);

    mode Matrix;
    Matrix.name                         = "Matrix";
    Matrix.value                        = MOUNTAIN60_KEYBOARD_MODE_MATRIX;
    Matrix.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Matrix.brightness_min               = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MIN;
    Matrix.brightness                   = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Matrix.brightness_max               = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Matrix.speed_min                    = MOUNTAIN60_KEYBOARD_SPEED_MIN;
    Matrix.speed                        = MOUNTAIN60_KEYBOARD_SPEED_DEFAULT;
    Matrix.speed_max                    = MOUNTAIN60_KEYBOARD_SPEED_MAX;
    Matrix.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Matrix.colors_min                   = 1;
    Matrix.colors_max                   = 2;
    Matrix.colors.resize(2);
    modes.push_back(Matrix);

    mode Yeti;
    Yeti.name                           = "Yeti";
    Yeti.value                          = MOUNTAIN60_KEYBOARD_MODE_YETI;
    Yeti.flags                          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Yeti.brightness_min                 = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MIN;
    Yeti.brightness                     = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Yeti.brightness_max                 = MOUNTAIN60_KEYBOARD_BRIGHTNESS_MAX;
    Yeti.speed_min                      = MOUNTAIN60_KEYBOARD_SPEED_MIN;
    Yeti.speed                          = MOUNTAIN60_KEYBOARD_SPEED_DEFAULT;
    Yeti.speed_max                      = MOUNTAIN60_KEYBOARD_SPEED_MAX;
    Yeti.color_mode                     = MODE_COLORS_MODE_SPECIFIC;
    Yeti.colors_min                     = 1;
    Yeti.colors_max                     = 2;
    Yeti.colors.resize(2);
    modes.push_back(Yeti);

    active_mode                         = 0;
    current_mode_value                  = -1;

    SetupZones();

    /*-----------------------------------------------------*\
    | The Mountain Everest 60 keyboard need to send a       |
    | specific packet frequently so that leds get updated   |
    \*-----------------------------------------------------*/
    mountain_thread                     = new std::thread(&RGBController_Mountain60Keyboard::UpdateMountain, this);
    mountain_thread_running             = true;
}

RGBController_Mountain60Keyboard::~RGBController_Mountain60Keyboard()
{
    mountain_thread_running             = false;
    mountain_thread->join();
    delete mountain_thread;

    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].type == ZONE_TYPE_MATRIX)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_Mountain60Keyboard::SetupZones()
{
    KeyboardLayoutManager new_kb(KEYBOARD_LAYOUT_ANSI_QWERTY, KEYBOARD_SIZE_SIXTY, mountain60_layout);
    new_kb.ChangeKeys(mountain60_keyboard_overlay_no_numpad);

    zone new_zone;
    matrix_map_type * new_map       = new matrix_map_type;

    new_zone.name                   = "Mountain Everest 60";
    new_zone.type                   = ZONE_TYPE_MATRIX;
    new_zone.matrix_map             = new_map;
    new_zone.matrix_map->height     = new_kb.GetRowCount();
    new_zone.matrix_map->width      = new_kb.GetColumnCount();
    new_zone.matrix_map->map        = new unsigned int[new_map->height * new_map->width];
    new_zone.leds_count             = new_kb.GetKeyCount();
    new_zone.leds_min               = new_zone.leds_count;
    new_zone.leds_max               = new_zone.leds_count;

    new_kb.GetKeyMap(new_map->map, KEYBOARD_MAP_FILL_TYPE_COUNT);

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

void RGBController_Mountain60Keyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Mountain60Keyboard::DeviceUpdateLEDs()
{
    unsigned char* color_data = new unsigned char[(leds.size()*4)];

    /*---------------------------------------------------------*\
    |   Filling the color_data vector with progressive index    |
    |   leaving space for RGB data                              |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        const unsigned int idx  = led_idx * 4;
        color_data[idx]         = leds[led_idx].value;
        color_data[idx + 1]     = RGBGetRValue(colors[led_idx]);
        color_data[idx + 2]     = RGBGetGValue(colors[led_idx]);
        color_data[idx + 3]     = RGBGetBValue(colors[led_idx]);
    }

    controller->SendDirect(modes[active_mode].brightness, color_data, (leds.size()*4));
    delete[] color_data;
}

void RGBController_Mountain60Keyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Mountain60Keyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Mountain60Keyboard::DeviceUpdateMode()
{
    if(modes[active_mode].value != current_mode_value)
    {
        current_mode_value = modes[active_mode].value;
        controller->SelectMode(modes[active_mode].value);
    }

    if(modes[active_mode].color_mode != MODE_FLAG_HAS_PER_LED_COLOR)
    {
        controller->SendModeDetails(&modes[active_mode]);
    }
}

void RGBController_Mountain60Keyboard::DeviceSaveMode()
{
    controller->SaveData(modes[active_mode].value);
}

void RGBController_Mountain60Keyboard::UpdateMountain()
{
    while(mountain_thread_running.load())
    {
        std::this_thread::sleep_for(MOUNTAIN60_KEEP_LIVE_PERIOD);

        controller->UpdateData();
    }
}
