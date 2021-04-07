/*-----------------------------------------*\
|  RGBController_HyperXAlloyElite2.cpp      |
|                                           |
|  Generic RGB Interface for HyperX Alloy   |
|  Elite2 RGB Keyboard                      |
|                                           |
|  KundaPanda (vojdo) 02/04/2021            |
\*-----------------------------------------*/

#include "RGBController_HyperXAlloyElite2.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[8][22] =
{
    {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 104, 107, 108,  98,  NA,  NA },
    { 110, 111, 112, 113,  NA, 114, 115, 116,  NA, 117, 118, 119, 120,  NA, 121, 122, 123,  NA, 124, 125, 126, 127 },
    {   0,  12,  18,  23,  28,  NA,  34,  39,  44,  50,  56,  62,  66,  70,  NA,  76,  80,  85,  NA,  NA,  NA,  NA },
    {   1,   7,  13,  19,  24,  29,  35,  40,  45,  51,  57,  63,  67,  71,  NA,  77,  81,  86,  89,  94,  99, 105 },
    {   2,  NA,   8,  14,  20,  25,  30,  36,  41,  46,  52,  58,  64,  68,  72,  78,  82,  87,  90,  95, 100, 106 },
    {   3,  NA,   9,  15,  21,  26,  31,  37,  42,  47,  53,  59,  65,  69,  73,  NA,  NA,  NA,  91,  96, 101,  NA },
    {   4,   6,  NA,  10,  16,  22,  27,  32,  38,  43,  48,  54,  60,  74,  NA,  NA,  83,  NA,  92,  97, 102, 109 },
    {   5,  11,  17,  NA,  NA,  NA,  NA,  33,  NA,  NA,  NA,  49,  61,  55,  75,  79,  84,  88,  93,  NA, 103,  NA }
};

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
    128,
};


// ISO 6, 75, !80
// ANSI !6, !75, 80

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
    "Key: Menu",
    "Key: F9",
    "Key: 0",
    "Key: P",
    "Key: ;",
    "Key: /",
    "Key: Right Windows",
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
    "Key: Media Mute", // Last multimedia key
    "Key: Number Pad *",
    "Key: Number Pad 9",
    "Key: Number Pad 3",
    "Key: Number Pad 6",
    "Key: Number Pad .",
    "Key: Media Previous", // First multimedia key
    "Key: Number Pad -",
    "Key: Number Pad +",
    "Key: Media Play/Pause", // Second multimedia key
    "Key: Media Next", // Third multimedia key
    "Key: Number Pad Enter",
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
};

RGBController_HyperXAlloyElite2::RGBController_HyperXAlloyElite2(HyperXAlloyElite2Controller* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name        = "HyperX Alloy Elite 2 Keyboard Device";
    vendor      = "HyperX";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "HyperX Alloy Elite 2 Keyboard Device";
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
    keepalive_thread_run = true;
    keepalive_thread = new std::thread(&RGBController_HyperXAlloyElite2::KeepaliveThreadFunction, this);
}

RGBController_HyperXAlloyElite2::~RGBController_HyperXAlloyElite2()
{
    keepalive_thread_run = false;
    keepalive_thread->join();
    delete keepalive_thread;

    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != nullptr)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete hyperx;
}

void RGBController_HyperXAlloyElite2::SetupZones()
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
            new_zone.matrix_map->height = 8;
            new_zone.matrix_map->width  = 22;
            new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
        }
        else
        {
            new_zone.matrix_map         = nullptr;
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

void RGBController_HyperXAlloyElite2::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXAlloyElite2::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    if(active_mode == 0)
    {
        hyperx->SetLEDsDirect(colors);
    }
}

void RGBController_HyperXAlloyElite2::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyElite2::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyElite2::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_HyperXAlloyElite2::DeviceUpdateMode()
{

}

void RGBController_HyperXAlloyElite2::KeepaliveThreadFunction()
{
    while(keepalive_thread_run)
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(1000))
            {
                DeviceUpdateLEDs();
            }
        }
        std::this_thread::sleep_for(50ms);
    }
}
