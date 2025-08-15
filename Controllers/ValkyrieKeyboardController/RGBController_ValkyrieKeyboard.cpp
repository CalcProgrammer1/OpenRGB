/*---------------------------------------------------------*\
| RGBController_ValkyrieKeyboard.cpp                        |
|                                                           |
|   RGBController for Valkyrie keyboard                     |
|                                                           |
|   Nollie (Nuonuo)                             06 Dec 2023 |
|   Bartholomew Ho (imnotmental)                01 Feb 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_ValkyrieKeyboard.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][22] =
    { {   0,  NA,   1,   2,   3,   4,  NA,   5,   6,   7,   8,  NA,   9,  10,  11,  12,  NA,  13,  NA,  NA,  NA,  NA },
      {  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  NA,  NA,  NA,  28,  29,  30,  31,  32 },
      {  33,  NA,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  NA,  NA,  47,  48,  49,  50,  51 },
      {  52,  NA,  NA,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  NA,  63,  64,  NA,  65,  66,  67,  68,  NA },
      {  69,  NA,  NA,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  NA,  NA,  81,  NA,  82,  83,  84,  85 },
      {  86,  87,  88,  NA,  NA,  NA,  NA,  89,  NA,  NA,  NA,  NA,  90,  91,  92,  93,  94,  95,  NA,  96,  97,  NA } };

static unsigned int normal_matrix_map[6][22] =
    {{   0,  NA,   1,   2,   3,   4,  NA,   5,   6,   7,   8,  NA,   9,  10,  11,  12,  NA,  13,  14,  15,  16,  17 },
     {  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  NA,  NA,  NA,  32,  33,  34,  35,  36 },
     {  37,  NA,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  NA,  NA,  51,  52,  53,  54,  55 },
     {  56,  NA,  NA,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  NA,  67,  68,  NA,  69,  70,  71,  72,  NA },
     {  73,  NA,  NA,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  NA,  NA,  85,  NA,  86,  87,  88,  89 },
     {  90,  91,  92,  NA,  NA,  NA,  NA,  93,  NA,  NA,  NA,  NA,  94,  95,  96,  97,  98,  99,  NA, 100, 101,  NA } };

static const char* zone_names[] =
{
    ZONE_EN_KEYBOARD
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const unsigned int zone_sizes[] =
{
    98
};

static const unsigned int normal_zone_sizes[] =
{
    102
};

static const char *led_names[] =
{
    KEY_EN_ESCAPE,
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
    KEY_EN_DELETE,
    KEY_EN_BACK_TICK,
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
    KEY_EN_PLUS,
    KEY_EN_BACKSPACE,
    KEY_EN_INSERT,
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_TAB,
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
    KEY_EN_BACK_SLASH,
    KEY_EN_PAGE_UP,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_CAPS_LOCK,
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
    KEY_EN_PAGE_DOWN,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_6,
    KEY_EN_LEFT_SHIFT,
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
    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    KEY_EN_SPACE,
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW,
    KEY_EN_NUMPAD_0,
    KEY_EN_NUMPAD_PERIOD
};

static const char *normal_led_names[] =
    {
        KEY_EN_ESCAPE,
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
        KEY_EN_DELETE,
        KEY_EN_PRINT_SCREEN,
        KEY_EN_PAUSE_BREAK,
        KEY_EN_HOME,
        KEY_EN_END,
        KEY_EN_BACK_TICK,
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
        KEY_EN_PLUS,
        KEY_EN_BACKSPACE,
        KEY_EN_INSERT,
        KEY_EN_NUMPAD_LOCK,
        KEY_EN_NUMPAD_DIVIDE,
        KEY_EN_NUMPAD_TIMES,
        KEY_EN_NUMPAD_MINUS,
        KEY_EN_TAB,
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
        KEY_EN_BACK_SLASH,
        KEY_EN_PAGE_UP,
        KEY_EN_NUMPAD_7,
        KEY_EN_NUMPAD_8,
        KEY_EN_NUMPAD_9,
        KEY_EN_NUMPAD_PLUS,
        KEY_EN_CAPS_LOCK,
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
        KEY_EN_PAGE_DOWN,
        KEY_EN_NUMPAD_4,
        KEY_EN_NUMPAD_5,
        KEY_EN_NUMPAD_6,
        KEY_EN_LEFT_SHIFT,
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
        KEY_EN_LEFT_CONTROL,
        KEY_EN_LEFT_WINDOWS,
        KEY_EN_LEFT_ALT,
        KEY_EN_SPACE,
        KEY_EN_RIGHT_ALT,
        KEY_EN_RIGHT_FUNCTION,
        KEY_EN_RIGHT_CONTROL,
        KEY_EN_LEFT_ARROW,
        KEY_EN_DOWN_ARROW,
        KEY_EN_RIGHT_ARROW,
        KEY_EN_NUMPAD_0,
        KEY_EN_NUMPAD_PERIOD
};
/**------------------------------------------------------------------*\
    @name Valkyrie
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectValkyrieKeyboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_ValkyrieKeyboard::RGBController_ValkyrieKeyboard(ValkyrieKeyboardController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetNameString();
    vendor              = "Valkyrie";
    type                = DEVICE_TYPE_KEYBOARD;
    description         = "Valkyrie Keyboard Device";
    location            = controller->GetDeviceLocation();
    serial              = controller->GetSerialString();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0xFFFF;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_ValkyrieKeyboard::~RGBController_ValkyrieKeyboard()
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
        break;
    }

    delete controller;
}

void RGBController_ValkyrieKeyboard::SetupZones()
{
    ValkyrieKeyboardMappingLayoutType layout;
    switch(controller->GetInterfaceNum())
    {
        case 3:
            layout = PRO_LAYOUT;
            break;
        default:
            layout = NORMAL_LAYOUT;
            break;
    }

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        unsigned int    zone_size       = 0;
        unsigned int    matrix_width    = 0;
        unsigned int*   matrix_map_ptr  = NULL;

        switch(layout)
        {
            case PRO_LAYOUT:
                zone_size               = zone_sizes[zone_idx];
                matrix_width            = 22;
                matrix_map_ptr          = (unsigned int *)&matrix_map;
                break;

            default:
                zone_size               = normal_zone_sizes[zone_idx];
                matrix_width            = 22;
                matrix_map_ptr          = (unsigned int *)&normal_matrix_map;
                break;
        }

        zone new_zone;
        new_zone.name                   = zone_names[zone_idx];
        new_zone.type                   = zone_types[zone_idx];
        new_zone.leds_min               = zone_size;
        new_zone.leds_max               = zone_size;
        new_zone.leds_count             = zone_size;
        new_zone.matrix_map             = new matrix_map_type;
        new_zone.matrix_map->height     = 6;
        new_zone.matrix_map->width      = matrix_width;
        new_zone.matrix_map->map        = matrix_map_ptr;
        zones.push_back(new_zone);

        total_led_count += zone_size;
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;

        switch(layout)
        {
            case PRO_LAYOUT:
                new_led.name = led_names[led_idx];
                break;
            default:
                new_led.name = normal_led_names[led_idx];
                break;
        }

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_ValkyrieKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ValkyrieKeyboard::DeviceUpdateLEDs()
{
    unsigned char colordata[1024];

    for(std::size_t color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        colordata[(color_idx*3)+0] = RGBGetRValue(colors[color_idx]);
        colordata[(color_idx*3)+1] = RGBGetGValue(colors[color_idx]);
        colordata[(color_idx*3)+2] = RGBGetBValue(colors[color_idx]);
    }

    controller->SendColors(colordata, sizeof(colordata));
}

void RGBController_ValkyrieKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ValkyrieKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ValkyrieKeyboard::DeviceUpdateMode()
{

}
