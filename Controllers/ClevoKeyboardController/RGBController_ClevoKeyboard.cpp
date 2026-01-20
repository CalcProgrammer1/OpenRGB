/*---------------------------------------------------------*\
| RGBController_ClevoKeyboard.cpp                           |
|                                                           |
|   RGBController for Clevo per-key RGB keyboard (ITE 8291) |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_ClevoKeyboard.h"
#include "RGBControllerKeyNames.h"

/**------------------------------------------------------------------*\
    @name CLEVO Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectClevoKeyboardControllers
    @comment Per-key RGB keyboard on CLEVO laptops using ITE 8291 controller.
\*-------------------------------------------------------------------*/

#define NA 0xFFFFFFFF

/*---------------------------------------------------------*\
| CLEVO Keyboard matrix layout                             |
| LED indices are stored bottom-to-top in hardware          |
| Visual layout maps top-to-bottom for display              |
|                                                           |
| Visual row 0 = LED row 5 (Esc, F-keys)                    |
| Visual row 1 = LED row 4 (Number row)                     |
| Visual row 2 = LED row 3 (QWERTY row)                     |
| Visual row 3 = LED row 2 (Home row)                       |
| Visual row 4 = LED row 1 (Z row)                          |
| Visual row 5 = LED row 0 (Bottom/modifier row)            |
| Visual row 6 = Arrow keys (inverted-T bottom)             |
\*---------------------------------------------------------*/

static unsigned int matrix_map[7][21] =
{
    { 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124,  NA },  // Esc, F1-F12, PrtSc, Ins, Del, Home, End, PgUp, PgDn
    {  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  98,  98,  NA,  99, 100, 101, 102,  NA },  // `, 1-0, -, =, Bksp (2-wide), NumLock, /, *, -
    {  63,  NA,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  NA,  78,  79,  80,  81,  NA },  // Tab, Q-], Enter, Num 7-9, +
    {  42,  NA,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  77,  NA,  57,  58,  59,  NA,  NA },  // Caps, A-\, Enter (2-high), Num 4-6
    {  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  35,  35,  35,  NA,  NA,  36,  37,  38,  39,  NA },  // LShift, \, Z-./, RShift (3-wide), Num 1-3, NumEnter
    {   0,   2,   3,   4,  NA,  NA,   7,  NA,  10,  12,  12,  NA,  NA,  14,  NA,  NA,  16,  NA,  17,  NA,  NA },  // Ctrl, Fn, Super, Alt, Space, AltGr, RCtrl (2-wide), Up, Num0, NumDel
    {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  13,  18,  15,  NA,  NA,  NA,  NA,  NA,  NA },  // Left, Down, Right (inverted-T bottom)
};

static const char* key_names[] =
{
    // Row 0 (LED indices 0-20) - Bottom row
    KEY_EN_LEFT_CONTROL,        // 0
    KEY_EN_UNUSED,              // 1
    KEY_EN_LEFT_FUNCTION,       // 2
    KEY_EN_LEFT_WINDOWS,        // 3
    KEY_EN_LEFT_ALT,            // 4
    KEY_EN_UNUSED,              // 5
    KEY_EN_UNUSED,              // 6
    KEY_EN_SPACE,               // 7
    KEY_EN_UNUSED,              // 8
    KEY_EN_UNUSED,              // 9
    KEY_EN_RIGHT_ALT,           // 10
    KEY_EN_UNUSED,              // 11
    KEY_EN_RIGHT_CONTROL,       // 12
    KEY_EN_LEFT_ARROW,          // 13
    KEY_EN_UP_ARROW,            // 14
    KEY_EN_RIGHT_ARROW,         // 15
    KEY_EN_NUMPAD_0,            // 16
    KEY_EN_NUMPAD_PERIOD,       // 17
    KEY_EN_DOWN_ARROW,          // 18
    KEY_EN_UNUSED,              // 19
    KEY_EN_UNUSED,              // 20

    // Row 1 (LED indices 21-41) - Z row
    KEY_EN_UNUSED,              // 21
    KEY_EN_LEFT_SHIFT,          // 22
    KEY_EN_ISO_BACK_SLASH,      // 23
    KEY_EN_Z,                   // 24
    KEY_EN_X,                   // 25
    KEY_EN_C,                   // 26
    KEY_EN_V,                   // 27
    KEY_EN_B,                   // 28
    KEY_EN_N,                   // 29
    KEY_EN_M,                   // 30
    KEY_EN_COMMA,               // 31
    KEY_EN_PERIOD,              // 32
    KEY_EN_FORWARD_SLASH,       // 33
    KEY_EN_UNUSED,              // 34
    KEY_EN_RIGHT_SHIFT,         // 35
    KEY_EN_NUMPAD_1,            // 36
    KEY_EN_NUMPAD_2,            // 37
    KEY_EN_NUMPAD_3,            // 38
    KEY_EN_NUMPAD_ENTER,        // 39
    KEY_EN_UNUSED,              // 40
    KEY_EN_UNUSED,              // 41

    // Row 2 (LED indices 42-62) - Home row
    KEY_EN_CAPS_LOCK,           // 42
    KEY_EN_UNUSED,              // 43
    KEY_EN_A,                   // 44
    KEY_EN_S,                   // 45
    KEY_EN_D,                   // 46
    KEY_EN_F,                   // 47
    KEY_EN_G,                   // 48
    KEY_EN_H,                   // 49
    KEY_EN_J,                   // 50
    KEY_EN_K,                   // 51
    KEY_EN_L,                   // 52
    KEY_EN_SEMICOLON,           // 53
    KEY_EN_QUOTE,               // 54
    KEY_EN_ANSI_BACK_SLASH,     // 55
    KEY_EN_UNUSED,              // 56
    KEY_EN_NUMPAD_4,            // 57
    KEY_EN_NUMPAD_5,            // 58
    KEY_EN_NUMPAD_6,            // 59
    KEY_EN_UNUSED,              // 60
    KEY_EN_UNUSED,              // 61
    KEY_EN_UNUSED,              // 62

    // Row 3 (LED indices 63-83) - QWERTY row
    KEY_EN_TAB,                 // 63
    KEY_EN_UNUSED,              // 64
    KEY_EN_Q,                   // 65
    KEY_EN_W,                   // 66
    KEY_EN_E,                   // 67
    KEY_EN_R,                   // 68
    KEY_EN_T,                   // 69
    KEY_EN_Y,                   // 70
    KEY_EN_U,                   // 71
    KEY_EN_I,                   // 72
    KEY_EN_O,                   // 73
    KEY_EN_P,                   // 74
    KEY_EN_LEFT_BRACKET,        // 75
    KEY_EN_RIGHT_BRACKET,       // 76
    KEY_EN_ANSI_ENTER,          // 77
    KEY_EN_NUMPAD_7,            // 78
    KEY_EN_NUMPAD_8,            // 79
    KEY_EN_NUMPAD_9,            // 80
    KEY_EN_NUMPAD_PLUS,         // 81
    KEY_EN_UNUSED,              // 82
    KEY_EN_UNUSED,              // 83

    // Row 4 (LED indices 84-104) - Number row
    KEY_EN_BACK_TICK,           // 84
    KEY_EN_1,                   // 85
    KEY_EN_2,                   // 86
    KEY_EN_3,                   // 87
    KEY_EN_4,                   // 88
    KEY_EN_5,                   // 89
    KEY_EN_6,                   // 90
    KEY_EN_7,                   // 91
    KEY_EN_8,                   // 92
    KEY_EN_9,                   // 93
    KEY_EN_0,                   // 94
    KEY_EN_MINUS,               // 95
    KEY_EN_EQUALS,              // 96
    KEY_EN_UNUSED,              // 97
    KEY_EN_BACKSPACE,           // 98
    KEY_EN_NUMPAD_LOCK,         // 99
    KEY_EN_NUMPAD_DIVIDE,       // 100
    KEY_EN_NUMPAD_TIMES,        // 101
    KEY_EN_NUMPAD_MINUS,        // 102
    KEY_EN_UNUSED,              // 103
    KEY_EN_UNUSED,              // 104

    // Row 5 (LED indices 105-125) - Function row
    KEY_EN_ESCAPE,              // 105
    KEY_EN_F1,                  // 106
    KEY_EN_F2,                  // 107
    KEY_EN_F3,                  // 108
    KEY_EN_F4,                  // 109
    KEY_EN_F5,                  // 110
    KEY_EN_F6,                  // 111
    KEY_EN_F7,                  // 112
    KEY_EN_F8,                  // 113
    KEY_EN_F9,                  // 114
    KEY_EN_F10,                 // 115
    KEY_EN_F11,                 // 116
    KEY_EN_F12,                 // 117
    KEY_EN_PRINT_SCREEN,        // 118
    KEY_EN_INSERT,              // 119
    KEY_EN_DELETE,              // 120
    KEY_EN_HOME,                // 121
    KEY_EN_END,                 // 122
    KEY_EN_PAGE_UP,             // 123
    KEY_EN_PAGE_DOWN,           // 124
    KEY_EN_UNUSED,              // 125
};

RGBController_ClevoKeyboard::RGBController_ClevoKeyboard(ClevoKeyboardController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = "CLEVO Keyboard";
    vendor                          = "CLEVO Computers";
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = "CLEVO Laptop Keyboard";
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = CLEVO_KEYBOARD_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min           = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
    Direct.brightness_max           = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Direct.brightness               = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Rainbow;
    Rainbow.name                    = "Rainbow";
    Rainbow.value                   = CLEVO_KEYBOARD_MODE_RAINBOW;
    Rainbow.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Rainbow.speed_min               = CLEVO_KEYBOARD_SPEED_MAX;
    Rainbow.speed_max               = CLEVO_KEYBOARD_SPEED_MIN;
    Rainbow.speed                   = 0x05;
    Rainbow.brightness_min          = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
    Rainbow.brightness_max          = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Rainbow.brightness              = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Rainbow.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Wave;
    Wave.name                       = "Wave";
    Wave.value                      = CLEVO_KEYBOARD_MODE_WAVE;
    Wave.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Wave.speed_min                  = CLEVO_KEYBOARD_SPEED_MAX;
    Wave.speed_max                  = CLEVO_KEYBOARD_SPEED_MIN;
    Wave.speed                      = 0x05;
    Wave.brightness_min             = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
    Wave.brightness_max             = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Wave.brightness                 = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Wave.direction                  = MODE_DIRECTION_LEFT;
    Wave.colors_min                 = 1;
    Wave.colors_max                 = 1;
    Wave.colors.resize(1);
    Wave.color_mode                 = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Wave);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = CLEVO_KEYBOARD_MODE_BREATH;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.speed_min             = CLEVO_KEYBOARD_SPEED_MAX;
    Breathing.speed_max             = CLEVO_KEYBOARD_SPEED_MIN;
    Breathing.speed                 = 0x05;
    Breathing.brightness_min        = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
    Breathing.brightness_max        = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Breathing.brightness            = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.colors.resize(1);
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Breathing);

    mode Reactive;
    Reactive.name                   = "Reactive";
    Reactive.value                  = CLEVO_KEYBOARD_MODE_REACTIVE;
    Reactive.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Reactive.speed_min              = CLEVO_KEYBOARD_SPEED_MAX;
    Reactive.speed_max              = CLEVO_KEYBOARD_SPEED_MIN;
    Reactive.speed                  = 0x05;
    Reactive.brightness_min         = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
    Reactive.brightness_max         = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Reactive.brightness             = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Reactive.colors_min             = 1;
    Reactive.colors_max             = 1;
    Reactive.colors.resize(1);
    Reactive.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Reactive);

    mode Ripple;
    Ripple.name                     = "Ripple";
    Ripple.value                    = CLEVO_KEYBOARD_MODE_RIPPLE;
    Ripple.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Ripple.speed_min                = CLEVO_KEYBOARD_SPEED_MAX;
    Ripple.speed_max                = CLEVO_KEYBOARD_SPEED_MIN;
    Ripple.speed                    = 0x05;
    Ripple.brightness_min           = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
    Ripple.brightness_max           = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Ripple.brightness               = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Ripple.colors_min               = 1;
    Ripple.colors_max               = 1;
    Ripple.colors.resize(1);
    Ripple.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Ripple);

    mode Marquee;
    Marquee.name                    = "Marquee";
    Marquee.value                   = CLEVO_KEYBOARD_MODE_MARQUEE;
    Marquee.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Marquee.speed_min               = CLEVO_KEYBOARD_SPEED_MAX;
    Marquee.speed_max               = CLEVO_KEYBOARD_SPEED_MIN;
    Marquee.speed                   = 0x05;
    Marquee.brightness_min          = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
    Marquee.brightness_max          = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Marquee.brightness              = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Marquee.colors_min              = 1;
    Marquee.colors_max              = 1;
    Marquee.colors.resize(1);
    Marquee.color_mode              = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Marquee);

    mode Raindrop;
    Raindrop.name                   = "Raindrop";
    Raindrop.value                  = CLEVO_KEYBOARD_MODE_RAINDROP;
    Raindrop.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Raindrop.speed_min              = CLEVO_KEYBOARD_SPEED_MAX;
    Raindrop.speed_max              = CLEVO_KEYBOARD_SPEED_MIN;
    Raindrop.speed                  = 0x05;
    Raindrop.brightness_min         = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
    Raindrop.brightness_max         = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Raindrop.brightness             = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Raindrop.colors_min             = 1;
    Raindrop.colors_max             = 1;
    Raindrop.colors.resize(1);
    Raindrop.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Raindrop);

    mode Aurora;
    Aurora.name                     = "Aurora";
    Aurora.value                    = CLEVO_KEYBOARD_MODE_AURORA;
    Aurora.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Aurora.speed_min                = CLEVO_KEYBOARD_SPEED_MAX;
    Aurora.speed_max                = CLEVO_KEYBOARD_SPEED_MIN;
    Aurora.speed                    = 0x05;
    Aurora.brightness_min           = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
    Aurora.brightness_max           = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Aurora.brightness               = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Aurora.colors_min               = 1;
    Aurora.colors_max               = 1;
    Aurora.colors.resize(1);
    Aurora.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Aurora);

    mode Spark;
    Spark.name                      = "Spark";
    Spark.value                     = CLEVO_KEYBOARD_MODE_SPARK;
    Spark.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Spark.speed_min                 = CLEVO_KEYBOARD_SPEED_MAX;
    Spark.speed_max                 = CLEVO_KEYBOARD_SPEED_MIN;
    Spark.speed                     = 0x05;
    Spark.brightness_min            = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
    Spark.brightness_max            = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Spark.brightness                = CLEVO_KEYBOARD_BRIGHTNESS_MAX;
    Spark.colors_min                = 1;
    Spark.colors_max                = 1;
    Spark.colors.resize(1);
    Spark.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Spark);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = 0xFF;
    Off.flags                       = 0;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_ClevoKeyboard::~RGBController_ClevoKeyboard()
{
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        if(zones[zone_idx].matrix_map != nullptr)
        {
            delete[] zones[zone_idx].matrix_map->map;
            delete zones[zone_idx].matrix_map;
        }
    }

    delete controller;
}

void RGBController_ClevoKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Create a matrix zone for the keyboard                     |
    \*---------------------------------------------------------*/
    zone keyboard_zone;

    keyboard_zone.name              = ZONE_EN_KEYBOARD;
    keyboard_zone.type              = ZONE_TYPE_MATRIX;
    keyboard_zone.leds_min          = CLEVO_KEYBOARD_NUM_LEDS;
    keyboard_zone.leds_max          = CLEVO_KEYBOARD_NUM_LEDS;
    keyboard_zone.leds_count        = CLEVO_KEYBOARD_NUM_LEDS;

    keyboard_zone.matrix_map        = new matrix_map_type;
    keyboard_zone.matrix_map->height = 7;
    keyboard_zone.matrix_map->width  = 21;
    keyboard_zone.matrix_map->map    = new unsigned int[7 * 21];

    for(int row = 0; row < 7; row++)
    {
        for(int col = 0; col < 21; col++)
        {
            keyboard_zone.matrix_map->map[row * 21 + col] = matrix_map[row][col];
        }
    }

    zones.push_back(keyboard_zone);

    /*---------------------------------------------------------*\
    | Create LEDs with proper key names                         |
    \*---------------------------------------------------------*/
    for(int i = 0; i < CLEVO_KEYBOARD_NUM_LEDS; i++)
    {
        led new_led;
        new_led.name = key_names[i];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_ClevoKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ClevoKeyboard::DeviceUpdateLEDs()
{
    /*---------------------------------------------------------*\
    | Build color data array and send to device                 |
    \*---------------------------------------------------------*/
    unsigned char color_data[CLEVO_KEYBOARD_NUM_LEDS * 3];

    for(int i = 0; i < CLEVO_KEYBOARD_NUM_LEDS; i++)
    {
        color_data[i * 3 + 0] = RGBGetRValue(colors[i]);
        color_data[i * 3 + 1] = RGBGetGValue(colors[i]);
        color_data[i * 3 + 2] = RGBGetBValue(colors[i]);
    }

    controller->SendColors(color_data, modes[active_mode].brightness);
}

void RGBController_ClevoKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ClevoKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ClevoKeyboard::DeviceUpdateMode()
{
    unsigned char mode_value = modes[active_mode].value;

    /*---------------------------------------------------------*\
    | Handle Off mode                                           |
    \*---------------------------------------------------------*/
    if(mode_value == 0xFF)
    {
        controller->TurnOff();
        return;
    }

    /*---------------------------------------------------------*\
    | Handle Direct (per-key) mode                              |
    \*---------------------------------------------------------*/
    if(mode_value == CLEVO_KEYBOARD_MODE_DIRECT)
    {
        DeviceUpdateLEDs();
        return;
    }

    /*---------------------------------------------------------*\
    | Handle built-in effect modes                              |
    \*---------------------------------------------------------*/
    unsigned char brightness = modes[active_mode].brightness;
    unsigned char speed      = modes[active_mode].speed;
    unsigned char behaviour  = 0x00;

    /*---------------------------------------------------------*\
    | Set direction for wave mode                               |
    \*---------------------------------------------------------*/
    if(mode_value == CLEVO_KEYBOARD_MODE_WAVE)
    {
        switch(modes[active_mode].direction)
        {
            case MODE_DIRECTION_LEFT:
                behaviour = CLEVO_KEYBOARD_DIRECTION_LEFT;
                break;
            case MODE_DIRECTION_RIGHT:
                behaviour = CLEVO_KEYBOARD_DIRECTION_RIGHT;
                break;
            case MODE_DIRECTION_UP:
                behaviour = CLEVO_KEYBOARD_DIRECTION_UP;
                break;
            case MODE_DIRECTION_DOWN:
                behaviour = CLEVO_KEYBOARD_DIRECTION_DOWN;
                break;
        }
    }

    /*---------------------------------------------------------*\
    | Set mode color if applicable                              |
    \*---------------------------------------------------------*/
    if(modes[active_mode].colors.size() > 0)
    {
        unsigned char red   = RGBGetRValue(modes[active_mode].colors[0]);
        unsigned char green = RGBGetGValue(modes[active_mode].colors[0]);
        unsigned char blue  = RGBGetBValue(modes[active_mode].colors[0]);

        controller->SetModeColor(1, red, green, blue);
    }

    controller->SetMode(mode_value, brightness, speed, behaviour);
}
