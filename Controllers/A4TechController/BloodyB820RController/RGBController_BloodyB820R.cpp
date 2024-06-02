/*---------------------------------------------------------*\
| RGBController_BloodyB820R.cpp                             |
|                                                           |
|   RGBController for A4Tech Bloody B820R Keyboard          |
|                                                           |
|   Mohammed Julfikar Ali Mahbub (o-julfikar)   01 Apr 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_BloodyB820R.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][21] =
    {
        {0,  NA, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, NA, NA,  NA,  NA,  13, 14,  15},
        {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  32,  33,  34, 35,  36},
        {37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,  53,  54,  55, 56,  57},
        {58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, NA, NA, NA,  NA,  71,  72, 73,  NA},
        {74, NA, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, NA, NA, 86,  NA,  87,  88, 89,  90},
        {91, 92, 93, 94, NA, NA, NA, NA, NA, NA, 95, 96, 97, 98, 99, 100, 101, 102, NA, 103, NA},
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
        KEY_EN_F10,
        KEY_EN_F11,
        KEY_EN_F12,
        KEY_EN_PRINT_SCREEN,
        KEY_EN_SCROLL_LOCK,
        KEY_EN_PAUSE_BREAK,

        KEY_EN_BACK_TICK,           //10
        KEY_EN_1,
        KEY_EN_2,
        KEY_EN_3,
        KEY_EN_4,
        KEY_EN_5,
        KEY_EN_6,
        KEY_EN_7,
        KEY_EN_8,
        KEY_EN_9,
        KEY_EN_0,
        KEY_EN_MINUS,
        KEY_EN_EQUALS,
        KEY_EN_BACKSPACE,
        KEY_EN_INSERT,
        KEY_EN_HOME,
        KEY_EN_PAGE_UP,
        KEY_EN_NUMPAD_LOCK,
        KEY_EN_NUMPAD_DIVIDE,
        KEY_EN_NUMPAD_TIMES,
        KEY_EN_NUMPAD_MINUS,

        KEY_EN_TAB,                 //20
        KEY_EN_Q,
        KEY_EN_W,
        KEY_EN_E,
        KEY_EN_R,
        KEY_EN_T,
        KEY_EN_Y,
        KEY_EN_U,
        KEY_EN_I,
        KEY_EN_O,
        KEY_EN_P,
        KEY_EN_LEFT_BRACKET,
        KEY_EN_RIGHT_BRACKET,
        KEY_EN_ANSI_BACK_SLASH,
        KEY_EN_DELETE,
        KEY_EN_END,
        KEY_EN_PAGE_DOWN,
        KEY_EN_NUMPAD_7,
        KEY_EN_NUMPAD_8,
        KEY_EN_NUMPAD_9,
        KEY_EN_NUMPAD_PLUS,

        KEY_EN_CAPS_LOCK,           //30
        KEY_EN_A,
        KEY_EN_S,
        KEY_EN_D,
        KEY_EN_F,
        KEY_EN_G,
        KEY_EN_H,
        KEY_EN_J,
        KEY_EN_K,
        KEY_EN_L,
        KEY_EN_SEMICOLON,
        KEY_EN_QUOTE,
        KEY_EN_ANSI_ENTER,
        KEY_EN_NUMPAD_4,
        KEY_EN_NUMPAD_5,
        KEY_EN_NUMPAD_6,

        KEY_EN_LEFT_SHIFT,          //40
        KEY_EN_Z,
        KEY_EN_X,
        KEY_EN_C,
        KEY_EN_V,
        KEY_EN_B,
        KEY_EN_N,
        KEY_EN_M,
        KEY_EN_COMMA,
        KEY_EN_PERIOD,
        KEY_EN_FORWARD_SLASH,
        KEY_EN_RIGHT_SHIFT,
        KEY_EN_UP_ARROW,
        KEY_EN_NUMPAD_1,
        KEY_EN_NUMPAD_2,
        KEY_EN_NUMPAD_3,
        KEY_EN_NUMPAD_ENTER,

        KEY_EN_LEFT_CONTROL,        //50
        KEY_EN_LEFT_WINDOWS,
        KEY_EN_LEFT_ALT,
        KEY_EN_SPACE,
        KEY_EN_RIGHT_ALT,
        KEY_EN_RIGHT_FUNCTION,
        KEY_EN_MENU,
        KEY_EN_RIGHT_CONTROL,
        KEY_EN_LEFT_ARROW,
        KEY_EN_DOWN_ARROW,
        KEY_EN_RIGHT_ARROW,
        KEY_EN_NUMPAD_0,
        KEY_EN_NUMPAD_PERIOD
    };

/**------------------------------------------------------------------*\
    @name A4Tech Bloody B820R
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectBloodyB820R
    @comment The A4Tech Bloody B820R keyboard controller currently
        supports the full size (ANSI layout).
\*-------------------------------------------------------------------*/

RGBController_BloodyB820R::RGBController_BloodyB820R(BloodyB820RController *controller_ptr)
{
    controller                  = controller_ptr;

    name                        = "Bloody B820R";
    vendor                      = "A4Tech";
    type                        = DEVICE_TYPE_KEYBOARD;
    description                 = "A4Tech Bloody Keyboard";
    serial                      = controller->GetSerial();
    location                    = controller->GetLocation();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = BLOODY_B820R_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_BloodyB820R::~RGBController_BloodyB820R()
{
    delete controller;
}

void RGBController_BloodyB820R::SetupZones()
{
    /*-------------------------------------------------*\
    | Create the Keyboard zone and add the matrix map   |
    \*-------------------------------------------------*/
    zone KB_zone;
    KB_zone.name                = ZONE_EN_KEYBOARD;
    KB_zone.type                = ZONE_TYPE_MATRIX;
    KB_zone.leds_min            = BLOODY_B820R_KEYCOUNT;
    KB_zone.leds_max            = BLOODY_B820R_KEYCOUNT;
    KB_zone.leds_count          = BLOODY_B820R_KEYCOUNT;

    KB_zone.matrix_map          = new matrix_map_type;
    KB_zone.matrix_map->height  = 6;
    KB_zone.matrix_map->width   = 21;
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
    for(unsigned int led_index = 0; led_index < BLOODY_B820R_KEYCOUNT; led_index++)
    {
        led new_led;
        new_led.name        = led_names[led_index];
        new_led.value       = led_index;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_BloodyB820R::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}


void RGBController_BloodyB820R::DeviceUpdateLEDs()
{
    controller->SetLEDDirect(colors);
}

void RGBController_BloodyB820R::UpdateZoneLEDs(int zone)
{
    std::vector<RGBColor> colour;

    for(size_t i = 0; i < zones[zone].leds_count; i++)
    {
        colour.push_back(zones[zone].colors[i]);
    }

    controller->SetLEDDirect(colour);
}

void RGBController_BloodyB820R::UpdateSingleLED(int led)
{
    std::vector<RGBColor> colour;
    colour.push_back(colors[led]);

    controller->SetLEDDirect(colour);
}

void RGBController_BloodyB820R::DeviceUpdateMode()
{
    /* This device does not support modes yet */
}
