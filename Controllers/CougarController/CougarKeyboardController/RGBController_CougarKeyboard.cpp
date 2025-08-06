/*---------------------------------------------------------*\
| RGBController_CougarKeyboard.cpp                          |
|                                                           |
|   RGBController for Cougar keyboard                       |
|                                                           |
|   Chris M (DrNo)                              05 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#define NA 0xFFFFFFFF

#include <chrono>
#include "hsv.h"
#include "RGBControllerKeyNames.h"
#include "RGBController_CougarKeyboard.h"

using namespace std::chrono_literals;

static unsigned int matrix_map[6][COUGARKEYBOARDCONTROLLER_MATRIX_WIDTH] =
{
    {  NA,   0,  NA,   1,   2,   3,   4,   5,   6,   7,   8,  NA,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19 },
    { 108,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  NA,  34,  35,  36,  37,  38,  39,  40 },
    { 109,  41,  NA,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61 },
    { 110,  62,  NA,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  NA,  74,  NA,  NA,  NA,  75,  76,  77,  NA },
    { 111,  78,  NA,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  NA,  89,  NA,  NA,  90,  NA,  91,  92,  93,  94 },
    { 112,  95,  96,  97,  NA,  NA,  NA,  98,  NA,  NA,  NA,  99, 100,  NA, 101, 102, 103, 104, 105, 106,  NA, 107,  NA }
};

static const char *led_names[] =
{
    KEY_EN_ESCAPE,              //00
    KEY_EN_F1,
    KEY_EN_F2,
    KEY_EN_F3,
    KEY_EN_F4,
    KEY_EN_F5,
    KEY_EN_F6,
    KEY_EN_F7,
    KEY_EN_F8,
    KEY_EN_F9,
    KEY_EN_F10,                 //10
    KEY_EN_F11,
    KEY_EN_F12,
    KEY_EN_PRINT_SCREEN,
    KEY_EN_SCROLL_LOCK,
    KEY_EN_PAUSE_BREAK,
    KEY_EN_MEDIA_PLAY_PAUSE,    //OPEN MEDIA
    KEY_EN_MEDIA_VOLUME_DOWN,
    KEY_EN_MEDIA_VOLUME_UP,
    KEY_EN_MEDIA_MUTE,

    KEY_EN_BACK_TICK,           //20
    KEY_EN_1,
    KEY_EN_2,
    KEY_EN_3,
    KEY_EN_4,
    KEY_EN_5,
    KEY_EN_6,
    KEY_EN_7,
    KEY_EN_8,
    KEY_EN_9,
    KEY_EN_0,                   //30
    KEY_EN_MINUS,
    KEY_EN_EQUALS,
    KEY_EN_BACKSPACE,
    KEY_EN_INSERT,
    KEY_EN_HOME,
    KEY_EN_PAGE_UP,
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_MINUS,        //40

    KEY_EN_TAB,
    KEY_EN_Q,
    KEY_EN_W,
    KEY_EN_E,
    KEY_EN_R,
    KEY_EN_T,
    KEY_EN_Y,
    KEY_EN_U,
    KEY_EN_I,
    KEY_EN_O,                   //50
    KEY_EN_P,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_DELETE,
    KEY_EN_END,
    KEY_EN_PAGE_DOWN,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_9,            //60
    KEY_EN_NUMPAD_PLUS,

    KEY_EN_CAPS_LOCK,
    KEY_EN_A,
    KEY_EN_S,
    KEY_EN_D,
    KEY_EN_F,
    KEY_EN_G,
    KEY_EN_H,
    KEY_EN_J,
    KEY_EN_K,                   //70
    KEY_EN_L,
    KEY_EN_SEMICOLON,
    KEY_EN_QUOTE,
    KEY_EN_ANSI_ENTER,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_6,

    KEY_EN_LEFT_SHIFT,
    KEY_EN_Z,
    KEY_EN_X,                   //80
    KEY_EN_C,
    KEY_EN_V,
    KEY_EN_B,
    KEY_EN_N,
    KEY_EN_M,
    KEY_EN_COMMA,
    KEY_EN_PERIOD,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_UP_ARROW,            //90
    KEY_EN_NUMPAD_1,
    KEY_EN_NUMPAD_2,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_ENTER,

    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    KEY_EN_SPACE,
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_WINDOWS,       //100
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW,
    KEY_EN_NUMPAD_0,
    KEY_EN_NUMPAD_PERIOD,

    "Key: G1",
    "Key: G2",
    "Key: G3",                  //110
    "Key: G4",
    "Key: G5"
};

/**------------------------------------------------------------------*\
    @name Cougar 700K Evo Keyboard
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCougar700kEvo
    @comment The Cougar 700K Evo controller implements all hardware modes
      found in the OEM software but has not been able to include all
      options for some modes. eg. Rainbow colour mode. Music mode was
      deteremined to be a software driven effect which can be added with
      the (OpenRGB Effects Engine plugin)[https://gitlab.com/OpenRGBDevelopers/OpenRGBEffectsPlugin].
\*-------------------------------------------------------------------*/

RGBController_CougarKeyboard::RGBController_CougarKeyboard(CougarKeyboardController *controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "Cougar";
    type                        = DEVICE_TYPE_KEYBOARD;
    description                 = "Cougar Keyboard Device";
    serial                      = controller->GetSerial();
    location                    = controller->GetLocation();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = COUGARKEYBOARDCONTROLLER_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = COUGARKEYBOARDCONTROLLER_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(Static.colors_max);
    Static.brightness_min       = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MIN;
    Static.brightness_max       = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Static.brightness           = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = COUGARKEYBOARDCONTROLLER_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR |
                                  MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.colors.resize(Breathing.colors_max);
    Breathing.brightness_min    = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MIN;
    Breathing.brightness_max    = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Breathing.brightness        = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Breathing.speed_min         = COUGARKEYBOARDCONTROLLER_SPEED_SLOWEST;
    Breathing.speed_max         = COUGARKEYBOARDCONTROLLER_SPEED_FASTEST;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed             = COUGARKEYBOARDCONTROLLER_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode Circle;
    Circle.name                 = "Spectrum Cycle";
    Circle.value                = COUGARKEYBOARDCONTROLLER_MODE_CIRCLE;
    Circle.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Circle.brightness_min       = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MIN;
    Circle.brightness_max       = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Circle.brightness           = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Circle.speed_min            = COUGARKEYBOARDCONTROLLER_SPEED_SLOWEST;
    Circle.speed_max            = COUGARKEYBOARDCONTROLLER_SPEED_FASTEST;
    Circle.color_mode           = MODE_COLORS_NONE;
    Circle.speed                = COUGARKEYBOARDCONTROLLER_SPEED_NORMAL;
    modes.push_back(Circle);

    mode Reactive;
    Reactive.name               = "Reactive";
    Reactive.value              = COUGARKEYBOARDCONTROLLER_MODE_REACTIVE;
    Reactive.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR |
                                  MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Reactive.colors_min         = 1;
    Reactive.colors_max         = 1;
    Reactive.colors.resize(Reactive.colors_max);
    Reactive.brightness_min     = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MIN;
    Reactive.brightness_max     = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Reactive.brightness         = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Reactive.speed_min          = COUGARKEYBOARDCONTROLLER_SPEED_SLOWEST;
    Reactive.speed_max          = COUGARKEYBOARDCONTROLLER_SPEED_FASTEST;
    Reactive.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Reactive.speed              = COUGARKEYBOARDCONTROLLER_SPEED_NORMAL;
    modes.push_back(Reactive);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = COUGARKEYBOARDCONTROLLER_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR |
                                  MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_MANUAL_SAVE;
    Wave.colors_min             = 1;
    Wave.colors_max             = 1;
    Wave.colors.resize(Wave.colors_max);
    Wave.brightness_min         = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MIN;
    Wave.brightness_max         = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Wave.brightness             = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Wave.speed_min              = COUGARKEYBOARDCONTROLLER_SPEED_SLOWEST;
    Wave.speed_max              = COUGARKEYBOARDCONTROLLER_SPEED_FASTEST;
    Wave.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Wave.speed                  = COUGARKEYBOARDCONTROLLER_SPEED_NORMAL;
    modes.push_back(Wave);

    mode Ripple;
    Ripple.name                 = "Ripple";
    Ripple.value                = COUGARKEYBOARDCONTROLLER_MODE_RIPPLE;
    Ripple.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR |
                                  MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Ripple.colors_min           = 1;
    Ripple.colors_max           = 1;
    Ripple.colors.resize(Ripple.colors_max);
    Ripple.brightness_min       = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MIN;
    Ripple.brightness_max       = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Ripple.brightness           = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Ripple.speed_min            = COUGARKEYBOARDCONTROLLER_SPEED_SLOWEST;
    Ripple.speed_max            = COUGARKEYBOARDCONTROLLER_SPEED_FASTEST;
    Ripple.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Ripple.speed                = COUGARKEYBOARDCONTROLLER_SPEED_NORMAL;
    modes.push_back(Ripple);

    mode Star;
    Star.name                   = "Star";
    Star.value                  = COUGARKEYBOARDCONTROLLER_MODE_STAR;
    Star.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR |
                                  MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Star.colors_min             = 1;
    Star.colors_max             = 1;
    Star.colors.resize(Star.colors_max);
    Star.brightness_min         = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MIN;
    Star.brightness_max         = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Star.brightness             = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Star.speed_min              = COUGARKEYBOARDCONTROLLER_SPEED_SLOWEST;
    Star.speed_max              = COUGARKEYBOARDCONTROLLER_SPEED_FASTEST;
    Star.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Star.speed                  = COUGARKEYBOARDCONTROLLER_SPEED_NORMAL;
    modes.push_back(Star);

    mode Scan;
    Scan.name                   = "Scan";
    Scan.value                  = COUGARKEYBOARDCONTROLLER_MODE_SCAN;
    Scan.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR |
                                  MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD |
                                  MODE_FLAG_HAS_DIRECTION_HV | MODE_FLAG_MANUAL_SAVE;
    Scan.colors_min             = 1;
    Scan.colors_max             = 1;
    Scan.colors.resize(Scan.colors_max);
    Scan.brightness_min         = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MIN;
    Scan.brightness_max         = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Scan.brightness             = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Scan.speed_min              = COUGARKEYBOARDCONTROLLER_SPEED_SLOWEST;
    Scan.speed_max              = COUGARKEYBOARDCONTROLLER_SPEED_FASTEST;
    Scan.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Scan.speed                  = COUGARKEYBOARDCONTROLLER_SPEED_NORMAL;
    modes.push_back(Scan);

    mode Rhythm;
    Rhythm.name                 = "Rhythm";
    Rhythm.value                = COUGARKEYBOARDCONTROLLER_MODE_RHYTHM;
    Rhythm.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR |
                                  MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Rhythm.colors_min           = 1;
    Rhythm.colors_max           = 1;
    Rhythm.colors.resize(Rhythm.colors_max);
    Rhythm.brightness_min       = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MIN;
    Rhythm.brightness_max       = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Rhythm.brightness           = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Rhythm.speed_min            = COUGARKEYBOARDCONTROLLER_SPEED_SLOWEST;
    Rhythm.speed_max            = COUGARKEYBOARDCONTROLLER_SPEED_FASTEST;
    Rhythm.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Rhythm.speed                = COUGARKEYBOARDCONTROLLER_SPEED_NORMAL;
    modes.push_back(Rhythm);

    mode Rain;
    Rain.name                   = "Rain";
    Rain.value                  = COUGARKEYBOARDCONTROLLER_MODE_RAIN;
    Rain.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR |
                                  MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Rain.colors_min             = 1;
    Rain.colors_max             = 1;
    Rain.colors.resize(Rain.colors_max);
    Rain.brightness_min         = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MIN;
    Rain.brightness_max         = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Rain.brightness             = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Rain.speed_min              = COUGARKEYBOARDCONTROLLER_SPEED_SLOWEST;
    Rain.speed_max              = COUGARKEYBOARDCONTROLLER_SPEED_FASTEST;
    Rain.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Rain.speed                  = COUGARKEYBOARDCONTROLLER_SPEED_NORMAL;
    modes.push_back(Rain);

    mode Snake;
    Snake.name                  = "Snake";
    Snake.value                 = COUGARKEYBOARDCONTROLLER_MODE_SNAKE;
    Snake.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR |
                                  MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Snake.colors_min            = 1;
    Snake.colors_max            = 1;
    Snake.colors.resize(Snake.colors_max);
    Snake.brightness_min        = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MIN;
    Snake.brightness_max        = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Snake.brightness            = COUGARKEYBOARDCONTROLLER_BRIGHTNESS_MAX;
    Snake.speed_min             = COUGARKEYBOARDCONTROLLER_SPEED_SLOWEST;
    Snake.speed_max             = COUGARKEYBOARDCONTROLLER_SPEED_FASTEST;
    Snake.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Snake.speed                 = COUGARKEYBOARDCONTROLLER_SPEED_NORMAL;
    modes.push_back(Snake);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = COUGARKEYBOARDCONTROLLER_MODE_OFF;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_CougarKeyboard::~RGBController_CougarKeyboard()
{
    delete controller;
}

void RGBController_CougarKeyboard::SetupZones()
{
    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    zone KB_zone;
    KB_zone.name                = ZONE_EN_KEYBOARD;
    KB_zone.type                = ZONE_TYPE_MATRIX;
    KB_zone.leds_count          = 113;
    KB_zone.leds_min            = KB_zone.leds_count;
    KB_zone.leds_max            = KB_zone.leds_count;

    KB_zone.matrix_map          = new matrix_map_type;
    KB_zone.matrix_map->height  = 6;
    KB_zone.matrix_map->width   = COUGARKEYBOARDCONTROLLER_MATRIX_WIDTH;
    KB_zone.matrix_map->map     = (unsigned int *)&matrix_map;
    zones.push_back(KB_zone);

    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(std::size_t zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        for(unsigned int led_index = 0; led_index < zones[zone_index].leds_count; led_index++)
        {
            led new_led;
            new_led.name        = led_names[led_index];
            new_led.value       = led_index;
            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_CougarKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CougarKeyboard::DeviceUpdateLEDs()
{
    controller->SetLedsDirect(colors);
}

void RGBController_CougarKeyboard::UpdateZoneLEDs(int zone)
{
    std::vector<RGBColor> colour;
    for(size_t i = 0; i < zones[zone].leds_count; i++)
    {
        colour.push_back(zones[zone].colors[i]);
    }

    controller->SetLedsDirect(colour);
}

void RGBController_CougarKeyboard::UpdateSingleLED(int led)
{
    std::vector<RGBColor> colour;
    colour.push_back(colors[led]);

    controller->SetLedsDirect(colour);
}

void RGBController_CougarKeyboard::DeviceUpdateMode()
{
    mode set_mode                   = modes[active_mode];
    std::vector<RGBColor> colours   = (set_mode.colors);

    /*---------------------------------------------------------*\
    | No mode set packets required for Direct mode              |
    | Wave mode requires 5 colours based on the selected colour |
    \*---------------------------------------------------------*/
    switch(set_mode.value)
    {
        case COUGARKEYBOARDCONTROLLER_MODE_DIRECT:
            return;
        case COUGARKEYBOARDCONTROLLER_MODE_WAVE:
            if(set_mode.color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                hsv_t temp;
                colours.resize(1);
                rgb2hsv( colours[0], &temp);

                temp.value          = temp.value / 2;
                RGBColor half       = hsv2rgb(&temp);

                temp.value          = temp.value / 4;
                RGBColor eighth     = hsv2rgb(&temp);

                colours.push_back(half);
                colours.push_back(eighth);
                colours.push_back(half);
                colours.push_back(colours[0]);
            }
            break;
    }

    uint8_t direction               = direction_map[set_mode.direction];
    bool random_colours             = (set_mode.color_mode == MODE_COLORS_RANDOM);

    controller->SetMode( set_mode.value, set_mode.speed, set_mode.brightness, direction, colours, random_colours );
}

void RGBController_CougarKeyboard::DeviceSaveMode()
{
    const uint8_t start             = 0x02;
    const uint8_t end               = 0x03;

    /*---------------------------------------------------------*\
    | The Keyboard has the ability to save 3 light modes across |
    | 3 profiles but currently we will only set the 1st of each |
    |     Profiles = 1 thru 3                                   |
    |     Lights   = 0 thru 2                                   |
    \*---------------------------------------------------------*/
    controller->Save(start);
    std::this_thread::sleep_for(10ms);
    controller->SendProfile(1, 0);
    std::this_thread::sleep_for(150ms);
    DeviceUpdateMode();
    std::this_thread::sleep_for(10ms);
    controller->Save(end);
}
