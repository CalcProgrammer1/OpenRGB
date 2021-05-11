/*-----------------------------------------*\
|  RGBController_DuckyKeyboard.cpp          |
|                                           |
|  Generic RGB Interface for Ducky RGB      |
|  keyboard devices                         |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/4/2020    |
\*-----------------------------------------*/

#include "RGBController_DuckyKeyboard.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   0,  NA,  12,  18,  24,  30,  NA,  42,  48,  54,  60,  NA,  66,  72,  78,  84,  90,  96, 102, 108, 114, 120, 126 },
      {   1,   7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  NA,  67,  73,  85,  NA,  91,  97, 103, 109, 115, 121, 127 },
      {   2,  NA,   8,  14,  20,  26,  NA,  32,  38,  44,  50,  56,  62,  68,  74,  86,  92,  98, 104, 110, 116, 122, 128 },
      {   3,  NA,   9,  15,  21,  27,  NA,  33,  39,  45,  51,  57,  63,  69,  75,  87,  NA,  NA,  NA, 111, 117, 123,  NA },
      {   4,  10,  16,  22,  28,  34,  NA,  40,  NA,  46,  52,  58,  64,  70,  82,  NA,  NA, 100,  NA, 112, 118, 124, 131 },
      {   5,  11,  17,  NA,  NA,  NA,  NA,  41,  NA,  NA,  NA,  NA,  65,  77,  83,  89,  95, 101, 107, 113,  NA, 125,  NA } };

static const char* zone_names[] =
{
    "Keyboard"
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const unsigned int zone_sizes[] =
{
    132
};

static const char *led_names[] =
{
    "Key: Escape",
    "Key: `",
    "Key: Tab",
    "Key: Caps Lock",
    "Key: Left Shift",
    "Key: Left Control",
    "Unused",
    "Key: 1",
    "Key: Q",
    "Key: A",
    "Key: \\ (ISO)",
    "Key: Left Windows",
    "Key: F1",
    "Key: 2",
    "Key: W",
    "Key: S",
    "Key: Z",
    "Key: Left Alt",
    "Key: F2",
    "Key: 3",
    "Key: E",
    "Key: D",
    "Key: X",
    "Unused",
    "Key: F3",
    "Key: 4",
    "Key: R",
    "Key: F",
    "Key: C",
    "Unused",
    "Key: F4",
    "Key: 5",
    "Key: T",
    "Key: G",
    "Key: V",
    "Unused",
    "Unused",
    "Key: 6",
    "Key: Y",
    "Key: H",
    "Key: B",
    "Key: Space",
    "Key: F5",
    "Key: 7",
    "Key: U",
    "Key: J",
    "Key: N",
    "Unused",
    "Key: F6",
    "Key: 8",
    "Key: I",
    "Key: K",
    "Key: M",
    "Unused",
    "Key: F7",
    "Key: 9",
    "Key: O",
    "Key: L",
    "Key: ,",
    "Unused",
    "Key: F8",
    "Key: 0",
    "Key: P",
    "Key: ;",
    "Key: .",
    "Key: Right Alt",
    "Key: F9",
    "Key: -",
    "Key: [",
    "Key: '",
    "Key: /",
    "Unused",
    "Key: F10",
    "Key: =",
    "Key: ]",
    "Key: #",
    "Unused",
    "Key: Right Windows",
    "Key: F11",
    "Unused",
    "Unused",
    "Unused",
    "Key: Right Shift",
    "Key: Right Fn",
    "Key: F12",
    "Key: Backspace",
    "Key: \\ (ANSI)",
    "Key: Enter",
    "Unused",
    "Key: Right Control",
    "Key: Print Screen",
    "Key: Insert",
    "Key: Delete",
    "Unused",
    "Unused",
    "Key: Left Arrow",
    "Key: Scroll Lock",
    "Key: Home",
    "Key: End",
    "Unused",
    "Key: Up Arrow",
    "Key: Down Arrow",
    "Key: Pause/Break",
    "Key: Page Up",
    "Key: Page Down",
    "Unused",
    "Unused",
    "Key: Right Arrow",
    "Key: Calculator",
    "Key: Num Lock",
    "Key: Number Pad 7",
    "Key: Number Pad 4",
    "Key: Number Pad 1",
    "Key: Number Pad 0",
    "Key: Media Mute",
    "Key: Number Pad /",
    "Key: Number Pad 8",
    "Key: Number Pad 5",
    "Key: Number Pad 2",
    "Unused",
    "Key: Media Volume Down",
    "Key: Number Pad *",
    "Key: Number Pad 9",
    "Key: Number Pad 6",
    "Key: Number Pad 3",
    "Key: Number Pad .",
    "Key: Media Volume Up",
    "Key: Number Pad -",
    "Key: Number Pad +",
    "Unused",
    "Unused",
    "Key: Number Pad Enter",
};

RGBController_DuckyKeyboard::RGBController_DuckyKeyboard(DuckyKeyboardController* ducky_ptr)
{
    ducky = ducky_ptr;

    name        = "Ducky Keyboard Device";
    vendor      = "Ducky";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Ducky Keyboard Device";
    location    = ducky->GetDeviceLocation();
    serial      = ducky->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_DuckyKeyboard::~RGBController_DuckyKeyboard()
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

    delete ducky;
}

void RGBController_DuckyKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        zone new_zone;
        new_zone.name               = zone_names[zone_idx];
        new_zone.type               = zone_types[zone_idx];
        new_zone.leds_min           = zone_sizes[zone_idx];
        new_zone.leds_max           = zone_sizes[zone_idx];
        new_zone.leds_count         = zone_sizes[zone_idx];
        new_zone.matrix_map         = new matrix_map_type;
        new_zone.matrix_map->height = 6;
        new_zone.matrix_map->width  = 23;
        new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
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

void RGBController_DuckyKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}
    
void RGBController_DuckyKeyboard::DeviceUpdateLEDs()
{
    unsigned char colordata[155*3];
    
    for(std::size_t color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        colordata[(color_idx*3)+0] = RGBGetRValue(colors[color_idx]);
        colordata[(color_idx*3)+1] = RGBGetGValue(colors[color_idx]);
        colordata[(color_idx*3)+2] = RGBGetBValue(colors[color_idx]);
    }

    ducky->SendColors(colordata, sizeof(colordata));
}

void RGBController_DuckyKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_DuckyKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_DuckyKeyboard::SetCustomMode()
{

}

void RGBController_DuckyKeyboard::DeviceUpdateMode()
{

}
