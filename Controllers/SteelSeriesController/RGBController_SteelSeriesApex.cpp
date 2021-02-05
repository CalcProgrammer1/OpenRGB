/*-----------------------------------------*\
|  RGBController_SteelSeriesApex.cpp        |
|                                           |
|  Generic RGB Interface for SteelSeries    |
|  Apex 7 Keyboard                          |
|                                           |
|  Eric Samuelson (edbgon) 7/5/2020         |
\*-----------------------------------------*/

#include "RGBController_SteelSeriesApex.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] = 
    { {   37,  NA,  53,  54,  55,  56,  NA,  57,  58,  59,  60,  NA,  61 ,  62 ,  63 ,  64 ,  65,  66,  67,  NA,  NA,  NA,  NA },
      {   48,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  41,  42 ,  NA ,  38 ,  105,  68,  69,  70,  78,  79,  80,  81 },
      {   39,  NA,  16,  22,  4 ,  17,  19,  24,  20,  8 ,  14,  15,  43 ,  44 ,  NA ,  36 ,  71,  72,  73,  90,  91,  92,  82 },
      {   52,  NA,  0 ,  18,  3 ,  5 ,  6 ,  7 ,  9 ,  10,  11,  46,  47 ,  45 ,  NA ,  NA ,  NA,  NA,  NA,  87,  88,  89,  NA },
      {   97,  95,  25,  23,  2 ,  21,  NA,  1 ,  13,  12,  49,  50,  51 ,  NA ,  101,  NA ,  NA,  77,  NA,  84,  85,  86,  83 },
      {   96,  99,  98,  NA,  NA,  NA,  NA,  40,  NA,  NA,  NA,  NA,  102,  103,  104,  100,  75,  76,  74,  93,  NA,  94,  NA } };

static unsigned int matrix_map_tkl[6][19] = 
    { {   37,  NA,  53,  54,  55,  56,  NA,  57,  58,  59,  60,  NA,  61 ,  62 ,  63 ,  64 ,  65,  66,  67},
      {   48,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  41,  42 ,  NA ,  38 ,  105,  68,  69,  70},
      {   39,  NA,  16,  22,  4 ,  17,  19,  24,  20,  8 ,  14,  15,  43 ,  44 ,  NA ,  36 ,  71,  72,  73},
      {   52,  NA,  0 ,  18,  3 ,  5 ,  6 ,  7 ,  9 ,  10,  11,  46,  47 ,  45 ,  NA ,  NA ,  NA,  NA,  NA},
      {   97,  95,  25,  23,  2 ,  21,  NA,  1 ,  13,  12,  49,  50,  51 ,  NA ,  101,  NA ,  NA,  77,  NA},
      {   96,  99,  98,  NA,  NA,  NA,  NA,  40,  NA,  NA,  NA,  NA,  102,  103,  104,  100,  75,  76,  74} };

static const char* zone_names[] =
{
    "Keyboard"
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
    "Key: A",
    "Key: B",
    "Key: C",
    "Key: D",
    "Key: E",
    "Key: F",
    "Key: G",
    "Key: H",
    "Key: I",
    "Key: J",
    "Key: K",
    "Key: L",
    "Key: M",
    "Key: N",
    "Key: O",
    "Key: P",
    "Key: Q",
    "Key: R",
    "Key: S",
    "Key: T",
    "Key: U",
    "Key: V",
    "Key: W",
    "Key: X",
    "Key: Y",
    "Key: Z",
    "Key: 1",
    "Key: 2",
    "Key: 3",
    "Key: 4",
    "Key: 5",
    "Key: 6",
    "Key: 7",
    "Key: 8",
    "Key: 9",
    "Key: 0",
    "Key: Enter",
    "Key: Escape",
    "Key: Backspace",
    "Key: Tab",
    "Key: Space",
    "Key: -",
    "Key: =",
    "Key: [",
    "Key: ]",
    "Key: #",
    "Key: ;",
    "Key: '",
    "Key: `",
    "Key: ,",
    "Key: .",
    "Key: /",
    "Key: Caps Lock",
    "Key: F1",
    "Key: F2",
    "Key: F3",
    "Key: F4",
    "Key: F5",
    "Key: F6",
    "Key: F7",
    "Key: F8",
    "Key: F9",
    "Key: F10",
    "Key: F11",
    "Key: F12",
    "Key: Print Screen",
    "Key: Scroll Lock",
    "Key: Pause",
    "Key: Insert",
    "Key: Home",
    "Key: Page Up",
    "Key: Delete",
    "Key: End",
    "Key: Page Down",
    "Key: Right Arrow",
    "Key: Left Arrow",
    "Key: Down Arrow",
    "Key: Up Arrow",
    "Key: Num Lock",
    "Key: Number Pad /",
    "Key: Number Pad *",
    "Key: Number Pad -",
    "Key: Number Pad +",
    "Key: Number Pad Enter",
    "Key: Number Pad 1",
    "Key: Number Pad 2",
    "Key: Number Pad 3",
    "Key: Number Pad 4",
    "Key: Number Pad 5",
    "Key: Number Pad 6",
    "Key: Number Pad 7",
    "Key: Number Pad 8",
    "Key: Number Pad 9",
    "Key: Number Pad 0",
    "Key: Number Pad .",
    "Key: \\ (ISO)",
    "Key: Left Control",
    "Key: Left Shift",
    "Key: Left Alt",
    "Key: Left Windows",
    "Key: Right Control",
    "Key: Right Shift",
    "Key: Right Alt",
    "Key: Right Windows",
    "Key: Right Fn",
    "Key: \\ (ANSI)",
};

static const char *led_names_tkl[] =
{
    "Key: A",
    "Key: B",
    "Key: C",
    "Key: D",
    "Key: E",
    "Key: F",
    "Key: G",
    "Key: H",
    "Key: I",
    "Key: J",
    "Key: K",
    "Key: L",
    "Key: M",
    "Key: N",
    "Key: O",
    "Key: P",
    "Key: Q",
    "Key: R",
    "Key: S",
    "Key: T",
    "Key: U",
    "Key: V",
    "Key: W",
    "Key: X",
    "Key: Y",
    "Key: Z",
    "Key: 1",
    "Key: 2",
    "Key: 3",
    "Key: 4",
    "Key: 5",
    "Key: 6",
    "Key: 7",
    "Key: 8",
    "Key: 9",
    "Key: 0",
    "Key: Enter",
    "Key: Escape",
    "Key: Backspace",
    "Key: Tab",
    "Key: Space",
    "Key: -",
    "Key: =",
    "Key: [",
    "Key: ]",
    "Key: #",
    "Key: ;",
    "Key: '",
    "Key: `",
    "Key: ,",
    "Key: .",
    "Key: /",
    "Key: Caps Lock",
    "Key: F1",
    "Key: F2",
    "Key: F3",
    "Key: F4",
    "Key: F5",
    "Key: F6",
    "Key: F7",
    "Key: F8",
    "Key: F9",
    "Key: F10",
    "Key: F11",
    "Key: F12",
    "Key: Print Screen",
    "Key: Scroll Lock",
    "Key: Pause",
    "Key: Insert",
    "Key: Home",
    "Key: Page Up",
    "Key: Delete",
    "Key: End",
    "Key: Page Down",
    "Key: Right Arrow",
    "Key: Left Arrow",
    "Key: Down Arrow",
    "Key: Up Arrow",
    "Key: \\ (ISO)",
    "Key: Left Control",
    "Key: Left Shift",
    "Key: Left Alt",
    "Key: Left Windows",
    "Key: Right Control",
    "Key: Right Shift",
    "Key: Right Alt",
    "Key: Right Windows",
    "Key: Right Fn",
    "Key: \\",
};

RGBController_SteelSeriesApex::RGBController_SteelSeriesApex(SteelSeriesApexBaseController* steelseries_ptr)
{
    steelseries = steelseries_ptr;

    name        = "SteelSeries Apex RGB Keyboard";
    vendor      = "SteelSeries";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "SteelSeries Apex RGB Device";
    location    = steelseries->GetDeviceLocation();
    serial      = steelseries->GetSerialString();

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
        if((proto_type == APEX) || (proto_type == APEX_M))
        {
            new_led.name = led_names[led_idx];
        }
        else
        {
            new_led.name = led_names_tkl[led_idx];
        }
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
