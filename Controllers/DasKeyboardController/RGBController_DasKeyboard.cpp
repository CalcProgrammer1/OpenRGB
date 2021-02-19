/*-----------------------------------------*\
|  RGBController_DasKeyboard.cpp            |
|                                           |
|  Generic RGB Interface for Das Keyboard   |
|  RGB keyboard devices                     |
|                                           |
|  Frank Niessen (denk_mal) 12/16/2020      |
\*-----------------------------------------*/

#include "RGBController_DasKeyboard.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

// US Layout TODO: mus be checked/corrected
static unsigned int matrix_map_us[7][21] =
    {
        {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA, 126,  NA,  NA,  NA},
        { 5, NA, 17, 23, 29, 35, 41, 47, 53, 59, 65, 71, 77, 83, 89, 95, 101, 127, 128, 129, 130},
        { 4, 10, 16, 22, 28, 34, 40, 46, 52, 58, 64, 70, 76, 82, 88, 94, 100, 106, 112, 118, 124},
        { 3,  9, 15, 21, 27, 33, 39, 45, 51, 57, 63, 69, 75,  7, 87, 93,  99, 105, 111, 117, 123},
        { 2,  8, 14, 20, 26, 32, 38, 44, 50, 56, 62, 68, NA, 80, NA, NA,  NA, 104, 110, 116,  NA},
        { 1, NA, 13, 19, 25, 31, 37, 43, 49, 55, 61, 67, 79, NA, NA, 91,  NA, 103, 109, 115, 122},
        { 0,  6, 12, NA, NA, NA, 36, NA, NA, NA, 60, 66, 72, 78, 84, 90,  96, 102,  NA, 114,  NA}
    };

// EU Layout
static unsigned int matrix_map_eu[7][21] =
    {
        {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA, 126,  NA,  NA,  NA},
        { 5, NA, 17, 23, 29, 35, 41, 47, 53, 59, 65, 71, 77, 83, 89, 95, 101, 127, 128, 129, 130},
        { 4, 10, 16, 22, 28, 34, 40, 46, 52, 58, 64, 70, 76, 82, 88, 94, 100, 106, 112, 118, 124},
        { 3,  9, 15, 21, 27, 33, 39, 45, 51, 57, 63, 69, 75, NA, 87, 93,  99, 105, 111, 117, 123},
        { 2,  8, 14, 20, 26, 32, 38, 44, 50, 56, 62, 68, 81, 80, NA, NA,  NA, 104, 110, 116,  NA},
        { 1,  7, 13, 19, 25, 31, 37, 43, 49, 55, 61, 67, 79, NA, NA, 91,  NA, 103, 109, 115, 122},
        { 0,  6, 12, NA, NA, NA, 36, NA, NA, NA, 60, 66, 72, 78, 84, 90,  96, 102,  NA, 114,  NA}
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
        131
    };

// UK Layout
static const char *led_names[] =
    {
        "Key: Left Control",
        "Key: Left Shift",
        "Key: Caps Lock",
        "Key: Tab",
        "Key: `",
        "Key: Escape",
        "Key: Left Windows",
        "Key: \\ (ANSI)",
        "Key: A",
        "Key: Q",
        "Key: 1",
        "Undefined",
        "Key: Left Alt",
        "Key: Z",
        "Key: S",
        "Key: W",
        "Key: 2",
        "Key: F1",
        "Undefined",
        "Key: X",
        "Key: D",
        "Key: E",
        "Key: 3",
        "Key: F2",
        "Undefined",
        "Key: C",
        "Key: F",
        "Key: R",
        "Key: 4",
        "Key: F3",
        "Undefined",
        "Key: V",
        "Key: G",
        "Key: T",
        "Key: 5",
        "Key: F4",
        "Key: Space",
        "Key: B",
        "Key: H",
        "Key: Y",
        "Key: 6",
        "Key: F5",
        "Undefined",
        "Key: N",
        "Key: J",
        "Key: U",
        "Key: 7",
        "Key: F6",
        "Undefined",
        "Key: M",
        "Key: K",
        "Key: I",
        "Key: 8",
        "Key: F7",
        "Undefined",
        "Key: ,",
        "Key: L",
        "Key: O",
        "Key: 9",
        "Key: F8",
        "Key: Right Alt",
        "Key: .",
        "Key: ;",
        "Key: P",
        "Key: 0",
        "Key: F9",
        "Key: Right Windows",
        "Key: /",
        "Key: '",
        "Key: [",
        "Key: -",
        "Key: F10",
        "Key: Menu",
        "Undefined",
        "Undefined",
        "Key: ]",
        "Key: =",
        "Key: F11",
        "Key: Right Control",
        "Key: Right Shift",
        "Key: Enter",
        "Key: #",
        "Key: Backspace",
        "Key: F12",
        "Key: Left Arrow",
        "Undefined",
        "Undefined",
        "Key: Delete",
        "Key: Insert",
        "Key: Print Screen",
        "Key: Down Arrow",
        "Key: Up Arrow",
        "Undefined",
        "Key: End",
        "Key: Home",
        "Key: Scroll Lock",
        "Key: Right Arrow",
        "Undefined",
        "Undefined",
        "Key: Page Down",
        "Key: Page Up",
        "Key: Pause/Break",
        "Key: Number Pad 0",
        "Key: Number Pad 1",
        "Key: Number Pad 4",
        "Key: Number Pad 7",
        "Key: Num Lock",
        "Undefined",
        "Undefined",
        "Key: Number Pad 2",
        "Key: Number Pad 5",
        "Key: Number Pad 8",
        "Key: Number Pad /",
        "Undefined",
        "Key: Number Pad .",
        "Key: Number Pad 3",
        "Key: Number Pad 6",
        "Key: Number Pad 9",
        "Key: Number Pad *",
        "Undefined",
        "Undefined",
        "Undefined",
        "Key: Number Pad Enter",
        "Key: Number Pad +",
        "Key: Number Pad -",
        "Undefined",
        "Key: Sleep",
        "Key: Brightness",
        "Key: Media Play/Pause",
        "Key: Media Next",
        "Key: Q-Button"
    };

RGBController_DasKeyboard::RGBController_DasKeyboard(DasKeyboardController *das_ptr)
{
    das = das_ptr;

    for(unsigned int ii = 0; ii < zone_sizes[0]; ii++)
    {
        double_buffer.push_back(-1);
    }

    updateDevice = true;

    name        = "Das Keyboard Device";
    vendor      = "Metadot";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Das Keyboard Device";
    location    = das->GetDeviceLocation();
    serial      = das->GetSerialString();
    version     = das->GetVersionString();

    modes.resize(4);
    modes[0].name       = "Direct";
    modes[0].value      = DAS_KEYBOARD_MODE_DIRECT;
    modes[0].flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    modes[0].color_mode = MODE_COLORS_PER_LED;

    modes[1].name       = "Flashing";
    modes[1].value      = DAS_KEYBOARD_MODE_FLASHING;
    modes[1].flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    modes[1].color_mode = MODE_COLORS_PER_LED;

    modes[2].name       = "Breathing";
    modes[2].value      = DAS_KEYBOARD_MODE_BREATHING;
    modes[2].flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    modes[2].color_mode = MODE_COLORS_PER_LED;

    modes[3].name       = "Spectrum Cycle";
    modes[3].value      = DAS_KEYBOARD_MODE_SPECTRUM_CYCLE;
    modes[3].flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    modes[3].color_mode = MODE_COLORS_PER_LED;
}

RGBController_DasKeyboard::~RGBController_DasKeyboard()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    unsigned int zone_size = zones.size();

    for(unsigned int zone_index = 0; zone_index < zone_size; zone_index++)
    {
        delete zones[zone_index].matrix_map;
    }

    delete das;
}

void RGBController_DasKeyboard::SetupZones()
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
        new_zone.matrix_map->height = 7;
        new_zone.matrix_map->width  = 21;

        if(das->GetLayoutString() == "US")
        {
            new_zone.matrix_map->map = (unsigned int *) &matrix_map_us;
        }
        else
        {
            new_zone.matrix_map->map = (unsigned int *) &matrix_map_eu;
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

void RGBController_DasKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_DasKeyboard::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
}

void RGBController_DasKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    updateDevice = false;

    for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        UpdateSingleLED(static_cast<int>(led_idx));
    }

    updateDevice = true;

    das->SendApply();
}

void RGBController_DasKeyboard::UpdateSingleLED(int led)
{
    mode selected_mode = modes[active_mode];

    if(double_buffer[led] == colors[led])
    {
        return;
    }

    das->SendColors(led, selected_mode.value,
                    RGBGetRValue(colors[led]),
                    RGBGetGValue(colors[led]),
                    RGBGetBValue(colors[led]));

    double_buffer[led] = colors[led];

    if(updateDevice)
    {
        das->SendApply();
    }
}

void RGBController_DasKeyboard::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_DasKeyboard::DeviceUpdateMode()
{
}
