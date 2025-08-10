/*---------------------------------------------------------*\
| RGBController_HyperXAlloyOriginsCore.cpp                  |
|                                                           |
|   RGBController for HyperX Alloy Origins Core keyboard    |
|                                                           |
|   Volodymyr Nazarchuk (Vavooon)               28 Apr 2021 |
|   carlos jordao                               15 Mar 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_HyperXAlloyOriginsCore.h"

using namespace std::chrono_literals;

#define HYPERX_MIN_BRIGHTNESS   0
#define HYPERX_MAX_BRIGHTNESS 255

#define NA  0xFFFFFFFF

/*----------------------------------*\
|  Maps LED position number to keys  |
|  * based on ANSI QWERTY            |
\*----------------------------------*/
static unsigned int matrix_map[6][19] =
{
        {   0,  NA,   1,   2,   3,   4,   5,   6,   7,  48,  NA,  49,  50,  51,  52,  NA,  53,  54,  55 },
        {   8,   9,  10,  11,  12,  13,  14,  15,  16,  56,  57,  58,  59,  60,  NA,  NA,  61,  62,  63 },
        {  17,  NA,  18,  19,  20,  21,  22,  23,  24,  64,  65,  66,  67,  68,  69,  NA,  70,  71,  72 },
        {  25,  NA,  26,  27,  28,  29,  30,  31,  32,  73,  74,  75,  76,  78,  NA,  NA,  NA,  NA,  NA },
        {  33,  NA,  35,  36,  37,  38,  39,  40,  79,  80,  81,  NA,  83,  84,  NA,  NA,  NA,  85,  NA },
        {  41,  42,  43,  NA,  NA,  NA,  45,  NA,  NA,  NA,  NA,  86,  87,  88,  89,  NA,  90,  91,  92 }
};

static const char* zone_names[] =
{
    ZONE_EN_KEYBOARD,
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};


/*--------------------------------------------------------------------------------*\
| This keyboard (TKL) always receives 93 led colors.                               |
| * Some indexes are just blank (unused).                                          |
| * Regional layouts have a few different enabled or disabled led indexes.         |
| * All indexes need to be sent anyway. Declaring all of them makes it easier to   |
|   add new layouts without creating a new matrix map.                             |
| * led_names contains the indexes used by the HyperX keyboard                     |
\*--------------------------------------------------------------------------------*/
struct led_index
{
   const char *name;
   unsigned int index;
};

static led_index led_names[] =
{
    {KEY_EN_ESCAPE,           0},
    {KEY_EN_F1,               1},
    {KEY_EN_F2,               2},
    {KEY_EN_F3,               3},
    {KEY_EN_F4,               4},
    {KEY_EN_F5,               5},
    {KEY_EN_F6,               6},
    {KEY_EN_F7,               7},

    {KEY_EN_BACK_TICK,        8},
    {KEY_EN_1,                9},
    {KEY_EN_2,               10},
    {KEY_EN_3,               11},
    {KEY_EN_4,               12},
    {KEY_EN_5,               13},
    {KEY_EN_6,               14},
    {KEY_EN_7,               15},
    {KEY_EN_8,               16},

    {KEY_EN_TAB,             17},
    {KEY_EN_Q,               18},
    {KEY_EN_W,               19},
    {KEY_EN_E,               20},
    {KEY_EN_R,               21},
    {KEY_EN_T,               22},
    {KEY_EN_Y,               23},
    {KEY_EN_U,               24},

    {KEY_EN_CAPS_LOCK,       25},
    {KEY_EN_A,               26},
    {KEY_EN_S,               27},
    {KEY_EN_D,               28},
    {KEY_EN_F,               29},
    {KEY_EN_G,               30},
    {KEY_EN_H,               31},
    {KEY_EN_J,               32},

    {KEY_EN_LEFT_SHIFT,      33},
    {KEY_EN_UNUSED,          34},
    {KEY_EN_Z,               35},
    {KEY_EN_X,               36},
    {KEY_EN_C,               37},
    {KEY_EN_V,               38},
    {KEY_EN_B,               39},
    {KEY_EN_N,               40},

    {KEY_EN_LEFT_CONTROL,    41},
    {KEY_EN_LEFT_WINDOWS,    42},
    {KEY_EN_LEFT_ALT,        43},

    {KEY_EN_UNUSED,          44},
    {KEY_EN_SPACE,           45},
    {KEY_EN_UNUSED,          46},
    {KEY_EN_UNUSED,          47},

    // End of first section

    {KEY_EN_F8,              48},
    {KEY_EN_F9,              49},
    {KEY_EN_F10,             50},
    {KEY_EN_F11,             51},
    {KEY_EN_F12,             52},
    {KEY_EN_PRINT_SCREEN,    53},
    {KEY_EN_SCROLL_LOCK,     54},
    {KEY_EN_PAUSE_BREAK,     55},

    {KEY_EN_9,               56},
    {KEY_EN_0,               57},
    {KEY_EN_MINUS,           58},
    {KEY_EN_EQUALS,          59},
    {KEY_EN_BACKSPACE,       60},
    {KEY_EN_INSERT,          61},
    {KEY_EN_HOME,            62},
    {KEY_EN_PAGE_UP,         63},

    {KEY_EN_I,               64},
    {KEY_EN_O,               65},
    {KEY_EN_P,               66},
    {KEY_EN_LEFT_BRACKET,    67},
    {KEY_EN_RIGHT_BRACKET,   68},
    {KEY_EN_ANSI_BACK_SLASH, 69},

    {KEY_EN_DELETE,          70},
    {KEY_EN_END,             71},
    {KEY_EN_PAGE_DOWN,       72},

    {KEY_EN_K,               73},
    {KEY_EN_L,               74},
    {KEY_EN_SEMICOLON,       75},
    {KEY_EN_QUOTE,           76},
    {KEY_EN_POUND,           77},
    {KEY_EN_ANSI_ENTER,      78},

    {KEY_EN_M,               79},
    {KEY_EN_COMMA,           80},
    {KEY_EN_PERIOD,          81},
    {KEY_EN_UNUSED,          82},
    {KEY_EN_FORWARD_SLASH,   83},
    {KEY_EN_RIGHT_SHIFT,     84},

    {KEY_EN_UP_ARROW,        85},

    {KEY_EN_RIGHT_ALT,       86},
    {KEY_EN_RIGHT_FUNCTION,  87},
    {KEY_EN_MENU,            88},
    {KEY_EN_RIGHT_CONTROL,   89},

    {KEY_EN_LEFT_ARROW,      90},
    {KEY_EN_DOWN_ARROW,      91},
    {KEY_EN_RIGHT_ARROW,     92}
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
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "HyperX";
    type                    = DEVICE_TYPE_KEYBOARD;
    description             = "HyperX Alloy Origins Core Keyboard Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();
    version                 = controller->GetFirmwareVersion();
    variant                 = controller->GetVariant();

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
    const unsigned int total_leds = sizeof(led_names) / sizeof(led_names[0]);
    zone new_zone;

    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        new_zone.name  = zone_names[zone_idx];
        new_zone.type  = zone_types[zone_idx];

        /*-----------------------------------------------------*\
        |   Regional configuration can be done by setting       |
        |   the relevant keys based on the led map.             |
        \*-----------------------------------------------------*/
        if(variant == HYPERX_ALLOY_ORIGINS_CORE_ABNT2)
        {
            // Sets the led indexes of this variant
            matrix_map[2][14] = NA;
            matrix_map[3][13] = 77; // ]
            matrix_map[3][14] = 78; // enter
            matrix_map[4][ 1] = 34; // backslash
            matrix_map[4][11] = 82; // ;

            // corrects the visual representantion
            led_names[ 8].name = KEY_EN_QUOTE;
            led_names[34].name = KEY_EN_ISO_BACK_SLASH;
            led_names[67].name = KEY_NORD_ACUTE_GRAVE;
            led_names[68].name = KEY_EN_LEFT_BRACKET;
            led_names[69].name = KEY_EN_UNUSED;
            led_names[75].name = KEY_FR_CEDILLA_C;
            led_names[76].name = KEY_BR_TILDE;
            led_names[77].name = KEY_EN_RIGHT_BRACKET;
            led_names[78].name = KEY_EN_ISO_ENTER;
            led_names[82].name = KEY_EN_SEMICOLON;
        }

        for(unsigned int led_idx = 0; led_idx < total_leds; led_idx++)
        {
            led new_led;
            new_led.name  = led_names[led_idx].name;
            new_led.value = led_names[led_idx].index;
            leds.push_back(new_led);
        }

        matrix_map_type * keyboard_map  = new matrix_map_type;
        new_zone.leds_count             = total_leds;
        new_zone.leds_min               = new_zone.leds_count;
        new_zone.leds_max               = new_zone.leds_count;

        if(zone_types[zone_idx] == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map         = keyboard_map;
            new_zone.matrix_map->height = 6;
            new_zone.matrix_map->width  = 19;
            new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
        }
        else
        {
            new_zone.matrix_map         = NULL;
        }
        zones.push_back(new_zone);
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
