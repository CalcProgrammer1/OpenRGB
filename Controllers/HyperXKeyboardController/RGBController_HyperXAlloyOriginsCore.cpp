/*------------------------------------------*\
|  RGBController_HyperXAlloyOriginsCore.cpp  |
|                                            |
|  Generic RGB Interface for HyperX Alloy    |
|  Origins Core RGB Keyboard                 |
|                                            |
|  Volodymyr Nazarchuk (Vavooon) 4/28/2021   |
\*------------------------------------------*/

#include "RGBController_HyperXAlloyOriginsCore.h"

using namespace std::chrono_literals;

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
    "Keyboard",
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
    "Key: Escape",
    "Key: F1",
    "Key: F2",
    "Key: F3",
    "Key: F4",
    "Key: F5",
    "Key: F6",
    "Key: F7",

    "Key: `",
    "Key: 1",
    "Key: 2",
    "Key: 3",
    "Key: 4",
    "Key: 5",
    "Key: 6",
    "Key: 7",
    "Key: 8",

    "Key: Tab",
    "Key: Q",
    "Key: W",
    "Key: E",
    "Key: R",
    "Key: T",
    "Key: Y",
    "Key: U",

    "Key: Caps Lock",
    "Key: A",
    "Key: S",
    "Key: D",
    "Key: F",
    "Key: G",
    "Key: H",
    "Key: J",

    "Key: Left Shift",
    "Key: Z",
    "Key: X",
    "Key: C",
    "Key: V",
    "Key: B",
    "Key: N",

    "Key: Left Control",
    "Key: Left Windows",
    "Key: Left Alt",
    "Key: Space",

	// End of first section

    "Key: F8",
    "Key: F9",
    "Key: F10",
    "Key: F11",
    "Key: F12",
    "Key: Print Screen",
    "Key: Scroll Lock",
    "Key: Pause/Break",

    "Key: 9",
    "Key: 0",
    "Key: -",
    "Key: =",
    "Key: Backspace",
    "Key: Insert",
    "Key: Page Up",
    "Key: Home",


    "Key: I",
    "Key: O",
    "Key: P",
    "Key: [",
    "Key: ]",
    "Key: \\ (ANSI)",
    "Key: Delete",
    "Key: Page Down",
    "Key: End",


    "Key: K",
    "Key: L",
    "Key: ;",
    "Key: '",
    "Key: Enter",


    "Key: M",
    "Key: ,",
    "Key: .",
    "Key: /",
    "Key: Right Shift",
    "Key: Up Arrow",
    "Key: Right Alt",
    "Key: Right Fn",
    "Key: Menu",
    "Key: Right Control",
    "Key: Left Arrow",
    "Key: Down Arrow",
    "Key: Right Arrow"
};

RGBController_HyperXAlloyOriginsCore::RGBController_HyperXAlloyOriginsCore(HyperXAlloyOriginsCoreController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name        = "HyperX Alloy Origins Core Keyboard Device";
    vendor      = "HyperX";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "HyperX Alloy Origins Core Keyboard Device";
    location    = hyperx->GetDeviceLocation();
    serial      = hyperx->GetSerialString();
    version     = hyperx->GetFirmwareVersion();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
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

    delete hyperx;
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
    hyperx->SetLEDsDirect(colors);
}

void RGBController_HyperXAlloyOriginsCore::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOriginsCore::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOriginsCore::SetCustomMode()
{

}

void RGBController_HyperXAlloyOriginsCore::DeviceUpdateMode()
{

}

void RGBController_HyperXAlloyOriginsCore::KeepaliveThread()
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
        std::this_thread::sleep_for(10ms);
    }
}
