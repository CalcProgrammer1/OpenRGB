/*---------------------------------------------------------*\
| RGBController_CorsairK55RGBPROXT.cpp                      |
|                                                           |
|   RGBController for Corsair K55 RGB PRO XT keyboard       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_CorsairK55RGBPROXT.h"
#include "RGBControllerKeyNames.h"
#include "LogManager.h"

using namespace std::chrono_literals;

#define NA  0xFFFFFFFF
#define WIDTH   24
#define HEIGHT  6

static unsigned int matrix_map[HEIGHT][WIDTH] =
    { { 0,   6,  NA,  15,  21,  26,  31,  NA,  37,  43,  47,  52,  NA,  60,  66,  72,  78,  81,  85,  90,  NA,  NA,  NA,  NA },
      { 1,   7,  12,  16,  22,  27,  32,  36,  38,  44,  48,  53,  NA,  61,  67,  73,  NA,  82,  86,  91,  94,  99, 103, 108 },
      { 2,   8,  NA,  17,  23,  28,  33,  NA,  39,  45,  49,  54,  57,  62,  68,  74,  79,  83,  87,  92,  95, 100, 104, 109 },
      { 3,   9,  NA,  18,  24,  29,  34,  NA,  40,  46,  50,  55,  58,  63,  69,  75,  NA,  NA,  NA,  NA,  96, 101, 105,  NA },
      { 4,  10,  13,  19,  25,  30,  35,  NA,  41,  NA,  51,  56,  59,  64,  70,  76,  NA,  NA,  88,  NA,  97, 102, 106, 110 },
      { 5,  11,  14,  20,  NA,  NA,  NA,  NA,  42,  NA,  NA,  NA,  NA,  65,  71,  77,  80,  84,  89,  93,  98,  NA, 107,  NA } };

std::vector<std::string> key_names =
{
    // col 0
    "Key: G1",
    "Key: G2",
    "Key: G3",
    "Key: G4",
    "Key: G5",
    "Key: G6",

    // col 1
    KEY_EN_ESCAPE,
    KEY_EN_BACK_TICK,
    KEY_EN_TAB,
    KEY_EN_CAPS_LOCK,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_LEFT_CONTROL,

    // col 2
    KEY_EN_1,
    KEY_EN_ISO_BACK_SLASH,
    KEY_EN_LEFT_WINDOWS,

    // col 3
    KEY_EN_F1,
    KEY_EN_2,
    KEY_EN_Q,
    KEY_EN_A,
    KEY_EN_Z,
    KEY_EN_LEFT_ALT,

    // col 4
    KEY_EN_F2,
    KEY_EN_3,
    KEY_EN_W,
    KEY_EN_S,
    KEY_EN_X,

    // col 5
    KEY_EN_F3,
    KEY_EN_4,
    KEY_EN_E,
    KEY_EN_D,
    KEY_EN_C,

    // col 6
    KEY_EN_F4,
    KEY_EN_5,
    KEY_EN_R,
    KEY_EN_F,
    KEY_EN_V,

    // col 7
    KEY_EN_6,

    // col 8
    KEY_EN_F5,
    KEY_EN_7,
    KEY_EN_T,
    KEY_EN_G,
    KEY_EN_B,
    KEY_EN_SPACE,

    // col 9
    KEY_EN_F6,
    KEY_EN_8,
    KEY_EN_Y,
    KEY_EN_H,

    // col 10
    KEY_EN_F7,
    KEY_EN_9,
    KEY_EN_U,
    KEY_EN_J,
    KEY_EN_N,

    // col 11
    KEY_EN_F8,
    KEY_EN_0,
    KEY_EN_I,
    KEY_EN_K,
    KEY_EN_M,

    // col 12
    KEY_EN_O,
    KEY_EN_L,
    KEY_EN_COMMA,

    // col 13
    KEY_EN_F9,
    KEY_EN_MINUS,
    KEY_EN_P,
    KEY_EN_SEMICOLON,
    KEY_EN_PERIOD,
    KEY_EN_RIGHT_ALT,

    // col 14
    KEY_EN_F10,
    KEY_EN_EQUALS,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_QUOTE,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_RIGHT_FUNCTION,

    // col 15
    KEY_EN_F11,
    KEY_EN_BACKSPACE,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_POUND,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_MENU,

    // col 16
    KEY_EN_F12,
    KEY_EN_ANSI_ENTER,
    KEY_EN_RIGHT_CONTROL,

    // col 17
    KEY_EN_PRINT_SCREEN,
    KEY_EN_INSERT,
    KEY_EN_DELETE,
    KEY_EN_LEFT_ARROW,

    // col 18
    KEY_EN_SCROLL_LOCK,
    KEY_EN_HOME,
    KEY_EN_END,
    KEY_EN_UP_ARROW,
    KEY_EN_DOWN_ARROW,

    // col 19
    KEY_EN_PAUSE_BREAK,
    KEY_EN_PAGE_UP,
    KEY_EN_PAGE_DOWN,
    KEY_EN_RIGHT_ARROW,

    // col 20
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_1,
    KEY_EN_NUMPAD_0,

    // col 21
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_2,

    // col 22
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_PERIOD,

    // col 23
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_NUMPAD_ENTER
};


/**------------------------------------------------------------------*\
    @name Corsair K55 RGB Pro XT
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCorsairK55RGBPROXTControllers
    @comment
\*-------------------------------------------------------------------*/


RGBController_CorsairK55RGBPROXT::RGBController_CorsairK55RGBPROXT(CorsairK55RGBPROXTController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "Corsair";
    description             = "Corsair K55 RGB PRO XT Keyboard Device";
    type                    = DEVICE_TYPE_KEYBOARD;
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = CorsairK55RGBPROXTController::CORSAIR_MODE_DIRECT_VALUE;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name             = "Static";
    Static.value            = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_STATIC_VALUE;
    Static.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode ColorPulse;
    ColorPulse.name         = "ColorPulse";
    ColorPulse.value        = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_COLOR_PULSE_VALUE;
    ColorPulse.flags        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    ColorPulse.color_mode   = MODE_COLORS_RANDOM;
    ColorPulse.speed        = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MED;
    ColorPulse.speed_min    = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MIN;
    ColorPulse.speed_max    = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MAX;
    ColorPulse.colors.resize(2);
    modes.push_back(ColorPulse);

    mode ColorShift;
    ColorShift.name         = "ColorShift";
    ColorShift.value        = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_COLOR_SHIFT_VALUE;
    ColorShift.flags        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    ColorShift.color_mode   = MODE_COLORS_RANDOM;
    ColorShift.speed        = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MED;
    ColorShift.speed_min    = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MIN;
    ColorShift.speed_max    = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MAX;
    ColorShift.colors.resize(2);
    modes.push_back(ColorShift);

    mode ColorWave;
    ColorWave.name          = "ColorWave";
    ColorWave.value         = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_COLOR_WAVE_VALUE;
    ColorWave.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    ColorWave.color_mode    = MODE_COLORS_RANDOM;
    ColorWave.direction     = MODE_DIRECTION_RIGHT;
    ColorWave.speed         = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MED;
    ColorWave.speed_min     = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MIN;
    ColorWave.speed_max     = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MAX;
    ColorWave.colors.resize(2);
    modes.push_back(ColorWave);

    mode RainbowWave;
    RainbowWave.name        = "RainbowWave";
    RainbowWave.value       = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_RAINBOW_WAVE_VALUE;
    RainbowWave.flags       = MODE_FLAG_HAS_SPEED |  MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    RainbowWave.color_mode  = MODE_COLORS_NONE;
    RainbowWave.direction   = MODE_DIRECTION_RIGHT;
    RainbowWave.speed       = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MED;
    RainbowWave.speed_min   = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MIN;
    RainbowWave.speed_max   = CorsairK55RGBPROXTController:: CORSAIR_HW_MODE_SPEED_MAX;
    modes.push_back(RainbowWave);

    mode Rain;
    Rain.name               = "Rain";
    Rain.value              = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_RAIN_VALUE;
    Rain.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Rain.color_mode         = MODE_COLORS_RANDOM;
    Rain.direction          = MODE_DIRECTION_DOWN;
    Rain.speed              = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MED;
    Rain.speed_min          = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MIN;
    Rain.speed_max          = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MAX;
    Rain.colors.resize(2);
    modes.push_back(Rain);

    mode Spiral;
    Spiral.name             = "Spiral";
    Spiral.value            = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPIRAL_VALUE;
    Spiral.flags            = MODE_FLAG_HAS_SPEED |  MODE_FLAG_HAS_DIRECTION_LR;
    Spiral.color_mode       = MODE_COLORS_NONE;
    Spiral.direction        = MODE_DIRECTION_RIGHT;
    Spiral.speed            = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MED;
    Spiral.speed_min        = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MIN;
    Spiral.speed_max        = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MAX;
    modes.push_back(Spiral);

    mode WaterColor;
    WaterColor.name         = "WaterColor";
    WaterColor.value        = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_WATER_COLOR_VALUE;
    WaterColor.flags        = MODE_FLAG_HAS_SPEED;
    WaterColor.color_mode   = MODE_COLORS_NONE;
    WaterColor.speed        = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MED;
    WaterColor.speed_min    = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MIN;
    WaterColor.speed_max    = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MAX;
    WaterColor.colors.resize(1);
    WaterColor.colors[0]    = 0x00FFFFFF;
    modes.push_back(WaterColor);

    mode TypeKey;
    TypeKey.name            = "TypeKey";
    TypeKey.value           = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_TYPE_KEY_VALUE;
    TypeKey.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    TypeKey.color_mode      = MODE_COLORS_RANDOM;
    TypeKey.speed           = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MED;
    TypeKey.speed_min       = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MIN;
    TypeKey.speed_max       = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MAX;
    TypeKey.colors.resize(2);
    modes.push_back(TypeKey);

    mode TypeRipple;
    TypeRipple.name         = "TypeRipple";
    TypeRipple.value        = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_TYPE_RIPPLE_VALUE;
    TypeRipple.flags        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    TypeRipple.color_mode   = MODE_COLORS_RANDOM;
    TypeRipple.speed        = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MED;
    TypeRipple.speed_min    = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MIN;
    TypeRipple.speed_max    = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MAX;
    TypeRipple.colors.resize(2);
    modes.push_back(TypeRipple);

    mode Visor;
    Visor.name              = "Visor";
    Visor.value             = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_VISOR_VALUE;
    Visor.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Visor.color_mode        = MODE_COLORS_RANDOM;
    Visor.direction         = MODE_DIRECTION_RIGHT;
    Visor.speed             = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MED;
    Visor.speed_min         = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MIN;
    Visor.speed_max         = CorsairK55RGBPROXTController::CORSAIR_HW_MODE_SPEED_MAX;
    Visor.colors.resize(2);
    modes.push_back(Visor);

    SetupZones();
    /*-----------------------------------------------------*\
    | The Corsair K55 RGB PRO XT requires a packet within   |
    | 1 minutes of sending the lighting change in order     |
    | to not revert back into rainbow mode.  Start a thread |
    | to continuously send a keepalive packet every 50 sec  |
    \*-----------------------------------------------------*/
    keepalive_thread_run = true;
    keepalive_thread     = new std::thread(&RGBController_CorsairK55RGBPROXT::KeepaliveThread, this);
}

RGBController_CorsairK55RGBPROXT::~RGBController_CorsairK55RGBPROXT()
{
    /*-----------------------------------------------------*\
    | Close keepalive thread                                |
    \*-----------------------------------------------------*/
    keepalive_thread_run = false;
    keepalive_thread->join();
    delete keepalive_thread;
    delete[] zones[0].matrix_map;

    delete controller;
}

void RGBController_CorsairK55RGBPROXT::SetupZones()
{
    zone keyboard_zone;
    keyboard_zone.name               = "Keyboard";
    keyboard_zone.type               = ZONE_TYPE_MATRIX;

    keyboard_zone.matrix_map         = new matrix_map_type;
    keyboard_zone.matrix_map->map    = (unsigned int *)&matrix_map;
    keyboard_zone.matrix_map->height = HEIGHT;
    keyboard_zone.matrix_map->width  = WIDTH;

    for(size_t led_index = 0; led_index < key_names.size(); ++led_index)
    {
        led new_led;
        new_led.name                 = key_names[led_index];
        leds.push_back(new_led);
    }

    keyboard_zone.leds_min           = (unsigned int)leds.size();
    keyboard_zone.leds_max           = (unsigned int)leds.size();
    keyboard_zone.leds_count         = (unsigned int)leds.size();

    zones.push_back(keyboard_zone);

    SetupColors();
}

void RGBController_CorsairK55RGBPROXT::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairK55RGBPROXT::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    controller->SetLEDs(colors);
}

void RGBController_CorsairK55RGBPROXT::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetLEDs(colors);
}

void RGBController_CorsairK55RGBPROXT::UpdateSingleLED(int /*led*/)
{
    controller->SetLEDs(colors);
}

void RGBController_CorsairK55RGBPROXT::DeviceUpdateMode()
{
    if(modes[active_mode].value == CorsairK55RGBPROXTController::CORSAIR_MODE_DIRECT_VALUE)
    {
        controller->SwitchMode(true);
    }
    else
    {
        const mode& active = modes[active_mode];

        controller->SetHardwareMode(active.value, active.color_mode, active.colors, active.speed, active.direction);
        controller->SwitchMode(false);
    }
}

void RGBController_CorsairK55RGBPROXT::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(50000))
            {
                DeviceUpdateLEDs();
            }
        }
        std::this_thread::sleep_for(3000ms);
    }
}
