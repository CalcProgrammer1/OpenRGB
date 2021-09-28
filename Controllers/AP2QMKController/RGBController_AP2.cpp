/*-----------------------------------------*\
|  RGBController_AnnePro2.cpp               |
|                                           |
|  Generic RGB Interface for AnnePro2 w/QMK |
|                                           |
|  Sergey Gavrilov (DrZlo13) 06/06/2021     |
|  Tomasz Fortuna 26/09/2021 (QMK protocol) |
\*-----------------------------------------*/

#include "RGBController_AP2.h"

using namespace std::chrono_literals;

#define NA              0xFFFFFFFF
#define LED_REAL_COUNT  (5*14)
#define LED_COUNT       (LED_REAL_COUNT - 9)

static unsigned int matrix_map[5][14] =
    { { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13 },
      { 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 },
      { 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, NA },
      { 41, NA, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, NA }, 
      { 53, NA, 54, 55, NA, NA, 56, NA, NA, 57, 58, 59, 60, NA } };

static const char* zone_names[] =
{
    "Keyboard",
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX
};

static const unsigned int zone_sizes[] =
{
    LED_COUNT,
};

typedef struct
{
    const char *        name;
    const unsigned char idx;
} led_type;

static const led_type led_names[] =
{
    /* Key Label                Index  */
    { "Key: Escape",            0       },
    { "Key: 1",                 1       },
    { "Key: 2",                 2       },
    { "Key: 3",                 3       },
    { "Key: 4",                 4       },
    { "Key: 5",                 5       },
    { "Key: 6",                 6       },
    { "Key: 7",                 7       },
    { "Key: 8",                 8       },
    { "Key: 9",                 9       },
    { "Key: 0",                 10      },
    { "Key: -",                 11      },
    { "Key: =",                 12      },
    { "Key: Backspace",         13      },
    { "Key: Tab",               14      },
    { "Key: Q",                 15      },
    { "Key: W",                 16      },
    { "Key: E",                 17      },
    { "Key: R",                 18      },
    { "Key: T",                 19      },
    { "Key: Y",                 20      },
    { "Key: U",                 21      },
    { "Key: I",                 22      },
    { "Key: O",                 23      },
    { "Key: P",                 24      },
    { "Key: [",                 25      },
    { "Key: ]",                 26      },
    { "Key: \\ (ANSI)",         27      },
    { "Key: Caps Lock",         28      },
    { "Key: A",                 29      },
    { "Key: S",                 30      },
    { "Key: D",                 31      },
    { "Key: F",                 32      },
    { "Key: G",                 33      },
    { "Key: H",                 34      },
    { "Key: J",                 35      },
    { "Key: K",                 36      },
    { "Key: L",                 37      },
    { "Key: ;",                 38      },
    { "Key: '",                 39      },
    { "Key: Enter",             40      },
    { "Key: Left Shift",        41      },
    { "Key: Z",                 42      },
    { "Key: X",                 43      },
    { "Key: C",                 44      },
    { "Key: V",                 45      },
    { "Key: B",                 46      },
    { "Key: N",                 47      },
    { "Key: M",                 48      },
    { "Key: ,",                 49      },
    { "Key: .",                 50      },
    { "Key: /",                 51      },
    { "Key: Right Shift",       52      },
    { "Key: Left Control",      53      },
    { "Key: Left Windows",      54      },
    { "Key: Left Alt",          55      },
    { "Key: Space",             56      },
    { "Key: Right Alt",         57      },
    { "Key: Right Fn",          58      },
    { "Key: Menu",              59      },
    { "Key: Right Control",     60      },
};

RGBController_AP2::RGBController_AP2(AP2Controller* annepro2_ptr) 
{
    annepro2    = annepro2_ptr;

    name        = "Anne Pro 2 QMK/Shine";
    vendor      = "Obinslab";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Obinslab Anne Pro 2 Device with QMK firmware";
    location    = annepro2->GetDeviceLocation();
    serial      = annepro2->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_AP2::~RGBController_AP2() 
{
    delete annepro2;
}

void RGBController_AP2::SetupZones() 
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
            new_zone.matrix_map->height = 5;
            new_zone.matrix_map->width  = 14;
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
        new_led.name  = led_names[led_idx].name;
        new_led.value = led_names[led_idx].idx;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_AP2::ResizeZone(int /*zone*/, int /*new_size*/) 
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AP2::DeviceUpdateLEDs() 
{
    unsigned char frame_buf[64];

    /* Send rows separately */
    for (int row=0; row < 5; row++) {
        frame_buf[0] = row;
        /* NOTE: That's very not optimal, but easy for firmware, handles all
         * colors and works. Using a 256-color palette one could update whole
         * keyboard with a single RAW HID. */
        for (int col=0; col < 14; col++) {
            const unsigned int led_idx = matrix_map[row][col];
            if (led_idx == NA)
                continue;
            frame_buf[1 + col * 4 + 0] = RGBGetBValue(colors[led_idx]);
            frame_buf[1 + col * 4 + 1] = RGBGetGValue(colors[led_idx]);
            frame_buf[1 + col * 4 + 2] = RGBGetRValue(colors[led_idx]);
            frame_buf[1 + col * 4 + 3] = 0xff;
        }
        annepro2->SendDirect(4 * 14 + 1, frame_buf);

        /* Minimal processing time; without it, later frames get destroyed */
        std::this_thread::sleep_for(5ms);
    }
}

void RGBController_AP2::UpdateZoneLEDs(int /*zone*/) 
{
    DeviceUpdateLEDs();
}

void RGBController_AP2::UpdateSingleLED(int /*led*/) 
{
    DeviceUpdateLEDs();
}

void RGBController_AP2::SetCustomMode() 
{
    active_mode = 0;
}

void RGBController_AP2::DeviceUpdateMode() 
{
    
}
