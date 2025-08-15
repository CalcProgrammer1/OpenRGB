/*---------------------------------------------------------*\
| RGBController_CorsairK65Mini.cpp                          |
|                                                           |
|   RGBController for Corsair K65 Mini keyboard             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_CorsairK65Mini.h"
#include "LogManager.h"
#include "RGBControllerKeyNames.h"

using namespace std::chrono_literals;

#define NA      0xFFFFFFFF
#define WIDTH   15
#define HEIGHT  5

unsigned int matrix_map[HEIGHT][WIDTH] =
{
    {  0,  5,  8,  13, 17, 22, 26, 31, 35, 40, 44, 49, 54, NA, 60},
    {  1, NA,  9,  14, 18, 23, 27, 32, 36, 41, 45, 50, 55, 58, NA},
    {  2, NA,  10, 15, 19, 24, 28, 33, 37, 42, 46, 51, 56, 59, 61},
    {  3,  6,  11, 16, 20, 25, 29, 34, 38, 43, 47, 52, NA, NA, 62},
    {  4,  7,  12, NA, 21, NA, 30, NA, 39, NA, 48, 53, 57, NA, 63}
};

std::vector<std::tuple<unsigned int, std::string>> keys =
{
    // col 1
    {41,    KEY_EN_ESCAPE},
    {43,    KEY_EN_TAB},
    {57,    KEY_EN_CAPS_LOCK},
    {106,   KEY_EN_LEFT_SHIFT},
    {105,   KEY_EN_LEFT_CONTROL},

    // col 2
    {30,    KEY_EN_1},
    {100,   KEY_EN_ISO_BACK_SLASH},
    {108,   KEY_EN_LEFT_WINDOWS},

    // col 3
    {31,    KEY_EN_2},
    {20,    KEY_EN_Q},
    {4,     KEY_EN_A},
    {29,    KEY_EN_Z},
    {107,   KEY_EN_LEFT_ALT},

    // col 4
    {32,    KEY_EN_3},
    {26,    KEY_EN_W},
    {22,    KEY_EN_S},
    {27,    KEY_EN_X},

    // col 5
    {33,    KEY_EN_4},
    {8,     KEY_EN_E},
    {7,     KEY_EN_D},
    {6,     KEY_EN_C},
    {0,     ""},            // space bar, left LED

    // col 6
    {34,    KEY_EN_5},
    {21,    KEY_EN_R},
    {9,     KEY_EN_F},
    {25,    KEY_EN_V},

    // col 7
    {35,    KEY_EN_6},
    {23,    KEY_EN_T},
    {10,    KEY_EN_G},
    {5,     KEY_EN_B},
    {44,    KEY_EN_SPACE},

    // col 8
    {36,    KEY_EN_7},
    {28,    KEY_EN_Y},
    {11,    KEY_EN_H},
    {17,    KEY_EN_N},

    // col 9
    {37,    KEY_EN_8},
    {24,    KEY_EN_U},
    {13,    KEY_EN_J},
    {16,    KEY_EN_M},
    {1,     ""},            // space bar, right LED

    // col 10
    {38,    KEY_EN_9},
    {12,    KEY_EN_I},
    {14,    KEY_EN_K},
    {54,    KEY_EN_COMMA},

    // col 11
    {39,    KEY_EN_0},
    {18,    KEY_EN_O},
    {15,    KEY_EN_L},
    {55,    KEY_EN_PERIOD},
    {111,   KEY_EN_RIGHT_ALT},

    // col 12
    {45,    KEY_EN_MINUS},
    {19,    KEY_EN_P},
    {51,    KEY_EN_SEMICOLON},
    {56,    KEY_EN_FORWARD_SLASH},
    {122,   KEY_EN_RIGHT_FUNCTION},

    // col 13
    {46,    KEY_EN_EQUALS},
    {47,    KEY_EN_LEFT_BRACKET},
    {52,    KEY_EN_QUOTE},
    {101,   KEY_EN_MENU},

    // col 14
    {48,    KEY_EN_RIGHT_BRACKET},
    {50,    KEY_EN_POUND},

    // col 15
    {42,    KEY_EN_BACKSPACE},
    {40,    KEY_EN_ISO_ENTER},
    {110,   KEY_EN_RIGHT_SHIFT},
    {109,   KEY_EN_RIGHT_CONTROL}
};

/**------------------------------------------------------------------*\
    @name Corsair K65 Mini
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairK65MiniControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_CorsairK65Mini::RGBController_CorsairK65Mini(CorsairK65MiniController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetName();
    vendor              = "Corsair";
    description         = "Corsair K65 Mini Keyboard Device";
    type                = DEVICE_TYPE_KEYBOARD;
    location            = controller->GetDeviceLocation();
    serial              = controller->GetSerialString();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    /*-----------------------------------------------------*\
    | The Corsair K65 Mini requires a packet within         |
    | 1 minutes of sending the lighting change in order     |
    | to not revert back into rainbow mode.  Start a thread |
    | to continuously send a keepalive packet every 50 sec  |
    \*-----------------------------------------------------*/
    keepalive_thread_run = true;
    keepalive_thread = new std::thread(&RGBController_CorsairK65Mini::KeepaliveThread, this);
}

RGBController_CorsairK65Mini::~RGBController_CorsairK65Mini()
{
    /*-----------------------------------------------------*\
    | Close keepalive thread                                |
    \*-----------------------------------------------------*/
    keepalive_thread_run = false;
    keepalive_thread->join();
    delete keepalive_thread;

    delete controller;
}

void RGBController_CorsairK65Mini::SetupZones()
{
    unsigned int zone_size          = 0;

    zone keyboard_zone;
    keyboard_zone.name               = ZONE_EN_KEYBOARD;
    keyboard_zone.type               = ZONE_TYPE_MATRIX;

    keyboard_zone.matrix_map         = new matrix_map_type;
    keyboard_zone.matrix_map->height = HEIGHT;
    keyboard_zone.matrix_map->width  = WIDTH;

    keyboard_zone.matrix_map->map    = new unsigned int[HEIGHT * WIDTH];

    for(unsigned int w = 0; w < WIDTH; w++)
    {
        for(unsigned int h = 0; h < HEIGHT; h++)
        {
            unsigned int key = matrix_map[h][w];
            keyboard_zone.matrix_map->map[h * WIDTH + w] = key;

            if(key != NA)
            {
                led new_led;
                new_led.name = std::get<1>(keys[key]);
                leds.push_back(new_led);
                zone_size++;
                led_positions.push_back(std::get<0>(keys[key]));
            }
        }
    }

    keyboard_zone.leds_min           = zone_size;
    keyboard_zone.leds_max           = zone_size;
    keyboard_zone.leds_count         = zone_size;

    zones.push_back(keyboard_zone);

    SetupColors();
}

void RGBController_CorsairK65Mini::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairK65Mini::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();
    controller->SetLEDs(colors, led_positions);
}

void RGBController_CorsairK65Mini::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairK65Mini::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairK65Mini::DeviceUpdateMode()
{

}

void RGBController_CorsairK65Mini::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(50000))
        {
            DeviceUpdateLEDs();
        }

        std::this_thread::sleep_for(3000ms);
    }
}
