/*---------------------------------------------------------*\
| RGBController_HyperXAlloyOrigins.cpp                      |
|                                                           |
|   RGBController for HyperX Alloy Origins keyboard         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_HyperXAlloyOrigins.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   0,  NA,  12,  18,  23,  28,  NA,  34,  39,  44,  50,  NA,  55,  61,  65,  71,  77,  81,  86,  NA,  NA,  NA,  NA },
      {   1,   7,  13,  19,  24,  29,  35,  40,  45,  51,  56,  NA,  62,  66,  71,  72,  78,  82,  87,  90,  95,  99, 104 },
      {   2,  NA,   8,  14,  20,  25,  NA,  30,  36,  41,  46,  52,  57,  63,  67,  73,  79,  83,  88,  91,  96, 100, 105 },
      {   3,  NA,   9,  15,  21,  26,  NA,  31,  37,  42,  47,  53,  58,  64,  68,  74,  NA,  NA,  NA,  92,  97, 101,  NA },
      {   4,  6,   10,  16,  22,  27,  NA,  32,  NA,  38,  43,  48,  54,  59,  69,  75,  NA,  84,  NA,  93,  98, 102, 106 },
      {   5,  11,  17,  NA,  NA,  NA,  NA,  33,  NA,  NA,  NA,  NA,  49,  60,  70,  76,  80,  85,  89,  94,  NA, 103,  NA } };

static const char* zone_names[] =
{
    ZONE_EN_KEYBOARD,
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const unsigned int zone_sizes[] =
{
    107,
};

static const char *led_names[] =
{
    KEY_EN_ESCAPE,
    KEY_EN_BACK_TICK,
    KEY_EN_TAB,
    KEY_EN_CAPS_LOCK,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_ISO_BACK_SLASH,
    KEY_EN_1,
    KEY_EN_Q,
    KEY_EN_A,
    KEY_EN_Z,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_F1,
    KEY_EN_2,
    KEY_EN_W,
    KEY_EN_S,
    KEY_EN_X,
    KEY_EN_LEFT_ALT,
    KEY_EN_F2,
    KEY_EN_3,
    KEY_EN_E,
    KEY_EN_D,
    KEY_EN_C,
    // Skip index 23
    KEY_EN_F3,
    KEY_EN_4,
    KEY_EN_R,
    KEY_EN_F,
    KEY_EN_V,
    // Skip index 29
    KEY_EN_F4,
    KEY_EN_5,
    KEY_EN_T,
    KEY_EN_G,
    KEY_EN_B,
    KEY_EN_SPACE,
    KEY_EN_F5,
    KEY_EN_6,
    KEY_EN_Y,
    KEY_EN_H,
    KEY_EN_N,
    // Skip index 41
    KEY_EN_F6,
    KEY_EN_7,
    KEY_EN_U,
    KEY_EN_J,
    KEY_EN_M,
    // Skip index 47
    KEY_EN_F7,
    KEY_EN_8,
    KEY_EN_I,
    KEY_EN_K,
    KEY_EN_COMMA,
    KEY_EN_RIGHT_ALT,
    KEY_EN_F8,
    KEY_EN_9,
    KEY_EN_O,
    KEY_EN_L,
    KEY_EN_PERIOD,
    // Skip index 59
    KEY_EN_F9,
    KEY_EN_0,
    KEY_EN_P,
    KEY_EN_SEMICOLON,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_F10,
    KEY_EN_MINUS,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_QUOTE,
    // Skip index 70
    // Skip index 71
    KEY_EN_F11,
    KEY_EN_EQUALS,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_POUND,
    "Key: / (ABNT)",
    KEY_EN_MENU,
    KEY_EN_F12,
    KEY_EN_BACKSPACE,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_ANSI_ENTER,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_PRINT_SCREEN,
    KEY_EN_INSERT,
    KEY_EN_DELETE,
    // Skip index 87
    // Skip index 88
    KEY_EN_LEFT_ARROW,
    KEY_EN_SCROLL_LOCK,
    KEY_EN_HOME,
    KEY_EN_END,
    // Skip index 93
    KEY_EN_UP_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_PAUSE_BREAK,
    KEY_EN_PAGE_UP,
    KEY_EN_PAGE_DOWN,
    // Skip index 99
    // Skip index 100
    KEY_EN_RIGHT_ARROW,
    // Skip index 102
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_1,
    KEY_EN_NUMPAD_0,
    // Skip index 108
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_2,
    // Skip index 113
    // Skip index 114
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_PERIOD,
    // Skip index 120
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_NUMPAD_PLUS,
    // Skip index 123
    // Skip index 124
    KEY_EN_NUMPAD_ENTER,
};

/**------------------------------------------------------------------*\
    @name HyperX Alloy Origins
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectHyperXAlloyOrigins
    @comment
\*-------------------------------------------------------------------*/

RGBController_HyperXAlloyOrigins::RGBController_HyperXAlloyOrigins(HyperXAlloyOriginsController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetNameString();
    vendor              = "HyperX";
    type                = DEVICE_TYPE_KEYBOARD;
    description         = "HyperX Alloy Origins Keyboard Device";
    location            = controller->GetDeviceLocation();
    serial              = controller->GetSerialString();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0xFFFF;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    /*-----------------------------------------------------*\
    | The Corsair Lighting Node Pro requires a packet within|
    | 20 seconds of sending the lighting change in order    |
    | to not revert back into rainbow mode.  Start a thread |
    | to continuously send a keepalive packet every 5s      |
    \*-----------------------------------------------------*/
    keepalive_thread_run = 1;
    keepalive_thread = new std::thread(&RGBController_HyperXAlloyOrigins::KeepaliveThread, this);
}

RGBController_HyperXAlloyOrigins::~RGBController_HyperXAlloyOrigins()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

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

void RGBController_HyperXAlloyOrigins::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        zone new_zone;
        new_zone.name                   = zone_names[zone_idx];
        new_zone.type                   = zone_types[zone_idx];
        new_zone.leds_min               = zone_sizes[zone_idx];
        new_zone.leds_max               = zone_sizes[zone_idx];
        new_zone.leds_count             = zone_sizes[zone_idx];

        if(zone_types[zone_idx] == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map         = new matrix_map_type;
            new_zone.matrix_map->height = 6;
            new_zone.matrix_map->width  = 23;
            new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
        }
        else
        {
            new_zone.matrix_map         = NULL;
        }

        zones.push_back(new_zone);

        total_led_count += zone_sizes[zone_idx];
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_HyperXAlloyOrigins::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXAlloyOrigins::DeviceUpdateLEDs()
{
    controller->SetLEDsDirect(colors);
}

void RGBController_HyperXAlloyOrigins::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOrigins::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOrigins::DeviceUpdateMode()
{

}

void RGBController_HyperXAlloyOrigins::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(50))
            {
                UpdateLEDs();
            }
        }
        std::this_thread::sleep_for(10ms);;
    }
}
