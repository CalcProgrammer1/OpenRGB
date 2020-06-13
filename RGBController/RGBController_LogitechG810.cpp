/*-----------------------------------------*\
|  RGBController_LogitechG810.cpp           |
|                                           |
|  Generic RGB Interface for Logitech G810  |
|  Orion Spectrum Keyboard                  |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/12/2020   |
\*-----------------------------------------*/

#include "RGBController_LogitechG810.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[7][23] =
    { { 109,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 114, 112, 113,  NA, 111,  NA, 110, 108,  NA,  NA,  NA },
      {  37,  NA,  53,  54,  55,  56,  NA,  57,  58,  59,  60,  NA,  61,  62,  63,  64,  65,  66,  67, 107, 106, 105, 104 },
      {  48,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  NA,  41,  42,  38,  NA,  68,  69,  70,  78,  79,  80,  81 },
      {  39,  NA,  16,  22,   4,  17,  NA,  19,  24,  20,   8,  14,  15,  43,  44,  45,  71,  72,  73,  90,  91,  92,  82 },
      {  52,  NA,   0,  18,   3,   5,  NA,   6,   7,   9,  10,  11,  46,  47,  36,  NA,  NA,  NA,  NA,  87,  88,  89,  NA },
      {  97,  NA,  25,  23,   2,  21,  NA,   1,  NA,  13,  12,  49,  50,  51, 101,  NA,  NA,  77,  NA,  84,  85,  86,  83 },
      {  96,  99,  98,  NA,  NA,  NA,  NA,  40,  NA,  NA,  NA,  NA, 102, 103,  95, 100,  75,  76,  74,  93,  NA,  94,  NA } };

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
    115,
};

typedef struct
{
    const char *        name;
    const unsigned char zone;
    const unsigned char idx;
} led_type;

static const led_type led_names[] =
{
    /* Key Label                Zone,                                   Index  */
    { "Key: A",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x04    },
    { "Key: B",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x05    },
    { "Key: C",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x06    },
    { "Key: D",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x07    },
    { "Key: E",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x08    },
    { "Key: F",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x09    },
    { "Key: G",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x0A    },
    { "Key: H",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x0B    },
    { "Key: I",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x0C    },
    { "Key: J",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x0D    },
    { "Key: K",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x0E    },
    { "Key: L",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x0F    },
    { "Key: M",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x10    },
    { "Key: N",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x11    },
    { "Key: O",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x12    },
    { "Key: P",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x13    },
    { "Key: Q",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x14    },
    { "Key: R",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x15    },
    { "Key: S",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x16    },
    { "Key: T",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x17    },
    { "Key: U",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x18    },
    { "Key: V",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x19    },
    { "Key: W",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x1A    },
    { "Key: X",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x1B    },
    { "Key: Y",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x1C    },
    { "Key: Z",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x1D    },
    { "Key: 1",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x1E    },
    { "Key: 2",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x1F    },
    { "Key: 3",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x20    },
    { "Key: 4",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x21    },
    { "Key: 5",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x22    },
    { "Key: 6",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x23    },
    { "Key: 7",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x24    },
    { "Key: 8",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x25    },
    { "Key: 9",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x26    },
    { "Key: 0",                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x27    },
    { "Key: Enter",             LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x28    },
    { "Key: Escape",            LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x29    },
    { "Key: Backspace",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x2A    },
    { "Key: Tab",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x2B    },
    { "Key: Space",             LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x2C    },
    { "Key: -/_",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x2D    },
    { "Key: =/+",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x2E    },
    { "Key: [/{",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x2F    },
    { "Key: ]/}",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x30    },
    { "Key: \\/|",              LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x31    },
    { "Key: ;/:",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x33    },
    { "Key: '/'\"",             LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x34    },
    { "Key: `/~",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x35    },
    { "Key: ,/<",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x36    },
    { "Key: ./>",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x37    },
    { "Key: //?",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x38    },
    { "Key: Caps Lock",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x39    },
    { "Key: F1",                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x3A    },
    { "Key: F2",                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x3B    },
    { "Key: F3",                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x3C    },
    { "Key: F4",                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x3D    },
    { "Key: F5",                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x3E    },
    { "Key: F6",                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x3F    },
    { "Key: F7",                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x40    },
    { "Key: F8",                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x41    },
    { "Key: F9",                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x42    },
    { "Key: F10",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x43    },
    { "Key: F11",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x44    },
    { "Key: F12",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x45    },
    { "Key: Print Screen",      LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x46    },
    { "Key: Scroll Lock",       LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x47    },
    { "Key: Pause/Break",       LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x48    },
    { "Key: Insert",            LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x49    },
    { "Key: Home",              LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x4A    },
    { "Key: Page Up",           LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x4B    },
    { "Key: Delete",            LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x4C    },
    { "Key: End",               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x4D    },
    { "Key: Page Down",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x4E    },
    { "Key: Right Arrow",       LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x4F    },
    { "Key: Left Arrow",        LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x50    },
    { "Key: Down Arrow",        LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x51    },
    { "Key: Up Arrow",          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x52    },
    { "Key: Num Lock",          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x53    },
    { "Key: Num Pad /",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x54    },
    { "Key: Num Pad *",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x55    },
    { "Key: Num Pad -",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x56    },
    { "Key: Num Pad +",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x57    },
    { "Key: Num Pad Enter",     LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x58    },
    { "Key: Num Pad 1",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x59    },
    { "Key: Num Pad 2",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x5A    },
    { "Key: Num Pad 3",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x5B    },
    { "Key: Num Pad 4",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x5C    },
    { "Key: Num Pad 5",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x5D    },
    { "Key: Num Pad 6",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x5E    },
    { "Key: Num Pad 7",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x5F    },
    { "Key: Num Pad 8",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x60    },
    { "Key: Num Pad 9",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x61    },
    { "Key: Num Pad 0",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x62    },
    { "Key: Num Pad .",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x63    },
    { "Key: Context",           LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x65    },
    { "Key: Left Control",      LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE0    },
    { "Key: Left Shift",        LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE1    },
    { "Key: Left Alt",          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE2    },
    { "Key: Left Windows",      LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE3    },
    { "Key: Right Control",     LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE4    },
    { "Key: Right Shift",       LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE5    },
    { "Key: Right Alt",         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE6    },
    { "Key: Right Windows",     LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE7    },
    { "Media Next",             LOGITECH_G810_ZONE_DIRECT_MEDIA,        0xB5    },
    { "Media Previous",         LOGITECH_G810_ZONE_DIRECT_MEDIA,        0xB6    },
    { "Media Stop",             LOGITECH_G810_ZONE_DIRECT_MEDIA,        0xB7    },
    { "Media Play/Pause",       LOGITECH_G810_ZONE_DIRECT_MEDIA,        0xCD    },
    { "Media Mute",             LOGITECH_G810_ZONE_DIRECT_MEDIA,        0xE2    },
    { "Logo",                   LOGITECH_G810_ZONE_DIRECT_LOGO,         0x01    },
    { "Lighting",               LOGITECH_G810_ZONE_DIRECT_INDICATORS,   0x01    },
    { "Game Mode",              LOGITECH_G810_ZONE_DIRECT_INDICATORS,   0x02    },
    { "Caps Lock Indicator",    LOGITECH_G810_ZONE_DIRECT_INDICATORS,   0x03    },
    { "Scroll Lock Indicator",  LOGITECH_G810_ZONE_DIRECT_INDICATORS,   0x04    },
    { "Num Lock Indicator",     LOGITECH_G810_ZONE_DIRECT_INDICATORS,   0x05    },
};

RGBController_LogitechG810::RGBController_LogitechG810(LogitechG810Controller* logitech_ptr)
{
    logitech = logitech_ptr;

    name        = "Logitech Keyboard Device";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Logitech Keyboard Device";

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = LOGITECH_G810_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);
    
    mode Off;
    Off.name       = "Off";
    Off.value      = LOGITECH_G810_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Cycle;
    Cycle.name       = "Cycle";
    Cycle.value      = LOGITECH_G810_MODE_CYCLE;
    Cycle.flags      = MODE_FLAG_HAS_SPEED;
    Cycle.color_mode = MODE_COLORS_NONE;
    Cycle.speed_min  = LOGITECH_G810_SPEED_SLOWEST;
    Cycle.speed_max  = LOGITECH_G810_SPEED_FASTEST;
    Cycle.speed      = LOGITECH_G810_SPEED_NORMAL;
    modes.push_back(Cycle);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = LOGITECH_G810_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.speed_min  = LOGITECH_G810_SPEED_SLOWEST;
    Breathing.speed_max  = LOGITECH_G810_SPEED_FASTEST;
    Breathing.speed      = LOGITECH_G810_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
}

void RGBController_LogitechG810::SetupZones()
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
            new_zone.matrix_map->height = 7;
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
        new_led.name  = led_names[led_idx].name;
        new_led.value = ( led_names[led_idx].zone << 8 ) + led_names[led_idx].idx;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_LogitechG810::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechG810::DeviceUpdateLEDs()
{
    /*---------------------------------------------------------*\
    | TODO: Send packets with multiple LED frames               |
    \*---------------------------------------------------------*/
    for(int led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        UpdateSingleLED(led_idx);
    }
}

void RGBController_LogitechG810::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG810::UpdateSingleLED(int led)
{
    unsigned char frame[4];
    unsigned char zone;
    unsigned char idx;

    zone = ( leds[led].value >> 8 );
    idx  = ( leds[led].value & 0xFF );

    frame[0] = idx;
    frame[1] = RGBGetRValue(colors[led]);
    frame[2] = RGBGetGValue(colors[led]);
    frame[3] = RGBGetBValue(colors[led]);

    logitech->SetDirect(zone, 1, frame);
    logitech->Commit();
}

void RGBController_LogitechG810::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_LogitechG810::UpdateMode()
{
    /*---------------------------------------------------------*\
    | Direct mode does not send a mode packet                   |
    | Call UpdateLEDs to send direct packet                     |
    \*---------------------------------------------------------*/
    if(active_mode == 0xFFFF)
    {
        UpdateLEDs();
        return;
    }

    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    logitech->SetMode(modes[active_mode].value, modes[active_mode].speed, red, grn, blu);
}
