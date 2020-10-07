/*-----------------------------------------*\
|  RGBController_WootingKeyboard.cpp        |
|                                           |
|  Generic RGB Interface for Wooting RGB    |
|  keyboard devices                         |
|                                           |
|  Diogo Trindade (diogotr7)    3/4/2021    |
\*-----------------------------------------*/

#include "RGBController_WootingKeyboard.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA 0xFFFFFFFF
#define RGB_RAW_BUFFER_SIZE 96

static unsigned int matrix_map[6][17] =
{
    {0, NA, 12, 18, 24, 30, 36, 42, 48, 54, 60, 66, 72, 78, 84, 90, 96},
    {1, 7,  13, 19, 25, 31, 37, 43, 49, 55, 61, 67, 73, 79, 85, 91, 97},
    {2, 8,  14, 20, 26, 32, 38, 44, 50, 56, 62, 68, 74, 80, 86, 92, 98},
    {3, 9,  15, 21, 27, 33, 39, 45, 51, 57, 63, 69, 75, 81, NA, NA, NA},
    {4, 10, 16, 22, 28, 34, 40, 46, 52, 58, 64, 70, NA, 82, NA, 94, NA},
    {5, 11, 17, NA, NA, NA, 41, NA, NA, NA, 65, 71, 77, 83, 89, 95, 101}
};

static const char *zone_names[] =
{
    "Keyboard"
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const unsigned int zone_sizes[] =
{
    102
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
    "Key: \\ (ISO)", //iso key - 10
    "Key: Left Windows",
    "Key: F1",
    "Key: 2",
    "Key: W",
    "Key: S",
    "Key: Z",
    "Key: Left Alt",
    "Key: F2",
    "Key: 3",
    "Key: E", //20
    "Key: D",
    "Key: X",
    "Unused", //space
    "Key: F3",
    "Key: 4",
    "Key: R",
    "Key: F",
    "Key: C",
    "Unused",  //space
    "Key: F4", //30
    "Key: 5",
    "Key: T",
    "Key: G",
    "Key: V",
    "Unused", //space
    "Key: F5",
    "Key: 6",
    "Key: Y",
    "Key: H",
    "Key: B", //40
    "Key: Space",
    "Key: F6",
    "Key: 7",
    "Key: U",
    "Key: J",
    "Key: N",
    "Unused", //space
    "Key: F7",
    "Key: 8",
    "Key: I",
    "Key: K",
    "Key: M",
    "Unused", //space
    "Key: F8",
    "Key: 9",
    "Key: O",
    "Key: L",
    "Key: ,",
    "Unused", //space
    "Key: F9",
    "Key: 0",
    "Key: P",
    "Key: ;",
    "Key: .",
    "Key: Right Alt",
    "Key: F10",
    "Key: -",
    "Key: [",
    "Key: '",
    "Key: /",
    "Key: Right Windows",
    "Key: F11",
    "Key: =",
    "Key: ]",
    "Key: #", //iso only
    "Unused",
    "Key: Right Fn",
    "Key: F12",
    "Key: Backspace",
    "Key: \\ (ANSI)",
    "Key: Enter",
    "Key: Right Shift",
    "Key: Right Control",
    "Key: Print Screen",
    "Key: Insert",
    "Key: Delete",
    "Unused",
    "Unused",
    "Key: Left Arrow",
    "Key: Scroll Lock",
    "Key: Pause/Break",
    "Key: Home",
    "Key: End",
    "Unused",
    "Key: Up Arrow",
    "Key: Down Arrow",
    "Key: Mode",
    "Key: Page Up",
    "Key: Page Down",
    "Unused",
    "Unused",
    "Key: Right Arrow"
};

RGBController_WootingKeyboard::RGBController_WootingKeyboard(WootingKeyboardController *wooting_ptr)
{
    wooting     = wooting_ptr;

    name        = "Wooting keyboard Device";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Wooting Keyboard Device";

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_WootingKeyboard::~RGBController_WootingKeyboard()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for (unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if (zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }
}

void RGBController_WootingKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;

    for (unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        zone new_zone;

        new_zone.name               = zone_names[zone_idx];
        new_zone.type               = zone_types[zone_idx];
        new_zone.leds_min           = zone_sizes[zone_idx];
        new_zone.leds_max           = zone_sizes[zone_idx];
        new_zone.leds_count         = zone_sizes[zone_idx];
        new_zone.matrix_map         = new matrix_map_type;
        new_zone.matrix_map->height = 6;
        new_zone.matrix_map->width  = 17;
        new_zone.matrix_map->map    = (unsigned int *)&matrix_map;

        zones.push_back(new_zone);

        total_led_count += zone_sizes[zone_idx];
    }

    for (unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;

        new_led.name                = led_names[led_idx];

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_WootingKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_WootingKeyboard::DeviceUpdateLEDs()
{
    wooting->SendDirect(&colors[0], colors.size());
}

void RGBController_WootingKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WootingKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WootingKeyboard::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_WootingKeyboard::DeviceUpdateMode()
{
}
