/*-----------------------------------------*\
|  RGBController_HyperXAlloyOrigins.cpp     |
|                                           |
|  Generic RGB Interface for HyperX Alloy   |
|  Origins RGB Keyboard                     |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/11/2020   |
\*-----------------------------------------*/

#include "RGBController_HyperXAlloyOrigins.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   0,  NA,  12,  18,  23,  28,  NA,  34,  39,  44,  50,  NA,  55,  61,  65,  70,  76,  80,  85,  NA,  NA,  NA,  NA },
      {   1,   7,  13,  19,  24,  29,  35,  40,  45,  51,  56,  NA,  62,  66,  71,  NA,  77,  81,  86,  89,  94,  98, 103 },
      {   2,  NA,   8,  14,  20,  25,  NA,  30,  36,  41,  46,  52,  57,  63,  67,  72,  78,  82,  87,  90,  95,  99, 104 },
      {   3,  NA,   9,  15,  21,  26,  NA,  31,  37,  42,  47,  53,  58,  64,  68,  73,  NA,  NA,  NA,  91,  96, 100,  NA },
      {   4,  6,   10,  16,  22,  27,  NA,  32,  NA,  38,  43,  48,  54,  59,  74,  NA,  NA,  83,  NA,  92,  97, 101, 105 },
      {   5,  11,  17,  NA,  NA,  NA,  NA,  33,  NA,  NA,  NA,  NA,  49,  60,  69,  75,  79,  84,  88,  93,  NA, 102,  NA } };

static const char* zone_names[] =
{
    "Keyboard",
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const unsigned int zone_sizes[] =
{
    106,
};

static const char *led_names[] =
{
    "Key: Escape",
    "Key: `",
    "Key: Tab",
    "Key: Caps Lock",
    "Key: Left Shift",
    "Key: Left Control",
    "Key: \\ (ISO)",
    "Key: 1",
    "Key: Q",
    "Key: A",
    "Key: Z",
    "Key: Left Windows",
    "Key: F1",
    "Key: 2",
    "Key: W",
    "Key: S",
    "Key: X",
    "Key: Left Alt",
    "Key: F2",
    "Key: 3",
    "Key: E",
    "Key: D",
    "Key: C",
    // Skip index 23
    "Key: F3",
    "Key: 4",
    "Key: R",
    "Key: F",
    "Key: V",
    // Skip index 29
    "Key: F4",
    "Key: 5",
    "Key: T",
    "Key: G",
    "Key: B",
    "Key: Space",
    "Key: F5",
    "Key: 6",
    "Key: Y",
    "Key: H",
    "Key: N",
    // Skip index 41
    "Key: F6",
    "Key: 7",
    "Key: U",
    "Key: J",
    "Key: M",
    // Skip index 47
    "Key: F7",
    "Key: 8",
    "Key: I",
    "Key: K",
    "Key: ,",
    "Key: Right Alt",
    "Key: F8",
    "Key: 9",
    "Key: O",
    "Key: L",
    "Key: .",
    // Skip index 59
    "Key: F9",
    "Key: 0",
    "Key: P",
    "Key: ;",
    "Key: /",
    "Key: Right Fn",
    "Key: F10",
    "Key: -",
    "Key: [",
    "Key: '",
    // Skip index 70
    // Skip index 71
    "Key: F11",
    "Key: =",
    "Key: ]",
    "Key: #",
    // Skip index 76
    "Key: Menu",
    "Key: F12",
    "Key: Backspace",
    "Key: \\ (ANSI)",
    "Key: Enter",
    "Key: Right Shift",
    "Key: Right Control",
    "Key: Print Screen",
    "Key: Insert",
    "Key: Delete",
    // Skip index 87
    // Skip index 88
    "Key: Left Arrow",
    "Key: Scroll Lock",
    "Key: Home",
    "Key: End",
    // Skip index 93
    "Key: Up Arrow",
    "Key: Down Arrow",
    "Key: Pause/Break",
    "Key: Page Up",
    "Key: Page Down",
    // Skip index 99
    // Skip index 100
    "Key: Right Arrow",
    // Skip index 102
    "Key: Num Lock",
    "Key: Number Pad 7",
    "Key: Number Pad 4",
    "Key: Number Pad 1",
    "Key: Number Pad 0",
    // Skip index 108
    "Key: Number Pad /",
    "Key: Number Pad 8",
    "Key: Number Pad 5",
    "Key: Number Pad 2",
    // Skip index 113
    // Skip index 114
    "Key: Number Pad *",
    "Key: Number Pad 9",
    "Key: Number Pad 6",
    "Key: Number Pad 3",
    "Key: Number Pad .",
    // Skip index 120
    "Key: Number Pad -",
    "Key: Number Pad +",
    // Skip index 123
    // Skip index 124
    "Key: Number Pad Enter",
};

RGBController_HyperXAlloyOrigins::RGBController_HyperXAlloyOrigins(HyperXAlloyOriginsController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name        = "HyperX Alloy Origins Keyboard Device";
    vendor      = "HyperX";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "HyperX Alloy Origins Keyboard Device";
    location    = hyperx->GetDeviceLocation();
    serial      = hyperx->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
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
    keepalive_thread = new std::thread(&RGBController_HyperXAlloyOrigins::KeepaliveThread, this);
}

RGBController_HyperXAlloyOrigins::~RGBController_HyperXAlloyOrigins()
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

    delete hyperx;
}

void RGBController_HyperXAlloyOrigins::SetupZones()
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
        new_led.name = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_HyperXAlloyOrigins::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}
    
void RGBController_HyperXAlloyOrigins::DeviceUpdateLEDs()
{
    hyperx->SetLEDsDirect(colors);
}

void RGBController_HyperXAlloyOrigins::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOrigins::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOrigins::SetCustomMode()
{

}

void RGBController_HyperXAlloyOrigins::DeviceUpdateMode()
{

}

void RGBController_HyperXAlloyOrigins::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(50))
            {
                DeviceUpdateLEDs();
            }
        }
        std::this_thread::sleep_for(10ms);;
    }
}
