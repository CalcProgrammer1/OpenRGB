/*---------------------------------------------------------*\
| RGBController_LogitechG810.cpp                            |
|                                                           |
|   RGBController for Logitech G810 Orion Spectrum          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                12 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_LogitechG810.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[7][23] =
    { { 111,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA, 116, 114, 115,  NA, 113,  NA, 112, 110,  NA,  NA,  NA },
      {  37,  NA,  54,  55,  56,  57,  NA,  58,  59,  60,  61,  NA,  62,  63,  64,  65,  66,  67,  68, 109, 108, 107, 106 },
      {  49,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  NA,  41,  42,  38,  NA,  69,  70,  71,  79,  80,  81,  82 },
      {  39,  NA,  16,  22,   4,  17,  NA,  19,  24,  20,   8,  14,  15,  43,  44,  45,  72,  73,  74,  91,  92,  93,  83 },
      {  53,  NA,   0,  18,   3,   5,  NA,   6,   7,   9,  10,  11,  47,  48,  46,  36,  NA,  NA,  NA,  88,  89,  90,  NA },
      {  99,  96,  25,  23,   2,  21,  NA,   1,  NA,  13,  12,  50,  51,  52, 103,  NA,  NA,  78,  NA,  85,  86,  87,  84 },
      {  98, 101, 100,  NA,  NA,  NA,  NA,  40,  NA,  NA,  NA,  NA, 104, 105,  97, 102,  76,  77,  75,  94,  NA,  95,  NA } };

static const char* zone_names[] =
{
    ZONE_EN_KEYBOARD,
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const unsigned int zone_sizes[] =
{
    117,
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
    { KEY_EN_A,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x04    },
    { KEY_EN_B,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x05    },
    { KEY_EN_C,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x06    },
    { KEY_EN_D,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x07    },
    { KEY_EN_E,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x08    },
    { KEY_EN_F,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x09    },
    { KEY_EN_G,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x0A    },
    { KEY_EN_H,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x0B    },
    { KEY_EN_I,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x0C    },
    { KEY_EN_J,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x0D    },
    { KEY_EN_K,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x0E    },
    { KEY_EN_L,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x0F    },
    { KEY_EN_M,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x10    },
    { KEY_EN_N,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x11    },
    { KEY_EN_O,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x12    },
    { KEY_EN_P,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x13    },
    { KEY_EN_Q,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x14    },
    { KEY_EN_R,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x15    },
    { KEY_EN_S,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x16    },
    { KEY_EN_T,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x17    },
    { KEY_EN_U,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x18    },
    { KEY_EN_V,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x19    },
    { KEY_EN_W,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x1A    },
    { KEY_EN_X,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x1B    },
    { KEY_EN_Y,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x1C    },
    { KEY_EN_Z,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x1D    },
    { KEY_EN_1,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x1E    },
    { KEY_EN_2,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x1F    },
    { KEY_EN_3,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x20    },
    { KEY_EN_4,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x21    },
    { KEY_EN_5,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x22    },
    { KEY_EN_6,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x23    },
    { KEY_EN_7,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x24    },
    { KEY_EN_8,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x25    },
    { KEY_EN_9,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x26    },
    { KEY_EN_0,                 LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x27    },
    { KEY_EN_ANSI_ENTER,        LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x28    },
    { KEY_EN_ESCAPE,            LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x29    },
    { KEY_EN_BACKSPACE,         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x2A    },
    { KEY_EN_TAB,               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x2B    },
    { KEY_EN_SPACE,             LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x2C    },
    { KEY_EN_MINUS,             LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x2D    },
    { KEY_EN_EQUALS,            LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x2E    },
    { KEY_EN_LEFT_BRACKET,      LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x2F    },
    { KEY_EN_RIGHT_BRACKET,     LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x30    },
    { KEY_EN_ANSI_BACK_SLASH,   LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x31    },//ANSI only
    { KEY_EN_POUND,             LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x32    },//ISO only
    { KEY_EN_SEMICOLON,         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x33    },
    { KEY_EN_QUOTE,             LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x34    },
    { KEY_EN_BACK_TICK,         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x35    },
    { KEY_EN_COMMA,             LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x36    },
    { KEY_EN_PERIOD,            LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x37    },
    { KEY_EN_FORWARD_SLASH,     LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x38    },
    { KEY_EN_CAPS_LOCK,         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x39    },
    { KEY_EN_F1,                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x3A    },
    { KEY_EN_F2,                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x3B    },
    { KEY_EN_F3,                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x3C    },
    { KEY_EN_F4,                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x3D    },
    { KEY_EN_F5,                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x3E    },
    { KEY_EN_F6,                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x3F    },
    { KEY_EN_F7,                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x40    },
    { KEY_EN_F8,                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x41    },
    { KEY_EN_F9,                LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x42    },
    { KEY_EN_F10,               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x43    },
    { KEY_EN_F11,               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x44    },
    { KEY_EN_F12,               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x45    },
    { KEY_EN_PRINT_SCREEN,      LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x46    },
    { KEY_EN_SCROLL_LOCK,       LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x47    },
    { KEY_EN_PAUSE_BREAK,       LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x48    },
    { KEY_EN_INSERT,            LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x49    },
    { KEY_EN_HOME,              LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x4A    },
    { KEY_EN_PAGE_UP,           LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x4B    },
    { KEY_EN_DELETE,            LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x4C    },
    { KEY_EN_END,               LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x4D    },
    { KEY_EN_PAGE_DOWN,         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x4E    },
    { KEY_EN_RIGHT_ARROW,       LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x4F    },
    { KEY_EN_LEFT_ARROW,        LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x50    },
    { KEY_EN_DOWN_ARROW,        LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x51    },
    { KEY_EN_UP_ARROW,          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x52    },
    { KEY_EN_NUMPAD_LOCK,       LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x53    },
    { KEY_EN_NUMPAD_DIVIDE,     LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x54    },
    { KEY_EN_NUMPAD_TIMES,      LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x55    },
    { KEY_EN_NUMPAD_MINUS,      LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x56    },
    { KEY_EN_NUMPAD_PLUS,       LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x57    },
    { KEY_EN_NUMPAD_ENTER,      LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x58    },
    { KEY_EN_NUMPAD_1,          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x59    },
    { KEY_EN_NUMPAD_2,          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x5A    },
    { KEY_EN_NUMPAD_3,          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x5B    },
    { KEY_EN_NUMPAD_4,          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x5C    },
    { KEY_EN_NUMPAD_5,          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x5D    },
    { KEY_EN_NUMPAD_6,          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x5E    },
    { KEY_EN_NUMPAD_7,          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x5F    },
    { KEY_EN_NUMPAD_8,          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x60    },
    { KEY_EN_NUMPAD_9,          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x61    },
    { KEY_EN_NUMPAD_0,          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x62    },
    { KEY_EN_NUMPAD_PERIOD,     LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x63    },
    { KEY_EN_ISO_BACK_SLASH,    LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x64    },//ISO only
    { KEY_EN_MENU,              LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0x65    },
    { KEY_EN_LEFT_CONTROL,      LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE0    },
    { KEY_EN_LEFT_SHIFT,        LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE1    },
    { KEY_EN_LEFT_ALT,          LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE2    },
    { KEY_EN_LEFT_WINDOWS,      LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE3    },
    { KEY_EN_RIGHT_CONTROL,     LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE4    },
    { KEY_EN_RIGHT_SHIFT,       LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE5    },
    { KEY_EN_RIGHT_ALT,         LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE6    },
    { KEY_EN_RIGHT_WINDOWS,     LOGITECH_G810_ZONE_DIRECT_KEYBOARD,     0xE7    },
    { KEY_EN_MEDIA_NEXT,        LOGITECH_G810_ZONE_DIRECT_MEDIA,        0xB5    },
    { KEY_EN_MEDIA_PREVIOUS,    LOGITECH_G810_ZONE_DIRECT_MEDIA,        0xB6    },
    { KEY_EN_MEDIA_STOP,        LOGITECH_G810_ZONE_DIRECT_MEDIA,        0xB7    },
    { KEY_EN_MEDIA_PLAY_PAUSE,  LOGITECH_G810_ZONE_DIRECT_MEDIA,        0xCD    },
    { KEY_EN_MEDIA_MUTE,        LOGITECH_G810_ZONE_DIRECT_MEDIA,        0xE2    },
    { "Logo",                   LOGITECH_G810_ZONE_DIRECT_LOGO,         0x01    },
    { "Lighting",               LOGITECH_G810_ZONE_DIRECT_INDICATORS,   0x01    },
    { "Game Mode",              LOGITECH_G810_ZONE_DIRECT_INDICATORS,   0x02    },
    { "Caps Lock Indicator",    LOGITECH_G810_ZONE_DIRECT_INDICATORS,   0x03    },
    { "Scroll Lock Indicator",  LOGITECH_G810_ZONE_DIRECT_INDICATORS,   0x04    },
    { "Num Lock Indicator",     LOGITECH_G810_ZONE_DIRECT_INDICATORS,   0x05    },
};

/**------------------------------------------------------------------*\
    @name Logitech G810
    @category Keyboard
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectLogitechKeyboardG810
    @comment
\*-------------------------------------------------------------------*/

RGBController_LogitechG810::RGBController_LogitechG810(LogitechG810Controller* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetNameString();
    vendor                          = "Logitech";
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = "Logitech Keyboard Device";
    serial                          = controller->GetSerialString();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = 0xFFFF;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = LOGITECH_G810_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = LOGITECH_G810_MODE_OFF;
    Off.flags                       = 0;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Cycle;
    Cycle.name                      = "Cycle";
    Cycle.value                     = LOGITECH_G810_MODE_CYCLE;
    Cycle.flags                     = MODE_FLAG_HAS_SPEED;
    Cycle.color_mode                = MODE_COLORS_NONE;
    Cycle.speed_min                 = LOGITECH_G810_SPEED_SLOWEST;
    Cycle.speed_max                 = LOGITECH_G810_SPEED_FASTEST;
    Cycle.speed                     = LOGITECH_G810_SPEED_NORMAL;
    modes.push_back(Cycle);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = LOGITECH_G810_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.speed_min             = LOGITECH_G810_SPEED_SLOWEST;
    Breathing.speed_max             = LOGITECH_G810_SPEED_FASTEST;
    Breathing.speed                 = LOGITECH_G810_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_LogitechG810::~RGBController_LogitechG810()
{
    delete controller;
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
            new_zone.matrix_map.Set(7, 23, (unsigned int *)&matrix_map);
        }

        zones.push_back(new_zone);

        total_led_count += zone_sizes[zone_idx];
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name                    = led_names[led_idx].name;
        new_led.value                   = ( led_names[led_idx].zone << 8 ) + led_names[led_idx].idx;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_LogitechG810::DeviceUpdateLEDs()
{
    #define MAX_FRAMES_PER_PACKET 0x0E

    unsigned char frame_buf[MAX_FRAMES_PER_PACKET * 4];
    unsigned char frame_cnt = 0;
    unsigned char prev_zone = 0;
    unsigned char zone      = 0;
    unsigned char idx       = 0;

    /*---------------------------------------------------------*\
    | TODO: Send packets with multiple LED frames               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        zone = ( leds[led_idx].value >> 8 );
        idx  = ( leds[led_idx].value & 0xFF );

        if((zone != prev_zone) && (frame_cnt != 0))
        {
            controller->SetDirect(prev_zone, frame_cnt, frame_buf);
            frame_cnt = 0;
        }

        frame_buf[(frame_cnt * 4) + 0] = idx;
        frame_buf[(frame_cnt * 4) + 1] = RGBGetRValue(colors[led_idx]);
        frame_buf[(frame_cnt * 4) + 2] = RGBGetGValue(colors[led_idx]);
        frame_buf[(frame_cnt * 4) + 3] = RGBGetBValue(colors[led_idx]);

        frame_cnt++;
        prev_zone = zone;

        if(frame_cnt == MAX_FRAMES_PER_PACKET)
        {
            controller->SetDirect(prev_zone, frame_cnt, frame_buf);
            frame_cnt = 0;
        }
    }

    if(frame_cnt != 0)
    {
        controller->SetDirect(prev_zone, frame_cnt, frame_buf);
    }

    controller->Commit();
}

void RGBController_LogitechG810::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG810::DeviceUpdateSingleLED(int led)
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

    controller->SetDirect(zone, 1, frame);
    controller->Commit();
}

void RGBController_LogitechG810::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | Direct mode does not send a mode packet                   |
    | Call UpdateLEDs to send direct packet                     |
    \*---------------------------------------------------------*/
    if(active_mode == 0xFFFF)
    {
        UpdateLEDsInternal();
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

    controller->SetMode(modes[active_mode].value, modes[active_mode].speed, red, grn, blu);
}
