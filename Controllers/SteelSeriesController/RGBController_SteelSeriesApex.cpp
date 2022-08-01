/*-----------------------------------------*\
|  RGBController_SteelSeriesApex.cpp        |
|                                           |
|  Generic RGB Interface for SteelSeries    |
|  Apex 7 Keyboard                          |
|                                           |
|  Eric Samuelson (edbgon) 7/5/2020         |
\*-----------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_SteelSeriesApex.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   37,  NA,  53,  54,  55,  56,  NA,  57,  58,  59,  60,  NA,  61 ,  62 ,  63 ,  64 ,  65,  66,  67,  NA,  NA,  NA,  NA },
      {   48,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  41,  42 ,  NA ,  38 ,  88 ,  68,  69,  70,  89,  90,  91,  92 },
      {   39,  NA,  16,  22,  4 ,  17,  19,  24,  20,  8 ,  14,  15,  43 ,  44 ,  NA ,  36 ,  71,  72,  73, 101, 102, 103,  93 },
      {   52,  NA,  0 ,  18,  3 ,  5 ,  6 ,  7 ,  9 ,  10,  11,  46,  47 ,  45 ,  NA ,  NA ,  NA,  NA,  NA,  98,  99, 100,  NA },
      {   80,  78,  25,  23,  2 ,  21,  NA,  1 ,  13,  12,  49,  50,  51 ,  NA ,  84 ,  NA ,  NA,  77,  NA,  95,  96,  97,  94 },
      {   79,  82,  81,  NA,  NA,  NA,  NA,  40,  NA,  NA,  NA,  NA,  85 ,  86 ,  87 ,  83 ,  75,  76,  74, 104,  NA, 105,  NA } };

static unsigned int matrix_map_tkl[6][19] =
    { {   37,  NA,  53,  54,  55,  56,  NA,  57,  58,  59,  60,  NA,  61 ,  62 ,  63 ,  64 ,  65,  66,  67},
      {   48,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  41,  42 ,  NA ,  38 ,  88 ,  68,  69,  70},
      {   39,  NA,  16,  22,  4 ,  17,  19,  24,  20,  8 ,  14,  15,  43 ,  44 ,  NA ,  36 ,  71,  72,  73},
      {   52,  NA,  0 ,  18,  3 ,  5 ,  6 ,  7 ,  9 ,  10,  11,  46,  47 ,  45 ,  NA ,  NA ,  NA,  NA,  NA},
      {   80,  78,  25,  23,  2 ,  21,  NA,  1 ,  13,  12,  49,  50,  51 ,  NA ,  84 ,  NA ,  NA,  77,  NA},
      {   79,  82,  81,  NA,  NA,  NA,  NA,  40,  NA,  NA,  NA,  NA,  85 ,  86,   87,   83 ,  75,  76,  74} };

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
    106,
};

static const unsigned int zone_sizes_tkl[] =
{
    89,
};

static const char *led_names[] =
{
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
    KEY_EN_POUND,
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
    "Key: Pause",
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
    KEY_EN_ISO_BACK_SLASH,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_LEFT_ALT,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_WINDOWS,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_ANSI_BACK_SLASH,
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
};

/**------------------------------------------------------------------*\
    @name Steel Series APEX
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectSteelSeriesApex,DetectSteelSeriesApexTKL,DetectSteelSeriesApexM
    @comment
\*-------------------------------------------------------------------*/

RGBController_SteelSeriesApex::RGBController_SteelSeriesApex(SteelSeriesApexBaseController* steelseries_ptr)
{
    steelseries = steelseries_ptr;

    name        = "SteelSeries Apex RGB Keyboard";
    vendor      = "SteelSeries";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "SteelSeries Apex RGB Device";
    location    = steelseries->GetDeviceLocation();
    serial      = "";

    proto_type = steelseries->proto_type;

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0x00;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

}

RGBController_SteelSeriesApex::~RGBController_SteelSeriesApex()
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

    delete steelseries;
}

void RGBController_SteelSeriesApex::SetupZones()
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

        if((proto_type == APEX) || (proto_type == APEX_M))
        {
            new_zone.leds_min               = zone_sizes[zone_idx];
            new_zone.leds_max               = zone_sizes[zone_idx];
            new_zone.leds_count             = zone_sizes[zone_idx];
        }
        else
        {
            new_zone.leds_min               = zone_sizes_tkl[zone_idx];
            new_zone.leds_max               = zone_sizes_tkl[zone_idx];
            new_zone.leds_count             = zone_sizes_tkl[zone_idx];
        }

        if(zone_types[zone_idx] == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map         = new matrix_map_type;
            new_zone.matrix_map->height = 6;
            if((proto_type == APEX) || (proto_type == APEX_M))
            {
                new_zone.matrix_map->width  = 23;
                new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
            }
            else
            {
                new_zone.matrix_map->width  = 19;
                new_zone.matrix_map->map    = (unsigned int *)&matrix_map_tkl;
            }
        }
        else
        {
            new_zone.matrix_map         = NULL;
        }

        zones.push_back(new_zone);

        if((proto_type == APEX) || (proto_type == APEX_M))
        {
            total_led_count += zone_sizes[zone_idx];
        }
        else
        {
            total_led_count += zone_sizes_tkl[zone_idx];
        }
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name = led_names[led_idx];

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_SteelSeriesApex::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SteelSeriesApex::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();
    steelseries->SetLEDsDirect(colors);
}

void RGBController_SteelSeriesApex::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesApex::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesApex::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_SteelSeriesApex::DeviceUpdateMode()
{
    std::vector<RGBColor> temp_colors;
    steelseries->SetMode(modes[active_mode].value, temp_colors);
}
