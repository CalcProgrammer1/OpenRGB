/*---------------------------------------------------------*\
| RGBController_CorsairWireless.cpp                         |
|                                                           |
|   RGBController for Corsair wireless keyboard             |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_CorsairWireless.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[7][24] =
    { {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,   0,   1,  NA,  NA,  NA },
      { 131,  41,  NA,  58,  59,  60,  61,  NA,  62,  63,  64,  65,  NA,  66,  67,  68,  69,  70,  71,  72,  NA,  NA,  NA,  NA },
      { 132,  53,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  NA,  45,  46,  42,  NA,  73,  74,  75,  83,  84,  85,  86 },
      { 133,  43,  NA,  20,  26,   8,  21,  NA,  23,  28,  24,  12,  18,  19,  47,  48,  49,  76,  77,  78,  95,  96,  97,  87 },
      { 134,  57,  NA,   4,  22,   7,   9,  NA,  10,  11,  13,  14,  15,  51,  52,  NA,  40,  NA,  NA,  NA,  92,  93,  94,  NA },
      { 135, 106,  NA,  29,  27,   6,  25,  NA,   5,  NA,  17,  16,  54,  55,  56, 110,  NA,  NA,  82,  NA,  89,  90,  91,  88 },
      { 136, 105, 108, 107,  NA,  NA,  NA,  NA,  44,  NA,  NA,  NA,  NA, 111, 122, 101, 109,  80,  81,  79,  98,  NA,  99,  NA } };

static const char* zone_names[] =
{
    ZONE_EN_KEYBOARD,
};

static const unsigned int zone_sizes[] =
{
    137
};

static const zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const char* led_names[] =
{
    "Power/Wireless Indicator",
    "Lock/Macro Indicator",
    "N/A",
    "N/A",
    KEY_EN_A,
    KEY_EN_B,
    KEY_EN_C,
    KEY_EN_D,
    KEY_EN_E,
    KEY_EN_F,
    KEY_EN_G,
    KEY_EN_H,
    KEY_EN_I,
    KEY_EN_J,
    KEY_EN_K,
    KEY_EN_L,
    KEY_EN_M,
    KEY_EN_N,
    KEY_EN_O,
    KEY_EN_P,
    KEY_EN_Q,
    KEY_EN_R,
    KEY_EN_S,
    KEY_EN_T,
    KEY_EN_U,
    KEY_EN_V,
    KEY_EN_W,
    KEY_EN_X,
    KEY_EN_Y,
    KEY_EN_Z,
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
    KEY_EN_ANSI_ENTER,
    KEY_EN_ESCAPE,
    KEY_EN_BACKSPACE,
    KEY_EN_TAB,
    KEY_EN_SPACE,
    KEY_EN_MINUS,
    KEY_EN_EQUALS,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_UNUSED,
    KEY_EN_SEMICOLON,
    KEY_EN_QUOTE,
    KEY_EN_BACK_TICK,
    KEY_EN_COMMA,
    KEY_EN_PERIOD,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_CAPS_LOCK,
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
    KEY_EN_INSERT,
    KEY_EN_HOME,
    KEY_EN_PAGE_UP,
    KEY_EN_DELETE,
    KEY_EN_END,
    KEY_EN_PAGE_DOWN,
    KEY_EN_RIGHT_ARROW,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_UP_ARROW,
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_NUMPAD_ENTER,
    KEY_EN_NUMPAD_1,
    KEY_EN_NUMPAD_2,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_0,
    KEY_EN_NUMPAD_PERIOD,
    KEY_EN_UNUSED,
    KEY_EN_MENU,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_LEFT_ALT,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_RIGHT_ALT,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    "Key: G1",
    "Key: G2",
    "Key: G3",
    "Key: G4",
    "Key: G5",
    "Key: G6",
};

/**------------------------------------------------------------------*\
    @name Corsair Wireless Peripheral
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairWirelessControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_CorsairWireless::RGBController_CorsairWireless(CorsairWirelessController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetName();
    vendor      = "Corsair";
    description = "Corsair RGB Peripheral Device";
    type        = controller->GetDeviceType();
    version     = controller->GetFirmwareString();
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    /*-----------------------------------------------------*\
    | The Corsair Lighting Node Pro requires a packet within|
    | 20 seconds of sending the lighting change in order    |
    | to not revert back into rainbow mode.  Start a thread |
    | to continuously send a keepalive packet every 5s      |
    \*-----------------------------------------------------*/
    keepalive_thread_run = true;
    keepalive_thread = new std::thread(&RGBController_CorsairWireless::KeepaliveThread, this);
}

RGBController_CorsairWireless::~RGBController_CorsairWireless()
{
    /*-----------------------------------------------------*\
    | Close keepalive thread                                |
    \*-----------------------------------------------------*/
    keepalive_thread_run = false;
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

void RGBController_CorsairWireless::SetupZones()
{
    /*---------------------------------------------------------*\
    | Determine number of zones                                 |
    |   For now, keyboard has 2 zones and mousemat has 1        |
    \*---------------------------------------------------------*/
    unsigned int num_zones = 1;

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < num_zones; zone_idx++)
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
            new_zone.matrix_map->height = 7;
            new_zone.matrix_map->width  = 24;
            new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
        }
        else
        {
            new_zone.matrix_map         = NULL;
        }

        zones.push_back(new_zone);
        total_led_count += new_zone.leds_count;
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;

        new_led.name                    = led_names[led_idx];

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_CorsairWireless::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairWireless::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    controller->SetLEDs(colors);
}

void RGBController_CorsairWireless::UpdateZoneLEDs(int /*zone*/)
{
    controller->SetLEDs(colors);
}

void RGBController_CorsairWireless::UpdateSingleLED(int /*led*/)
{
    controller->SetLEDs(colors);
}

void RGBController_CorsairWireless::DeviceUpdateMode()
{

}

void RGBController_CorsairWireless::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(5000))
            {
                DeviceUpdateLEDs();
            }
        }
        std::this_thread::sleep_for(1000ms);
    }
}
