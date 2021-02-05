/*-----------------------------------------*\
|  RGBController_HyperXKeyboard.cpp         |
|                                           |
|  Generic RGB Interface for HyperX RGB     |
|  Keyboard                                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/2/2020    |
\*-----------------------------------------*/

#include "RGBController_HyperXKeyboard.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   0,  NA,  16,  30,  44,  54,  NA,  65,  75,  84,  95,  NA,   8,  23 ,  38,   6 ,  22,  36,  49,  NA,  NA,  NA,  NA },
      {   1,  17,  31,  45,  55,  66,  76,  85,  96,   9,  24,  NA,  39,   7 ,  37,  NA ,  60,  70,  80,  52,  63,  73,  82 },
      {   2,  NA,  18,  32,  46,  56,  NA,  67,  77,  86,  97,  10,  25,  40 ,  90,  101,  50,  61,  71,  51,  62,  72,  93 },
      {   3,  NA,  19,  33,  47,  57,  NA,  68,  78,  87,  98,  11,  26,  41 ,  28,  14 ,  NA,  NA,  NA,  92, 103,  53,  NA },
      {   4,  20,  34,  48,  58,  69,  NA,  79,  NA,  88,  99,  12,  27,  42 ,  81,  NA ,  NA, 102,  NA,  64,  74,  83, 104 },
      {   5,  21,  35,  NA,  NA,  NA,  NA,  59,  NA,  NA,  NA,  NA,  89,  100,  13,  91 ,  15,  29,  43,  94,  NA, 105,  NA } };

static const char* zone_names[] =
{
    "Keyboard",
    "RGB Strip",
    "Media Keys"
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
    ZONE_TYPE_LINEAR,
    ZONE_TYPE_SINGLE
};

static const unsigned int zone_sizes[] =
{
    106,
    18,
    4
};

static const char *led_names[] =
{
    "Key: Escape",
    "Key: `",
    "Key: Tab",
    "Key: Caps Lock",
    "Key: Left Shift",
    "Key: Left Control",
    "Key: F12",
    "Key: =",
    "Key: F9",
    "Key: 9",
    "Key: O",
    "Key: L",
    "Key: ,",
    "Key: Menu",
    "Key: Enter (ISO)",
    "Key: Left Arrow",
    "Key: F1",
    "Key: 1",
    "Key: Q",
    "Key: A",
    "Key: \\ (ISO)",
    "Key: Left Windows",
    "Key: Print Screen",
    "Key: F10",
    "Key: 0",
    "Key: P",
    "Key: ;",
    "Key: .",
    "Key: Enter",
    "Key: Down Arrow",
    "Key: F2",
    "Key: 2",
    "Key: W",
    "Key: S",
    "Key: Z",
    "Key: Left Alt",
    "Key: Scroll Lock",
    "Key: Backspace",
    "Key: F11",
    "Key: -",
    "Key: [",
    "Key: '",
    "Key: /",
    "Key: Right Arrow",
    "Key: F3",
    "Key: 3",
    "Key: E",
    "Key: D",
    "Key: X",
    "Key: Pause/Break",
    "Key: Delete",
    "Key: Number Pad 7",
    "Key: Num Lock",
    "Key: Number Pad 6",
    "Key: F4",
    "Key: 4",
    "Key: R",
    "Key: F",
    "Key: C",
    "Key: Space",
    "Key: Insert",
    "Key: End",
    "Key: Number Pad 8",
    "Key: Number Pad /",
    "Key: Number Pad 1",
    "Key: F5",
    "Key: 5",
    "Key: T",
    "Key: G",
    "Key: V",
    "Key: Home",
    "Key: Page Down",
    "Key: Number Pad 9",
    "Key: Number Pad *",
    "Key: Number Pad 2",
    "Key: F6",
    "Key: 6",
    "Key: Y",
    "Key: H",
    "Key: B",
    "Key: Page Up",
    "Key: Right Shift",
    "Key: Number Pad -",
    "Key: Number Pad 3",
    "Key: F7",
    "Key: 7",
    "Key: U",
    "Key: J",
    "Key: N",
    "Key: Right Alt",
    "Key: ]",
    "Key: Right Control",
    "Key: Number Pad 4",
    "Key: Number Pad +",
    "Key: Number Pad 0",
    "Key: F8",
    "Key: 8",
    "Key: I",
    "Key: K",
    "Key: M",
    "Key: Right Windows",
    "Key: \\ (ANSI)",
    "Key: Up Arrow",
    "Key: Number Pad 5",
    "Key: Number Pad Enter",
    "Key: Number Pad .",
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
    "Key: Media Previous",
    "Key: Media Play/Pause",
    "Key: Media Next",
    "Key: Media Mute"
};

RGBController_HyperXKeyboard::RGBController_HyperXKeyboard(HyperXKeyboardController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name        = "HyperX RGB Keyboard";
    vendor      = "HyperX";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "HyperX RGB Keyboard Device";
    location    = hyperx->GetDeviceLocation();
    serial      = hyperx->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = HYPERX_MODE_STATIC;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = HYPERX_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Wave;
    Wave.name       = "Wave";
    Wave.value      = HYPERX_MODE_WAVE;
    Wave.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.speed_min  = 0x00;
    Wave.speed_max  = 0x09;
    Wave.color_mode = MODE_COLORS_NONE;
    Wave.speed      = 0x09;
    Wave.direction  = MODE_DIRECTION_LEFT;
    modes.push_back(Wave);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = HYPERX_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Breathing.speed_min  = 0x00;
    Breathing.speed_max  = 0x09;
    Breathing.colors_min = 1;
    Breathing.colors_max = 2;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed      = 0x09;
    Breathing.colors.resize(2);
    modes.push_back(Breathing);

    SetupZones();

    /*-----------------------------------------------------*\
    | The Corsair Lighting Node Pro requires a packet within|
    | 20 seconds of sending the lighting change in order    |
    | to not revert back into rainbow mode.  Start a thread |
    | to continuously send a keepalive packet every 5s      |
    \*-----------------------------------------------------*/
    keepalive_thread_run = true;
    keepalive_thread = new std::thread(&RGBController_HyperXKeyboard::KeepaliveThreadFunction, this);
}

RGBController_HyperXKeyboard::~RGBController_HyperXKeyboard()
{
    keepalive_thread_run = false;
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

void RGBController_HyperXKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < 3; zone_idx++)
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

void RGBController_HyperXKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXKeyboard::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    if(active_mode == 0)
    {
        hyperx->SetLEDsDirect(colors);
    }
    else
    {
        hyperx->SetLEDs(colors);
    }
}

void RGBController_HyperXKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXKeyboard::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_HyperXKeyboard::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        hyperx->SetMode(modes[active_mode].value, modes[active_mode].direction, modes[active_mode].speed, modes[active_mode].colors);
    }
    else
    {
        std::vector<RGBColor> temp_colors;
        hyperx->SetMode(modes[active_mode].value, modes[active_mode].direction, modes[active_mode].speed, temp_colors);
    }
}

void RGBController_HyperXKeyboard::KeepaliveThreadFunction()
{
    while(keepalive_thread_run)
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
