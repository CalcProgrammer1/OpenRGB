/*---------------------------------------------------------*\
| RGBController_GigabyteAorusLaptop.cpp                     |
|                                                           |
|   RGBController for Gigabyte Aorus laptop                 |
|                                                           |
|   Morgan Guimard (morg)                       05 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_GigabyteAorusLaptop.h"
#include "RGBControllerKeyNames.h"

/**------------------------------------------------------------------*\
    @name Aorus Laptop
    @category
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectGigabyteAorusLaptopKeyboardControllers,DetectGigabyteAorusLaptopBacklightControllers
    @comment Direct mode will only exposes the whole keyboard as one
             big led, only custom mode can do real per key lightning.
             This is impossible to determine if it auto saves to flash
             (the battery cannot be removed) then we assume it does.
             This device has 5 onboard memory profiles, we only use
             the first one.
\*-------------------------------------------------------------------*/

#define NA      0xFFFFFFFF

typedef struct
{
    const unsigned int                      width;                  /* matrix width         */
    const unsigned int                      height;                 /* matrix height        */
    std::vector<std::vector<unsigned int>>  matrix_map;             /* matrix map           */
    std::vector<std::string>                led_names;              /* led names            */
    std::vector<unsigned int>               led_sequence_positions; /* position in buffers  */
} aorus_laptop_keyboard_layout;

static aorus_laptop_keyboard_layout aorus_laptop_default_keyboard_layout =
{
    19,
    6,
    {
        {  0,   1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,  17, 18}, // 19
        {  19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, NA, 32, 33, 34,  35, 36}, // 18
        {  37, NA, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,  53, 54}, // 18
        {  55, NA, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, NA, 67, 68, 69,  70, NA}, // 16
        {  71, NA, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, NA, 82, 83, 84, 85,  86, 87}, // 17
        {  88, 89, 90, 91, NA, NA, NA, 92, NA, NA, 93, 94, 95, 96, 97, 98, 99, 100, NA}  // 13
    },
    {
        KEY_EN_ESCAPE,       KEY_EN_F1,            KEY_EN_F2,           KEY_EN_F3,       KEY_EN_F4,    KEY_EN_F5,        KEY_EN_F6,   KEY_EN_F7,            KEY_EN_F8,         KEY_EN_F9,         KEY_EN_F10,           KEY_EN_F11,          KEY_EN_F12,           KEY_EN_PAUSE_BREAK, KEY_EN_DELETE,      KEY_EN_HOME,          KEY_EN_PAGE_UP,      KEY_EN_PAGE_DOWN,    KEY_EN_END,
        KEY_EN_BACK_TICK,    KEY_EN_1,             KEY_EN_2,            KEY_EN_3,        KEY_EN_4,     KEY_EN_5,         KEY_EN_6,    KEY_EN_7,             KEY_EN_8,          KEY_EN_9,          KEY_EN_0,             KEY_EN_MINUS,        KEY_EN_EQUALS,        KEY_EN_BACKSPACE,   KEY_EN_NUMPAD_LOCK, KEY_EN_NUMPAD_DIVIDE, KEY_EN_NUMPAD_TIMES, KEY_EN_NUMPAD_MINUS,
        KEY_EN_TAB,          KEY_EN_Q,             KEY_EN_W,            KEY_EN_E,        KEY_EN_R,     KEY_EN_T,         KEY_EN_Y,    KEY_EN_U,             KEY_EN_I,          KEY_EN_O,          KEY_EN_P,             KEY_EN_LEFT_BRACKET, KEY_EN_RIGHT_BRACKET, KEY_EN_BACK_SLASH,  KEY_EN_NUMPAD_7,    KEY_EN_NUMPAD_8,      KEY_EN_NUMPAD_9,     KEY_EN_NUMPAD_PLUS,
        KEY_EN_CAPS_LOCK,    KEY_EN_A,             KEY_EN_S,            KEY_EN_D,        KEY_EN_F,     KEY_EN_G,         KEY_EN_H,    KEY_EN_J,             KEY_EN_K,          KEY_EN_L,          KEY_EN_SEMICOLON,     KEY_EN_QUOTE,        KEY_EN_ISO_ENTER,     KEY_EN_NUMPAD_4,    KEY_EN_NUMPAD_5,    KEY_EN_NUMPAD_6,
        KEY_EN_LEFT_SHIFT,   KEY_EN_Z,             KEY_EN_X,            KEY_EN_C,        KEY_EN_V,     KEY_EN_B,         KEY_EN_N,    KEY_EN_M,             KEY_EN_COMMA,      KEY_EN_PERIOD,     KEY_EN_FORWARD_SLASH, KEY_EN_RIGHT_SHIFT,  KEY_EN_UP_ARROW,      KEY_EN_NUMPAD_1,    KEY_EN_NUMPAD_2,    KEY_EN_NUMPAD_3,      KEY_EN_NUMPAD_ENTER,
        KEY_EN_LEFT_CONTROL, KEY_EN_LEFT_FUNCTION, KEY_EN_LEFT_WINDOWS, KEY_EN_LEFT_ALT, KEY_EN_SPACE, KEY_EN_RIGHT_ALT, KEY_EN_MENU, KEY_EN_RIGHT_CONTROL, KEY_EN_LEFT_ARROW, KEY_EN_DOWN_ARROW, KEY_EN_RIGHT_ARROW,   KEY_EN_NUMPAD_0,     KEY_EN_NUMPAD_PERIOD
    },
    {
        11, 17, 23, 29, 35, 41, 47, 53, 59, 65, 71, 77,  83, 89, 95,  101, 107, 113, 119,
        10, 16, 22, 28, 34, 40, 46, 52, 58, 64, 70, 76,  82, 94, 100, 106, 112, 118,
        9,  15, 21, 27, 33, 39, 45, 51, 57, 63, 69, 75,  81, 87, 99,  105, 111, 116,
        8,  14, 20, 26, 32, 38, 44, 50, 56, 62, 68, 74,  92, 98, 104, 110,
        7,  19, 25, 31, 37, 43, 49, 55, 61, 67, 73, 85,  91, 97, 103, 109, 114,
        6,  12, 18, 24, 42, 60, 66, 72, 84, 90, 96, 102, 108
    }
};


RGBController_GigabyteAorusLaptop::RGBController_GigabyteAorusLaptop(GigabyteAorusLaptopController* controller_ptr, GIGABYTE_AORUS_LAPTOP_DEV_TYPE dev_type)
{
    this->dev_type                          = dev_type;
    controller                              = controller_ptr;
    name                                    = controller->GetNameString();
    vendor                                  = "Gigabyte";
    type                                    = DEVICE_TYPE_LAPTOP;
    description                             = "Aorus Laptop";
    location                                = controller->GetDeviceLocation();
    serial                                  = controller->GetSerialString();

    /*---------------------------------------------------------*\
    | Only keyboard supports Direct mode                        |
    \*---------------------------------------------------------*/
    if(dev_type == GIGABYTE_AORUS_LAPTOP_KEYBOARD_TYPE)
    {
        mode Direct;
        Direct.name                         = "Direct";
        Direct.value                        = GIGABYTE_AORUS_LAPTOP_DIRECT_MODE_VALUE;
        Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Direct.color_mode                   = MODE_COLORS_PER_LED;
        Direct.brightness_min               = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
        Direct.brightness_max               = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
        Direct.brightness                   = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
        modes.push_back(Direct);
    }

    /*---------------------------------------------------------*\
    | Common modes to keyboard + backlight                      |
    \*---------------------------------------------------------*/
    mode Static;
    Static.name                             = "Static";
    Static.value                            = GIGABYTE_AORUS_LAPTOP_STATIC_MODE_VALUE;
    Static.flags                            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode                       = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    Static.colors_min                       = 1;
    Static.colors_max                       = 1;
    Static.brightness_min                   = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
    Static.brightness_max                   = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
    Static.brightness                       = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
    modes.push_back(Static);

    mode Pulse;
    Pulse.name                              = "Breathing";
    Pulse.value                             = GIGABYTE_AORUS_LAPTOP_PULSE_MODE_VALUE;
    Pulse.flags                             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Pulse.color_mode                        = MODE_COLORS_MODE_SPECIFIC;
    Pulse.colors.resize(1);
    Pulse.colors_min                        = 1;
    Pulse.colors_max                        = 1;
    Pulse.brightness_min                    = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
    Pulse.brightness_max                    = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
    Pulse.brightness                        = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
    Pulse.speed_min                         = GIGABYTE_AORUS_LAPTOP_SPEED_MIN;
    Pulse.speed_max                         = GIGABYTE_AORUS_LAPTOP_SPEED_MAX;
    Pulse.speed                             = GIGABYTE_AORUS_LAPTOP_SPEED_MAX/2;
    modes.push_back(Pulse);

    mode Wave;
    Wave.name                               = "Rainbow Wave";
    Wave.value                              = GIGABYTE_AORUS_LAPTOP_WAVE_MODE_VALUE;
    Wave.flags                              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.color_mode                         = MODE_COLORS_MODE_SPECIFIC;
    Wave.colors.resize(1);
    Wave.colors_min                         = 1;
    Wave.colors_max                         = 1;
    Wave.brightness_min                     = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
    Wave.brightness_max                     = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
    Wave.brightness                         = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
    Wave.speed_min                          = GIGABYTE_AORUS_LAPTOP_SPEED_MIN;
    Wave.speed_max                          = GIGABYTE_AORUS_LAPTOP_SPEED_MAX;
    Wave.speed                              = GIGABYTE_AORUS_LAPTOP_SPEED_MAX/2;
    Wave.direction                          = MODE_DIRECTION_LEFT;
    modes.push_back(Wave);

    mode Cycle;
    Cycle.name                              = "Spectrum Cycle";
    Cycle.value                             = GIGABYTE_AORUS_LAPTOP_CYCLE_MODE_VALUE;
    Cycle.flags                             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Cycle.color_mode                        = MODE_COLORS_MODE_SPECIFIC;
    Cycle.colors.resize(1);
    Cycle.colors_min                        = 1;
    Cycle.colors_max                        = 1;
    Cycle.brightness_min                    = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
    Cycle.brightness_max                    = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
    Cycle.brightness                        = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
    Cycle.speed_min                         = GIGABYTE_AORUS_LAPTOP_SPEED_MIN;
    Cycle.speed_max                         = GIGABYTE_AORUS_LAPTOP_SPEED_MAX;
    Cycle.speed                             = GIGABYTE_AORUS_LAPTOP_SPEED_MAX/2;
    modes.push_back(Cycle);

    mode Droplet;
    Droplet.name                            = "Droplet";
    Droplet.value                           = GIGABYTE_AORUS_LAPTOP_DROPLET_MODE_VALUE;
    Droplet.flags                           = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Droplet.color_mode                      = MODE_COLORS_MODE_SPECIFIC;
    Droplet.colors.resize(1);
    Droplet.colors_min                      = 1;
    Droplet.colors_max                      = 1;
    Droplet.brightness_min                  = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
    Droplet.brightness_max                  = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
    Droplet.brightness                      = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
    Droplet.speed_min                       = GIGABYTE_AORUS_LAPTOP_SPEED_MIN;
    Droplet.speed_max                       = GIGABYTE_AORUS_LAPTOP_SPEED_MAX;
    Droplet.speed                           = GIGABYTE_AORUS_LAPTOP_SPEED_MAX/2;
    modes.push_back(Droplet);

    mode Spiral;
    Spiral.name                             = "Spiral";
    Spiral.value                            = GIGABYTE_AORUS_LAPTOP_SPIRAL_MODE_VALUE;
    Spiral.flags                            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_AUTOMATIC_SAVE;
    Spiral.color_mode                       = MODE_COLORS_MODE_SPECIFIC;
    Spiral.colors.resize(1);
    Spiral.colors_min                       = 1;
    Spiral.colors_max                       = 1;
    Spiral.brightness_min                   = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
    Spiral.brightness_max                   = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
    Spiral.brightness                       = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
    Spiral.speed_min                        = GIGABYTE_AORUS_LAPTOP_SPEED_MIN;
    Spiral.speed_max                        = GIGABYTE_AORUS_LAPTOP_SPEED_MAX;
    Spiral.speed                            = GIGABYTE_AORUS_LAPTOP_SPEED_MAX/2;
    Spiral.direction                        = MODE_DIRECTION_LEFT;
    modes.push_back(Spiral);

    /*---------------------------------------------------------*\
    | Modes for backlight only                                  |
    \*---------------------------------------------------------*/
    if(dev_type == GIGABYTE_AORUS_LAPTOP_BACKLIGHT_TYPE)
    {
        mode Curtain;
        Curtain.name                        = "Curtain";
        Curtain.value                       = GIGABYTE_AORUS_LAPTOP_CURTAIN_MODE_VALUE;
        Curtain.flags                       = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        Curtain.color_mode                  = MODE_COLORS_MODE_SPECIFIC;
        Curtain.colors.resize(1);
        Curtain.colors_min                  = 1;
        Curtain.colors_max                  = 1;
        Curtain.brightness_min              = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
        Curtain.brightness_max              = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
        Curtain.brightness                  = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
        modes.push_back(Curtain);

        mode Comet;
        Comet.name                          = "Comet";
        Comet.value                         = GIGABYTE_AORUS_LAPTOP_COMET_MODE_VALUE;
        Comet.flags                         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        Comet.color_mode                    = MODE_COLORS_MODE_SPECIFIC;
        Comet.colors.resize(1);
        Comet.colors_min                    = 1;
        Comet.colors_max                    = 1;
        Comet.brightness_min                = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
        Comet.brightness_max                = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
        Comet.brightness                    = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
        modes.push_back(Comet);

        mode Chase;
        Chase.name                          = "Chase";
        Chase.value                         = GIGABYTE_AORUS_LAPTOP_CHASE_MODE_VALUE;
        Chase.flags                         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        Chase.color_mode                    = MODE_COLORS_MODE_SPECIFIC;
        Chase.colors.resize(1);
        Chase.colors_min                    = 1;
        Chase.colors_max                    = 1;
        Chase.brightness_min                = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
        Chase.brightness_max                = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
        Chase.brightness                    = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
        modes.push_back(Chase);
    }

    /*---------------------------------------------------------*\
    | Modes for keyboard only                                   |
    \*---------------------------------------------------------*/
    if(dev_type == GIGABYTE_AORUS_LAPTOP_KEYBOARD_TYPE)
    {
        mode Reactive;
        Reactive.name                       = "Reactive";
        Reactive.value                      = GIGABYTE_AORUS_LAPTOP_REACTIVE_MODE_VALUE;
        Reactive.flags                      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        Reactive.color_mode                 = MODE_COLORS_MODE_SPECIFIC;
        Reactive.colors.resize(1);
        Reactive.colors_min                 = 1;
        Reactive.colors_max                 = 1;
        Reactive.brightness_min             = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
        Reactive.brightness_max             = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
        Reactive.brightness                 = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
        modes.push_back(Reactive);

        mode Marquee;
        Marquee.name                        = "Marquee";
        Marquee.value                       = GIGABYTE_AORUS_LAPTOP_MARQUEE_MODE_VALUE;
        Marquee.flags                       = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
        Marquee.color_mode                  = MODE_COLORS_MODE_SPECIFIC;
        Marquee.colors.resize(1);
        Marquee.colors_min                  = 1;
        Marquee.colors_max                  = 1;
        Marquee.brightness_min              = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
        Marquee.brightness_max              = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
        Marquee.brightness                  = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
        Marquee.speed_min                   = GIGABYTE_AORUS_LAPTOP_SPEED_MIN;
        Marquee.speed_max                   = GIGABYTE_AORUS_LAPTOP_SPEED_MAX;
        Marquee.speed                       = GIGABYTE_AORUS_LAPTOP_SPEED_MAX/2;
        modes.push_back(Marquee);

        mode CircleMarquee;
        CircleMarquee.name                  = "Circle Marquee";
        CircleMarquee.value                 = GIGABYTE_AORUS_LAPTOP_CIRCLE_MARQUEE_MODE_VALUE;
        CircleMarquee.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
        CircleMarquee.color_mode            = MODE_COLORS_MODE_SPECIFIC;
        CircleMarquee.colors.resize(1);
        CircleMarquee.colors_min            = 1;
        CircleMarquee.colors_max            = 1;
        CircleMarquee.brightness_min        = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
        CircleMarquee.brightness_max        = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
        CircleMarquee.brightness            = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
        CircleMarquee.speed_min             = GIGABYTE_AORUS_LAPTOP_SPEED_MIN;
        CircleMarquee.speed_max             = GIGABYTE_AORUS_LAPTOP_SPEED_MAX;
        CircleMarquee.speed                 = GIGABYTE_AORUS_LAPTOP_SPEED_MAX/2;
        modes.push_back(CircleMarquee);

        mode RainbowMarquee;
        RainbowMarquee.name                 = "Rainbow Marquee";
        RainbowMarquee.value                = GIGABYTE_AORUS_LAPTOP_RAINBOW_MARQUEE_MODE_VALUE;
        RainbowMarquee.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
        RainbowMarquee.color_mode           = MODE_COLORS_MODE_SPECIFIC;
        RainbowMarquee.colors.resize(1);
        RainbowMarquee.colors_min           = 1;
        RainbowMarquee.colors_max           = 1;
        RainbowMarquee.brightness_min       = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
        RainbowMarquee.brightness_max       = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
        RainbowMarquee.brightness           = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
        RainbowMarquee.speed_min            = GIGABYTE_AORUS_LAPTOP_SPEED_MIN;
        RainbowMarquee.speed_max            = GIGABYTE_AORUS_LAPTOP_SPEED_MAX;
        RainbowMarquee.speed                = GIGABYTE_AORUS_LAPTOP_SPEED_MAX/2;
        modes.push_back(RainbowMarquee);

        mode Ripple;
        Ripple.name                         = "Ripple";
        Ripple.value                        = GIGABYTE_AORUS_LAPTOP_RIPPLE_MODE_VALUE;
        Ripple.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
        Ripple.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
        Ripple.colors.resize(1);
        Ripple.colors_min                   = 1;
        Ripple.colors_max                   = 1;
        Ripple.brightness_min               = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
        Ripple.brightness_max               = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
        Ripple.brightness                   = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
        Ripple.speed_min                    = GIGABYTE_AORUS_LAPTOP_SPEED_MIN;
        Ripple.speed_max                    = GIGABYTE_AORUS_LAPTOP_SPEED_MAX;
        Ripple.speed                        = GIGABYTE_AORUS_LAPTOP_SPEED_MAX/2;
        modes.push_back(Ripple);

        mode Hedge;
        Hedge.name                          = "Hedge";
        Hedge.value                         = GIGABYTE_AORUS_LAPTOP_HEDGE_MODE_VALUE;
        Hedge.flags                         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
        Hedge.color_mode                    = MODE_COLORS_MODE_SPECIFIC;
        Hedge.colors.resize(1);
        Hedge.colors_min                    = 1;
        Hedge.colors_max                    = 1;
        Hedge.brightness_min                = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
        Hedge.brightness_max                = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
        Hedge.brightness                    = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
        Hedge.speed_min                     = GIGABYTE_AORUS_LAPTOP_SPEED_MIN;
        Hedge.speed_max                     = GIGABYTE_AORUS_LAPTOP_SPEED_MAX;
        Hedge.speed                         = GIGABYTE_AORUS_LAPTOP_SPEED_MAX/2;
        modes.push_back(Hedge);

        mode Custom;
        Custom.name                         = "Custom";
        Custom.value                        = GIGABYTE_AORUS_LAPTOP_CUSTOM_MODE_VALUE;
        Custom.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        Custom.color_mode                   = MODE_COLORS_PER_LED;
        Custom.brightness_min               = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN;
        Custom.brightness_max               = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX;
        Custom.brightness                   = GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX/2;
        modes.push_back(Custom);
    }

    SetupZones();
}

RGBController_GigabyteAorusLaptop::~RGBController_GigabyteAorusLaptop()
{
    delete controller;
}

void RGBController_GigabyteAorusLaptop::SetupZones()
{
    /*---------------------------------------------------------*\
    | Main zone 1 LED only                                      |
    \*---------------------------------------------------------*/
    zone new_zone;

    switch(dev_type)
    {
    case GIGABYTE_AORUS_LAPTOP_KEYBOARD_TYPE:
        new_zone.name   = "Keyboard";
        break;
    case GIGABYTE_AORUS_LAPTOP_BACKLIGHT_TYPE:
        new_zone.name   = "Backlight";
        break;
    default:
        new_zone.name   = "Unknonw";
        break;
    }

    new_zone.type       = ZONE_TYPE_SINGLE;
    new_zone.leds_min   = 1;
    new_zone.leds_max   = 1;
    new_zone.leds_count = 1;
    new_zone.matrix_map = nullptr;

    zones.push_back(new_zone);

    led new_led;
    new_led.name = "LED";
    leds.push_back(new_led);

    /*---------------------------------------------------------*\
    | Adding an extra zone for the keyboard real layout         |
    \*---------------------------------------------------------*/
    if(dev_type == GIGABYTE_AORUS_LAPTOP_KEYBOARD_TYPE)
    {
        /*-----------------------------------------*\
        |  Create the zone                          |
        \*-----------------------------------------*/
        unsigned int zone_size = 0;

        zone keyboard_zone;
        keyboard_zone.name               = "Keyboard layout";
        keyboard_zone.type               = ZONE_TYPE_MATRIX;

        keyboard_zone.matrix_map         = new matrix_map_type;
        keyboard_zone.matrix_map->height = aorus_laptop_default_keyboard_layout.height;
        keyboard_zone.matrix_map->width  = aorus_laptop_default_keyboard_layout.width;

        keyboard_zone.matrix_map->map    = new unsigned int[aorus_laptop_default_keyboard_layout.height * aorus_laptop_default_keyboard_layout.width];

        for(unsigned int h = 0; h < aorus_laptop_default_keyboard_layout.height; h++)
        {
            for(unsigned int w = 0; w < aorus_laptop_default_keyboard_layout.width; w++)
            {
                unsigned int key = aorus_laptop_default_keyboard_layout.matrix_map[h][w];
                keyboard_zone.matrix_map->map[h * aorus_laptop_default_keyboard_layout.width + w] = key;

                if(key != NA)
                {
                    led new_led;
                    new_led.name = aorus_laptop_default_keyboard_layout.led_names[key];
                    leds.push_back(new_led);
                    zone_size++;
                }
            }
        }

        keyboard_zone.leds_min           = zone_size;
        keyboard_zone.leds_max           = zone_size;
        keyboard_zone.leds_count         = zone_size;

        zones.push_back(keyboard_zone);
    }

    SetupColors();
}

void RGBController_GigabyteAorusLaptop::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_GigabyteAorusLaptop::DeviceUpdateLEDs()
{
    /*---------------------------------------------------------*\
    | This device supports direct mode per LED for main zone    |
    | only                                                      |
    \*---------------------------------------------------------*/
    if(modes[active_mode].value == GIGABYTE_AORUS_LAPTOP_DIRECT_MODE_VALUE)
    {
        controller->SetDirect(modes[active_mode].brightness, colors[0]);
    }

    /*---------------------------------------------------------*\
    | This device supports custom mode per LED for the layout   |
    | zone only, this isnt a direct mode                        |
    | Skip first color from colors array (that's the first zone |
    \*---------------------------------------------------------*/
    else if(modes[active_mode].value == GIGABYTE_AORUS_LAPTOP_CUSTOM_MODE_VALUE)
    {
        std::vector<RGBColor> layout_zone_colors;

        for(unsigned int i = 1; i < colors.size(); i++)
        {
            layout_zone_colors.push_back(colors[i]);
        }

        controller->SetCustom(layout_zone_colors, aorus_laptop_default_keyboard_layout.led_sequence_positions, modes[active_mode].brightness);
    }
}

void RGBController_GigabyteAorusLaptop::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GigabyteAorusLaptop::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GigabyteAorusLaptop::DeviceUpdateMode()
{
    const mode& current_mode = modes[active_mode];

    /*---------------------------------------------------------*\
    | Redirect direct and custom mode to per led handler        |
    \*---------------------------------------------------------*/
    if(current_mode.value == GIGABYTE_AORUS_LAPTOP_DIRECT_MODE_VALUE || current_mode.value == GIGABYTE_AORUS_LAPTOP_CUSTOM_MODE_VALUE)
    {
        return DeviceUpdateLEDs();
    }

    /*---------------------------------------------------------*\
    | Hardware modes update                                     |
    \*---------------------------------------------------------*/
    unsigned char brightness = current_mode.colors[0] == 0 ? 0 : current_mode.brightness; // handles black color (not indexed)
    controller->SetMode(current_mode.value, current_mode.speed, brightness, current_mode.direction + 1, current_mode.colors[0]);
}
