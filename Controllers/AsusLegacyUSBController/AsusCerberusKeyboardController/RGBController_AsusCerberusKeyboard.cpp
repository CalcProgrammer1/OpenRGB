/*---------------------------------------------------------*\
| RGBController_AsusCerberusKeyboard.cpp                    |
|                                                           |
|   RGBController for ASUS Cerberus keyboard                |
|                                                           |
|   Mola19                                      28 May 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cmath>
#include <vector>
#include "RGBControllerKeyNames.h"
#include "RGBController_AsusCerberusKeyboard.h"

/**------------------------------------------------------------------*\
    @name Asus Cerberus Mech Keyboard
    @category Keyboard
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectAsusCerberusMech
    @comment
\*-------------------------------------------------------------------*/

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][24] =
{
    {   0,  NA,   9,  15,  20,  25,  NA,  35,  40,  45,  50,  56,  62,  67,  72,  NA,  75,  79,  84,  NA,  NA,  NA,  NA,  NA },
    {   1,   6,  10,  16,  21,  26,  30,  36,  41,  46,  51,  57,  63,  68,  NA,  NA,  76,  80,  85,  NA,  88,  93,  97, 102 },
    {   2,  NA,  11,  17,  22,  27,  31,  37,  42,  47,  52,  58,  64,  69,  NA,  NA,  77,  81,  86,  NA,  89,  94,  98, 103 },
    {   3,  NA,  12,  18,  23,  28,  32,  38,  43,  48,  53,  59,  65,  70,  73,  NA,  NA,  NA,  NA,  NA,  90,  95,  99,  NA },
    {   4,   7,  13,  19,  24,  29,  33,  39,  44,  49,  54,  60,  NA,  71,  NA,  NA,  NA,  82,  NA,  NA,  91,  96, 100, 104 },
    {   5,   8,  14,  NA,  NA,  NA,  34,  NA,  NA,  NA,  55,  61,  66,  NA,  74,  NA,  78,  83,  87,  NA,  92,  NA, 101,  NA }
};

struct led_value
{
    const char*     name;
    uint8_t         id;
};

static const std::vector<led_value> led_names =
{
    { KEY_EN_ESCAPE,              0x0B },
    { KEY_EN_BACK_TICK,           0x0E },
    { KEY_EN_TAB,                 0x09 },
    { KEY_EN_CAPS_LOCK,           0x11 },
    { KEY_EN_LEFT_SHIFT,          0x79 },
    { KEY_EN_LEFT_CONTROL,        0x06 },

    { KEY_EN_1,                   0x0F },
    { KEY_EN_ISO_BACK_SLASH,      0x13 },
    { KEY_EN_LEFT_WINDOWS,        0x7C },

    { KEY_EN_F1,                  0x16 },
    { KEY_EN_2,                   0x17 },
    { KEY_EN_Q,                   0x08 },
    { KEY_EN_A,                   0x0A },
    { KEY_EN_Z,                   0x0C },
    { KEY_EN_LEFT_ALT,            0x4B },

    { KEY_EN_F2,                  0x1E },
    { KEY_EN_3,                   0x1F },
    { KEY_EN_W,                   0x10 },
    { KEY_EN_S,                   0x12 },
    { KEY_EN_X,                   0x14 },

    { KEY_EN_F3,                  0x19 },
    { KEY_EN_4,                   0x27 },
    { KEY_EN_E,                   0x18 },
    { KEY_EN_D,                   0x1A },
    { KEY_EN_C,                   0x1C },

    { KEY_EN_F4,                  0x1B },
    { KEY_EN_5,                   0x26 },
    { KEY_EN_R,                   0x20 },
    { KEY_EN_F,                   0x22 },
    { KEY_EN_V,                   0x24 },

    { KEY_EN_6,                   0x2E },
    { KEY_EN_T,                   0x21 },
    { KEY_EN_G,                   0x23 },
    { KEY_EN_B,                   0x25 },
    { KEY_EN_SPACE,               0x5B },

    { KEY_EN_F5,                  0x07 },
    { KEY_EN_7,                   0x2F },
    { KEY_EN_Y,                   0x29 },
    { KEY_EN_H,                   0x2B },
    { KEY_EN_N,                   0x2D },

    { KEY_EN_F6,                  0x33 },
    { KEY_EN_8,                   0x37 },
    { KEY_EN_U,                   0x28 },
    { KEY_EN_J,                   0x2A },
    { KEY_EN_M,                   0x2C },

    { KEY_EN_F7,                  0x39 },
    { KEY_EN_9,                   0x3F },
    { KEY_EN_I,                   0x30 },
    { KEY_EN_K,                   0x32 },
    { KEY_EN_COMMA,               0x34 },

    { KEY_EN_F8,                  0x3E },
    { KEY_EN_0,                   0x47 },
    { KEY_EN_O,                   0x38 },
    { KEY_EN_L,                   0x3A },
    { KEY_EN_PERIOD,              0x3C },
    { KEY_EN_RIGHT_ALT,           0x4D },

    { KEY_EN_F9,                  0x56 },
    { KEY_EN_MINUS,               0x46 },
    { KEY_EN_P,                   0x40 },
    { KEY_EN_SEMICOLON,           0x42 },
    { KEY_EN_FORWARD_SLASH,       0x45 },
    { KEY_EN_RIGHT_FUNCTION,      0x7D },

    { KEY_EN_F10,                 0x57 },
    { KEY_EN_EQUALS,              0x36 },
    { KEY_EN_LEFT_BRACKET,        0x41 },
    { KEY_EN_QUOTE,               0x43 },
    { KEY_EN_MENU,                0x3D },

    { KEY_EN_F11,                 0x53 },
    { KEY_EN_BACKSPACE,           0x51 },
    { KEY_EN_RIGHT_BRACKET,       0x31 },
    { KEY_EN_POUND,               0x44 },
    { KEY_EN_RIGHT_SHIFT,         0x7A },

    { KEY_EN_F12,                 0x55 },
    { KEY_EN_ISO_ENTER,           0x54 },
    { KEY_EN_RIGHT_CONTROL,       0x04 },
    { KEY_EN_PRINT_SCREEN,        0x4F },

    { KEY_EN_INSERT,              0x66 },
    { KEY_EN_DELETE,              0x5E },
    { KEY_EN_LEFT_ARROW,          0x75 },

    { KEY_EN_SCROLL_LOCK,         0x48 },
    { KEY_EN_HOME,                0x76 },
    { KEY_EN_END,                 0x77 },
    { KEY_EN_UP_ARROW,            0x73 },
    { KEY_EN_DOWN_ARROW,          0x5D },

    { KEY_EN_PAUSE_BREAK,         0x00 },
    { KEY_EN_PAGE_UP,             0x6E },
    { KEY_EN_PAGE_DOWN,           0x6F },
    { KEY_EN_RIGHT_ARROW,         0x65 },

    { KEY_EN_NUMPAD_LOCK,         0x5C },
    { KEY_EN_NUMPAD_7,            0x58 },
    { KEY_EN_NUMPAD_4,            0x59 },
    { KEY_EN_NUMPAD_1,            0x5A },
    { KEY_EN_NUMPAD_0,            0x63 },

    { KEY_EN_NUMPAD_DIVIDE,       0x64 },
    { KEY_EN_NUMPAD_8,            0x60 },
    { KEY_EN_NUMPAD_5,            0x61 },
    { KEY_EN_NUMPAD_2,            0x62 },

    { KEY_EN_NUMPAD_TIMES,        0x6C },
    { KEY_EN_NUMPAD_9,            0x68 },
    { KEY_EN_NUMPAD_6,            0x69 },
    { KEY_EN_NUMPAD_3,            0x6A },
    { KEY_EN_NUMPAD_PERIOD,       0x6B },

    { KEY_EN_NUMPAD_MINUS,        0x6D },
    { KEY_EN_NUMPAD_PLUS,         0x70 },
    { KEY_EN_NUMPAD_ENTER,        0x72 }
};

RGBController_AsusCerberusKeyboard::RGBController_AsusCerberusKeyboard(AsusCerberusKeyboardController* controller_ptr)
{
    controller                      = controller_ptr;

    /*------------------------------------------------------------------------------------------------------*\
    | this device has 6 different profiles, but there is no way to fetch them from the device,               |
    | hence this device always sends to the first profile until a better solution is implemented in OpenRGB  |
    \*------------------------------------------------------------------------------------------------------*/

    controller->SetProfile(1);

    name                            = controller->GetDeviceName();
    vendor                          = "ASUS";
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = "ASUS Cerberus Keyboard Device";
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();
    version                         = controller->GetVersion();

    mode Custom;
    Custom.name                     = "Custom";
    Custom.value                    = CERBERUS_KEYBOARD_MODE_STATIC;
    Custom.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Custom.brightness_min           = CERBERUS_MECH_BRIGHTNESS_MIN;
    Custom.brightness_max           = CERBERUS_MECH_BRIGHTNESS_MAX;
    Custom.brightness               = CERBERUS_MECH_BRIGHTNESS_DEFAULT;
    Custom.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = CERBERUS_KEYBOARD_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Static.brightness_min           = CERBERUS_MECH_BRIGHTNESS_MIN;
    Static.brightness_max           = CERBERUS_MECH_BRIGHTNESS_MAX;
    Static.brightness               = CERBERUS_MECH_BRIGHTNESS_DEFAULT;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = CERBERUS_KEYBOARD_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.brightness_min        = CERBERUS_MECH_BRIGHTNESS_MIN;
    Breathing.brightness_max        = CERBERUS_MECH_BRIGHTNESS_MAX;
    Breathing.brightness            = CERBERUS_MECH_BRIGHTNESS_DEFAULT;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Reactive;
    Reactive.name                   = "Reactive";
    Reactive.value                  = CERBERUS_KEYBOARD_MODE_REACTIVE;
    Reactive.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Reactive.brightness_min         = CERBERUS_MECH_BRIGHTNESS_MIN;
    Reactive.brightness_max         = CERBERUS_MECH_BRIGHTNESS_MAX;
    Reactive.brightness             = CERBERUS_MECH_BRIGHTNESS_DEFAULT;
    Reactive.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Reactive.colors_min             = 1;
    Reactive.colors_max             = 1;
    Reactive.colors.resize(1);
    modes.push_back(Reactive);

    mode Explosion;
    Explosion.name                  = "Explosion";
    Explosion.value                 = CERBERUS_KEYBOARD_MODE_EXPLOSION;
    Explosion.flags                 = MODE_FLAG_HAS_DIRECTION_HV | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Explosion.brightness_min        = CERBERUS_MECH_BRIGHTNESS_MIN;
    Explosion.brightness_max        = CERBERUS_MECH_BRIGHTNESS_MAX;
    Explosion.brightness            = CERBERUS_MECH_BRIGHTNESS_DEFAULT;
    Explosion.direction             = MODE_DIRECTION_VERTICAL;
    Explosion.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Explosion.colors_min            = 1;
    Explosion.colors_max            = 1;
    Explosion.colors.resize(1);
    modes.push_back(Explosion);

    mode Color_Cycle;
    Color_Cycle.name                = "Spectrum Cycle";
    Color_Cycle.value               = CERBERUS_KEYBOARD_MODE_COLOR_CYCLE;
    Color_Cycle.flags               = MODE_FLAG_HAS_DIRECTION_HV | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Color_Cycle.brightness_min      = CERBERUS_MECH_BRIGHTNESS_MIN;
    Color_Cycle.brightness_max      = CERBERUS_MECH_BRIGHTNESS_MAX;
    Color_Cycle.brightness          = CERBERUS_MECH_BRIGHTNESS_DEFAULT;
    Color_Cycle.direction           = MODE_DIRECTION_HORIZONTAL;
    Color_Cycle.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Color_Cycle);

    mode Wave;
    Wave.name                       = "Rainbow Wave";
    Wave.value                      = CERBERUS_KEYBOARD_MODE_WAVE;
    Wave.flags                      = MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Wave.brightness_min             = CERBERUS_MECH_BRIGHTNESS_MIN;
    Wave.brightness_max             = CERBERUS_MECH_BRIGHTNESS_MAX;
    Wave.brightness                 = CERBERUS_MECH_BRIGHTNESS_DEFAULT;
    Wave.direction                  = MODE_DIRECTION_LEFT;
    Wave.color_mode                 = MODE_COLORS_NONE;
    modes.push_back(Wave);

    SetupZones();
}

RGBController_AsusCerberusKeyboard::~RGBController_AsusCerberusKeyboard()
{
    delete controller;
}

void RGBController_AsusCerberusKeyboard::SetupZones()
{
    int zone_size = 105;

    zone keyboard;
    keyboard.name               = "Keyboard";
    keyboard.type               = ZONE_TYPE_MATRIX;
    keyboard.leds_min           = zone_size;
    keyboard.leds_max           = zone_size;
    keyboard.leds_count         = zone_size;
    keyboard.matrix_map         = new matrix_map_type;
    keyboard.matrix_map->height = 6;
    keyboard.matrix_map->width  = 24;
    keyboard.matrix_map->map    = *matrix_map;
    zones.push_back(keyboard);

    for(int led_id = 0; led_id < zone_size; led_id++)
    {
        led new_led;
        new_led.name  = led_names[led_id].name;
        new_led.value = led_names[led_id].id;

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_AsusCerberusKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AsusCerberusKeyboard::DeviceUpdateLEDs()
{
    for(unsigned int i = 0; i < colors.size(); i++)
    {
        uint8_t red   = RGBGetRValue(colors[i]);
        uint8_t green = RGBGetGValue(colors[i]);
        uint8_t blue  = RGBGetBValue(colors[i]);

        controller->SetPerLEDColor(led_names[i].id, red, green, blue);
    }
    controller->SendPerLEDColorEnd();
}

void RGBController_AsusCerberusKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AsusCerberusKeyboard::UpdateSingleLED(int led)
{
    uint8_t red   = RGBGetRValue(colors[led]);
    uint8_t green = RGBGetGValue(colors[led]);
    uint8_t blue  = RGBGetBValue(colors[led]);


    controller->SetPerLEDColor(led_names[led].id, red, green, blue);
    controller->SendPerLEDColorEnd();
}

void RGBController_AsusCerberusKeyboard::DeviceUpdateMode()
{
    uint8_t direction   = 0;
    uint8_t red         = 0;
    uint8_t green       = 0;
    uint8_t blue        = 0;

    uint8_t mode        = modes[active_mode].value;


    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        mode        = CERBERUS_KEYBOARD_MODE_CUSTOM;
        direction   = 1;

        controller->SetPerLEDMode(modes[active_mode].value);
    }

    switch(modes[active_mode].value)
    {
        case CERBERUS_KEYBOARD_MODE_EXPLOSION:
            direction = (modes[active_mode].direction == MODE_DIRECTION_HORIZONTAL) ? 1 : 0;
            break;

        case CERBERUS_KEYBOARD_MODE_COLOR_CYCLE:
            direction = (modes[active_mode].direction == MODE_DIRECTION_HORIZONTAL) ? 0 : 1;
            break;

        case CERBERUS_KEYBOARD_MODE_WAVE:
            direction = modes[active_mode].direction;
            break;
    }

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        red     = RGBGetRValue(modes[active_mode].colors[0]);
        green   = RGBGetGValue(modes[active_mode].colors[0]);
        blue    = RGBGetBValue(modes[active_mode].colors[0]);
    }

    controller->SetMode(mode, red, green, blue, direction, modes[active_mode].brightness);
}
