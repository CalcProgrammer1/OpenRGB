/*---------------------------------------------------------*\
| RGBController_DuckyKeyboard.cpp                           |
|                                                           |
|   RGBController for Ducky keyboard                        |
|                                                           |
|   Adam Honse (CalcProgrammer1)                04 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_DuckyKeyboard.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   0,  NA,  12,  18,  24,  30,  NA,  42,  48,  54,  60,  NA,  66,  72,  78,  84,  90,  96, 102, 108, 114, 120, 126 },
      {   1,   7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  NA,  67,  73,  85,  NA,  91,  97, 103, 109, 115, 121, 127 },
      {   2,  NA,   8,  14,  20,  26,  NA,  32,  38,  44,  50,  56,  62,  68,  74,  86,  92,  98, 104, 110, 116, 122, 128 },
      {   3,  NA,   9,  15,  21,  27,  NA,  33,  39,  45,  51,  57,  63,  69,  75,  87,  NA,  NA,  NA, 111, 117, 123,  NA },
      {   4,  10,  16,  22,  28,  34,  NA,  40,  NA,  46,  52,  58,  64,  70,  82,  NA,  NA, 100,  NA, 112, 118, 124, 131 },
      {   5,  11,  17,  NA,  NA,  NA,  NA,  41,  NA,  NA,  NA,  NA,  65,  77,  83,  89,  95, 101, 107, 113,  NA, 125,  NA } };

static unsigned int matrix_map_tkl[6][19] =
    { {   0,  NA,  12,  18,  24,  30,  NA,  42,  48,  54,  60,  NA,  66,  72,  78,  84,  90,  96, 102 },
      {   1,   7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  NA,  67,  73,  85,  NA,  91,  97, 103 },
      {   2,  NA,   8,  14,  20,  26,  NA,  32,  38,  44,  50,  56,  62,  68,  74,  86,  92,  98, 104 },
      {   3,  NA,   9,  15,  21,  27,  NA,  33,  39,  45,  51,  57,  63,  69,  75,  87,  NA,  NA,  NA },
      {   4,  10,  16,  22,  28,  34,  NA,  40,  NA,  46,  52,  58,  64,  70,  82,  NA,  NA, 100,  NA },
      {   5,  11,  17,  NA,  NA,  NA,  NA,  41,  NA,  NA,  NA,  NA,  65,  77,  83,  89,  95, 101, 107 } };


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
    132
};

static const unsigned int zone_sizes_tkl[] =
{
    108
};

static const char *led_names[] =
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
    KEY_EN_ISO_BACK_SLASH,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_F1,
    KEY_EN_2,
    KEY_EN_W,
    KEY_EN_S,
    KEY_EN_Z,
    KEY_EN_LEFT_ALT,
    KEY_EN_F2,
    KEY_EN_3,
    KEY_EN_E,
    KEY_EN_D,
    KEY_EN_X,
    KEY_EN_UNUSED,
    KEY_EN_F3,
    KEY_EN_4,
    KEY_EN_R,
    KEY_EN_F,
    KEY_EN_C,
    KEY_EN_UNUSED,
    KEY_EN_F4,
    KEY_EN_5,
    KEY_EN_T,
    KEY_EN_G,
    KEY_EN_V,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_6,
    KEY_EN_Y,
    KEY_EN_H,
    KEY_EN_B,
    KEY_EN_SPACE,
    KEY_EN_F5,
    KEY_EN_7,
    KEY_EN_U,
    KEY_EN_J,
    KEY_EN_N,
    KEY_EN_UNUSED,
    KEY_EN_F6,
    KEY_EN_8,
    KEY_EN_I,
    KEY_EN_K,
    KEY_EN_M,
    KEY_EN_UNUSED,
    KEY_EN_F7,
    KEY_EN_9,
    KEY_EN_O,
    KEY_EN_L,
    KEY_EN_COMMA,
    KEY_EN_UNUSED,
    KEY_EN_F8,
    KEY_EN_0,
    KEY_EN_P,
    KEY_EN_SEMICOLON,
    KEY_EN_PERIOD,
    KEY_EN_RIGHT_ALT,
    KEY_EN_F9,
    KEY_EN_MINUS,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_QUOTE,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_UNUSED,
    KEY_EN_F10,
    KEY_EN_EQUALS,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_POUND,
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_WINDOWS,
    KEY_EN_F11,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_F12,
    KEY_EN_BACKSPACE,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_ANSI_ENTER,
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_PRINT_SCREEN,
    KEY_EN_INSERT,
    KEY_EN_DELETE,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_LEFT_ARROW,
    KEY_EN_SCROLL_LOCK,
    KEY_EN_HOME,
    KEY_EN_END,
    KEY_EN_UNUSED,
    KEY_EN_UP_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_PAUSE_BREAK,
    KEY_EN_PAGE_UP,
    KEY_EN_PAGE_DOWN,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_ARROW,
    "Key: Calculator",
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_1,
    KEY_EN_NUMPAD_0,
    KEY_EN_MEDIA_MUTE,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_2,
    KEY_EN_UNUSED,
    KEY_EN_MEDIA_VOLUME_DOWN,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_PERIOD,
    KEY_EN_MEDIA_VOLUME_UP,
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_NUMPAD_ENTER,
};

/**------------------------------------------------------------------*\
    @name Ducky Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectDuckyKeyboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_DuckyKeyboard::RGBController_DuckyKeyboard(DuckyKeyboardController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetNameString();
    vendor      = "Ducky";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Ducky Keyboard Device";
    location    = controller->GetLocationString();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_DuckyKeyboard::~RGBController_DuckyKeyboard()
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

void RGBController_DuckyKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        unsigned int    zone_size       = 0;
        unsigned int    matrix_width    = 0;
        unsigned int*   matrix_map_ptr  = NULL;

        switch(controller->GetUSBPID())
        {
            case DUCKY_SHINE_7_ONE_2_RGB_PID:
                zone_size               = zone_sizes[zone_idx];
                matrix_width            = 23;
                matrix_map_ptr          = (unsigned int *)&matrix_map;
                break;

            case DUCKY_ONE_2_RGB_TKL_PID:
                zone_size               = zone_sizes_tkl[zone_idx];
                matrix_width            = 19;
                matrix_map_ptr          = (unsigned int *)&matrix_map_tkl;
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
        new_led.name = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_DuckyKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_DuckyKeyboard::DeviceUpdateLEDs()
{
    unsigned char colordata[155*3];

    for(std::size_t color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        colordata[(color_idx*3)+0] = RGBGetRValue(colors[color_idx]);
        colordata[(color_idx*3)+1] = RGBGetGValue(colors[color_idx]);
        colordata[(color_idx*3)+2] = RGBGetBValue(colors[color_idx]);
    }

    controller->SendColors(colordata, sizeof(colordata));
}

void RGBController_DuckyKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_DuckyKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_DuckyKeyboard::DeviceUpdateMode()
{

}
