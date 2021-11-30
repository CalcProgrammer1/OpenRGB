/*-----------------------------------------*\
|  RGBController_RoccatVulcanAimo.cpp       |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|                                           |
|                                           |
|  Mola19 17/12/2021                        |
\*-----------------------------------------*/

#include "RGBController_RoccatVulcanAimo.h"
#include <vector>

#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][22] =
    { {   0,  NA,  11,  17,  23,  28,  NA,  48,  53,  59,  65,  78,  84,  85,  86,  99, 103, 108,  NA,  NA,  NA,  NA },
      {   1,   6,  12,  18,  24,  29,  33,  49,  54,  60,  66,  72,  79,  87,  NA, 100, 104, 109, 113, 119, 124, 129 },
      {   2,  NA,   7,  13,  19,  25,  30,  34,  50,  55,  61,  67,  73,  80,  NA, 101, 105, 110, 114, 120, 125, 130 },
      {   3,  NA,   8,  14,  20,  26,  31,  35,  51,  56,  62,  68,  74,  96,  88,  NA,  NA,  NA, 115, 121, 126,  NA },
      {   4,  NA,   9,  15,  21,  27,  32,  36,  52,  57,  63,  69,  75,  82,  NA,  NA, 106,  NA, 116, 122, 127, 131 },
      {   5,  10,  16,  NA,  NA,  NA,  37,  NA,  NA,  NA,  NA,  70,  76,  83,  89, 102, 107, 111, 117,  NA, 128,  NA } };

static const unsigned int zone_size = 132;

static const char *led_names[] =
{
    "Key: Escape",
    "Key: ^",
    "Key: Tab",
    "Key: Caps Lock",
    "Key: Left Shift",
    "Key: Left Control",
    "Key: 1",
    "Key: Q",
    "Key: A",
    "Key: <",
    "Key: Left Windows",
    "Key: F1",
    "Key: 2",
    "Key: W",
    "Key: S",
    "Key: Y",
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
    "Key: F4",
    "Key: 5",
    "Key: T",
    "Key: G",
    "Key: V",
    "Key: 6",
    "Key: Z",
    "Key: H",
    "Key: B",
    "Key: Space",
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
    "Key: F5",
    "Key: 7",
    "Key: U",
    "Key: J",
    "Key: N",
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
    "Key: Ö",
    "Key: .",
    "Key: Right Alt",
    "Unused",
    "Key: ß",
    "Key: Ü",
    "Key: Ä",
    "Key: -",
    "Key: Right Fn",
    "Unused",
    "Key: F9",
    "Key: ´",
    "Key: +",
    "Unused",
    "Key: Right Shift",
    "Key: Menu",
    "Key: F10",
    "Key: F11",
    "Key: F12",
    "Key: Backspace",
    "Key: Enter",
    "Key: Right Control",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Key: #",
    "Unused",
    "Unused",
    "Key: Print Screen",
    "Key: Insert",
    "Key: Delete",
    "Key: Left Arrow",
    "Key: Scroll Lock",
    "Key: Home",
    "Key: End",
    "Key: Up Arrow",
    "Key: Down Arrow",
    "Key: Pause/Break",
    "Key: Page Up",
    "Key: Page Down",
    "Key: Right Arrow",
    "Unused",
    "Key: Num Lock",
    "Key: Number Pad 7",
    "Key: Number Pad 4",
    "Key: Number Pad 1",
    "Key: Number Pad 0",
    "Unused",
    "Key: Number Pad /",
    "Key: Number Pad 8",
    "Key: Number Pad 5",
    "Key: Number Pad 2",
    "Unused",
    "Key: Number Pad *",
    "Key: Number Pad 9",
    "Key: Number Pad 6",
    "Key: Number Pad 3",
    "Key: Number Pad ,",
    "Key: Number Pad -",
    "Key: Number Pad +",
    "Key: Number Pad Enter",
};

RGBController_RoccatVulcanAimo::RGBController_RoccatVulcanAimo(RoccatVulcanAimoController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Roccat Vulcan 120 Aimo";
    vendor      = "Roccat";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Roccat Vulcan Aimo Keyboard";
    location    = controller->GetLocation();
    serial      = controller->GetSerial();

    mode Direct;
    Direct.name       = "Direct";
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_RoccatVulcanAimo::~RGBController_RoccatVulcanAimo()
{
    delete controller;
}

void RGBController_RoccatVulcanAimo::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones and leds per zone                            |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name               = "Keyboard";
    new_zone.type               = ZONE_TYPE_MATRIX;
    new_zone.leds_min           = zone_size;
    new_zone.leds_max           = zone_size;
    new_zone.leds_count         = zone_size;
    new_zone.matrix_map         = new matrix_map_type;
    new_zone.matrix_map->height = 6;
    new_zone.matrix_map->width  = 22;
    new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
    zones.push_back(new_zone);

    for(unsigned int led_idx = 0; led_idx < zone_size; led_idx++)
    {
        led new_led;
        new_led.name = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_RoccatVulcanAimo::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RoccatVulcanAimo::DeviceUpdateLEDs()
{
    controller->SendColors(colors);
}

void RGBController_RoccatVulcanAimo::UpdateZoneLEDs(int /*zone_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatVulcanAimo::UpdateSingleLED(int /*led_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatVulcanAimo::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_RoccatVulcanAimo::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | Changing modes is currently not implemented               |
    \*---------------------------------------------------------*/
}
