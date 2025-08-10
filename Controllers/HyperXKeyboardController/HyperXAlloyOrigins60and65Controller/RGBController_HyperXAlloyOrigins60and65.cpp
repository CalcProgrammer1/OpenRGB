/*---------------------------------------------------------*\
| RGBController_HyperXAlloyOrigins60and65.cpp               |
|                                                           |
|   RGBController for HyperX Alloy Origins 60 and 65        |
|   keyboard                                                |
|                                                           |
|   Derek Huber                                 18 Mar 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_HyperXAlloyOrigins60and65.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map_60[5][14] =
    { {   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  15 },
      {  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29 },
      {  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  NA,  43 },
      {  44,  NA,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  NA,  57 },
      {  58,  59,  60,  61,  NA,  NA,  62,  NA,  NA,  63,  64,  65,  66,  70 } };

static unsigned int matrix_map_65[5][15] =
    { {   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  15,  69  },
      {  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  70  },
      {  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  NA,  43,  71  },
      {  44,  NA,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  57,  73,  72  },
      {  58,  59,  60,  61,  NA,  NA,  62,  NA,  NA,  63,  64,  68,  74,  75,  76  } };

static const char *led_names_60[] =
{
    // First row
    KEY_EN_UNUSED,
    KEY_EN_ESCAPE,
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
    KEY_EN_UNUSED,
    KEY_EN_BACKSPACE,

    // Second row
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
    KEY_EN_ANSI_BACK_SLASH,

    // Third row
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
    KEY_EN_UNUSED,
    KEY_EN_ANSI_ENTER,

    // Fourth row
    KEY_EN_LEFT_SHIFT,
    KEY_EN_UNUSED,
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
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_SHIFT,

    // Fifth row
    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    "Left Space",
    KEY_EN_SPACE,
    "Right Space",
    KEY_EN_RIGHT_ALT,
    KEY_EN_MENU,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_FUNCTION
};

static const char *led_names_65[] =
{
    KEY_EN_UNUSED,
    KEY_EN_ESCAPE,
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
    KEY_EN_UNUSED,
    KEY_EN_BACKSPACE,
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
    KEY_EN_ANSI_BACK_SLASH,
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
    KEY_EN_UNUSED,
    KEY_EN_ANSI_ENTER,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_UNUSED,
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
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    "Left Space",
    KEY_EN_SPACE,
    "Right Space",
    KEY_EN_RIGHT_ALT,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_HOME,
    KEY_EN_DELETE,
    KEY_EN_PAGE_UP,
    KEY_EN_PAGE_DOWN,
    KEY_EN_UP_ARROW,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW
};

/**------------------------------------------------------------------*\
    @name HyperX Alloy Origins 60 and 65
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectHyperXAlloyOrigins60and65
    @comment
\*-------------------------------------------------------------------*/

RGBController_HyperXAlloyOrigins60and65::RGBController_HyperXAlloyOrigins60and65(HyperXAlloyOrigins60and65Controller* controller_ptr, AlloyOrigins60and65MappingLayoutType keyboard_layout)
{
    controller          = controller_ptr;
    layout              = keyboard_layout;

    switch(layout)
    {
        case ALLOY_ORIGINS_60_LAYOUT:
            description = "HyperX Alloy Origins 60 Keyboard Device";
            break;

        case ALLOY_ORIGINS_65_LAYOUT:
            description = "HyperX Alloy Origins 65 Keyboard Device";
            break;
    }

    name                = controller->GetNameString();
    vendor              = "HyperX";
    type                = DEVICE_TYPE_KEYBOARD;
    location            = controller->GetDeviceLocation();
    serial              = controller->GetSerialString();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0xFFFF;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    keepalive_thread_run = 1;
    keepalive_thread = new std::thread(&RGBController_HyperXAlloyOrigins60and65::KeepaliveThread, this);
}

RGBController_HyperXAlloyOrigins60and65::~RGBController_HyperXAlloyOrigins60and65()
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

void RGBController_HyperXAlloyOrigins60and65::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/

    std::vector<led_zone> led_zones;
    const char* const *led_names;

    switch(layout)
    {
        case ALLOY_ORIGINS_60_LAYOUT:
        default:
            led_names = led_names_60;
            led_zones.push_back({ZONE_EN_KEYBOARD, ZONE_TYPE_MATRIX, 71, new matrix_map_type{5, 14, (unsigned int *)&matrix_map_60}});
            break;
        case ALLOY_ORIGINS_65_LAYOUT:
            led_names = led_names_65;
            led_zones.push_back({ZONE_EN_KEYBOARD, ZONE_TYPE_MATRIX, 77, new matrix_map_type{5, 15, (unsigned int *)&matrix_map_65}});
            break;
    }

    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        zone new_zone;
        new_zone.name            = led_zones[zone_idx].name;
        new_zone.type            = led_zones[zone_idx].type;
        new_zone.leds_min        = led_zones[zone_idx].size;
        new_zone.leds_max        = led_zones[zone_idx].size;
        new_zone.leds_count      = led_zones[zone_idx].size;

        if(led_zones[zone_idx].type == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map  = led_zones[zone_idx].matrix;
        }
        else
        {
            new_zone.matrix_map  = NULL;
        }

        zones.push_back(new_zone);

        total_led_count += led_zones[zone_idx].size;
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name  = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_HyperXAlloyOrigins60and65::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXAlloyOrigins60and65::DeviceUpdateLEDs()
{
    controller->SetLEDsDirect(colors);
}

void RGBController_HyperXAlloyOrigins60and65::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOrigins60and65::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOrigins60and65::DeviceUpdateMode()
{

}

void RGBController_HyperXAlloyOrigins60and65::KeepaliveThread()
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
