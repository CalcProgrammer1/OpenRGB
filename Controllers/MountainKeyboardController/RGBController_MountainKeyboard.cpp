/*---------------------------------------------------------*\
| RGBController_MountainKeyboard.cpp                        |
|                                                           |
|   RGBController for Mountain keyboard                     |
|                                                           |
|   Wojciech Lazarski                              Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_MountainKeyboard.h"

static const unsigned char colorwave_speed_values   [MOUNTAIN_KEYBOARD_SPEED_MAX+1] = { 10,  9,  8,  7,  6};
static const unsigned char tornado_speed_values     [MOUNTAIN_KEYBOARD_SPEED_MAX+1] = { 10,  9,  8,  7,  6};
static const unsigned char breathing_speed_values   [MOUNTAIN_KEYBOARD_SPEED_MAX+1] = {  5,  4,  3,  1,  0};
static const unsigned char matrix_speed_values      [MOUNTAIN_KEYBOARD_SPEED_MAX+1] = { 20, 15, 10,  5,  0};
static const unsigned char reactive_speed_values    [MOUNTAIN_KEYBOARD_SPEED_MAX+1] = {  5,  4,  3,  1,  0};

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

#define KEYBOARD_MATRIX_TKL_HEIGHT            6
#define KEYBOARD_MATRIX_TKL_WIDTH            19
#define KEYBOARD_MATRIX_TKL_KEYS_NO          87

#define KEYBOARD_MATRIX_NUM_HEIGHT            6
#define KEYBOARD_MATRIX_NUM_WIDTH             4
#define KEYBOARD_MATRIX_NUM_KEYS_NO          17

#define KEYBOARD_MATRIX_EDGE_TKL_HEIGHT       6
#define KEYBOARD_MATRIX_EDGE_TKL_WIDTH       14
#define KEYBOARD_MATRIX_EDGE_TKL_KEYS_NO     32

#define KEYBOARD_MATRIX_EDGE_NUMPAD_HEIGHT    6
#define KEYBOARD_MATRIX_EDGE_NUMPAD_WIDTH     5
#define KEYBOARD_MATRIX_EDGE_NUMPAD_KEYS_NO  14

#define KEYBOARD_MATRIX_KEYS_NO              (KEYBOARD_MATRIX_TKL_KEYS_NO + KEYBOARD_MATRIX_NUM_KEYS_NO)
#define KEYBOARD_MATRIX_EDGE_KEYS_NO         (KEYBOARD_MATRIX_EDGE_TKL_KEYS_NO + KEYBOARD_MATRIX_EDGE_NUMPAD_KEYS_NO)

/*-------------------------------*\
| TODO: Detect detached keypad    |
\*-------------------------------*/
enum
{
    IDX_EDGE_00,
    IDX_EDGE_01,
    IDX_EDGE_02,
    IDX_EDGE_03,
    IDX_EDGE_04,
    IDX_EDGE_05,
    IDX_EDGE_06,
    IDX_EDGE_07,
    IDX_EDGE_08,
    IDX_EDGE_09,
    IDX_EDGE_10,
    IDX_EDGE_11,
    IDX_EDGE_12,
    IDX_EDGE_13,
    IDX_EDGE_14,
    IDX_EDGE_15,
    IDX_EDGE_16,
    IDX_EDGE_17,
    IDX_EDGE_18,
    IDX_EDGE_19,
    IDX_EDGE_20,
    IDX_EDGE_21,
    IDX_EDGE_22,
    IDX_EDGE_23,
    IDX_EDGE_24,
    IDX_EDGE_25,
    IDX_EDGE_26,
    IDX_EDGE_27,
    IDX_EDGE_28,
    IDX_EDGE_29,
    IDX_EDGE_30,
    IDX_EDGE_31,
    IDX_EDGE_32,
    IDX_EDGE_33,
    IDX_EDGE_34,
    IDX_EDGE_35,
    IDX_EDGE_36,
    IDX_EDGE_37,
    IDX_EDGE_38,
    IDX_EDGE_39,
    IDX_EDGE_40,
    IDX_EDGE_41,
    IDX_EDGE_42,
    IDX_EDGE_43,
    IDX_EDGE_44,
    IDX_EDGE_45
};

enum
{
    IDX_KEY_EN_ESCAPE       = 0,
    IDX_KEY_EN_BACK_TICK,
    IDX_KEY_EN_TAB,
    IDX_KEY_EN_CAPS_LOCK,
    IDX_KEY_EN_LEFT_SHIFT,
    IDX_KEY_EN_LEFT_CONTROL,
    IDX_KEY_EN_NUMPAD_LOCK,
    IDX_KEY_EN_NUMPAD_PLUS,
    IDX_KEY_EN_UNUSED_8,
    IDX_KEY_EN_F1,
    IDX_KEY_EN_1,
    IDX_KEY_EN_Q,
    IDX_KEY_EN_A,
    IDX_KEY_EN_UNUSED_13,
    IDX_KEY_EN_LEFT_WINDOWS,
    IDX_KEY_EN_NUMPAD_TIMES,
    IDX_KEY_EN_NUMPAD_MINUS,
    IDX_KEY_EN_UNUSED_17,
    IDX_KEY_EN_F2,
    IDX_KEY_EN_2,
    IDX_KEY_EN_W,
    IDX_KEY_EN_S,
    IDX_KEY_EN_Z,
    IDX_KEY_EN_LEFT_ALT,
    IDX_KEY_EN_NUMPAD_DIVIDE,
    IDX_KEY_EN_UNUSED_25,
    IDX_KEY_EN_UNUSED_26,
    IDX_KEY_EN_F3,
    IDX_KEY_EN_3,
    IDX_KEY_EN_E,
    IDX_KEY_EN_D,
    IDX_KEY_EN_X,
    IDX_KEY_EN_UNUSED_32,
    IDX_KEY_EN_NUMPAD_ENTER,
    IDX_KEY_EN_NUMPAD_1,
    IDX_KEY_EN_UNUSED_35,
    IDX_KEY_EN_F4,
    IDX_KEY_EN_4,
    IDX_KEY_EN_R,
    IDX_KEY_EN_F,
    IDX_KEY_EN_C,
    IDX_KEY_EN_SPACE,
    IDX_KEY_EN_NUMPAD_2,
    IDX_KEY_EN_NUMPAD_3,
    IDX_KEY_EN_UNUSED_44,
    IDX_KEY_EN_F5,
    IDX_KEY_EN_5,
    IDX_KEY_EN_T,
    IDX_KEY_EN_G,
    IDX_KEY_EN_V,
    IDX_KEY_EN_UNUSED_50,
    IDX_KEY_EN_NUMPAD_4,
    IDX_KEY_EN_NUMPAD_5,
    IDX_KEY_EN_UNUSED_53,
    IDX_KEY_EN_F6,
    IDX_KEY_EN_6,
    IDX_KEY_EN_Y,
    IDX_KEY_EN_H,
    IDX_KEY_EN_B,
    IDX_KEY_EN_UNUSED_59,
    IDX_KEY_EN_NUMPAD_6,
    IDX_KEY_EN_NUMPAD_7,
    IDX_KEY_EN_UNUSED_62,
    IDX_KEY_EN_F7,
    IDX_KEY_EN_7,
    IDX_KEY_EN_U,
    IDX_KEY_EN_J,
    IDX_KEY_EN_N,
    IDX_KEY_EN_RIGHT_ALT,
    IDX_KEY_EN_NUMPAD_8,
    IDX_KEY_EN_NUMPAD_9,
    IDX_KEY_EN_UNUSED_71,
    IDX_KEY_EN_F8,
    IDX_KEY_EN_8,
    IDX_KEY_EN_I,
    IDX_KEY_EN_K,
    IDX_KEY_EN_M,
    IDX_KEY_EN_RIGHT_WINDOWS,
    IDX_KEY_EN_NUMPAD_0,
    IDX_KEY_EN_NUMPAD_PERIOD,
    IDX_KEY_EN_UNUSED_80,
    IDX_KEY_EN_F9,
    IDX_KEY_EN_9,
    IDX_KEY_EN_O,
    IDX_KEY_EN_L,
    IDX_KEY_EN_COMMA,
    IDX_KEY_EN_RIGHT_FUNCTION,
    IDX_KEY_EN_BACKSPACE,
    IDX_KEY_EN_DELETE,
    IDX_KEY_EN_UNUSED_89,
    IDX_KEY_EN_F10,
    IDX_KEY_EN_0,
    IDX_KEY_EN_P,
    IDX_KEY_EN_SEMICOLON,
    IDX_KEY_EN_PERIOD,
    IDX_KEY_EN_RIGHT_CONTROL,
    IDX_KEY_EN_INSERT,
    IDX_KEY_EN_END,
    IDX_KEY_EN_UNUSED_98,
    IDX_KEY_EN_F11,
    IDX_KEY_EN_MINUS,
    IDX_KEY_EN_LEFT_BRACKET,
    IDX_KEY_EN_QUOTE,
    IDX_KEY_EN_FORWARD_SLASH,
    IDX_KEY_EN_LEFT_ARROW,
    IDX_KEY_EN_HOME,
    IDX_KEY_EN_PAGE_DOWN,
    IDX_KEY_EN_UNUSED_107,
    IDX_KEY_EN_F12,
    IDX_KEY_EN_EQUALS,
    IDX_KEY_EN_RIGHT_BRACKET,
    IDX_KEY_EN_UNUSED_111,
    IDX_KEY_EN_UNUSED_112,
    IDX_KEY_EN_DOWN_ARROW,
    IDX_KEY_EN_SCROLL_LOCK,
    IDX_KEY_EN_PAGE_UP,
    IDX_KEY_EN_UNUSED_116,
    IDX_KEY_EN_PRINT_SCREEN,
    IDX_KEY_EN_UNUSED_118,
    IDX_KEY_EN_ANSI_BACK_SLASH,
    IDX_KEY_EN_ANSI_ENTER,
    IDX_KEY_EN_RIGHT_SHIFT,
    IDX_KEY_EN_RIGHT_ARROW,
    IDX_KEY_EN_PAUSE_BREAK,
    IDX_KEY_EN_UP_ARROW,
};

enum
{
    IDX_WHEEL
};

static unsigned int matrix_tkl_map[KEYBOARD_MATRIX_TKL_HEIGHT][KEYBOARD_MATRIX_TKL_WIDTH] =
    { {   0,  NA,   8,  14,  19,  24,  NA,  30,  36,  40,  45,  NA,  53,  59,  65,  70,  74,  78,  83},
      {   1,   6,   9,  15,  20,  25,  29,  31,  37,  41,  46,  NA,  54,  60,  66,  NA,  75,  79,  84},
      {   2,  NA,  10,  16,  21,  26,  NA,  32,  38,  42,  47,  50,  55,  61,  67,  71,  76,  80,  85},
      {   3,  NA,  11,  17,  22,  27,  NA,  33,  39,  43,  48,  51,  56,  62,  NA,  72,  NA,  NA,  NA},
      {   4,  NA,  12,  18,  23,  28,  NA,  34,  NA,  44,  49,  52,  57,  63,  68,  NA,  NA,  81,  NA},
      {   5,   7,  13,  NA,  NA,  NA,  NA,  35,  NA,  NA,  NA,  NA,  58,  64,  69,  73,  77,  82,  86} };

static unsigned int matrix_num_map[KEYBOARD_MATRIX_NUM_HEIGHT][KEYBOARD_MATRIX_NUM_WIDTH] =
    { {  NA,  NA,  NA,  NA },
      {   0,   5,   9,  14 },
      {   1,   6,  10,  15 },
      {   2,   7,  11,  NA },
      {   3,   8,  12,  16 },
      {   4,  NA,  13,  NA } };

static unsigned int matrix_tkl_map_lut[KEYBOARD_MATRIX_TKL_KEYS_NO] =
{
    IDX_KEY_EN_ESCAPE, IDX_KEY_EN_BACK_TICK, IDX_KEY_EN_TAB, IDX_KEY_EN_CAPS_LOCK, IDX_KEY_EN_LEFT_SHIFT, IDX_KEY_EN_LEFT_CONTROL,
    IDX_KEY_EN_1, IDX_KEY_EN_LEFT_WINDOWS,
    IDX_KEY_EN_F1, IDX_KEY_EN_2, IDX_KEY_EN_Q, IDX_KEY_EN_A, IDX_KEY_EN_Z, IDX_KEY_EN_LEFT_ALT,
    IDX_KEY_EN_F2, IDX_KEY_EN_3, IDX_KEY_EN_W, IDX_KEY_EN_S, IDX_KEY_EN_X,
    IDX_KEY_EN_F3, IDX_KEY_EN_4, IDX_KEY_EN_E, IDX_KEY_EN_D, IDX_KEY_EN_C,
    IDX_KEY_EN_F4, IDX_KEY_EN_5, IDX_KEY_EN_R, IDX_KEY_EN_F, IDX_KEY_EN_V,
    IDX_KEY_EN_6,
    IDX_KEY_EN_F5, IDX_KEY_EN_7, IDX_KEY_EN_T, IDX_KEY_EN_G, IDX_KEY_EN_B, IDX_KEY_EN_SPACE,
    IDX_KEY_EN_F6, IDX_KEY_EN_8, IDX_KEY_EN_Y, IDX_KEY_EN_H,
    IDX_KEY_EN_F7, IDX_KEY_EN_9, IDX_KEY_EN_U, IDX_KEY_EN_J, IDX_KEY_EN_N,
    IDX_KEY_EN_F8, IDX_KEY_EN_0, IDX_KEY_EN_I, IDX_KEY_EN_K, IDX_KEY_EN_M,
    IDX_KEY_EN_O, IDX_KEY_EN_L, IDX_KEY_EN_COMMA,
    IDX_KEY_EN_F9, IDX_KEY_EN_MINUS, IDX_KEY_EN_P, IDX_KEY_EN_SEMICOLON, IDX_KEY_EN_PERIOD, IDX_KEY_EN_RIGHT_ALT,
    IDX_KEY_EN_F10, IDX_KEY_EN_EQUALS, IDX_KEY_EN_LEFT_BRACKET, IDX_KEY_EN_QUOTE, IDX_KEY_EN_FORWARD_SLASH, IDX_KEY_EN_RIGHT_WINDOWS,
    IDX_KEY_EN_F11, IDX_KEY_EN_BACKSPACE, IDX_KEY_EN_RIGHT_BRACKET, IDX_KEY_EN_RIGHT_SHIFT, IDX_KEY_EN_RIGHT_FUNCTION,
    IDX_KEY_EN_F12, IDX_KEY_EN_ANSI_BACK_SLASH, IDX_KEY_EN_ANSI_ENTER, IDX_KEY_EN_RIGHT_CONTROL,
    IDX_KEY_EN_PRINT_SCREEN, IDX_KEY_EN_INSERT, IDX_KEY_EN_DELETE, IDX_KEY_EN_LEFT_ARROW,
    IDX_KEY_EN_SCROLL_LOCK, IDX_KEY_EN_HOME, IDX_KEY_EN_END, IDX_KEY_EN_UP_ARROW, IDX_KEY_EN_DOWN_ARROW,
    IDX_KEY_EN_PAUSE_BREAK, IDX_KEY_EN_PAGE_UP, IDX_KEY_EN_PAGE_DOWN, IDX_KEY_EN_RIGHT_ARROW
};

static unsigned int matrix_num_map_lut[KEYBOARD_MATRIX_NUM_KEYS_NO] =
{
    IDX_KEY_EN_NUMPAD_LOCK, IDX_KEY_EN_NUMPAD_7, IDX_KEY_EN_NUMPAD_4, IDX_KEY_EN_NUMPAD_1, IDX_KEY_EN_NUMPAD_0,
    IDX_KEY_EN_NUMPAD_DIVIDE, IDX_KEY_EN_NUMPAD_8, IDX_KEY_EN_NUMPAD_5, IDX_KEY_EN_NUMPAD_2,
    IDX_KEY_EN_NUMPAD_TIMES, IDX_KEY_EN_NUMPAD_9, IDX_KEY_EN_NUMPAD_6, IDX_KEY_EN_NUMPAD_3, IDX_KEY_EN_NUMPAD_PERIOD,
    IDX_KEY_EN_NUMPAD_MINUS, IDX_KEY_EN_NUMPAD_PLUS, IDX_KEY_EN_NUMPAD_ENTER
};

static unsigned int matrix_edge_tkl_map_lut[KEYBOARD_MATRIX_EDGE_TKL_KEYS_NO] =
{
    IDX_EDGE_16, IDX_EDGE_17, IDX_EDGE_18, IDX_EDGE_19,
    IDX_EDGE_13, IDX_EDGE_20,
    IDX_EDGE_14, IDX_EDGE_21,
    IDX_EDGE_15, IDX_EDGE_22,
    IDX_EDGE_07, IDX_EDGE_23,
    IDX_EDGE_06, IDX_EDGE_24,
    IDX_EDGE_05, IDX_EDGE_25,
    IDX_EDGE_04, IDX_EDGE_26,
    IDX_EDGE_03, IDX_EDGE_27,
    IDX_EDGE_02, IDX_EDGE_28,
    IDX_EDGE_01, IDX_EDGE_29,
    IDX_EDGE_00, IDX_EDGE_30,
    IDX_EDGE_45, IDX_EDGE_12,
    IDX_EDGE_09, IDX_EDGE_08, IDX_EDGE_10, IDX_EDGE_11
};

static unsigned int matrix_edge_numpad_map_lut[KEYBOARD_MATRIX_EDGE_NUMPAD_KEYS_NO]=
{
    IDX_EDGE_31, IDX_EDGE_32, IDX_EDGE_33, IDX_EDGE_34,
    IDX_EDGE_44, IDX_EDGE_35,
    IDX_EDGE_43, IDX_EDGE_36,
    IDX_EDGE_42, IDX_EDGE_37,
    IDX_EDGE_41, IDX_EDGE_40, IDX_EDGE_39, IDX_EDGE_38
};

static unsigned int single_wheel_lut[1]=
{
    IDX_WHEEL
};

static unsigned int matrix_edge_tkl_map[KEYBOARD_MATRIX_EDGE_TKL_HEIGHT][KEYBOARD_MATRIX_EDGE_TKL_WIDTH] =
    { { NA,  4,  6,  8, 10, 12, 14, 16, 18, 20, 22, 24, 26, NA },
      {  0, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, 28 },
      {  1, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, 29 },
      {  2, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, 30 },
      {  3, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, 31 },
      { NA,  5,  7,  9, 11, 13, 15, 17, 19, 21, 23, 25, 27, NA } };

static unsigned int matrix_edge_numpad_map[KEYBOARD_MATRIX_EDGE_NUMPAD_HEIGHT][KEYBOARD_MATRIX_EDGE_NUMPAD_WIDTH] =
    { { NA,   4,  6,  8, NA },
      {  0,  NA, NA, NA, 10 },
      {  1,  NA, NA, NA, 11 },
      {  2,  NA, NA, NA, 12 },
      {  3,  NA, NA, NA, 13 },
      { NA,   5,  7,  9, NA } };

/*---------------------------------------------------*\
| TODO: Figure out how to read RGB state              |
| TODO: separatee display brughness from backlight    |
\*---------------------------------------------------*/

typedef struct
{
    const char*     name;
    zone_type       type;
    unsigned int*   ptr;
    unsigned int*   ptr_lut;
    unsigned int    size;
    unsigned int    height;
    unsigned int    width;
} mountain_zone_t;

static const mountain_zone_t zone_definitions[] =
{
    {
        ZONE_EN_KEYBOARD,
        ZONE_TYPE_MATRIX,
        (unsigned int*)&matrix_tkl_map,
        (unsigned int*)&matrix_tkl_map_lut,
        KEYBOARD_MATRIX_TKL_KEYS_NO,
        KEYBOARD_MATRIX_TKL_HEIGHT,
        KEYBOARD_MATRIX_TKL_WIDTH
    },
    {
        "Numpad",
        ZONE_TYPE_MATRIX,
        (unsigned int*)&matrix_num_map,
        (unsigned int*)&matrix_num_map_lut,
        KEYBOARD_MATRIX_NUM_KEYS_NO,
        KEYBOARD_MATRIX_NUM_HEIGHT,
        KEYBOARD_MATRIX_NUM_WIDTH
    },
    {
        "Keyboard Edge",
        ZONE_TYPE_MATRIX,
        (unsigned int*)&matrix_edge_tkl_map,
        (unsigned int*)&matrix_edge_tkl_map_lut,
        KEYBOARD_MATRIX_EDGE_TKL_KEYS_NO,
        KEYBOARD_MATRIX_EDGE_TKL_HEIGHT,
        KEYBOARD_MATRIX_EDGE_TKL_WIDTH
    },
    {
        "Numpad Edge",
        ZONE_TYPE_MATRIX,
        (unsigned int*)&matrix_edge_numpad_map,
        (unsigned int*)&matrix_edge_numpad_map_lut,
        KEYBOARD_MATRIX_EDGE_NUMPAD_KEYS_NO,
        KEYBOARD_MATRIX_EDGE_NUMPAD_HEIGHT,
        KEYBOARD_MATRIX_EDGE_NUMPAD_WIDTH
    },
    {
        "Wheel Selector",
        ZONE_TYPE_SINGLE,
        NULL,
        (unsigned int *) &single_wheel_lut,
        1,
        1,
        1
    }
};


static const char *led_names[MOUNTAIN_KEYBOARD_MAX_TRANSFER_COLORS] =
{
    KEY_EN_ESCAPE,
    KEY_EN_BACK_TICK,
    KEY_EN_TAB,
    KEY_EN_CAPS_LOCK,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_UNUSED,/*8*/
    KEY_EN_F1,
    KEY_EN_1, /*10*/
    KEY_EN_Q,
    KEY_EN_A,
    KEY_EN_UNUSED,/*13*/
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_UNUSED,/*17*/
    KEY_EN_F2,
    KEY_EN_2,
    KEY_EN_W,
    KEY_EN_S,
    KEY_EN_Z,
    KEY_EN_LEFT_ALT,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_UNUSED,/*25*/
    KEY_EN_UNUSED,/*26*/
    KEY_EN_F3,
    KEY_EN_3,
    KEY_EN_E,
    KEY_EN_D,
    KEY_EN_X,
    KEY_EN_UNUSED,/*32*/
    KEY_EN_NUMPAD_ENTER,
    KEY_EN_NUMPAD_1,
    KEY_EN_UNUSED,/*35*/
    KEY_EN_F4,
    KEY_EN_4,
    KEY_EN_R,
    KEY_EN_F,
    KEY_EN_C,
    KEY_EN_SPACE,
    KEY_EN_NUMPAD_2,
    KEY_EN_NUMPAD_3,
    KEY_EN_UNUSED, /*44*/
    KEY_EN_F5,
    KEY_EN_5,
    KEY_EN_T,
    KEY_EN_G,
    KEY_EN_V,
    KEY_EN_UNUSED, /*50*/
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_5,
    KEY_EN_UNUSED, /*53*/
    KEY_EN_F6,
    KEY_EN_6,
    KEY_EN_Y,
    KEY_EN_H,
    KEY_EN_B,
    KEY_EN_UNUSED, /*59*/
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_7,
    KEY_EN_UNUSED, /*62*/
    KEY_EN_F7,
    KEY_EN_7,
    KEY_EN_U,
    KEY_EN_J,
    KEY_EN_N,
    KEY_EN_RIGHT_ALT,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_9,
    KEY_EN_UNUSED, /*71*/
    KEY_EN_F8,
    KEY_EN_8,
    KEY_EN_I,
    KEY_EN_K,
    KEY_EN_M,
    KEY_EN_RIGHT_WINDOWS,
    KEY_EN_NUMPAD_0,
    KEY_EN_NUMPAD_PERIOD,
    KEY_EN_UNUSED, /*80*/
    KEY_EN_F9,
    KEY_EN_9,
    KEY_EN_O,
    KEY_EN_L,
    KEY_EN_COMMA,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_BACKSPACE,
    KEY_EN_DELETE,
    KEY_EN_UNUSED,/*89*/
    KEY_EN_F10,
    KEY_EN_0,
    KEY_EN_P,
    KEY_EN_SEMICOLON,
    KEY_EN_PERIOD,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_INSERT,
    KEY_EN_END,
    KEY_EN_UNUSED,/*98*/
    KEY_EN_F11,
    KEY_EN_MINUS,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_QUOTE,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_LEFT_ARROW,
    KEY_EN_HOME,
    KEY_EN_PAGE_DOWN,
    KEY_EN_UNUSED,/*107*/
    KEY_EN_F12,
    KEY_EN_EQUALS,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_UNUSED,/*111*/
    KEY_EN_UNUSED,/*112*/
    KEY_EN_DOWN_ARROW,
    KEY_EN_SCROLL_LOCK,
    KEY_EN_PAGE_UP,
    KEY_EN_UNUSED,/*116*/
    KEY_EN_PRINT_SCREEN,
    KEY_EN_UNUSED,/*118*/
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_ANSI_ENTER,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_RIGHT_ARROW,
    KEY_EN_PAUSE_BREAK,
    KEY_EN_UP_ARROW,
};

/**------------------------------------------------------------------*\
    @name Mountain Keyboard
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :rotating_light:
    @effects :white_check_mark:
    @detectors DetectMountainKeyboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_MountainKeyboard::RGBController_MountainKeyboard(MountainKeyboardController* controller_ptr)
{
    controller                          = controller_ptr;

    memset(wheel_color, 0, 3);

    wheel_config * wheel_conf           = controller->GetWheelConfig();

    if(wheel_conf != nullptr)
    {
        wheel_color[0]                  = wheel_conf->r;
        wheel_color[1]                  = wheel_conf->g;
        wheel_color[2]                  = wheel_conf->b;
    }

    name                                = controller->GetNameString();
    vendor                              = "Mountain";
    type                                = DEVICE_TYPE_KEYBOARD;
    description                         = "Mountain Everest Keyboard";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Direct;
    Direct.name                         = "Direct";
    Direct.value                        = MOUNTAIN_KEYBOARD_MODE_DIRECT;
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Custom;
    Custom.name                         = "Custom";
    Custom.value                        = MOUNTAIN_KEYBOARD_MODE_DIRECT;
    Custom.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Custom.brightness_min               = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    Custom.brightness                   = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Custom.brightness_max               = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Custom.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode Off;
    Off.name                            = "Off";
    Off.value                           = MOUNTAIN_KEYBOARD_MODE_OFF;
    Off.flags                           = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                         = "Static";
    Static.value                        = MOUNTAIN_KEYBOARD_MODE_STATIC;
    Static.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.brightness_min               = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    Static.brightness                   = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Static.brightness_max               = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Static.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min                   = 1;
    Static.colors_max                   = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode ColorWaveRainbow;
    ColorWaveRainbow.name               = "Rainbow Wave";
    ColorWaveRainbow.value              = MOUNTAIN_KEYBOARD_MODE_COLOR_WAVE;
    ColorWaveRainbow.flags              = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_MANUAL_SAVE;
    ColorWaveRainbow.brightness_min     = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    ColorWaveRainbow.brightness         = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    ColorWaveRainbow.brightness_max     = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    ColorWaveRainbow.speed_min          = MOUNTAIN_KEYBOARD_SPEED_MIN;
    ColorWaveRainbow.speed              = MOUNTAIN_KEYBOARD_SPEED_DEFAULT;
    ColorWaveRainbow.speed_max          = MOUNTAIN_KEYBOARD_SPEED_MAX;
    ColorWaveRainbow.color_mode         = MODE_COLORS_RANDOM;
    modes.push_back(ColorWaveRainbow);

    mode ColorWave;
    ColorWave.name                      = "ColorWave";
    ColorWave.value                     = MOUNTAIN_KEYBOARD_MODE_COLOR_WAVE;
    ColorWave.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_MANUAL_SAVE;
    ColorWave.brightness_min            = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    ColorWave.brightness                = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    ColorWave.brightness_max            = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    ColorWave.speed_min                 = MOUNTAIN_KEYBOARD_SPEED_MIN;
    ColorWave.speed                     = MOUNTAIN_KEYBOARD_SPEED_DEFAULT;
    ColorWave.speed_max                 = MOUNTAIN_KEYBOARD_SPEED_MAX;
    ColorWave.colors_min                = 1;
    ColorWave.colors_max                = 2;
    ColorWave.colors.resize(2);
    ColorWave.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(ColorWave);

    mode Tornado;
    Tornado.name                        = "Tornado";
    Tornado.value                       = MOUNTAIN_KEYBOARD_MODE_TORNADO;
    Tornado.flags                       = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_MANUAL_SAVE;
    Tornado.brightness_min              = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    Tornado.brightness                  = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Tornado.brightness_max              = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Tornado.speed_min                   = MOUNTAIN_KEYBOARD_SPEED_MIN;
    Tornado.speed                       = MOUNTAIN_KEYBOARD_SPEED_DEFAULT;
    Tornado.speed_max                   = MOUNTAIN_KEYBOARD_SPEED_MAX;
    Tornado.colors_min                  = 1;
    Tornado.colors_max                  = 1;
    Tornado.colors.resize(1);
    Tornado.color_mode                  = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Tornado);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = MOUNTAIN_KEYBOARD_MODE_BREATHING;
    Breathing.flags                     = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Breathing.brightness_min            = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    Breathing.brightness                = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Breathing.brightness_max            = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Breathing.speed_min                 = MOUNTAIN_KEYBOARD_SPEED_MIN;
    Breathing.speed                     = MOUNTAIN_KEYBOARD_SPEED_DEFAULT;
    Breathing.speed_max                 = MOUNTAIN_KEYBOARD_SPEED_MAX;
    Breathing.colors_min                = 1;
    Breathing.colors_max                = 2;
    Breathing.colors.resize(2);
    Breathing.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Breathing);

    mode Reactive;
    Reactive.name                       = "Reactive";
    Reactive.value                      = MOUNTAIN_KEYBOARD_MODE_REACTIVE;
    Reactive.flags                      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Reactive.brightness_min             = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    Reactive.brightness                 = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Reactive.brightness_max             = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Reactive.speed_min                  = MOUNTAIN_KEYBOARD_SPEED_MIN;
    Reactive.speed                      = MOUNTAIN_KEYBOARD_SPEED_DEFAULT;
    Reactive.speed_max                  = MOUNTAIN_KEYBOARD_SPEED_MAX;
    Reactive.color_mode                 = MODE_COLORS_MODE_SPECIFIC;
    Reactive.colors_min                 = 1;
    Reactive.colors_max                 = 2;
    Reactive.colors.resize(2);
    modes.push_back(Reactive);

    mode Matrix;
    Matrix.name                         = "Matrix";
    Matrix.value                        = MOUNTAIN_KEYBOARD_MODE_MATRIX;
    Matrix.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Matrix.brightness_min               = MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN;
    Matrix.brightness                   = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Matrix.brightness_max               = MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX;
    Matrix.speed_min                    = MOUNTAIN_KEYBOARD_SPEED_MIN;
    Matrix.speed                        = MOUNTAIN_KEYBOARD_SPEED_DEFAULT;
    Matrix.speed_max                    = MOUNTAIN_KEYBOARD_SPEED_MAX;
    Matrix.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Matrix.colors_min                   = 1;
    Matrix.colors_max                   = 2;
    Matrix.colors.resize(2);
    modes.push_back(Matrix);

    SetupZones();
}

RGBController_MountainKeyboard::~RGBController_MountainKeyboard()
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

void RGBController_MountainKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < sizeof(zone_definitions)/sizeof(zone_definitions[0]); zone_idx++)
    {
        zone new_zone;

        new_zone.name                   = zone_definitions[zone_idx].name;
        new_zone.type                   = zone_definitions[zone_idx].type;
        new_zone.leds_min               = zone_definitions[zone_idx].size;
        new_zone.leds_max               = zone_definitions[zone_idx].size;
        new_zone.leds_count             = zone_definitions[zone_idx].size;
        new_zone.matrix_map             = NULL;
        if (zone_definitions[zone_idx].type == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map             = new matrix_map_type;
            new_zone.matrix_map->height     = zone_definitions[zone_idx].height;
            new_zone.matrix_map->width      = zone_definitions[zone_idx].width;
            new_zone.matrix_map->map        = zone_definitions[zone_idx].ptr;
        }
        zones.push_back(new_zone);
    }

    for(unsigned int zone_idx = 0; zone_idx < sizeof(zone_definitions)/sizeof(zone_definitions[0]); zone_idx++)
    {
        if (zone_definitions[zone_idx].ptr_lut)
        {
            for(unsigned int led_idx=0;led_idx<zones[zone_idx].leds_count;led_idx++)
            {
                led* new_led = new led();
                switch(zone_idx)
                {
                    case 0:
                    case 1:
                    {
                        new_led->name = led_names[zone_definitions[zone_idx].ptr_lut[led_idx]];
                    }
                    break;

                    case 2:
                    case 3:
                    {
                        new_led->name = zones[zone_idx].name + " LED:";
                        new_led->name.append(std::to_string(led_idx + 1));
                    }
                    break;

                    case 4:
                    {
                        new_led->name = zones[zone_idx].name + " LED";
                    }
                    break;
                    default:
                    break;
                }
                leds.push_back(*new_led);
            }
        }
    }
    SetupColors();

    DeviceUpdateMode();
}

void RGBController_MountainKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_MountainKeyboard::DeviceUpdate(const mode& current_mode)
{
    switch(current_mode.value)
    {
        case MOUNTAIN_KEYBOARD_MODE_DIRECT:
        {
            unsigned char color_data[MOUNTAIN_KEYBOARD_TRANSFER_BUFFER_SIZE] = {0};
            unsigned char color_edge_data[MOUNTAIN_KEYBOARD_TRANSFER_EDGE_BUFFER_SIZE] = {0};

            for(unsigned int led_idx = 0; led_idx < colors.size(); led_idx++)
            {
                if(led_idx < zones[0].leds_count)
                {
                    unsigned int zone_led_idx = led_idx;
                    unsigned int idx = zone_definitions[0].ptr_lut[zone_led_idx];
                    color_data[(3 * idx)]   = RGBGetRValue(colors[led_idx]);
                    color_data[(3 * idx)+1] = RGBGetGValue(colors[led_idx]);
                    color_data[(3 * idx)+2] = RGBGetBValue(colors[led_idx]);
                }
                else if (led_idx < zones[0].leds_count + zones[1].leds_count)
                {
                    unsigned int zone_led_idx = led_idx - zones[0].leds_count;
                    unsigned int idx = zone_definitions[1].ptr_lut[zone_led_idx];
                    color_data[(3 * idx)]   = RGBGetRValue(colors[led_idx]);
                    color_data[(3 * idx)+1] = RGBGetGValue(colors[led_idx]);
                    color_data[(3 * idx)+2] = RGBGetBValue(colors[led_idx]);

                }
                else if (led_idx < zones[0].leds_count + zones[1].leds_count + zones[2].leds_count)
                {
                    unsigned int zone_led_idx = led_idx - zones[0].leds_count - zones[1].leds_count;
                    unsigned int idx = zone_definitions[2].ptr_lut[zone_led_idx];
                    color_edge_data[(3 * idx)]   = RGBGetRValue(colors[led_idx]);
                    color_edge_data[(3 * idx)+1] = RGBGetGValue(colors[led_idx]);
                    color_edge_data[(3 * idx)+2] = RGBGetBValue(colors[led_idx]);
                }
                else if (led_idx < zones[0].leds_count + zones[1].leds_count + zones[2].leds_count + zones[3].leds_count)
                {
                    unsigned int zone_led_idx = led_idx - zones[0].leds_count - zones[1].leds_count - zones[2].leds_count;
                    unsigned int idx = zone_definitions[3].ptr_lut[zone_led_idx];
                    color_edge_data[(3 * idx)]   = RGBGetRValue(colors[led_idx]);
                    color_edge_data[(3 * idx)+1] = RGBGetGValue(colors[led_idx]);
                    color_edge_data[(3 * idx)+2] = RGBGetBValue(colors[led_idx]);
                }
                else
                {
                    wheel_color[0] = RGBGetRValue(colors[led_idx]);
                    wheel_color[1] = RGBGetGValue(colors[led_idx]);
                    wheel_color[2] = RGBGetBValue(colors[led_idx]);
                }


            }

            /*---------------------------------------------------------*\
            | Check if we running pseud DIRECT mode or CUSTOM one       |
            \*---------------------------------------------------------*/
            if(current_mode.flags & MODE_FLAG_MANUAL_SAVE)
            {
                controller->SendDirectColorCmd(false, current_mode.brightness, color_data,MOUNTAIN_KEYBOARD_TRANSFER_BUFFER_SIZE);
                controller->SendDirectColorEdgeCmd(false, current_mode.brightness, color_edge_data,MOUNTAIN_KEYBOARD_TRANSFER_EDGE_BUFFER_SIZE);
            }
            else
            {
                controller->SendDirectColorCmd(true, current_mode.brightness, color_data,MOUNTAIN_KEYBOARD_TRANSFER_BUFFER_SIZE);
                controller->SendDirectColorEdgeCmd(true, current_mode.brightness, color_edge_data,MOUNTAIN_KEYBOARD_TRANSFER_EDGE_BUFFER_SIZE);
            }
        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_OFF:
        {
            controller->SendOffCmd();
        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_STATIC:
        {
            color_setup setup;

            setup.brightness        = current_mode.brightness;
            setup.mode.one_color.r  = RGBGetRValue(current_mode.colors[0]);
            setup.mode.one_color.g  = RGBGetGValue(current_mode.colors[0]);
            setup.mode.one_color.b  = RGBGetBValue(current_mode.colors[0]);

            unsigned char colors [3] = {setup.mode.one_color.r, setup.mode.one_color.g, setup.mode.one_color.b};
            controller->SendColorStaticCmd(setup);
            wheel_color[0] = colors[0];
            wheel_color[1] = colors[1];
            wheel_color[2] = colors[2];

        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_COLOR_WAVE:
        {
            color_setup setup;

            setup.speed         = colorwave_speed_values[current_mode.speed];
            setup.direction     = ConvertDirection(current_mode.direction,false);
            setup.brightness    = current_mode.brightness;

            if(current_mode.color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                if(current_mode.colors.size() == 2)
                {
                    setup.color_mode            = MOUNTAIN_KEYBOARD_COLOR_MODE_DUAL;
                    setup.mode.two_colors.r1    = RGBGetRValue(current_mode.colors[0]);
                    setup.mode.two_colors.g1    = RGBGetGValue(current_mode.colors[0]);
                    setup.mode.two_colors.b1    = RGBGetBValue(current_mode.colors[0]);
                    setup.mode.two_colors.r2    = RGBGetRValue(current_mode.colors[1]);
                    setup.mode.two_colors.g2    = RGBGetGValue(current_mode.colors[1]);
                    setup.mode.two_colors.b2    = RGBGetBValue(current_mode.colors[1]);
                }
                else
                {
                    if(current_mode.colors.size() == 1)
                    {
                        setup.color_mode        = MOUNTAIN_KEYBOARD_COLOR_MODE_SINGLE;
                        setup.mode.one_color.r  = RGBGetRValue(current_mode.colors[0]);
                        setup.mode.one_color.g  = RGBGetGValue(current_mode.colors[0]);
                        setup.mode.one_color.b  = RGBGetBValue(current_mode.colors[0]);
                    }
                }
            }
            else
            {
                setup.color_mode    = MOUNTAIN_KEYBOARD_COLOR_MODE_RAINBOW;
            }

            controller->SendColorWaveCmd(setup);
        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_TORNADO:
        {
            color_setup setup;

            setup.speed      = tornado_speed_values[current_mode.speed];
            setup.direction  = ConvertDirection(current_mode.direction,true);
            setup.brightness = current_mode.brightness;

            if(current_mode.color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                setup.color_mode        = MOUNTAIN_KEYBOARD_COLOR_MODE_SINGLE;
                setup.mode.one_color.r  = RGBGetRValue(current_mode.colors[0]);
                setup.mode.one_color.g  = RGBGetGValue(current_mode.colors[0]);
                setup.mode.one_color.b  = RGBGetBValue(current_mode.colors[0]);
            }
            else
            {
                setup.color_mode    = MOUNTAIN_KEYBOARD_COLOR_MODE_RAINBOW;
            }

            controller->SendColorTornadoCmd(setup);
        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_BREATHING:
        {
            color_setup setup;

            setup.speed         = breathing_speed_values[current_mode.speed];
            setup.brightness    = current_mode.brightness;

            if(current_mode.color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                if(current_mode.colors.size() == 2)
                {
                    setup.color_mode            = MOUNTAIN_KEYBOARD_COLOR_MODE_DUAL;
                    setup.mode.two_colors.r1    = RGBGetRValue(current_mode.colors[0]);
                    setup.mode.two_colors.g1    = RGBGetGValue(current_mode.colors[0]);
                    setup.mode.two_colors.b1    = RGBGetBValue(current_mode.colors[0]);
                    setup.mode.two_colors.r2    = RGBGetRValue(current_mode.colors[1]);
                    setup.mode.two_colors.g2    = RGBGetGValue(current_mode.colors[1]);
                    setup.mode.two_colors.b2    = RGBGetBValue(current_mode.colors[1]);
                }
                else
                {
                    if(current_mode.colors.size() == 1)
                    {
                        setup.color_mode        = MOUNTAIN_KEYBOARD_COLOR_MODE_SINGLE;
                        setup.mode.one_color.r  = RGBGetRValue(current_mode.colors[0]);
                        setup.mode.one_color.g  = RGBGetGValue(current_mode.colors[0]);
                        setup.mode.one_color.b  = RGBGetBValue(current_mode.colors[0]);
                    }
                }
            }
            else
            {
                setup.color_mode        = MOUNTAIN_KEYBOARD_COLOR_MODE_RAINBOW;
            }

            controller->SendColorBreathingCmd(setup);
        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_MATRIX:
        {
            color_setup setup;

            setup.speed       = matrix_speed_values[current_mode.speed];
            setup.brightness  = current_mode.brightness;
            setup.mode.two_colors.r1    = RGBGetRValue(current_mode.colors[0]);
            setup.mode.two_colors.g1    = RGBGetGValue(current_mode.colors[0]);
            setup.mode.two_colors.b1    = RGBGetBValue(current_mode.colors[0]);
            setup.mode.two_colors.r2    = RGBGetRValue(current_mode.colors[1]);
            setup.mode.two_colors.g2    = RGBGetGValue(current_mode.colors[1]);
            setup.mode.two_colors.b2    = RGBGetBValue(current_mode.colors[1]);

            controller->SendColorMatrixCmd(setup);
        }
        break;

        case MOUNTAIN_KEYBOARD_MODE_REACTIVE:
        {
            color_setup setup;

            setup.speed         = reactive_speed_values[current_mode.speed];
            setup.brightness    = current_mode.brightness;
            setup.mode.two_colors.r1    = RGBGetRValue(current_mode.colors[0]);
            setup.mode.two_colors.g1    = RGBGetGValue(current_mode.colors[0]);
            setup.mode.two_colors.b1    = RGBGetBValue(current_mode.colors[0]);
            setup.mode.two_colors.r2    = RGBGetRValue(current_mode.colors[1]);
            setup.mode.two_colors.g2    = RGBGetGValue(current_mode.colors[1]);
            setup.mode.two_colors.b2    = RGBGetBValue(current_mode.colors[1]);

            controller->SendColorReactiveCmd(setup);
        }
        break;

        default:
        break;
    }
}


void RGBController_MountainKeyboard::DeviceUpdateLEDs()
{
    mode current_mode = modes[active_mode];
    DeviceUpdate(current_mode);
}



void RGBController_MountainKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MountainKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MountainKeyboard::DeviceUpdateMode()
{
    static int prv_mode =-1;

    mode current_mode = modes[active_mode];
    if(prv_mode != current_mode.value)
    {
        controller->SelectMode(current_mode.value);
        prv_mode = current_mode.value;
    }

    DeviceUpdate(current_mode);
}

unsigned char RGBController_MountainKeyboard::ConvertDirection(unsigned int direction, bool rotation)
{
    unsigned char ret;
    switch(direction)
    {
        case MODE_DIRECTION_LEFT:
        {
            ret = rotation?MOUNTAIN_KEYBOARD_DIRECTION_ANTICLK:MOUNTAIN_KEYBOARD_DIRECTION_LEFT;
        }
        break;

        case MODE_DIRECTION_RIGHT:
        {
            ret = rotation?MOUNTAIN_KEYBOARD_DIRECTION_CLK:MOUNTAIN_KEYBOARD_DIRECTION_RIGHT;
        }
        break;

        case MODE_DIRECTION_UP:
        {
            ret = MOUNTAIN_KEYBOARD_DIRECTION_UP;
        }
        break;

        case MODE_DIRECTION_DOWN:
        {
            ret = MOUNTAIN_KEYBOARD_DIRECTION_DOWN;
        }
        break;

        default:
        {
            ret = MOUNTAIN_KEYBOARD_DIRECTION_LEFT;
        }
        break;
    }
    return ret;
}

void RGBController_MountainKeyboard::DeviceSaveMode()
{
    controller->SaveData(modes[active_mode].value);
    controller->SendWheelColorChange(wheel_color);
}
