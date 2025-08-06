/*---------------------------------------------------------*\
| RGBController_DasKeyboard.cpp                             |
|                                                           |
|   RGBController for Das Keyboard keyboard                 |
|                                                           |
|   Frank Niessen (denk_mal)                    16 Dec 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_DasKeyboard.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

// US Layout
static unsigned int matrix_map_us[7][21] =
    {
        {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,  NA,  NA,  NA},
        { 5, NA, 17, 23, 29, 35, 41, 47, 53, 59, 65, 71, 77, 83, 89, 95, 101, 127, 128, 129, 130},
        { 4, 10, 16, 22, 28, 34, 40, 46, 52, 58, 64, 70, 76, 82, 88, 94, 100, 106, 112, 118, 124},
        { 3,  9, 15, 21, 27, 33, 39, 45, 51, 57, 63, 69, 75,  7, 87, 93,  99, 105, 111, 117, 123},
        { 2,  8, 14, 20, 26, 32, 38, 44, 50, 56, 62, 68, NA, 80, NA, NA,  NA, 104, 110, 116,  NA},
        { 1, NA, 13, 19, 25, 31, 37, 43, 49, 55, 61, 67, 79, NA, NA, 91,  NA, 103, 109, 115, 122},
        { 0,  6, 12, NA, NA, NA, 36, NA, NA, NA, 60, 66, 72, 78, 84, 90,  96, 102,  NA, 114,  NA}
    };

// EU Layout
static unsigned int matrix_map_eu[7][21] =
    {
        {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA, 126,  NA,  NA,  NA},
        { 5, NA, 17, 23, 29, 35, 41, 47, 53, 59, 65, 71, 77, 83, 89, 95, 101, 127, 128, 129, 130},
        { 4, 10, 16, 22, 28, 34, 40, 46, 52, 58, 64, 70, 76, 82, 88, 94, 100, 106, 112, 118, 124},
        { 3,  9, 15, 21, 27, 33, 39, 45, 51, 57, 63, 69, 75, NA, 87, 93,  99, 105, 111, 117, 123},
        { 2,  8, 14, 20, 26, 32, 38, 44, 50, 56, 62, 68, 81, 80, NA, NA,  NA, 104, 110, 116,  NA},
        { 1,  7, 13, 19, 25, 31, 37, 43, 49, 55, 61, 67, 79, NA, NA, 91,  NA, 103, 109, 115, 122},
        { 0,  6, 12, NA, NA, NA, 36, NA, NA, NA, 60, 66, 72, 78, 84, 90,  96, 102,  NA, 114,  NA}
    };

static const char *zone_names[] =
    {
        ZONE_EN_KEYBOARD
    };

static zone_type zone_types[] =
    {
        ZONE_TYPE_MATRIX,
    };

static const unsigned int zone_sizes[] =
    {
        131
    };

// UK Layout
static const char *led_names[] =
    {
        KEY_EN_LEFT_CONTROL,
        KEY_EN_LEFT_SHIFT,
        KEY_EN_CAPS_LOCK,
        KEY_EN_TAB,
        KEY_EN_BACK_TICK,
        KEY_EN_ESCAPE,
        KEY_EN_LEFT_WINDOWS,
        KEY_EN_ANSI_BACK_SLASH,
        KEY_EN_A,
        KEY_EN_Q,
        KEY_EN_1,
        KEY_EN_UNUSED,
        KEY_EN_LEFT_ALT,
        KEY_EN_Z,
        KEY_EN_S,
        KEY_EN_W,
        KEY_EN_2,
        KEY_EN_F1,
        KEY_EN_UNUSED,
        KEY_EN_X,
        KEY_EN_D,
        KEY_EN_E,
        KEY_EN_3,
        KEY_EN_F2,
        KEY_EN_UNUSED,
        KEY_EN_C,
        KEY_EN_F,
        KEY_EN_R,
        KEY_EN_4,
        KEY_EN_F3,
        KEY_EN_UNUSED,
        KEY_EN_V,
        KEY_EN_G,
        KEY_EN_T,
        KEY_EN_5,
        KEY_EN_F4,
        KEY_EN_SPACE,
        KEY_EN_B,
        KEY_EN_H,
        KEY_EN_Y,
        KEY_EN_6,
        KEY_EN_F5,
        KEY_EN_UNUSED,
        KEY_EN_N,
        KEY_EN_J,
        KEY_EN_U,
        KEY_EN_7,
        KEY_EN_F6,
        KEY_EN_UNUSED,
        KEY_EN_M,
        KEY_EN_K,
        KEY_EN_I,
        KEY_EN_8,
        KEY_EN_F7,
        KEY_EN_UNUSED,
        KEY_EN_COMMA,
        KEY_EN_L,
        KEY_EN_O,
        KEY_EN_9,
        KEY_EN_F8,
        KEY_EN_RIGHT_ALT,
        KEY_EN_PERIOD,
        KEY_EN_SEMICOLON,
        KEY_EN_P,
        KEY_EN_0,
        KEY_EN_F9,
        KEY_EN_RIGHT_WINDOWS,
        KEY_EN_FORWARD_SLASH,
        KEY_EN_QUOTE,
        KEY_EN_LEFT_BRACKET,
        KEY_EN_MINUS,
        KEY_EN_F10,
        KEY_EN_MENU,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_RIGHT_BRACKET,
        KEY_EN_EQUALS,
        KEY_EN_F11,
        KEY_EN_RIGHT_CONTROL,
        KEY_EN_RIGHT_SHIFT,
        KEY_EN_ANSI_ENTER,
        KEY_EN_POUND,
        KEY_EN_BACKSPACE,
        KEY_EN_F12,
        KEY_EN_LEFT_ARROW,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_DELETE,
        KEY_EN_INSERT,
        KEY_EN_PRINT_SCREEN,
        KEY_EN_DOWN_ARROW,
        KEY_EN_UP_ARROW,
        KEY_EN_UNUSED,
        KEY_EN_END,
        KEY_EN_HOME,
        KEY_EN_SCROLL_LOCK,
        KEY_EN_RIGHT_ARROW,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_PAGE_DOWN,
        KEY_EN_PAGE_UP,
        KEY_EN_PAUSE_BREAK,
        KEY_EN_NUMPAD_0,
        KEY_EN_NUMPAD_1,
        KEY_EN_NUMPAD_4,
        KEY_EN_NUMPAD_7,
        KEY_EN_NUMPAD_LOCK,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_NUMPAD_2,
        KEY_EN_NUMPAD_5,
        KEY_EN_NUMPAD_8,
        KEY_EN_NUMPAD_DIVIDE,
        KEY_EN_UNUSED,
        KEY_EN_NUMPAD_PERIOD,
        KEY_EN_NUMPAD_3,
        KEY_EN_NUMPAD_6,
        KEY_EN_NUMPAD_9,
        KEY_EN_NUMPAD_TIMES,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_NUMPAD_ENTER,
        KEY_EN_NUMPAD_PLUS,
        KEY_EN_NUMPAD_MINUS,
        KEY_EN_UNUSED,
        "Key: Sleep",
        "Key: Brightness",
        KEY_EN_MEDIA_PLAY_PAUSE,
        KEY_EN_MEDIA_NEXT,
        "Key: Q-Button"
    };

/**------------------------------------------------------------------*\
    @name Das Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectDasKeyboardControllers,DetectDas4QKeyboard
    @comment
\*-------------------------------------------------------------------*/

RGBController_DasKeyboard::RGBController_DasKeyboard(DasKeyboardController* controller_ptr)
{
    controller  = controller_ptr;

    for(unsigned int ii = 0; ii < zone_sizes[0]; ii++)
    {
        double_buffer.push_back(-1);
    }

    updateDevice = true;

    name        = controller->GetNameString();
    vendor      = "Metadot";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Das Keyboard Device";
    location    = controller->GetLocationString();
    serial      = controller->GetSerialString();
    version     = controller->GetVersionString();

    modes.resize(4);
    modes[0].name       = "Direct";
    modes[0].value      = DAS_KEYBOARD_MODE_DIRECT;
    modes[0].flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    modes[0].color_mode = MODE_COLORS_PER_LED;

    modes[1].name       = "Flashing";
    modes[1].value      = DAS_KEYBOARD_MODE_FLASHING;
    modes[1].flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    modes[1].color_mode = MODE_COLORS_PER_LED;

    modes[2].name       = "Breathing";
    modes[2].value      = DAS_KEYBOARD_MODE_BREATHING;
    modes[2].flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    modes[2].color_mode = MODE_COLORS_PER_LED;

    modes[3].name       = "Spectrum Cycle";
    modes[3].value      = DAS_KEYBOARD_MODE_SPECTRUM_CYCLE;
    modes[3].flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    modes[3].color_mode = MODE_COLORS_PER_LED;

    SetupZones();
}

RGBController_DasKeyboard::~RGBController_DasKeyboard()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    unsigned int zone_size = (unsigned int)zones.size();

    for(unsigned int zone_index = 0; zone_index < zone_size; zone_index++)
    {
        delete zones[zone_index].matrix_map;
    }

    delete controller;
}

void RGBController_DasKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;

    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        zone new_zone;
        new_zone.name               = zone_names[zone_idx];
        new_zone.type               = zone_types[zone_idx];
        new_zone.leds_min           = zone_sizes[zone_idx];
        new_zone.leds_max           = zone_sizes[zone_idx];
        new_zone.leds_count         = zone_sizes[zone_idx];
        new_zone.matrix_map         = new matrix_map_type;
        new_zone.matrix_map->height = 7;
        new_zone.matrix_map->width  = 21;

        if(controller->GetLayoutString() == "US")
        {
            new_zone.matrix_map->map = (unsigned int *) &matrix_map_us;
        }
        else
        {
            new_zone.matrix_map->map = (unsigned int *) &matrix_map_eu;
        }

        zones.push_back(new_zone);

        total_led_count += zone_sizes[zone_idx];
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name                = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_DasKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_DasKeyboard::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
}

void RGBController_DasKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    updateDevice = false;

    for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        UpdateSingleLED(static_cast<int>(led_idx));
    }

    updateDevice = true;

    controller->SendApply();
}

void RGBController_DasKeyboard::UpdateSingleLED(int led)
{
    mode selected_mode = modes[active_mode];

    if(double_buffer[led] == colors[led])
    {
        return;
    }

    controller->SendColors(led, selected_mode.value,
                           RGBGetRValue(colors[led]),
                           RGBGetGValue(colors[led]),
                           RGBGetBValue(colors[led]));

    double_buffer[led] = colors[led];

    if(updateDevice)
    {
        controller->SendApply();
    }
}

void RGBController_DasKeyboard::DeviceUpdateMode()
{
}
