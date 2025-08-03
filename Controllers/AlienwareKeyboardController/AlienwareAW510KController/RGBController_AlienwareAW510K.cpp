/*---------------------------------------------------------*\
| RGBController_AlienwareAW510K.cpp                         |
|                                                           |
|   RGBController for Alienware AW510K keyboard             |
|                                                           |
|   Mohamad Sallal (msallal)                    22 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <iterator>
#include "RGBControllerKeyNames.h"
#include "RGBController_AlienwareAW510K.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

int GetAW520K_WaveDirection(int input);

static unsigned int matrix_map[7][24] =
{ {  0,   NA,   1,   2,   3,   4,  NA,   5,   6,   7,   8,   9,  10,  11,  12,  NA,  13,  14,  15,  NA,  16,  NA,  NA,  NA },
  {  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  NA,  NA,  31,  32,  33,  NA,  34,  35,  36,  37 },
  {  38,  NA,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  NA,  52,  53,  54,  NA,  55,  56,  57,  74 },
  {  58,  NA,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  NA,  70,  NA,  NA,  NA,  NA,  NA,  71,  72,  73,  NA },
  {  75,  NA,  NA,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  NA,  NA,  NA,  87,  NA,  NA,  88,  89,  90, 104 },
  {  91,  NA,  92,  93,  NA,  94,  NA,  NA,  NA,  NA,  NA,  95,  96,  97,  98,  NA,  99, 100, 101,  NA, 102,  NA, 103,  NA },
  {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  105, NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA }};

static const char* zone_names[] =
{
    "AW510K",
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const unsigned int zone_sizes[] =
{
    106,
};

typedef struct
{
    const char *        name;
    const unsigned char zone;
    const unsigned char idx;
} aw510k_led_type;

static const aw510k_led_type led_names[] =
{
    /* Key Label                Zone,                                      Index  */
    { KEY_EN_ESCAPE,            ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0xB0    },
    { KEY_EN_F1,                ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x98    },
    { KEY_EN_F2,                ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x90    },
    { KEY_EN_F3,                ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x88    },
    { KEY_EN_F4,                ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x80    },
    { KEY_EN_F5,                ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x70    },
    { KEY_EN_F6,                ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x68    },
    { KEY_EN_F7,                ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x60    },
    { KEY_EN_F8,                ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x58    },
    { KEY_EN_F9,                ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x50    },
    { KEY_EN_F10,               ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x48    },
    { KEY_EN_F11,               ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x40    },
    { KEY_EN_F12,               ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x38    },
    { KEY_EN_PRINT_SCREEN,      ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x30    },
    { KEY_EN_SCROLL_LOCK,       ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x28    },
    { KEY_EN_PAUSE_BREAK,       ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x20    },
    { KEY_EN_MEDIA_MUTE,        ALIENWARE_AW510K_ZONE_DIRECT_MEDIA,        0x18    },
    { KEY_EN_BACK_TICK,         ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0xB1    },
    { KEY_EN_1,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0xA1    },
    { KEY_EN_2,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x99    },
    { KEY_EN_3,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x91    },
    { KEY_EN_4,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x89    },
    { KEY_EN_5,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x81    },
    { KEY_EN_6,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x79    },
    { KEY_EN_7,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x71    },
    { KEY_EN_8,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x69    },
    { KEY_EN_9,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x61    },
    { KEY_EN_0,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x59    },
    { KEY_EN_MINUS,             ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x51    },
    { KEY_EN_EQUALS,            ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x49    },
    { KEY_EN_BACKSPACE,         ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x39    },
    { KEY_EN_INSERT,            ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x31    },
    { KEY_EN_HOME,              ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x29    },
    { KEY_EN_PAGE_UP,           ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x21    },
    { KEY_EN_NUMPAD_LOCK,       ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x19    },
    { KEY_EN_NUMPAD_DIVIDE,     ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x11    },
    { KEY_EN_NUMPAD_TIMES,      ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x09    },
    { KEY_EN_NUMPAD_MINUS,      ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x01    },
    { KEY_EN_TAB,               ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0xB2    },
    { KEY_EN_Q,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0xA2    },
    { KEY_EN_W,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x9A    },
    { KEY_EN_E,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x92    },
    { KEY_EN_R,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x8A    },
    { KEY_EN_T,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x82    },
    { KEY_EN_Y,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x7A    },
    { KEY_EN_U,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x72    },
    { KEY_EN_I,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x6A    },
    { KEY_EN_O,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x62    },
    { KEY_EN_P,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x5A    },
    { KEY_EN_LEFT_BRACKET,      ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x52    },
    { KEY_EN_RIGHT_BRACKET,     ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x4A    },
    { KEY_EN_ANSI_BACK_SLASH,   ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x42    },//ANSI only
    { KEY_EN_DELETE,            ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x32    },
    { KEY_EN_END,               ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x2A    },
    { KEY_EN_PAGE_DOWN,         ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x22    },
    { KEY_EN_NUMPAD_7,          ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x1A    },
    { KEY_EN_NUMPAD_8,          ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x12    },
    { KEY_EN_NUMPAD_9,          ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x0A    },
    { KEY_EN_CAPS_LOCK,         ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0xB3    },
    { KEY_EN_A,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0xA3    },
    { KEY_EN_S,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x9B    },
    { KEY_EN_D,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x93    },
    { KEY_EN_F,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x8B    },
    { KEY_EN_G,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x83    },
    { KEY_EN_H,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x7B    },
    { KEY_EN_J,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x73    },
    { KEY_EN_K,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x6B    },
    { KEY_EN_L,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x63    },
    { KEY_EN_SEMICOLON,         ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x5B    },
    { KEY_EN_QUOTE,             ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x53    },
    { KEY_EN_ANSI_ENTER,        ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x43    },
    { KEY_EN_NUMPAD_4,          ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x1B    },
    { KEY_EN_NUMPAD_5,          ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x13    },
    { KEY_EN_NUMPAD_6,          ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x0B    },
    { KEY_EN_NUMPAD_PLUS,       ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x03    },
    { KEY_EN_LEFT_SHIFT,        ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0xB4    },
    { KEY_EN_Z,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0xA4    },
    { KEY_EN_X,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x9C    },
    { KEY_EN_C,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x94    },
    { KEY_EN_V,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x8C    },
    { KEY_EN_B,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x84    },
    { KEY_EN_N,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x7C    },
    { KEY_EN_M,                 ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x74    },
    { KEY_EN_COMMA,             ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x6C    },
    { KEY_EN_PERIOD,            ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x64    },
    { KEY_EN_FORWARD_SLASH,     ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x5C    },
    { KEY_EN_RIGHT_SHIFT,       ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x4C    },
    { KEY_EN_UP_ARROW,          ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x2C    },
    { KEY_EN_NUMPAD_1,          ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x1C    },
    { KEY_EN_NUMPAD_2,          ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x14    },
    { KEY_EN_NUMPAD_3,          ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x0C    },
    { KEY_EN_LEFT_CONTROL,      ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0xB5    },
    { KEY_EN_LEFT_WINDOWS,      ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0xAD    },
    { KEY_EN_LEFT_ALT,          ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0xA5    },
    { KEY_EN_SPACE,             ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x85    },
    { KEY_EN_RIGHT_ALT,         ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x65    },
    { KEY_EN_RIGHT_FUNCTION,    ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x5D    },
    { KEY_EN_MENU,              ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x55    },
    { KEY_EN_RIGHT_CONTROL,     ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x4D    },
    { KEY_EN_LEFT_ARROW,        ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x35    },
    { KEY_EN_DOWN_ARROW,        ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x2D    },
    { KEY_EN_RIGHT_ARROW,       ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x25    },
    { KEY_EN_NUMPAD_0,          ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x1D    },
    { KEY_EN_NUMPAD_PERIOD,     ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x0D    },
    { KEY_EN_NUMPAD_ENTER,      ALIENWARE_AW510K_ZONE_DIRECT_KEYBOARD,     0x05    },
    { "Logo",                   ALIENWARE_AW510K_ZONE_DIRECT_LOGO,         0x07    }
};

/**------------------------------------------------------------------*\
    @name Alienware AW510 Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAlienwareAW510KControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AlienwareAW510K::RGBController_AlienwareAW510K(AlienwareAW510KController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "Alienware";
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = "Alienware AW510K Keyboard Device";
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = ALIENWARE_AW510K_MODE_DIRECT_PER_LED;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = ALIENWARE_AW510K_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Pulse;
    Pulse.name                      = "Pulse";
    Pulse.value                     = ALIENWARE_AW510K_MODE_PULSE;
    Pulse.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Pulse.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Pulse.speed_min                 = ALIENWARE_AW510K_SPEED_SLOWEST;
    Pulse.speed_max                 = ALIENWARE_AW510K_SPEED_NORMAL;
    Pulse.speed                     = ALIENWARE_AW510K_SPEED_NORMAL;
    Pulse.colors_min                = 1;
    Pulse.colors_max                = 1;
    Pulse.colors.resize(1);
    modes.push_back(Pulse);

    mode Morph;
    Morph.name                      = "Morph";
    Morph.value                     = ALIENWARE_AW510K_MODE_MORPH;
    Morph.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Morph.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Morph.speed_min                 = ALIENWARE_AW510K_SPEED_SLOWEST;
    Morph.speed_max                 = ALIENWARE_AW510K_SPEED_NORMAL;
    Morph.speed                     = ALIENWARE_AW510K_SPEED_NORMAL;
    Morph.colors_min                = 2;
    Morph.colors_max                = 2;
    Morph.colors.resize(2);
    modes.push_back(Morph);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = ALIENWARE_AW510K_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.speed_min             = ALIENWARE_AW510K_SPEED_SLOWEST;
    Breathing.speed_max             = ALIENWARE_AW510K_SPEED_FASTEST;
    Breathing.speed                 = ALIENWARE_AW510K_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode SingleWave;
    SingleWave.name                  = "Single Wave";
    SingleWave.value                 = ALIENWARE_AW510K_MODE_SINGLE_WAVE;
    SingleWave.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    SingleWave.speed_min             = ALIENWARE_AW510K_SPEED_SLOWEST;
    SingleWave.speed_max             = ALIENWARE_AW510K_SPEED_FASTEST;
    SingleWave.speed                 = ALIENWARE_AW510K_SPEED_NORMAL;
    SingleWave.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    SingleWave.colors_min            = 1;
    SingleWave.colors_max            = 1;
    SingleWave.colors.resize(1);
    modes.push_back(SingleWave);

    mode RainbowWave;
    RainbowWave.name                  = "Rainbow Wave";
    RainbowWave.value                 = ALIENWARE_AW510K_MODE_RAINBOW_WAVE;
    RainbowWave.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    RainbowWave.speed_min             = ALIENWARE_AW510K_SPEED_SLOWEST;
    RainbowWave.speed_max             = ALIENWARE_AW510K_SPEED_FASTEST;
    RainbowWave.speed                 = ALIENWARE_AW510K_SPEED_NORMAL;
    RainbowWave.colors_min            = 1;
    RainbowWave.colors_max            = 1;
    RainbowWave.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    RainbowWave.colors.resize(1);
    modes.push_back(RainbowWave);

    mode Off;
    Off.name                          = "Off";
    Off.value                         = ALIENWARE_AW510K_MODE_OFF;
    Off.flags                         = 0;
    Off.color_mode                    = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
    std::copy(colors.begin(), colors.end(),std::back_inserter(current_colors));
}

RGBController_AlienwareAW510K::~RGBController_AlienwareAW510K()
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

    delete controller;
}

void RGBController_AlienwareAW510K::SetupZones()
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
            new_zone.matrix_map->width  = 24;
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

void RGBController_AlienwareAW510K::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AlienwareAW510K::DeviceUpdateLEDs()
{
    std::vector<SelectedKeys>   frame_buf_keys;
    std::vector<RGBColor>       new_colors;

    std::copy(colors.begin(), colors.end(),std::back_inserter(new_colors));

    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        if (current_colors[led_idx]==new_colors[led_idx])
        {
            /*-------------------------------------------------*\
            | Don't send if key color is not changed            |
            \*-------------------------------------------------*/
            continue;
        }

        if(RGBGetRValue(colors[led_idx]) != 0x00 || RGBGetBValue(colors[led_idx]) != 0x00 || RGBGetBValue(colors[led_idx]) != 0x00)
        {
            SelectedKeys key;

            key.idx     = (unsigned char)leds[led_idx].value;
            key.red     = RGBGetRValue(colors[led_idx]);
            key.green   = RGBGetGValue(colors[led_idx]);
            key.blue    = RGBGetBValue(colors[led_idx]);

            frame_buf_keys.push_back(key);
        }
        else
        {
            SelectedKeys key;

            key.idx     = (unsigned char)leds[led_idx].value;
            key.red     = RGBGetRValue(colors[led_idx]);
            key.green   = RGBGetGValue(colors[led_idx]);
            key.blue    = RGBGetBValue(colors[led_idx]);

            frame_buf_keys.push_back(key);
        }
    }

    controller->SendInitialize();
    controller->SendfeatureReport(0x05, 0x01, 0x51, 0x00);
    controller->SendCommit();

    if(frame_buf_keys.size() > 0)
    {
        controller->SendDirectOn(frame_buf_keys);
    }

    std::copy(new_colors.begin(), new_colors.end(),current_colors.begin());
}

void RGBController_AlienwareAW510K::UpdateZoneLEDs(int zone)
{
    controller->SetDirect((unsigned char) zone, RGBGetRValue(zones[zone].colors[0]), RGBGetGValue(zones[zone].colors[0]), RGBGetBValue(zones[zone].colors[0]));
}

void RGBController_AlienwareAW510K::UpdateSingleLED(int led)
{
    controller->UpdateSingleLED(leds[led].value, RGBGetRValue(colors[led]), RGBGetGValue(colors[led]), RGBGetBValue(colors[led]));
}

void RGBController_AlienwareAW510K::DeviceUpdateMode()
{
    if(active_mode == 0xFFFF)
    {
        UpdateLEDs();
        return;
    }

    controller->SendfeatureReport(0x05, 0x01, 0x51, 0x00);
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    switch(modes[active_mode].value)
    {
        case ALIENWARE_AW510K_MODE_MORPH:
            /*-------------------------------------------------------------*\
            | In case of morph it requires two colors (color1 and color2)   |
            \*-------------------------------------------------------------*/
            {
                unsigned char red2 = RGBGetRValue(modes[active_mode].colors[1]);
                unsigned char grn2 = RGBGetGValue(modes[active_mode].colors[1]);
                unsigned char blu2 = RGBGetBValue(modes[active_mode].colors[1]);

                controller->SetMorphMode(modes[active_mode].value, modes[active_mode].speed, red, grn, blu, red2, grn2, blu2);
            }
            break;

        case ALIENWARE_AW510K_MODE_SPECTRUM:
            /*-------------------------------------------------------------*\
            | Spectrum only set mode, speed and colorMode                   |
            \*-------------------------------------------------------------*/
            controller->SetMode(modes[active_mode].value, modes[active_mode].speed, 0x00, ALIENWARE_AW510K_RANBOW_COLOR_MODE, 0x00, 0x00, 0x00);
            break;

        case ALIENWARE_AW510K_MODE_SINGLE_WAVE:
            /*-------------------------------------------------------------*\
            | Wave only set mode, speed, direction and colorMode            |
            \*-------------------------------------------------------------*/
            {
                int waveDirection = GetAW520K_WaveDirection(modes[active_mode].direction);
                controller->SetMode(modes[active_mode].value, modes[active_mode].speed, waveDirection, ALIENWARE_AW510K_SINGLE_COLOR_MODE, red, grn, blu);
            }
            break;

        case ALIENWARE_AW510K_MODE_RAINBOW_WAVE:
            /*-------------------------------------------------------------*\
            | Wave only set mode, speed, direction and colorMode            |
            \*-------------------------------------------------------------*/
            {
            int waveDirection = GetAW520K_WaveDirection(modes[active_mode].direction);
            controller->SetMode(modes[active_mode].value, modes[active_mode].speed, waveDirection, ALIENWARE_AW510K_RANBOW_COLOR_MODE, 0x00, 0x00, 0x00);
            }
            break;


        default:
            controller->SetMode(modes[active_mode].value, modes[active_mode].speed, 0x00, ALIENWARE_AW510K_SINGLE_COLOR_MODE, red, grn, blu);
            break;
    }
}

int GetAW520K_WaveDirection(int input)
{
    switch(input)
    {
        case MODE_DIRECTION_LEFT:
            return(ALIENWARE_AW510K_DIRECTION_RIGHT_TO_LEFT);

        case MODE_DIRECTION_RIGHT:
            return(ALIENWARE_AW510K_DIRECTION_LEFT_TO_RIGHT);

        case MODE_DIRECTION_UP:
            return(ALIENWARE_AW510K_DIRECTION_BOTTOM_TO_TOP);

        case MODE_DIRECTION_DOWN:
            return(ALIENWARE_AW510K_DIRECTION_TOP_TO_BOTTOM);

        default:
            return(ALIENWARE_AW510K_DIRECTION_RIGHT_TO_LEFT);
    }
}

