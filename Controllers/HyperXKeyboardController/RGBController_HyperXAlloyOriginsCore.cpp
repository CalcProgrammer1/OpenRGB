/*------------------------------------------*\
|  RGBController_HyperXAlloyOriginsCore.cpp  |
|                                            |
|  Generic RGB Interface for HyperX Alloy    |
|  Origins Core RGB Keyboard                 |
|                                            |
|  Volodymyr Nazarchuk (Vavooon) 4/28/2021   |
\*------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_HyperXAlloyOriginsCore.h"

using namespace std::chrono_literals;


#define HYPERX_MIN_BRIGHTNESS   0
#define HYPERX_MAX_BRIGHTNESS 255

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][19] =
{
        {   0,  NA,   1,   2,   3,   4,   5,   6,   7,  44,  NA,  45,  46,  47,  48,  NA,  49,  50,  51 },
        {   8,   9,  10,  11,  12,  13,  14,  15,  16,  52,  53,  54,  55,  56,  NA,  NA,  57,  58,  59 },
        {  17,  NA,  18,  19,  20,  21,  22,  23,  24,  60,  61,  62,  63,  64,  65,  NA,  66,  67,  68 },
        {  25,  NA,  26,  27,  28,  29,  30,  31,  32,  69,  70,  71,  72,  73,  NA,  NA,  NA,  NA,  NA },
        {  33,  NA,  34,  35,  36,  37,  38,  NA,  39,  74,  75,  76,  77,  78,  NA,  NA,  NA,  79,  NA },
        {  40,  41,  42,  NA,  NA,  NA,  43,  NA,  NA,  NA,  NA,  80,  81,  82,  83,  NA,  84,  85,  86 }
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
    87,
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

    KEY_EN_BACK_TICK,
    KEY_EN_1,
    KEY_EN_2,
    KEY_EN_3,
    KEY_EN_4,
    KEY_EN_5,
    KEY_EN_6,
    KEY_EN_7,
    KEY_EN_8,

    KEY_EN_TAB,
    KEY_EN_Q,
    KEY_EN_W,
    KEY_EN_E,
    KEY_EN_R,
    KEY_EN_T,
    KEY_EN_Y,
    KEY_EN_U,

    KEY_EN_CAPS_LOCK,
    KEY_EN_A,
    KEY_EN_S,
    KEY_EN_D,
    KEY_EN_F,
    KEY_EN_G,
    KEY_EN_H,
    KEY_EN_J,

    KEY_EN_LEFT_SHIFT,
    KEY_EN_Z,
    KEY_EN_X,
    KEY_EN_C,
    KEY_EN_V,
    KEY_EN_B,
    KEY_EN_N,

    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    KEY_EN_SPACE,

    // End of first section

    KEY_EN_F8,
    KEY_EN_F9,
    KEY_EN_F10,
    KEY_EN_F11,
    KEY_EN_F12,
    KEY_EN_PRINT_SCREEN,
    KEY_EN_SCROLL_LOCK,
    KEY_EN_PAUSE_BREAK,

    KEY_EN_9,
    KEY_EN_0,
    KEY_EN_MINUS,
    KEY_EN_EQUALS,
    KEY_EN_BACKSPACE,
    KEY_EN_INSERT,
    KEY_EN_HOME,
    KEY_EN_PAGE_UP,


    KEY_EN_I,
    KEY_EN_O,
    KEY_EN_P,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_DELETE,
    KEY_EN_END,
    KEY_EN_PAGE_DOWN,


    KEY_EN_K,
    KEY_EN_L,
    KEY_EN_SEMICOLON,
    KEY_EN_QUOTE,
    KEY_EN_ANSI_ENTER,


    KEY_EN_M,
    KEY_EN_COMMA,
    KEY_EN_PERIOD,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_UP_ARROW,
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_MENU,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW
};

/**------------------------------------------------------------------*\
    @name HyperX Alloy Origins Core
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectHyperXAlloyOriginsCore
    @comment
\*-------------------------------------------------------------------*/

RGBController_HyperXAlloyOriginsCore::RGBController_HyperXAlloyOriginsCore(HyperXAlloyOriginsCoreController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "HyperX Alloy Origins Core Keyboard Device";
    vendor      = "HyperX";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "HyperX Alloy Origins Core Keyboard Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();
    version     = controller->GetFirmwareVersion();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0xFFFF;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min   = HYPERX_MIN_BRIGHTNESS;
    Direct.brightness_max   = HYPERX_MAX_BRIGHTNESS;
    Direct.brightness       = HYPERX_MAX_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    /*-----------------------------------------------------*\
    | The HyperX Origins Core requires a packet within few  |
    | seconds of sending the lighting change in order to    |
    | not revert back into current profile.  Start a thread |
    | to continuously send color values each 10ms           |
    \*-----------------------------------------------------*/
    keepalive_thread_run = 1;
    keepalive_thread = new std::thread(&RGBController_HyperXAlloyOriginsCore::KeepaliveThread, this);
}

RGBController_HyperXAlloyOriginsCore::~RGBController_HyperXAlloyOriginsCore()
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

void RGBController_HyperXAlloyOriginsCore::SetupZones()
{
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
            new_zone.matrix_map->width  = 19;
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

void RGBController_HyperXAlloyOriginsCore::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXAlloyOriginsCore::DeviceUpdateLEDs()
{
    controller->SetLEDsDirect(colors);
}

void RGBController_HyperXAlloyOriginsCore::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOriginsCore::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOriginsCore::DeviceUpdateMode()
{
    controller->SetBrightness(modes[active_mode].brightness);
}

void RGBController_HyperXAlloyOriginsCore::KeepaliveThread()
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
        std::this_thread::sleep_for(10ms);
    }
}
