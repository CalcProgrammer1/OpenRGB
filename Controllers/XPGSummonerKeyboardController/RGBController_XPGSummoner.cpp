/*---------------------------------------------------------*\
| RGBController_XPGSummoner.cpp                             |
|                                                           |
|   RGBController for XPG Summoner keyboard                 |
|                                                           |
|   Erick Granados (eriosgamer)                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_XPGSummoner.h"

#define NA 0xFFFFFFFF
#define LED_REAL_COUNT (6 * 21)
#define LED_COUNT (LED_REAL_COUNT - 22)

/*---------------------------------------------------------*\
| ordered_matrix: Physical LED layout                       |
\*---------------------------------------------------------*/
static unsigned int ordered_matrix[6][21] =
{
    {0, NA, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, NA, NA, NA, NA},
    {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36},
    {37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57},
    {58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, NA, 70, NA, NA, NA, 71, 72, 73, NA},
    {74, NA, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, NA, NA, 86, NA, 87, 88, 89, 90},
    {91, 92, 93, NA, NA, NA, 94, NA, NA, NA, 95, 96, 97, 98, 99, 100, 101, 102, NA, 103, NA}
};

/*---------------------------------------------------------*\
| matrix_map: Logical LED mapping                           |
\*---------------------------------------------------------*/
static unsigned int matrix_map[6][21] =
{
    {11, NA, 22, 30, 25, 27, 7, 51, 57, 62, 86, 87, 83, 85, 79, 72, 0, NA, NA, NA, NA},
    {14, 15, 23, 31, 39, 38, 46, 47, 55, 63, 71, 70, 54, 81, 102, 118, 110, 92, 100, 108, 109},
    {9, 8, 16, 24, 32, 33, 41, 40, 48, 56, 64, 65, 49, 82, 94, 119, 111, 88, 96, 104, 112},
    {17, 10, 18, 26, 34, 35, 43, 42, 50, 58, 66, 67, NA, 84, NA, NA, NA, 89, 97, 105, NA},
    {121, NA, 12, 20, 28, 36, 37, 45, 44, 52, 60, 69, 122, NA, NA, 115, NA, 90, 98, 106, 114},
    {6, 124, 75, NA, NA, NA, 91, NA, NA, NA, 77, 125, 61, 4, 117, 93, 101, 99, NA, 107, NA}
};

/*---------------------------------------------------------*\
| zone_names: Zone names                                    |
\*---------------------------------------------------------*/
const char *zone_names[] =
{
    ZONE_EN_KEYBOARD
};

zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX
};

const unsigned int zone_sizes[] =
{
    LED_COUNT
};

/*---------------------------------------------------------*\
| led_names: LED names                                      |
\*---------------------------------------------------------*/
static const char *led_names[] =
{
    KEY_EN_ESCAPE,          // Esc
    KEY_EN_F1,              // F1
    KEY_EN_F2,              // F2
    KEY_EN_F3,              // F3
    KEY_EN_F4,              // F4
    KEY_EN_F5,              // F5
    KEY_EN_F6,              // F6
    KEY_EN_F7,              // F7
    KEY_EN_F8,              // F8
    KEY_EN_F9,              // F9
    KEY_EN_F10,             // F10
    KEY_EN_F11,             // F11
    KEY_EN_F12,             // F12
    KEY_EN_PRINT_SCREEN,    // PrtSc
    KEY_EN_SCROLL_LOCK,     // Scroll
    KEY_EN_PAUSE_BREAK,     // Pause
    KEY_EN_BACK_TICK,       // `
    KEY_EN_1,               // 1
    KEY_EN_2,               // 2
    KEY_EN_3,               // 3
    KEY_EN_4,               // 4
    KEY_EN_5,               // 5
    KEY_EN_6,               // 6
    KEY_EN_7,               // 7
    KEY_EN_8,               // 8
    KEY_EN_9,               // 9
    KEY_EN_0,               // 0
    KEY_EN_MINUS,           // -
    KEY_EN_EQUALS,          // =
    KEY_EN_BACKSPACE,       // Backspace
    KEY_EN_INSERT,          // Insert
    KEY_EN_HOME,            // Home
    KEY_EN_PAGE_UP,         // PgUp
    KEY_EN_NUMPAD_LOCK,     // NumLock
    KEY_EN_NUMPAD_DIVIDE,   // /
    KEY_EN_NUMPAD_TIMES,    // *
    KEY_EN_NUMPAD_MINUS,    // -
    KEY_EN_TAB,             // Tab
    KEY_EN_Q,               // Q
    KEY_EN_W,               // W
    KEY_EN_E,               // E
    KEY_EN_R,               // R
    KEY_EN_T,               // T
    KEY_EN_Y,               // Y
    KEY_EN_U,               // U
    KEY_EN_I,               // I
    KEY_EN_O,               // O
    KEY_EN_P,               // P
    KEY_EN_LEFT_BRACKET,    // [
    KEY_EN_RIGHT_BRACKET,   // ]
    KEY_EN_ANSI_BACK_SLASH, // Backslash
    KEY_EN_DELETE,          // Del
    KEY_EN_END,             // End
    KEY_EN_PAGE_DOWN,       // PgDn
    KEY_EN_NUMPAD_7,        // 7
    KEY_EN_NUMPAD_8,        // 8
    KEY_EN_NUMPAD_9,        // 9
    KEY_EN_NUMPAD_PLUS,     // +
    KEY_EN_CAPS_LOCK,       // Caps
    KEY_EN_A,               // A
    KEY_EN_S,               // S
    KEY_EN_D,               // D
    KEY_EN_F,               // F
    KEY_EN_G,               // G
    KEY_EN_H,               // H
    KEY_EN_J,               // J
    KEY_EN_K,               // K
    KEY_EN_L,               // L
    KEY_EN_SEMICOLON,       // ;
    KEY_EN_QUOTE,           // '
    KEY_EN_ISO_ENTER,       // Enter
    KEY_EN_NUMPAD_4,        // 4
    KEY_EN_NUMPAD_5,        // 5
    KEY_EN_NUMPAD_6,        // 6
    KEY_EN_LEFT_SHIFT,      // Shift
    KEY_EN_Z,               // Z
    KEY_EN_X,               // X
    KEY_EN_C,               // C
    KEY_EN_V,               // V
    KEY_EN_B,               // B
    KEY_EN_N,               // N
    KEY_EN_M,               // M
    KEY_EN_COMMA,           // ,
    KEY_EN_PERIOD,          // .
    KEY_EN_FORWARD_SLASH,   // /
    KEY_EN_RIGHT_SHIFT,     // Shift
    KEY_EN_UP_ARROW,        // ↑
    KEY_EN_NUMPAD_1,        // 1
    KEY_EN_NUMPAD_2,        // 2
    KEY_EN_NUMPAD_3,        // 3
    KEY_EN_NUMPAD_ENTER,    // Enter
    KEY_EN_LEFT_CONTROL,    // Ctrl
    KEY_EN_LEFT_WINDOWS,    // Win
    KEY_EN_LEFT_ALT,        // Alt
    KEY_EN_SPACE,           // Space
    KEY_EN_RIGHT_ALT,       // AltGr
    KEY_EN_RIGHT_FUNCTION,  // Fn
    KEY_EN_MENU,            // Menu
    KEY_EN_RIGHT_CONTROL,   // Ctrl
    KEY_EN_LEFT_ARROW,      // ←
    KEY_EN_DOWN_ARROW,      // ↓
    KEY_EN_RIGHT_ARROW,     // →
    KEY_EN_NUMPAD_0,        // 0
    KEY_EN_NUMPAD_PERIOD    // .
};

/**------------------------------------------------------------------*\
    @name XPG Summoner Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectXPGSummonerControllers
    @comment
\*-------------------------------------------------------------------*/

/*---------------------------------------------------------*\
| RGBController_XPGSummoner constructor                     |
\*---------------------------------------------------------*/
RGBController_XPGSummoner::RGBController_XPGSummoner(XPGSummonerController *controller_ptr)
{
    controller   = controller_ptr;
    name         = controller->GetNameString();
    vendor       = "XPG";
    description  = "XPG Summoner Keyboard Device";
    location     = controller->GetLocationString();
    serial       = controller->GetSerialString();
    type         = DEVICE_TYPE_KEYBOARD;

    mode Direct;
    Direct.name        = "Direct";
    Direct.value       = XPG_SUMMONER_MODE_DIRECT;
    Direct.flags       = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode  = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name           = "Static";
    Static.value          = XPG_SUMMONER_MODE_STATIC;
    Static.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min     = 1;
    Static.colors_max     = 1;
    Static.brightness_min = 5;
    Static.brightness_max = 64;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Stars;
    Stars.name         = "Stars";
    Stars.value        = XPG_SUMMONER_MODE_STARS;
    Stars.flags        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Stars.colors_min   = 1;
    Stars.colors_max   = 1;
    Stars.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    Stars.colors.resize(1);
    modes.push_back(Stars);

    mode Off;
    Off.name           = "Off";
    Off.value          = XPG_SUMMONER_MODE_OFF;
    Off.flags          = 0;
    Off.color_mode     = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

/*---------------------------------------------------------*\
| Destructor                                                |
\*---------------------------------------------------------*/
RGBController_XPGSummoner::~RGBController_XPGSummoner()
{
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }
    delete controller;
}

/*---------------------------------------------------------*\
| SetupZones: Initializes zones and LEDs                    |
\*---------------------------------------------------------*/
void RGBController_XPGSummoner::SetupZones()
{
    leds.clear();
    colors.clear();
    zones.clear();
    leds.reserve(LED_COUNT);
    colors.reserve(LED_COUNT);
    zones.reserve(1);

    zone new_zone;
    new_zone.name       = zone_names[0];
    new_zone.type       = zone_types[0];
    new_zone.leds_min   = zone_sizes[0];
    new_zone.leds_max   = zone_sizes[0];
    new_zone.leds_count = zone_sizes[0];

    if(new_zone.type == ZONE_TYPE_MATRIX)
    {
        new_zone.matrix_map         = new matrix_map_type;
        new_zone.matrix_map->height = 6;
        new_zone.matrix_map->width  = 21;
        new_zone.matrix_map->map    = (unsigned int *)&ordered_matrix;
    }
    else
    {
        new_zone.matrix_map = NULL;
    }
    zones.push_back(new_zone);

    size_t linear_idx = 0;
    for(int row = 0; row < 6; ++row)
    {
        for(int col = 0; col < 21; ++col)
        {
            unsigned int led_id = matrix_map[row][col];
            if(led_id == NA)
                continue;
            led new_led;
            new_led.name  = led_names[linear_idx];
            new_led.value = led_id;
            leds.push_back(new_led);
            ++linear_idx;
        }
    }
    colors.assign(LED_COUNT, 0x000000);

    SetupColors();
}

/*---------------------------------------------------------*\
| ResizeZone: Not supported for this device                 |
\*---------------------------------------------------------*/
void RGBController_XPGSummoner::ResizeZone(int /*zone*/, int /*new_size*/)
{
    // This device does not support resizing zones
}

/*---------------------------------------------------------*\
| DeviceUpdateLEDs: Updates LED colors                      |
\*---------------------------------------------------------*/
void RGBController_XPGSummoner::DeviceUpdateLEDs()
{
    const unsigned char brightness            = 0x64;
    const unsigned int frame_buf_length       = 126 * 4;
    unsigned char frame_buf[frame_buf_length] = {0};

    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        if(leds[led_idx].value == NA)
            continue;
        if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
        {
            std::size_t real_idx          = leds[led_idx].value;
            frame_buf[(real_idx * 4) + 0] = brightness;
            frame_buf[(real_idx * 4) + 1] = RGBGetRValue(colors[led_idx]);
            frame_buf[(real_idx * 4) + 2] = RGBGetGValue(colors[led_idx]);
            frame_buf[(real_idx * 4) + 3] = RGBGetBValue(colors[led_idx]);
        }
        else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC && modes[active_mode].value == XPG_SUMMONER_MODE_STATIC)
        {
            std::size_t real_idx          = leds[led_idx].value;
            frame_buf[(real_idx * 4) + 0] = modes[active_mode].brightness;
            frame_buf[(real_idx * 4) + 1] = RGBGetRValue(modes[active_mode].colors[0]);
            frame_buf[(real_idx * 4) + 2] = RGBGetGValue(modes[active_mode].colors[0]);
            frame_buf[(real_idx * 4) + 3] = RGBGetBValue(modes[active_mode].colors[0]);
        }
        else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC && modes[active_mode].value == XPG_SUMMONER_MODE_STARS)
        {
            for(std::size_t i = 0; i < leds.size(); ++i)
            {
                std::size_t real_idx          = leds[i].value;
                frame_buf[(real_idx * 4) + 0] = 0;
                frame_buf[(real_idx * 4) + 1] = 0;
                frame_buf[(real_idx * 4) + 2] = 0;
                frame_buf[(real_idx * 4) + 3] = 0;
            }
            controller->SendColors(frame_buf, sizeof(frame_buf));

            /*---------------------------------------------------*\
            | Select a random central LED in the physical matrix  |
            \*---------------------------------------------------*/
            int rows                = 6;
            int cols                = 21;
            int center_row          = rand() % rows;
            int center_col          = rand() % cols;
            unsigned int center_led = matrix_map[center_row][center_col];

            if(center_led != NA)
            {
                frame_buf[(center_led * 4) + 0] = brightness;
                frame_buf[(center_led * 4) + 1] = RGBGetRValue(modes[active_mode].colors[0]);
                frame_buf[(center_led * 4) + 2] = RGBGetGValue(modes[active_mode].colors[0]);
                frame_buf[(center_led * 4) + 3] = RGBGetBValue(modes[active_mode].colors[0]);
            }

            /*---------------------------------------------------*\
            | Neighbors (fade effect)                             |
            \*---------------------------------------------------*/
            unsigned char fade_brightness = 0x32;
            int neighbor_offsets[4][2]    = { {0, -1}, {0, +1}, {-1, 0}, {+1, 0} };

            for(int k = 0; k < 4; ++k)
            {
                int n_row = center_row + neighbor_offsets[k][0];
                int n_col = center_col + neighbor_offsets[k][1];
                if(n_row >= 0 && n_row < rows && n_col >= 0 && n_col < cols)
                {
                    unsigned int neighbor_led = matrix_map[n_row][n_col];
                    if(neighbor_led != NA)
                    {
                        frame_buf[(neighbor_led * 4) + 0] = fade_brightness;
                        frame_buf[(neighbor_led * 4) + 1] = RGBGetRValue(modes[active_mode].colors[0]);
                        frame_buf[(neighbor_led * 4) + 2] = RGBGetGValue(modes[active_mode].colors[0]);
                        frame_buf[(neighbor_led * 4) + 3] = RGBGetBValue(modes[active_mode].colors[0]);
                    }
                }
            }
            controller->SendColors(frame_buf, sizeof(frame_buf));
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        else if(modes[active_mode].color_mode == MODE_COLORS_NONE)
        {
            std::size_t real_idx          = leds[led_idx].value;
            frame_buf[(real_idx * 4) + 0] = 0;
            frame_buf[(real_idx * 4) + 1] = 0;
            frame_buf[(real_idx * 4) + 2] = 0;
            frame_buf[(real_idx * 4) + 3] = 0;
        }
    }
    controller->SendColors(frame_buf, sizeof(frame_buf));
}

/*---------------------------------------------------------*\
| UpdateZoneLEDs: Updates all LEDs in a zone                |
\*---------------------------------------------------------*/
void RGBController_XPGSummoner::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

/*---------------------------------------------------------*\
| UpdateSingleLED: Updates a single LED                     |
\*---------------------------------------------------------*/
void RGBController_XPGSummoner::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

/*---------------------------------------------------------*\
| DeviceUpdateMode: Updates device mode                     |
\*---------------------------------------------------------*/
void RGBController_XPGSummoner::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

