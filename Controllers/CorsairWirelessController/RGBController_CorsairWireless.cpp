/*-----------------------------------------*\
|  RGBController_CorsairWireless.cpp        |
|                                           |
|  Generic RGB Interface for Corsair RGB    |
|  wireless keyboard devices                |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/8/2021    |
\*-----------------------------------------*/

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
    "Keyboard",
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
    "Key: \\ (ANSI)",
    "Unused",
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
    "Key: Pause/Break",
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
    "Unused",
    "Key: Menu",
    "Unused",
    "Unused",
    "Unused",
    "Left Control",
    "Left Shift",
    "Left Alt",
    "Left Windows",
    "Right Control",
    "Right Shift",
    "Right Alt",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Key: Right Fn",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Key: G1",
    "Key: G2",
    "Key: G3",
    "Key: G4",
    "Key: G5",
    "Key: G6",
};

RGBController_CorsairWireless::RGBController_CorsairWireless(CorsairWirelessController* corsair_ptr)
{
    corsair = corsair_ptr;

    name        = corsair->GetName();
    vendor      = "Corsair";
    description = "Corsair RGB Peripheral Device";
    type        = corsair->GetDeviceType();
    version     = corsair->GetFirmwareString();
    location    = corsair->GetDeviceLocation();
    serial      = corsair->GetSerialString();

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
    keepalive_thread_run = 1;
    keepalive_thread = new std::thread(&RGBController_CorsairWireless::KeepaliveThread, this);
}

RGBController_CorsairWireless::~RGBController_CorsairWireless()
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

    delete corsair;
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

        new_led.name = led_names[led_idx];

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

    corsair->SetLEDs(colors);
}

void RGBController_CorsairWireless::UpdateZoneLEDs(int /*zone*/)
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairWireless::UpdateSingleLED(int /*led*/)
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairWireless::SetCustomMode()
{
    active_mode = 0;
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
