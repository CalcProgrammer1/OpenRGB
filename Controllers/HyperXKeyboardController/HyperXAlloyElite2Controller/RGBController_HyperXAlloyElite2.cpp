/*---------------------------------------------------------*\
| RGBController_HyperXAlloyElite2.cpp                       |
|                                                           |
|   RGBController for HyperX Alloy Elite 2 keyboard         |
|                                                           |
|   KundaPanda (vojdo)                          02 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_HyperXAlloyElite2.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[8][22] =
{
    {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 104, 107, 108,  98,  NA,  NA },
    { 110, 111, 112, 113,  NA, 114, 115, 116,  NA, 117, 118, 119, 120,  NA, 121, 122, 123,  NA, 124, 125, 126, 127 },
    {   0,  12,  18,  23,  28,  NA,  34,  39,  44,  50,  56,  62,  66,  70,  NA,  76,  80,  85,  NA,  NA,  NA,  NA },
    {   1,   7,  13,  19,  24,  29,  35,  40,  45,  51,  57,  63,  67,  71,  NA,  77,  81,  86,  89,  94,  99, 105 },
    {   2,  NA,   8,  14,  20,  25,  30,  36,  41,  46,  52,  58,  64,  68,  72,  78,  82,  87,  90,  95, 100, 106 },
    {   3,  NA,   9,  15,  21,  26,  31,  37,  42,  47,  53,  59,  65,  69,  73,  NA,  NA,  NA,  91,  96, 101,  NA },
    {   4,   6,  NA,  10,  16,  22,  27,  32,  38,  43,  48,  54,  60,  74,  NA,  NA,  83,  NA,  92,  97, 102, 109 },
    {   5,  11,  17,  NA,  NA,  NA,  NA,  33,  NA,  NA,  NA,  49,  61,  55,  75,  79,  84,  88,  93,  NA, 103,  NA }
};

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
    128,
};


// ISO 6, 75, !80
// ANSI !6, !75, 80

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
    KEY_EN_MENU,
    KEY_EN_F9,
    KEY_EN_0,
    KEY_EN_P,
    KEY_EN_SEMICOLON,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_RIGHT_WINDOWS,
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
    // Skip index 76
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
    KEY_EN_MEDIA_MUTE, // Last multimedia key
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_PERIOD,
    KEY_EN_MEDIA_PREVIOUS, // First multimedia key
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_MEDIA_PLAY_PAUSE, // Second multimedia key
    KEY_EN_MEDIA_NEXT, // Third multimedia key
    KEY_EN_NUMPAD_ENTER,
    "RGB Strip 1",
    "RGB Strip 2",
    "RGB Strip 3",
    "RGB Strip 4",
    "RGB Strip 5",
    "RGB Strip 6",
    "RGB Strip 7",
    "RGB Strip 8",
    "RGB Strip 9",
    "RGB Strip 10",
    "RGB Strip 11",
    "RGB Strip 12",
    "RGB Strip 13",
    "RGB Strip 14",
    "RGB Strip 15",
    "RGB Strip 16",
    "RGB Strip 17",
    "RGB Strip 18",
};

/**------------------------------------------------------------------*\
    @name HyperX Alloy Elite 2
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectHyperXAlloyElite2
    @comment
\*-------------------------------------------------------------------*/

RGBController_HyperXAlloyElite2::RGBController_HyperXAlloyElite2(HyperXAlloyElite2Controller* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetNameString();
    vendor              = "HyperX";
    type                = DEVICE_TYPE_KEYBOARD;
    description         = "HyperX Alloy Elite 2 Keyboard Device";
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
    keepalive_thread_run = true;
    keepalive_thread = new std::thread(&RGBController_HyperXAlloyElite2::KeepaliveThreadFunction, this);
}

RGBController_HyperXAlloyElite2::~RGBController_HyperXAlloyElite2()
{
    keepalive_thread_run = false;
    keepalive_thread->join();
    delete keepalive_thread;

    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != nullptr)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_HyperXAlloyElite2::SetupZones()
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
            new_zone.matrix_map->height = 8;
            new_zone.matrix_map->width  = 22;
            new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
        }
        else
        {
            new_zone.matrix_map         = nullptr;
        }

        zones.push_back(new_zone);

        total_led_count += zone_sizes[zone_idx];
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name                    = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_HyperXAlloyElite2::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXAlloyElite2::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    if(active_mode == 0)
    {
        controller->SetLEDsDirect(colors);
    }
}

void RGBController_HyperXAlloyElite2::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyElite2::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyElite2::DeviceUpdateMode()
{

}

void RGBController_HyperXAlloyElite2::KeepaliveThreadFunction()
{
    while(keepalive_thread_run)
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(1000))
            {
                UpdateLEDs();
            }
        }
        std::this_thread::sleep_for(50ms);
    }
}
