/*---------------------------------------------------------*\
| RGBController_OKSKeyboard.cpp                             |
|                                                           |
|   RGBController for OKS keyboard                          |
|                                                           |
|   Merafour (OKS)                              24 Feb 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_OKSKeyboard.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map_optical[6][21] =
    { {   0,  NA,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  NA,  NA,  NA,  NA },
      {  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41 },
      {  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62 },
      {  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  NA,  76,  NA,  NA,  NA,  80,  81,  82,  NA },
      {  84,  NA,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  NA,  97,  NA,  99,  NA, 101, 102, 103, 104 },
      { 105, 106, 107,  NA,  NA,  NA, 111,  NA,  NA,  NA, 115, 116, 117, 118, 119, 120, 121, 122,  NA, 124,  NA } };

static unsigned int matrix_kb87_map[6][17] =
{ {   0,  NA,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16},
  {  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33},
  {  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50},
  {  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  NA,  64,  NA,  NA,  NA},
  {  68,  NA,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  NA,  81,  NA,  83,  NA},
  {  85,  86,  87,  NA,  NA,  NA,  91,  NA,  NA,  NA,  95,  96,  97,  98,  99, 100, 101} };

static const char* zone_names[] =
{
    "Keyboard",
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const unsigned int zone_sizes_optical[] =
{
    126
};

static const unsigned int zone_sizes_kb87[] =
{
    102
};

static const char *led_names[] =
{
    // R0--0
    KEY_EN_ESCAPE,       KEY_EN_UNUSED,       KEY_EN_F1,       KEY_EN_F2,     KEY_EN_F3,     KEY_EN_F4,     KEY_EN_F5,    KEY_EN_F6,     KEY_EN_F7,     KEY_EN_F8,     KEY_EN_F9,        KEY_EN_F10,            KEY_EN_F11,           KEY_EN_F12,             KEY_EN_PRINT_SCREEN, KEY_EN_SCROLL_LOCK, KEY_EN_PAUSE_BREAK, KEY_EN_UNUSED,      KEY_EN_UNUSED,        KEY_EN_UNUSED,        KEY_EN_UNUSED,
    // R1--21
    KEY_EN_BACK_TICK,    KEY_EN_1,            KEY_EN_2,        KEY_EN_3,      KEY_EN_4,      KEY_EN_5,      KEY_EN_6,     KEY_EN_7,      KEY_EN_8,      KEY_EN_9,      KEY_EN_0,         KEY_EN_MINUS,          KEY_EN_EQUALS,        KEY_EN_BACKSPACE,       KEY_EN_INSERT,      KEY_EN_HOME,         KEY_EN_PAGE_UP,     KEY_EN_NUMPAD_LOCK, KEY_EN_NUMPAD_DIVIDE, KEY_EN_NUMPAD_TIMES,  KEY_EN_NUMPAD_MINUS,
    // R2--42
    KEY_EN_TAB,          KEY_EN_Q,            KEY_EN_W,        KEY_EN_E,      KEY_EN_R,      KEY_EN_T,      KEY_EN_Y,     KEY_EN_U,      KEY_EN_I,      KEY_EN_O,      KEY_EN_P,         KEY_EN_LEFT_BRACKET,   KEY_EN_RIGHT_BRACKET, KEY_EN_ANSI_BACK_SLASH, KEY_EN_DELETE,      KEY_EN_END,          KEY_EN_PAGE_DOWN,   KEY_EN_NUMPAD_7,    KEY_EN_NUMPAD_8,      KEY_EN_NUMPAD_9,      KEY_EN_NUMPAD_PLUS,
    // R3--63
    KEY_EN_CAPS_LOCK,    KEY_EN_A,            KEY_EN_S,        KEY_EN_D,      KEY_EN_F,      KEY_EN_G,      KEY_EN_H,     KEY_EN_J,      KEY_EN_K,      KEY_EN_L,      KEY_EN_SEMICOLON, KEY_EN_QUOTE,          KEY_EN_UNUSED,        KEY_EN_ANSI_ENTER,      KEY_EN_UNUSED,      KEY_EN_UNUSED,       KEY_EN_UNUSED,      KEY_EN_NUMPAD_4,    KEY_EN_NUMPAD_5,      KEY_EN_NUMPAD_6,      KEY_EN_UNUSED,
    // R4--84
    KEY_EN_LEFT_SHIFT,   KEY_EN_UNUSED,       KEY_EN_Z,        KEY_EN_X,      KEY_EN_C,      KEY_EN_V,      KEY_EN_B,     KEY_EN_N,      KEY_EN_M,      KEY_EN_COMMA,  KEY_EN_PERIOD,    KEY_EN_FORWARD_SLASH,  KEY_EN_UNUSED,        KEY_EN_RIGHT_SHIFT,     KEY_EN_UNUSED,      KEY_EN_UP_ARROW,     KEY_EN_UNUSED,      KEY_EN_NUMPAD_1,    KEY_EN_NUMPAD_2,      KEY_EN_NUMPAD_3,      KEY_EN_NUMPAD_ENTER,
    // R5--105
    KEY_EN_LEFT_CONTROL, KEY_EN_LEFT_WINDOWS, KEY_EN_LEFT_ALT, KEY_EN_UNUSED, KEY_EN_UNUSED, KEY_EN_UNUSED, KEY_EN_SPACE, KEY_EN_UNUSED, KEY_EN_UNUSED, KEY_EN_UNUSED, KEY_EN_RIGHT_ALT, KEY_EN_RIGHT_FUNCTION, KEY_EN_MENU,          KEY_EN_RIGHT_CONTROL,   KEY_EN_LEFT_ARROW,  KEY_EN_DOWN_ARROW,   KEY_EN_RIGHT_ARROW, KEY_EN_NUMPAD_0,    KEY_EN_UNUSED,        KEY_EN_NUMPAD_PERIOD, KEY_EN_UNUSED,
    // "Key: Calculator",
};

static const char *led_kb87_names[] =
{
    // R0--0
    KEY_EN_ESCAPE,       KEY_EN_UNUSED,       KEY_EN_F1,       KEY_EN_F2,     KEY_EN_F3,     KEY_EN_F4,     KEY_EN_F5,    KEY_EN_F6,     KEY_EN_F7,     KEY_EN_F8,     KEY_EN_F9,        KEY_EN_F10,            KEY_EN_F11,           KEY_EN_F12,             KEY_EN_PRINT_SCREEN, KEY_EN_SCROLL_LOCK, KEY_EN_PAUSE_BREAK,
    // R1--17
    KEY_EN_BACK_TICK,    KEY_EN_1,            KEY_EN_2,        KEY_EN_3,      KEY_EN_4,      KEY_EN_5,      KEY_EN_6,     KEY_EN_7,      KEY_EN_8,      KEY_EN_9,      KEY_EN_0,         KEY_EN_MINUS,          KEY_EN_EQUALS,        KEY_EN_BACKSPACE,       KEY_EN_INSERT,      KEY_EN_HOME,         KEY_EN_PAGE_UP,
    // R2--34
    KEY_EN_TAB,          KEY_EN_Q,            KEY_EN_W,        KEY_EN_E,      KEY_EN_R,      KEY_EN_T,      KEY_EN_Y,     KEY_EN_U,      KEY_EN_I,      KEY_EN_O,      KEY_EN_P,         KEY_EN_LEFT_BRACKET,   KEY_EN_RIGHT_BRACKET, KEY_EN_ANSI_BACK_SLASH, KEY_EN_DELETE,      KEY_EN_END,          KEY_EN_PAGE_DOWN,
    // R3--51
    KEY_EN_CAPS_LOCK,    KEY_EN_A,            KEY_EN_S,        KEY_EN_D,      KEY_EN_F,      KEY_EN_G,      KEY_EN_H,     KEY_EN_J,      KEY_EN_K,      KEY_EN_L,      KEY_EN_SEMICOLON, KEY_EN_QUOTE,          KEY_EN_UNUSED,        KEY_EN_ANSI_ENTER,      KEY_EN_UNUSED,      KEY_EN_UNUSED,       KEY_EN_UNUSED,
    // R4--68
    KEY_EN_LEFT_SHIFT,   KEY_EN_UNUSED,       KEY_EN_Z,        KEY_EN_X,      KEY_EN_C,      KEY_EN_V,      KEY_EN_B,     KEY_EN_N,      KEY_EN_M,      KEY_EN_COMMA,  KEY_EN_PERIOD,    KEY_EN_FORWARD_SLASH,  KEY_EN_UNUSED,        KEY_EN_RIGHT_SHIFT,     KEY_EN_UNUSED,      KEY_EN_UP_ARROW,     KEY_EN_UNUSED,
    // R5--85
    KEY_EN_LEFT_CONTROL, KEY_EN_LEFT_WINDOWS, KEY_EN_LEFT_ALT, KEY_EN_UNUSED, KEY_EN_UNUSED, KEY_EN_UNUSED, KEY_EN_SPACE, KEY_EN_UNUSED, KEY_EN_UNUSED, KEY_EN_UNUSED, KEY_EN_RIGHT_ALT, KEY_EN_RIGHT_FUNCTION, KEY_EN_MENU,          KEY_EN_RIGHT_CONTROL,   KEY_EN_LEFT_ARROW,  KEY_EN_DOWN_ARROW,   KEY_EN_RIGHT_ARROW, KEY_EN_NUMPAD_0,
    // "Key: Calculator",
};
enum
{
    OKS_SPEED_SLOWEST              = 0x00, // Slowest speed
    OKS_SPEED_SLOWER               = 0x01, // Slower speed
    OKS_SPEED_SLOW                 = 0x02, // Slow speed
    OKS_SPEED_SLOWISH              = 0x02, // Slowish speed
    OKS_SPEED_NORMAL               = 0x02, // Normal speed
    OKS_SPEED_FASTISH              = 0x03, // Fastish speed
    OKS_SPEED_FAST                 = 0x04, // Fast speed
    OKS_SPEED_FASTER               = 0x05, // Faster speed
    OKS_SPEED_FASTEST              = 0x06, // Fastest speed
};
/**------------------------------------------------------------------*\
    @name OKS Keyboard                                                |
    @category Keyboard                                                |
    @type USB                                                         |
    @save :x:                                                         |
    @direct :white_check_mark:                                        |
    @effects :x:                                                      |
    @detectors DetectOKSKeyboardControllers                           |
    @comment                                                          |
\*-------------------------------------------------------------------*/

RGBController_OKSKeyboard::RGBController_OKSKeyboard(OKSKeyboardController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "OKS";
    type                    = DEVICE_TYPE_KEYBOARD;
    description             = "OKS Keyboard Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = UP_RGB_MODES_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS ;
    Direct.brightness_min   = 0;
    Direct.brightness_max   = 5;
    Direct.brightness       = 2;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.speed_min        = OKS_SPEED_FASTEST;
    Direct.speed_max        = OKS_SPEED_SLOWEST;
    Direct.speed            = OKS_SPEED_NORMAL;
    Direct.direction        = MODE_DIRECTION_RIGHT;
    modes.push_back(Direct);

    mode udef = Direct;
    udef.name               = "User mode1";
    udef.value              = UP_RGB_MODES_UDEF1;
    udef.direction          = MODE_DIRECTION_LEFT;
    udef.speed              = 0;
    modes.push_back(udef);
    udef.name               = "User mode2";
    udef.value              = UP_RGB_MODES_UDEF2;
    modes.push_back(udef);
    udef.name               = "User mode3";
    udef.value              = UP_RGB_MODES_UDEF3;
    modes.push_back(udef);
    udef.name               = "User mode4";
    udef.value              = UP_RGB_MODES_UDEF4;
    modes.push_back(udef);
    udef.name               = "User mode5";
    udef.value              = UP_RGB_MODES_UDEF5;
    modes.push_back(udef);
    /*---------------------------------------------------------*\
    | Delete the "Horse race lamp","Breathing"... mode          |
    \*---------------------------------------------------------*/
    SetupZones();
}

RGBController_OKSKeyboard::~RGBController_OKSKeyboard()
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

void RGBController_OKSKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        unsigned int    zone_size       = 0;
        unsigned int    matrix_width    = 0;
        unsigned int*   matrix_map_ptr  = NULL;

        switch(serial.c_str()[0])
        {
            case 'B':
                zone_size               = zone_sizes_kb87[zone_idx];
                matrix_width            = 17;
                matrix_map_ptr          = (unsigned int *)&matrix_kb87_map;
                break;
            default:
                zone_size               = zone_sizes_optical[zone_idx];
                matrix_width            = 21;
                matrix_map_ptr          = (unsigned int *)&matrix_map_optical;
                break;
        }

        zone new_zone;
        new_zone.name                   = zone_names[zone_idx];
        new_zone.type                   = zone_types[zone_idx];
        new_zone.leds_min               = zone_size;
        new_zone.leds_max               = zone_size;
        new_zone.leds_count             = zone_size;
        new_zone.matrix_map             = new matrix_map_type;
        new_zone.matrix_map->height     = 6;
        new_zone.matrix_map->width      = matrix_width;
        new_zone.matrix_map->map        = matrix_map_ptr;
        zones.push_back(new_zone);

        total_led_count += zone_size;
    }

    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        switch(serial.c_str()[0])
        {
            case 'B':
                new_led.name = led_kb87_names[led_idx];
                break;

            default:
                new_led.name = led_names[led_idx];
                break;
        }
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_OKSKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_OKSKeyboard::DeviceUpdateLEDs()
{
    unsigned char colordata[155*3];
    unsigned int            width;
    unsigned int            kb_idx;
    unsigned int            row_idx;
    unsigned int            col_idx;

    width = zones[0].matrix_map->width;
    for(std::size_t color_idx = 0; color_idx < 155; color_idx++)
    {
        colordata[(color_idx*3)+0] = 0x00;
        colordata[(color_idx*3)+1] = 0x00;
        colordata[(color_idx*3)+2] = 0x00;
    }
    /*---------------------------------------------------------*\
    | send 6x21 matrix                                          |
    \*---------------------------------------------------------*/
    for(std::size_t color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        row_idx = (unsigned int)(color_idx) / width;
        col_idx = (unsigned int)(color_idx) % width;
        kb_idx = row_idx*21+col_idx;
        colordata[(kb_idx*3)+0] = RGBGetRValue(colors[color_idx]);
        colordata[(kb_idx*3)+1] = RGBGetGValue(colors[color_idx]);
        colordata[(kb_idx*3)+2] = RGBGetBValue(colors[color_idx]);
    }

    controller->SendColors(colordata, (unsigned int)colors.size() * 3);
}

void RGBController_OKSKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_OKSKeyboard::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_OKSKeyboard::DeviceUpdateMode()
{
    mode m = modes[active_mode];
    unsigned char red    = 0x00;
    unsigned char grn    = 0x00;
    unsigned char blu    = 0x00;
    //unsigned char random = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);

    if(modes[active_mode].colors.size() > 0)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    controller->SendKeyboardModeEx(m, red, grn, blu);
}
