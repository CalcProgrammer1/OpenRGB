/*---------------------------------------------------------*\
| RGBController_HyperXAlloyElite.cpp                        |
|                                                           |
|   RGBController for HyperX Alloy Elite keyboard           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                02 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_HyperXAlloyElite.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   0,  NA,  16,  30,  44,  54,  NA,  65,  75,  84,  95,  NA,   8,  23 ,  38,   6 ,  22,  36,  49,  NA,  NA,  NA,  NA },
      {   1,  17,  31,  45,  55,  66,  76,  85,  96,   9,  24,  NA,  39,   7 ,  37,  NA ,  60,  70,  80,  52,  63,  73,  82 },
      {   2,  NA,  18,  32,  46,  56,  NA,  67,  77,  86,  97,  10,  25,  40 ,  90,  101,  50,  61,  71,  51,  62,  72,  93 },
      {   3,  NA,  19,  33,  47,  57,  NA,  68,  78,  87,  98,  11,  26,  41 ,  28,  14 ,  NA,  NA,  NA,  92, 103,  53,  NA },
      {   4,  20,  34,  48,  58,  69,  NA,  79,  NA,  88,  99,  12,  27,  42 ,  81,  NA ,  NA, 102,  NA,  64,  74,  83, 104 },
      {   5,  21,  35,  NA,  NA,  NA,  NA,  59,  NA,  NA,  NA,  NA,  89,  100,  13,  91 ,  15,  29,  43,  94,  NA, 105,  NA } };

static const char* zone_names[] =
{
    ZONE_EN_KEYBOARD,
    "RGB Strip",
    "Media Keys"
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
    ZONE_TYPE_LINEAR,
    ZONE_TYPE_SINGLE
};

static const unsigned int zone_sizes[] =
{
    106,
    18,
    4
};

static const char *led_names[] =
{
    KEY_EN_ESCAPE,
    KEY_EN_BACK_TICK,
    KEY_EN_TAB,
    KEY_EN_CAPS_LOCK,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_F12,
    KEY_EN_EQUALS,
    KEY_EN_F9,
    KEY_EN_9,
    KEY_EN_O,
    KEY_EN_L,
    KEY_EN_COMMA,
    KEY_EN_MENU,
    KEY_EN_ISO_ENTER,
    KEY_EN_LEFT_ARROW,
    KEY_EN_F1,
    KEY_EN_1,
    KEY_EN_Q,
    KEY_EN_A,
    KEY_EN_ISO_BACK_SLASH,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_PRINT_SCREEN,
    KEY_EN_F10,
    KEY_EN_0,
    KEY_EN_P,
    KEY_EN_SEMICOLON,
    KEY_EN_PERIOD,
    KEY_EN_ANSI_ENTER,
    KEY_EN_DOWN_ARROW,
    KEY_EN_F2,
    KEY_EN_2,
    KEY_EN_W,
    KEY_EN_S,
    KEY_EN_Z,
    KEY_EN_LEFT_ALT,
    KEY_EN_SCROLL_LOCK,
    KEY_EN_BACKSPACE,
    KEY_EN_F11,
    KEY_EN_MINUS,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_QUOTE,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_RIGHT_ARROW,
    KEY_EN_F3,
    KEY_EN_3,
    KEY_EN_E,
    KEY_EN_D,
    KEY_EN_X,
    KEY_EN_PAUSE_BREAK,
    KEY_EN_DELETE,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_6,
    KEY_EN_F4,
    KEY_EN_4,
    KEY_EN_R,
    KEY_EN_F,
    KEY_EN_C,
    KEY_EN_SPACE,
    KEY_EN_INSERT,
    KEY_EN_END,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_1,
    KEY_EN_F5,
    KEY_EN_5,
    KEY_EN_T,
    KEY_EN_G,
    KEY_EN_V,
    KEY_EN_HOME,
    KEY_EN_PAGE_DOWN,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_2,
    KEY_EN_F6,
    KEY_EN_6,
    KEY_EN_Y,
    KEY_EN_H,
    KEY_EN_B,
    KEY_EN_PAGE_UP,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_NUMPAD_MINUS,
    KEY_EN_NUMPAD_3,
    KEY_EN_F7,
    KEY_EN_7,
    KEY_EN_U,
    KEY_EN_J,
    KEY_EN_N,
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_NUMPAD_0,
    KEY_EN_F8,
    KEY_EN_8,
    KEY_EN_I,
    KEY_EN_K,
    KEY_EN_M,
    KEY_EN_RIGHT_WINDOWS,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_UP_ARROW,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_ENTER,
    KEY_EN_NUMPAD_PERIOD,
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
    KEY_EN_MEDIA_PREVIOUS,
    KEY_EN_MEDIA_PLAY_PAUSE,
    KEY_EN_MEDIA_NEXT,
    KEY_EN_MEDIA_MUTE
};

/**------------------------------------------------------------------*\
    @name HyperX Alloy Elite
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectHyperXAlloyElite
    @comment
\*-------------------------------------------------------------------*/

RGBController_HyperXAlloyElite::RGBController_HyperXAlloyElite(HyperXAlloyEliteController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "HyperX";
    type                    = DEVICE_TYPE_KEYBOARD;
    description             = "HyperX Alloy Elite Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = HYPERX_ALLOY_ELITE_MODE_STATIC;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name             = "Static";
    Static.value            = HYPERX_ALLOY_ELITE_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Wave;
    Wave.name               = "Wave";
    Wave.value              = HYPERX_ALLOY_ELITE_MODE_WAVE;
    Wave.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.speed_min          = 0x00;
    Wave.speed_max          = 0x09;
    Wave.color_mode         = MODE_COLORS_NONE;
    Wave.speed              = 0x09;
    Wave.direction          = MODE_DIRECTION_LEFT;
    modes.push_back(Wave);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = HYPERX_ALLOY_ELITE_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.speed_min     = 0x00;
    Breathing.speed_max     = 0x09;
    Breathing.colors_min    = 1;
    Breathing.colors_max    = 2;
    Breathing.color_mode    = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed         = 0x09;
    Breathing.colors.resize(2);
    modes.push_back(Breathing);

    SetupZones();

    /*-----------------------------------------------------*\
    | The HyperX Alloy Elite requires a steady stream of    |
    | packets in order to not revert out of direct mode.    |
    | Start a thread to continuously refresh the device     |
    \*-----------------------------------------------------*/
    keepalive_thread_run = true;
    keepalive_thread = new std::thread(&RGBController_HyperXAlloyElite::KeepaliveThreadFunction, this);
}

RGBController_HyperXAlloyElite::~RGBController_HyperXAlloyElite()
{
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

void RGBController_HyperXAlloyElite::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < 3; zone_idx++)
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
        new_led.name                    = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_HyperXAlloyElite::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXAlloyElite::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    if(active_mode == 0)
    {
        controller->SetLEDsDirect(colors);
    }
    else
    {
        controller->SetLEDs(colors);
    }
}

void RGBController_HyperXAlloyElite::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyElite::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyElite::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        controller->SetMode(modes[active_mode].value, modes[active_mode].direction, modes[active_mode].speed, modes[active_mode].colors);
    }
    else
    {
        std::vector<RGBColor> temp_colors;
        controller->SetMode(modes[active_mode].value, modes[active_mode].direction, modes[active_mode].speed, temp_colors);
    }
}

void RGBController_HyperXAlloyElite::KeepaliveThreadFunction()
{
    while(keepalive_thread_run)
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
