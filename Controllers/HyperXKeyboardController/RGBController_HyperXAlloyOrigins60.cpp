/*-----------------------------------------*\
|  RGBController_HyperXAlloyOrigins60.cpp   |
|                                           |
|  Generic RGB Interface for HyperX Alloy   |
|  Origins 60 RGB Keyboard                  |
|                                           |
|  Derek Huber 10/24/2022                   |
\*-----------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_HyperXAlloyOrigins60.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[5][14] =
    { {   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  15 },
      {  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29 },
      {  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  NA,  43 },
      {  44,  NA,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  NA,  57 },
      {  58,  59,  60,  61,  NA,  NA,  62,  NA,  NA,  63,  64,  65,  66,  70 } };


static const char* zone_names[] =
{
    ZONE_EN_KEYBOARD
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX
};

static const unsigned int zone_sizes[] =
{
    71
};

static const char *led_names[] =
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
    KEY_EN_UNUSED,
    KEY_EN_SPACE,
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_ALT,
    KEY_EN_MENU,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_FUNCTION
};

/**------------------------------------------------------------------*\
    @name HyperX Alloy Origins 60
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectHyperXAlloyOrigins60
    @comment
\*-------------------------------------------------------------------*/

RGBController_HyperXAlloyOrigins60::RGBController_HyperXAlloyOrigins60(HyperXAlloyOrigins60Controller* controller_ptr)
{
    controller  = controller_ptr;

    name        = "HyperX Alloy Origins 60 Keyboard Device";
    vendor      = "HyperX";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "HyperX Alloy Origins 60 Keyboard Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    keepalive_thread_run = 1;
    keepalive_thread = new std::thread(&RGBController_HyperXAlloyOrigins60::KeepaliveThread, this);
}

RGBController_HyperXAlloyOrigins60::~RGBController_HyperXAlloyOrigins60()
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

void RGBController_HyperXAlloyOrigins60::SetupZones()
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
            new_zone.matrix_map->height = 5;
            new_zone.matrix_map->width  = 14;
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

void RGBController_HyperXAlloyOrigins60::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXAlloyOrigins60::DeviceUpdateLEDs()
{
    controller->SetLEDsDirect(colors);
}

void RGBController_HyperXAlloyOrigins60::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOrigins60::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOrigins60::DeviceUpdateMode()
{

}

void RGBController_HyperXAlloyOrigins60::KeepaliveThread()
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
