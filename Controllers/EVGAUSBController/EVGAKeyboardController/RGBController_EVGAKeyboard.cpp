/*---------------------------------------------------------*\
| RGBController_EVGAKeyboard.cpp                            |
|                                                           |
|   RGBController for EVGA keyboard                         |
|                                                           |
|   Chris M (Dr_No)                             25 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <time.h>
#include "Colors.h"
#include "RGBControllerKeyNames.h"
#include "RGBController_EVGAKeyboard.h"

static unsigned int full_matrix_map[6][21] =
{
    {   0,  NA,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15, 104, 105, 106, 107 },
    {  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  87,  88,  89,  90 },
    {  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  99, 100, 101,  91 },
    {  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  NA,  NA,  NA,  NA,  96,  97,  98,  NA },
    {  63,  NA,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  NA,  NA,  75,  NA,  93,  94,  95,  92 },
    {  76,  77,  78,  NA,  NA,  NA,  79,  NA,  NA,  NA,  80,  81,  82,  83,  84,  85,  86, 102,  NA, 103,  NA }
};

static unsigned int Z20_extra_zones[EVGA_KEYBOARD_Z20_EXTRA_ZONES][9] =
{
    {   0,   1,   2,   3,   4,   5,   6,   7,   8 },    //Index 108
    {   0,   1,   2,   3,   4,   5,   6,   7,   8 },    //Index 117
    {   0,   1,   2,   3,   4,   5,  NA,  NA,  NA }     //Index 126
};

const char* Z20_zone_names[EVGA_KEYBOARD_Z20_EXTRA_ZONES] =
{
    "Right Side LEDs",
    "Left Side LEDs",
    "Macro Keys"
};

static const char *led_names[] =
{
    KEY_EN_ESCAPE,          //  00
    KEY_EN_F1,
    KEY_EN_F2,
    KEY_EN_F3,
    KEY_EN_F4,
    KEY_EN_F5,
    KEY_EN_F6,
    KEY_EN_F7,
    KEY_EN_F8,
    KEY_EN_F9,
    KEY_EN_F10,             //  10
    KEY_EN_F11,
    KEY_EN_F12,
    KEY_EN_PRINT_SCREEN,
    KEY_EN_SCROLL_LOCK,
    KEY_EN_PAUSE_BREAK,

    KEY_EN_BACK_TICK,
    KEY_EN_1,
    KEY_EN_2,
    KEY_EN_3,
    KEY_EN_4,               //  20
    KEY_EN_5,
    KEY_EN_6,
    KEY_EN_7,
    KEY_EN_8,
    KEY_EN_9,
    KEY_EN_0,
    KEY_EN_MINUS,
    KEY_EN_EQUALS,
    KEY_EN_BACKSPACE,
    KEY_EN_INSERT,          //  30
    KEY_EN_HOME,
    KEY_EN_PAGE_UP,

    KEY_EN_TAB,
    KEY_EN_Q,
    KEY_EN_W,
    KEY_EN_E,
    KEY_EN_R,
    KEY_EN_T,
    KEY_EN_Y,
    KEY_EN_U,               //  40
    KEY_EN_I,
    KEY_EN_O,
    KEY_EN_P,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_DELETE,
    KEY_EN_END,
    KEY_EN_PAGE_DOWN,

    KEY_EN_CAPS_LOCK,       //  50
    KEY_EN_A,
    KEY_EN_S,
    KEY_EN_D,
    KEY_EN_F,
    KEY_EN_G,
    KEY_EN_H,
    KEY_EN_J,
    KEY_EN_K,
    KEY_EN_L,
    KEY_EN_SEMICOLON,       //  60
    KEY_EN_QUOTE,
    KEY_EN_ANSI_ENTER,

    KEY_EN_LEFT_SHIFT,
    KEY_EN_Z,
    KEY_EN_X,
    KEY_EN_C,
    KEY_EN_V,
    KEY_EN_B,
    KEY_EN_N,
    KEY_EN_M,               //  70
    KEY_EN_COMMA,
    KEY_EN_PERIOD,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_UP_ARROW,

    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    KEY_EN_SPACE,
    KEY_EN_RIGHT_ALT,       //  80
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_MENU,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW,

    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_MINUS,    //  90
    KEY_EN_NUMPAD_PLUS,
    KEY_EN_NUMPAD_ENTER,
    KEY_EN_NUMPAD_1,
    KEY_EN_NUMPAD_2,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_8,        // 100
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_0,
    KEY_EN_NUMPAD_PERIOD,

    KEY_EN_MEDIA_PREVIOUS,
    KEY_EN_MEDIA_PLAY_PAUSE,
    KEY_EN_MEDIA_NEXT,
    KEY_EN_MEDIA_MUTE,

    "Key: Right LED 1",
    "Key: Right LED 2",
    "Key: Right LED 3",     //110
    "Key: Right LED 4",
    "Key: Right LED 5",
    "Key: Right LED 6",
    "Key: Right LED 7",
    "Key: Right LED 8",
    "Key: Right LED 9",

    "Key: Left LED 1",
    "Key: Left LED 2",
    "Key: Left LED 3",
    "Key: Left LED 4",      //120
    "Key: Left LED 5",
    "Key: Left LED 6",
    "Key: Left LED 7",
    "Key: Left LED 8",
    "Key: Left LED 9",

    "Key: Feature Button",
    "Key: Macro 1",
    "Key: Macro 2",
    "Key: Macro 3",
    "Key: Macro 4",         //130
    "Key: Macro 5",
};

/**------------------------------------------------------------------*\
    @name EVGA USB Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectEVGAKeyboardControllers
    @comment The EVGA USB keyboard controller currently supports
        the Z15 (both ISO & ANSI) as well as the Z20 ANSI keyboards
\*-------------------------------------------------------------------*/

RGBController_EVGAKeyboard::RGBController_EVGAKeyboard(EVGAKeyboardController* controller_ptr)
{
    /*-----------------------------------------------------*\
    | Initialise the random functions from the clock        |
    \*-----------------------------------------------------*/
    std::srand((unsigned int)time(NULL));

    controller                      = controller_ptr;

    name                            = controller->GetName();
    vendor                          = "EVGA";
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = "EVGA Keyboard Device";
    serial                          = controller->GetSerial();
    location                        = controller->GetLocation();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = EVGA_KEYBOARD_CONTROLLER_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = EVGA_KEYBOARD_CONTROLLER_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.brightness_min           = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MIN;
    Static.brightness_max           = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;

    //controller->GetStatus(&Static);
    Static.colors.resize(Static.colors_max);
    Static.brightness               = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = EVGA_KEYBOARD_CONTROLLER_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 2;
    Breathing.brightness_min        = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MIN;
    Breathing.brightness_max        = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Breathing.speed_min             = EVGA_KEYBOARD_CONTROLLER_SPEED_SLOWEST;
    Breathing.speed_max             = EVGA_KEYBOARD_CONTROLLER_SPEED_FASTEST;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;

    //controller->GetStatus(&Breathing);
    Breathing.colors.resize(Breathing.colors_max);
    Breathing.brightness            = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Breathing.speed                 = EVGA_KEYBOARD_CONTROLLER_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode Pulse;
    Pulse.name                      = "Flashing";
    Pulse.value                     = EVGA_KEYBOARD_CONTROLLER_MODE_PULSE;
    Pulse.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Pulse.colors_min                = 2;
    Pulse.colors_max                = 7;
    Pulse.brightness_min            = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MIN;
    Pulse.brightness_max            = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Pulse.speed_min                 = EVGA_KEYBOARD_CONTROLLER_SPEED_SLOWEST;
    Pulse.speed_max                 = EVGA_KEYBOARD_CONTROLLER_SPEED_FASTEST;
    Pulse.color_mode                = MODE_COLORS_MODE_SPECIFIC;

    //controller->GetStatus(&Pulse);
    Pulse.colors.resize(Pulse.colors_max);
    Pulse.brightness                = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Pulse.speed                     = EVGA_KEYBOARD_CONTROLLER_SPEED_NORMAL;
    modes.push_back(Pulse);

    mode Spiral;
    Spiral.name                     = "Spiral";
    Spiral.value                    = EVGA_KEYBOARD_CONTROLLER_MODE_SPIRAL;
    Spiral.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Spiral.colors_min               = 7;
    Spiral.colors_max               = 7;
    Spiral.brightness_min           = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MIN;
    Spiral.brightness_max           = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Spiral.speed_min                = EVGA_KEYBOARD_CONTROLLER_SPEED_SLOWEST;
    Spiral.speed_max                = EVGA_KEYBOARD_CONTROLLER_SPEED_FASTEST;
    Spiral.color_mode               = MODE_COLORS_MODE_SPECIFIC;

    //controller->GetStatus(&Spiral);
    Spiral.colors.resize(Spiral.colors_max);
    Spiral.colors                   = { COLOR_RED, COLOR_DARKORANGE, COLOR_YELLOW, COLOR_LIME, COLOR_BLUE, COLOR_DARKVIOLET, COLOR_MAGENTA};
    Spiral.brightness               = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Spiral.speed                    = EVGA_KEYBOARD_CONTROLLER_SPEED_NORMAL;
    modes.push_back(Spiral);

    mode Rainbow;
    Rainbow.name                    = "Rainbow Wave";
    Rainbow.value                   = EVGA_KEYBOARD_CONTROLLER_MODE_RAINBOW;
    Rainbow.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Rainbow.colors_min              = 3;
    Rainbow.colors_max              = 7;
    Rainbow.brightness_min          = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MIN;
    Rainbow.brightness_max          = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Rainbow.speed_min               = EVGA_KEYBOARD_CONTROLLER_SPEED_SLOWISH;
    Rainbow.speed_max               = EVGA_KEYBOARD_CONTROLLER_SPEED_FASTEST;
    Rainbow.color_mode              = MODE_COLORS_MODE_SPECIFIC;

    //controller->GetStatus(&Rainbow);
    Rainbow.colors.resize(Rainbow.colors_max);
    Rainbow.colors                  = { COLOR_RED, COLOR_DARKORANGE, COLOR_YELLOW, COLOR_LIME, COLOR_BLUE, COLOR_DARKVIOLET, COLOR_MAGENTA};
    Rainbow.brightness              = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Rainbow.speed                   = EVGA_KEYBOARD_CONTROLLER_SPEED_NORMAL;
    modes.push_back(Rainbow);

    mode Star;
    Star.name                       = "Star Shining";
    Star.value                      = EVGA_KEYBOARD_CONTROLLER_MODE_STAR;
    Star.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Star.brightness_min             = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MIN;
    Star.brightness_max             = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Star.speed_min                  = EVGA_KEYBOARD_CONTROLLER_SPEED_SLOWEST;
    Star.speed_max                  = EVGA_KEYBOARD_CONTROLLER_SPEED_FASTEST;
    Star.color_mode                 = MODE_COLORS_NONE;

    //controller->GetStatus(&Star);
    Star.brightness                 = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Star.speed                      = EVGA_KEYBOARD_CONTROLLER_SPEED_NORMAL;
    modes.push_back(Star);

    mode Typing;
    Typing.name                     = "Typing Lighting";
    Typing.value                    = EVGA_KEYBOARD_CONTROLLER_MODE_TRIGGER;
    Typing.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Typing.colors_min               = 1;
    Typing.colors_max               = 7;
    Typing.brightness_min           = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MIN;
    Typing.brightness_max           = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Typing.speed_min                = EVGA_KEYBOARD_CONTROLLER_SPEED_SLOWEST;
    Typing.speed_max                = EVGA_KEYBOARD_CONTROLLER_SPEED_FASTEST;
    Typing.color_mode               = MODE_COLORS_MODE_SPECIFIC;

    //controller->GetStatus(&Typing);
    Typing.direction                = 0;
    Typing.colors.resize(Typing.colors_max);
    Typing.colors                   = { COLOR_RED, COLOR_DARKORANGE, COLOR_YELLOW, COLOR_LIME, COLOR_BLUE, COLOR_DARKVIOLET, COLOR_MAGENTA};
    Typing.brightness               = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Typing.speed                    = EVGA_KEYBOARD_CONTROLLER_SPEED_NORMAL;
    modes.push_back(Typing);

    mode Single;
    Single.name                     = "Reactive (Single Key)";
    Single.value                    = EVGA_KEYBOARD_CONTROLLER_MODE_TRIGGER;
    Single.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Single.colors_min               = 1;
    Single.colors_max               = 7;
    Single.brightness_min           = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MIN;
    Single.brightness_max           = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Single.speed_min                = EVGA_KEYBOARD_CONTROLLER_SPEED_SLOWEST;
    Single.speed_max                = EVGA_KEYBOARD_CONTROLLER_SPEED_FASTEST;
    Single.color_mode               = MODE_COLORS_MODE_SPECIFIC;

    //controller->GetStatus(&Single);
    Single.direction                = 1;
    Single.colors.resize(Single.colors_max);
    Single.colors                   = { COLOR_RED, COLOR_DARKORANGE, COLOR_YELLOW, COLOR_LIME, COLOR_BLUE, COLOR_DARKVIOLET, COLOR_MAGENTA};
    Single.brightness               = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    Single.speed                    = EVGA_KEYBOARD_CONTROLLER_SPEED_NORMAL;
    modes.push_back(Single);

    mode ThreeBy3;
    ThreeBy3.name                   = "Reactive (3x3 Key)";
    ThreeBy3.value                  = EVGA_KEYBOARD_CONTROLLER_MODE_TRIGGER;
    ThreeBy3.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    ThreeBy3.colors_min             = 1;
    ThreeBy3.colors_max             = 7;
    ThreeBy3.brightness_min         = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MIN;
    ThreeBy3.brightness_max         = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    ThreeBy3.speed_min              = EVGA_KEYBOARD_CONTROLLER_SPEED_SLOWEST;
    ThreeBy3.speed_max              = EVGA_KEYBOARD_CONTROLLER_SPEED_FASTEST;
    ThreeBy3.color_mode             = MODE_COLORS_MODE_SPECIFIC;

    //controller->GetStatus(&ThreeBy3);
    ThreeBy3.direction              = 2;
    ThreeBy3.colors.resize(ThreeBy3.colors_max);
    ThreeBy3.colors                 = { COLOR_RED, COLOR_DARKORANGE, COLOR_YELLOW, COLOR_LIME, COLOR_BLUE, COLOR_DARKVIOLET, COLOR_MAGENTA};
    ThreeBy3.brightness             = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
    ThreeBy3.speed                  = EVGA_KEYBOARD_CONTROLLER_SPEED_NORMAL;
    modes.push_back(ThreeBy3);

    //uint8_t set_mode                = controller->GetMode();
    //active_mode                     = set_mode;
    SetupZones();
}

RGBController_EVGAKeyboard::~RGBController_EVGAKeyboard()
{
    delete controller;
}

void RGBController_EVGAKeyboard::SetupZones()
{
    /*-------------------------------------------------*\
    | Set up the base configuration common to           |
    |   Z15 and Z20                                     |
    \*-------------------------------------------------*/
    zone KB_zone;
    KB_zone.name                = ZONE_EN_KEYBOARD;
    KB_zone.type                = ZONE_TYPE_MATRIX;
    KB_zone.leds_min            = EVGA_KEYBOARD_FULL_SIZE_KEYCOUNT;
    KB_zone.leds_max            = EVGA_KEYBOARD_FULL_SIZE_KEYCOUNT;
    KB_zone.leds_count          = EVGA_KEYBOARD_FULL_SIZE_KEYCOUNT;

    KB_zone.matrix_map          = new matrix_map_type;
    KB_zone.matrix_map->height  = 6;
    KB_zone.matrix_map->width   = 21;
    KB_zone.matrix_map->map     = (unsigned int *)&full_matrix_map;
    zones.push_back(KB_zone);

    /*-------------------------------------------------*\
    | Add configuration for the Z20                     |
    \*-------------------------------------------------*/
    if(controller->GetPid() == 0x260A || controller->GetPid() == 0x2610)
    {

        for(uint8_t i = 0; i < EVGA_KEYBOARD_Z20_EXTRA_ZONES; i++)
        {
            uint8_t zone_size = sizeof(Z20_extra_zones[i]) / sizeof(Z20_extra_zones[i][0]);

            for(uint8_t count = 0; count < zone_size; count++)
            {
                if(Z20_extra_zones[i][count] == NA)
                {
                    zone_size = count;
                    break;
                }
            }

            zone new_zone;
            new_zone.name               = Z20_zone_names[i];
            new_zone.type               = ZONE_TYPE_MATRIX;
            new_zone.leds_min           = zone_size;
            new_zone.leds_max           = zone_size;
            new_zone.leds_count         = zone_size;

            new_zone.matrix_map         = new matrix_map_type;
            new_zone.matrix_map->height = 1;
            new_zone.matrix_map->width  = zone_size;
            new_zone.matrix_map->map    = (unsigned int *)&Z20_extra_zones[i];
            zones.push_back(new_zone);
        }
    }

    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    /*---------------------------------------------------------*\
    | Set up leds                                               |
    \*---------------------------------------------------------*/
    for(std::size_t zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        unsigned int zone_offset = (unsigned int)leds.size();

        for(unsigned int led_index = 0; led_index < zones[zone_index].leds_count; led_index++)
        {
            led new_led;
            new_led.value       = led_index + zone_offset;
            new_led.name        = led_names[new_led.value];
            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_EVGAKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_EVGAKeyboard::DeviceUpdateLEDs()
{
    controller->SetLedsDirect(colors);
}

void RGBController_EVGAKeyboard::UpdateZoneLEDs(int zone)
{
    std::vector<RGBColor> colour;
    for(size_t i = 0; i < zones[zone].leds_count; i++)
    {
        colour.push_back(zones[zone].colors[i]);
    }

    controller->SetLedsDirect(colour);
}

void RGBController_EVGAKeyboard::UpdateSingleLED(int led)
{
    std::vector<RGBColor> colour;
    colour.push_back(colors[led]);

    controller->SetLedsDirect(colour);
}

void RGBController_EVGAKeyboard::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | No mode set packets required for Direct mode but an       |
    |   extra packet is sent when switching back to HW modes    |
    \*---------------------------------------------------------*/
    mode set_mode                   = modes[active_mode];

    if(current_mode == EVGA_KEYBOARD_CONTROLLER_MODE_DIRECT)
    {
        controller->SetHWModes();
    }
    current_mode                    = set_mode.value;

    if(set_mode.value == EVGA_KEYBOARD_CONTROLLER_MODE_DIRECT)
    {
        return;
    }

    /*---------------------------------------------------------*\
    | Random colours are generated randoms from software        |
    \*---------------------------------------------------------*/
    uint8_t direction               = direction_map[set_mode.value][set_mode.direction];
    std::vector<RGBColor> colours   = (set_mode.colors);

    if(set_mode.color_mode == MODE_COLORS_RANDOM)
    {
        for(unsigned int i = 0; i < colours.size(); i++)
        {
            colours[i] = GetRandomColor();
        }
    }

    controller->SetMode( set_mode.value, set_mode.speed, set_mode.brightness, direction, colours );
}

void RGBController_EVGAKeyboard::DeviceSaveMode()
{
    controller->SaveMode();
}

RGBColor RGBController_EVGAKeyboard::GetRandomColor()
{
    return (rand() % 16777215);
}
