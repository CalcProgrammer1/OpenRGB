/*---------------------------------------------------------*\
| RGBController_WootingKeyboard.cpp                         |
|                                                           |
|   RGBController for Wooting keyboard                      |
|                                                           |
|   Diogo Trindade (diogotr7)                   04 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_WootingKeyboard.h"
#include "LogManager.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA 0xFFFFFFFF
#define RGB_RAW_BUFFER_SIZE 96

static unsigned int matrix_map_tkl[6][17] =
{
    { 0,  NA,  12,  18,  24,  30,  36,  42,  48,  54,  60,  66,  72,  78,  84,  90,  96},
    { 1,   7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  67,  73,  79,  85,  91,  97},
    { 2,   8,  14,  20,  26,  32,  38,  44,  50,  56,  62,  68,  74,  80,  86,  92,  98},
    { 3,   9,  15,  21,  27,  33,  39,  45,  51,  57,  63,  69,  75,  81,  NA,  NA,  NA},
    { 4,  10,  16,  22,  28,  34,  40,  46,  52,  58,  64,  70,  NA,  82,  NA,  94,  NA},
    { 5,  11,  17,  NA,  NA,  NA,  41,  NA,  NA,  NA,  65,  71,  77,  83,  89,  95,  101}
};

static unsigned int matrix_map_80HE[6][17] =
{
    { 0,   6,  12,  18,  24,  30,  36,  42,  48,  NA,  60,  66,  72,  78,  84,  90,  100},
    { 1,   7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  67,  73,  79,  85,  91,  97},
    { 2,   8,  14,  20,  26,  32,  38,  44,  50,  56,  62,  68,  74,  80,  86,  92,  98},
    { 3,   9,  15,  21,  27,  33,  39,  45,  51,  57,  63,  69,  75,  81,  87,  NA,  NA},
    { 4,  10,  16,  22,  28,  34,  40,  46,  52,  58,  64,  70,  NA,  82,  88,  94,  NA},
    { 5,  11,  17,  NA,  NA,  NA,  41,  NA,  NA,  NA,  65,  71,  77,  NA,  89,  95,  101}
};

static unsigned int matrix_map_full[6][21] =
{
    {  0,  NA,  12,  18,  24,  30,  36,  42,  48,  54,  60,  66,  72,  78,  84,  90, 100, 102, 108, 114, 120},
    {  1,   7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  67,  73,  79,  85,  91,  97, 103, 109, 115, 121},
    {  2,   8,  14,  20,  26,  32,  38,  44,  50,  56,  62,  68,  74,  80,  86,  92,  98, 104, 110, 116, 122},
    {  3,   9,  15,  21,  27,  33,  39,  45,  51,  57,  63,  69,  75,  81,  NA,  NA,  NA, 105, 111, 117,  NA},
    {  4,  10,  16,  22,  28,  34,  40,  46,  52,  58,  64,  70,  NA,  82,  NA,  94,  NA, 106, 112, 118, 124},
    {  5,  11,  17,  NA,  NA,  NA,  41,  NA,  NA,  NA,  65,  71,  77,  83,  89,  95, 101,  NA, 113, 119,  NA}
};

static const unsigned int zone_sizes[] =
{
    102,
    126,
    102
};

static const char *led_names_default[] =
{
    KEY_EN_ESCAPE,
    KEY_EN_BACK_TICK,
    KEY_EN_TAB,
    KEY_EN_CAPS_LOCK,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_UNUSED,
    KEY_EN_1,
    KEY_EN_Q,
    KEY_EN_A,
    KEY_EN_ISO_BACK_SLASH,  //iso key - 10
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_F1,
    KEY_EN_2,
    KEY_EN_W,
    KEY_EN_S,
    KEY_EN_Z,
    KEY_EN_LEFT_ALT,
    KEY_EN_F2,
    KEY_EN_3,
    KEY_EN_E,               //20
    KEY_EN_D,
    KEY_EN_X,
    KEY_EN_UNUSED,          //space
    KEY_EN_F3,
    KEY_EN_4,
    KEY_EN_R,
    KEY_EN_F,
    KEY_EN_C,
    KEY_EN_UNUSED,          //space
    KEY_EN_F4,              //30
    KEY_EN_5,
    KEY_EN_T,
    KEY_EN_G,
    KEY_EN_V,
    KEY_EN_UNUSED,          //space
    KEY_EN_F5,
    KEY_EN_6,
    KEY_EN_Y,
    KEY_EN_H,
    KEY_EN_B,               //40
    KEY_EN_SPACE,
    KEY_EN_F6,
    KEY_EN_7,
    KEY_EN_U,
    KEY_EN_J,
    KEY_EN_N,
    KEY_EN_UNUSED,          //space
    KEY_EN_F7,
    KEY_EN_8,
    KEY_EN_I,               //50
    KEY_EN_K,
    KEY_EN_M,
    KEY_EN_UNUSED,          //space
    KEY_EN_F8,
    KEY_EN_9,
    KEY_EN_O,
    KEY_EN_L,
    KEY_EN_COMMA,
    KEY_EN_UNUSED,          //space
    KEY_EN_F9,              //60
    KEY_EN_0,
    KEY_EN_P,
    KEY_EN_SEMICOLON,
    KEY_EN_PERIOD,
    KEY_EN_RIGHT_ALT,
    KEY_EN_F10,
    KEY_EN_MINUS,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_QUOTE,
    KEY_EN_FORWARD_SLASH,      //70
    KEY_EN_RIGHT_WINDOWS,
    KEY_EN_F11,
    KEY_EN_EQUALS,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_POUND,           //iso only
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_F12,
    KEY_EN_BACKSPACE,
    KEY_EN_ANSI_BACK_SLASH,      //80
    KEY_EN_ANSI_ENTER,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_PRINT_SCREEN,
    KEY_EN_INSERT,
    KEY_EN_DELETE,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_LEFT_ARROW,
    KEY_EN_PAUSE_BREAK,      //90
    KEY_EN_HOME,
    KEY_EN_END,
    KEY_EN_UNUSED,
    KEY_EN_UP_ARROW,
    KEY_EN_DOWN_ARROW,
    "Key: Mode",
    KEY_EN_PAGE_UP,
    KEY_EN_PAGE_DOWN,
    KEY_EN_UNUSED,
    KEY_EN_SCROLL_LOCK,      //100 - Scroll lock for WootingTwo KB's
    KEY_EN_RIGHT_ARROW,
    "Key: A1",
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_1,
    KEY_EN_UNUSED,
    "Key: A2",
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_8,        //110
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_2,
    KEY_EN_NUMPAD_0,
    "Key: A3",
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_PERIOD,
    "Key: Mode",            //120 - Mode key for WootingTwo KB's
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_UNUSED,
    KEY_EN_NUMPAD_ENTER,
    KEY_EN_UNUSED
};

static const char *led_names_80HE[] =
{
    KEY_EN_ESCAPE,
    KEY_EN_BACK_TICK,
    KEY_EN_TAB,
    KEY_EN_CAPS_LOCK,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_F1,
    KEY_EN_1,
    KEY_EN_Q,
    KEY_EN_A,
    KEY_EN_ISO_BACK_SLASH,  //iso key - 10
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_F2,
    KEY_EN_2,
    KEY_EN_W,
    KEY_EN_S,
    KEY_EN_Z,
    KEY_EN_LEFT_ALT,
    KEY_EN_F3,
    KEY_EN_3,
    KEY_EN_E,               //20
    KEY_EN_D,
    KEY_EN_X,
    KEY_EN_UNUSED,          //space
    KEY_EN_F4,
    KEY_EN_4,
    KEY_EN_R,
    KEY_EN_F,
    KEY_EN_C,
    KEY_EN_UNUSED,          //space
    KEY_EN_F5,              //30
    KEY_EN_5,
    KEY_EN_T,
    KEY_EN_G,
    KEY_EN_V,
    KEY_EN_UNUSED,          //space
    KEY_EN_F6,
    KEY_EN_6,
    KEY_EN_Y,
    KEY_EN_H,
    KEY_EN_B,               //40
    KEY_EN_SPACE,
    KEY_EN_F7,
    KEY_EN_7,
    KEY_EN_U,
    KEY_EN_J,
    KEY_EN_N,
    KEY_EN_UNUSED,          //space
    KEY_EN_F8,
    KEY_EN_8,
    KEY_EN_I,               //50
    KEY_EN_K,
    KEY_EN_M,
    KEY_EN_UNUSED,          //space
    KEY_EN_UNUSED,
    KEY_EN_9,
    KEY_EN_O,
    KEY_EN_L,
    KEY_EN_COMMA,
    KEY_EN_UNUSED,          //space
    KEY_EN_F9,              //60
    KEY_EN_0,
    KEY_EN_P,
    KEY_EN_SEMICOLON,
    KEY_EN_PERIOD,
    KEY_EN_RIGHT_ALT,
    KEY_EN_F10,
    KEY_EN_MINUS,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_QUOTE,
    KEY_EN_FORWARD_SLASH,      //70
    KEY_EN_RIGHT_WINDOWS,
    KEY_EN_F11,
    KEY_EN_EQUALS,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_POUND,           //iso only
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_F12,
    KEY_EN_UNUSED,
    KEY_EN_ANSI_BACK_SLASH,      //80
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_CONTROL,
    "Key: Mode",
    KEY_EN_BACKSPACE,
    KEY_EN_UNUSED,
    KEY_EN_ANSI_ENTER,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_LEFT_ARROW,
    KEY_EN_PRINT_SCREEN,      //90
    KEY_EN_HOME,
    KEY_EN_END,
    KEY_EN_UNUSED,
    KEY_EN_UP_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_UNUSED,
    KEY_EN_PAGE_UP,
    KEY_EN_PAGE_DOWN,
    KEY_EN_UNUSED,
    KEY_EN_PAUSE_BREAK,      //100 - Scroll lock for WootingTwo KB's
    KEY_EN_RIGHT_ARROW
};

/**------------------------------------------------------------------*\
    @name Wooting Keyboards
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectWootingOneKeyboardControllers,DetectWootingTwoKeyboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_WootingKeyboard::RGBController_WootingKeyboard(WootingKeyboardController* controller_ptr)
{
    controller          = controller_ptr;

    LOG_DEBUG("%sAdding meta data", WOOTING_CONTROLLER_NAME);
    name                = controller->GetName();
    vendor              = controller->GetVendor();
    type                = DEVICE_TYPE_KEYBOARD;
    description         = controller->GetDescription();
    location            = controller->GetLocation();
    serial              = controller->GetSerial();

    LOG_DEBUG("%sAdding modes", WOOTING_CONTROLLER_NAME);
    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0xFFFF;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_WootingKeyboard::~RGBController_WootingKeyboard()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for (unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if (zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_WootingKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    uint8_t         wooting_type    = controller->GetWootingType();
    unsigned int    total_led_count = zone_sizes[wooting_type];

    LOG_DEBUG("%sCreating New Zone", WOOTING_CONTROLLER_NAME);
    zone new_zone;

    new_zone.name                   = name.append(" zone");
    new_zone.type                   = ZONE_TYPE_MATRIX;
    new_zone.leds_min               = total_led_count;
    new_zone.leds_max               = total_led_count;
    new_zone.leds_count             = total_led_count;
    new_zone.matrix_map             = new matrix_map_type;
    new_zone.matrix_map->height     = WOOTING_RGB_ROWS;

    if(wooting_type == WOOTING_KB_TKL)
    {
        new_zone.matrix_map->width  =  WOOTING_ONE_RGB_COLUMNS;
        new_zone.matrix_map->map    = (unsigned int *)&matrix_map_tkl;
    }
    else if(wooting_type == WOOTING_80HE)
    {
        new_zone.matrix_map->width  =  WOOTING_ONE_RGB_COLUMNS;
        new_zone.matrix_map->map    = (unsigned int *)&matrix_map_80HE;
    }
    else
    {
        new_zone.matrix_map->width  =  WOOTING_TWO_RGB_COLUMNS;
        new_zone.matrix_map->map    = (unsigned int *)&matrix_map_full;
    }

    zones.push_back(new_zone);

    LOG_DEBUG("%sCreating LED array - total_led_count %03i", WOOTING_CONTROLLER_NAME, total_led_count);

    static const char **led_names = (wooting_type == WOOTING_80HE) ? led_names_80HE : led_names_default;
    for (unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;

        new_led.name                = led_names[led_idx];

        LOG_DEBUG("%sPushing LED %03i - %s into vector", WOOTING_CONTROLLER_NAME, led_idx, new_led.name.c_str());
        leds.push_back(new_led);
    }

    LOG_DEBUG("%sLEDs created - Initialising Colours", WOOTING_CONTROLLER_NAME);

    SetupColors();
}

void RGBController_WootingKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_WootingKeyboard::DeviceUpdateLEDs()
{
    controller->SendDirect(&colors[0], (uint8_t)colors.size());
}

void RGBController_WootingKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WootingKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WootingKeyboard::DeviceUpdateMode()
{
}
