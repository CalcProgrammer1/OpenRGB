/*-----------------------------------------*\
|  RGBController_AsusAuraTUFKeyboard.cpp    |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Mola19 03/03/2020                        |
\*-----------------------------------------*/

#include "RGBController_AsusAuraTUFKeyboard.h"
#include <vector>

#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   0,  NA,  18,  24,  30,  36,  NA,  48,  54,  60,  66,  72,  78,  84,  90,  NA,  96, 102, 108,  NA,  NA,  NA,  NA },
      {   1,  13,  19,  25,  31,  37,  43,  49,  55,  61,  67,  73,  79,  91,  NA,  NA,  97, 103, 109, 115, 121, 127, 133 },
      {   2,  NA,  14,  20,  26,  32,  38,  44,  50,  56,  62,  68,  74,  80,  NA,  NA,  98, 104, 110, 116, 122, 128, 134 },
      {   3,  NA,  15,  21,  27,  33,  39,  45,  51,  57,  63,  69,  75,  81,  93,  NA,  NA,  NA,  NA, 117, 123, 129,  NA },
      {   4,  10,  16,  22,  28,  34,  40,  46,  52,  58,  64,  70,  NA,  94,  NA,  NA,  NA, 106,  NA, 118, 124, 130, 136 },
      {   5,  11,  17,  NA,  NA,  NA,  41,  NA,  NA,  NA,  59,  71,  77,  NA,  95,  NA, 101, 107, 113, 119,  NA, 131,  NA } };

static const char* zone_names[] =
{
    "Keyboard"
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const unsigned int zone_sizes[] =
{
    138
};

static const char *led_names[] =
{
    "Key: Escape",
    "Key: ^",
    "Key: Tab",
    "Key: Caps Lock",
    "Key: Left Shift",
    "Key: Left Control",

    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Key: <",
    "Key: Left Windows",

    "Unused",
    "Key: 1",
    "Key: Q",
    "Key: A",
    "Key: Y",
    "Key: Left Alt",

    "Key: F1",
    "Key: 2",
    "Key: W",
    "Key: S",
    "Key: X",
    "Unused",

    "Key: F2",
    "Key: 3",
    "Key: E",
    "Key: D",
    "Key: C",
    "Unused",

    "Key: F3",
    "Key: 4",
    "Key: R",
    "Key: F",
    "Key: V",
    "Unused",

    "Key: F4",
    "Key: 5",
    "Key: T",
    "Key: G",
    "Key: B",
    "Key: Space",

    "Unused",
    "Key: 6",
    "Key: Z",
    "Key: H",
    "Key: N",
    "Unused",

    "Key: F5",
    "Key: 7",
    "Key: U",
    "Key: J",
    "Key: M",
    "Unused",

    "Key: F6",
    "Key: 8",
    "Key: I",
    "Key: K",
    "Key: ,",
    "Key: Right Alt",

    "Key: F7",
    "Key: 9",
    "Key: O",
    "Key: L",
    "Key: .",
    "Unused",

    "Key: F8",
    "Key: 0",
    "Key: P",
    "Key: Ö",
    "Key: -",
    "Key: Right Fn",

    "Key: F9",
    "Key: ß",
    "Key: Ü",
    "Key: Ä",
    "Unused",
    "Key: Menu",

    "Key: F10",
    "Key: ´",
    "Key: +",
    "Key: #",
    "Unused",
    "Unused",

    "Key: F11",
    "Unused",
    "Unused",
    "Unused",
    "Unused",
    "Unused",

    "Key: F12",
    "Key: Backspace",
    "Unused",
    "Key: Enter",
    "Key: Right Shift",
    "Key: Right Control",

    "Key: Print Screen",
    "Key: Insert",
    "Key: Delete",
    "Unused",
    "Unused",
    "Key: Left Arrow",

    "Key: Scroll Lock",
    "Key: Home",
    "Key: End",
    "Unused",
    "Key: Up Arrow",
    "Key: Down Arrow",

    "Key: Pause/Break",
    "Key: Page Up",
    "Key: Page Down",
    "Unused",
    "Unused",
    "Key: Right Arrow",

    "Unused",
    "Key: Num Lock",
    "Key: Number Pad 7",
    "Key: Number Pad 4",
    "Key: Number Pad 1",
    "Key: Number Pad 0",

    "Unused",
    "Key: Number Pad /",
    "Key: Number Pad 8",
    "Key: Number Pad 5",
    "Key: Number Pad 2",
    "Unused",

    "Unused",
    "Key: Number Pad *",
    "Key: Number Pad 9",
    "Key: Number Pad 6",
    "Key: Number Pad 3",
    "Key: Number Pad ,",

    "Unused",
    "Key: Number Pad -",
    "Key: Number Pad +",
    "Unused",
    "Key: Number Pad Enter",
    "Unused",
};

RGBController_AuraTUFKeyboard::RGBController_AuraTUFKeyboard(AuraTUFKeyboardController* aura_ptr)
{
    aura = aura_ptr;

    name        = "ASUS Aura Keyboard";
    vendor      = "ASUS";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "ASUS Aura Keyboard Device";
    location    = aura->GetDeviceLocation();
    serial      = aura->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = AURA_KEYBOARD_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = AURA_KEYBOARD_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = AURA_KEYBOARD_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed_min  = 0;
    Breathing.speed_max  = 15;
    Breathing.speed      = 8;
    Breathing.colors_min = 1;
    Breathing.colors_max = 2;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Color_Cycle;
    Color_Cycle.name       = "Color Cycle";
    Color_Cycle.value      = AURA_KEYBOARD_MODE_COLOR_CYCLE;
    Color_Cycle.flags      = MODE_FLAG_HAS_SPEED;
    Color_Cycle.color_mode = MODE_COLORS_NONE;
    Color_Cycle.speed_min  = 0;
    Color_Cycle.speed_max  = 15;
    Color_Cycle.speed      = 8;
    modes.push_back(Color_Cycle);

    mode Wave;
    Wave.name       = "Wave";
    Wave.value      = AURA_KEYBOARD_MODE_WAVE;
    Wave.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_DIRECTION_HV;
    Wave.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Wave.speed_min  = 0;
    Wave.speed_max  = 15;
    Wave.speed      = 8;
    Wave.direction  = MODE_DIRECTION_LEFT;
    Wave.colors_min = 1;
    Wave.colors_max = 7;
    Wave.colors.resize(7);
    modes.push_back(Wave);

    mode Ripple;
    Ripple.name       = "Ripple";
    Ripple.value      = AURA_KEYBOARD_MODE_RIPPLE;
    Ripple.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED;
    Ripple.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Ripple.speed_min  = 0;
    Ripple.speed_max  = 15;
    Ripple.speed      = 8;
    Ripple.colors_min = 1;
    Ripple.colors_max = 8;
    Ripple.colors.resize(7);
    modes.push_back(Ripple);

    mode Reactive;
    Reactive.name       = "Reactive";
    Reactive.value      = AURA_KEYBOARD_MODE_REACTIVE;
    Reactive.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED;
    Reactive.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Reactive.speed_min  = 0;
    Reactive.speed_max  = 15;
    Reactive.speed      = 8;
    Reactive.colors_min = 1;
    Reactive.colors_max = 2;
    Reactive.colors.resize(1);
    modes.push_back(Reactive);

    mode Starry_Night;
    Starry_Night.name       = "Starry Night";
    Starry_Night.value      = AURA_KEYBOARD_MODE_STARRY_NIGHT;
    Starry_Night.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED;
    Starry_Night.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Starry_Night.speed_min  = 0;
    Starry_Night.speed_max  = 15;
    Starry_Night.speed      = 8;
    Starry_Night.colors_min = 1;
    Starry_Night.colors_max = 3;
    Starry_Night.colors.resize(1);
    modes.push_back(Starry_Night);

    mode Quicksand;
    Quicksand.name       = "Quicksand";
    Quicksand.value      = AURA_KEYBOARD_MODE_QUICKSAND;
    Quicksand.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD;
    Quicksand.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Quicksand.direction  = MODE_DIRECTION_DOWN;
    Quicksand.speed_min  = 0;
    Quicksand.speed_max  = 15;
    Quicksand.speed      = 8;
    Quicksand.colors_min = 6;
    Quicksand.colors_max = 6;
    Quicksand.colors.resize(6);
    modes.push_back(Quicksand);

    mode Current;
    Current.name       = "Current";
    Current.value      = AURA_KEYBOARD_MODE_CURRENT;
    Current.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED;
    Current.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Current.speed_min  = 0;
    Current.speed_max  = 15;
    Current.speed      = 8;
    Current.colors_min = 1;
    Current.colors_max = 3;
    Current.colors.resize(1);
    modes.push_back(Current);

    mode Rain_Drop;
    Rain_Drop.name       = "Rain Drop";
    Rain_Drop.value      = AURA_KEYBOARD_MODE_RAIN_DROP;
    Rain_Drop.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED;
    Rain_Drop.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Rain_Drop.speed_min  = 0;
    Rain_Drop.speed_max  = 15;
    Rain_Drop.speed      = 8;
    Rain_Drop.colors_min = 1;
    Rain_Drop.colors_max = 3;
    Rain_Drop.colors.resize(1);
    modes.push_back(Rain_Drop);

    SetupZones();
}

RGBController_AuraTUFKeyboard::~RGBController_AuraTUFKeyboard()
{
    delete aura;
}

void RGBController_AuraTUFKeyboard::SetupZones()
{
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
        new_zone.matrix_map->height = 6;
        new_zone.matrix_map->width  = 23;
        new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
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

void RGBController_AuraTUFKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AuraTUFKeyboard::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_AuraTUFKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    aura->UpdateLeds(colors);
}

void RGBController_AuraTUFKeyboard::UpdateSingleLED(int led)
{
    unsigned char red   = RGBGetRValue(colors[led]);
    unsigned char green = RGBGetGValue(colors[led]);
    unsigned char blue  = RGBGetBValue(colors[led]);
    aura->UpdateSingleLed(led, red, green, blue);
}

void RGBController_AuraTUFKeyboard::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_AuraTUFKeyboard::DeviceUpdateMode()
{   
    unsigned char color_mode;
    std::vector<RGBColor> mode_colors;

    if(modes[active_mode].value == AURA_KEYBOARD_MODE_DIRECT)
    {
        aura->UpdateLeds(colors);
    }
    else
    {
        bool mode_with_double = (modes[active_mode].value == 1 || modes[active_mode].value == 3 || modes[active_mode].value == 6 || modes[active_mode].value == 8 || modes[active_mode].value == 9);

        if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC && mode_with_double && modes[active_mode].colors.size() > 1)
        {
            if(modes[active_mode].colors[1] == 000)
            {
                color_mode = 0;
            }
            else
            {
                color_mode = 16;
            }
        }
        else
        {
            color_mode = modes[active_mode].color_mode == MODE_COLORS_RANDOM ? 1 : 0;
        }

        int direction = 0;

        switch (modes[active_mode].direction)
        {
            case MODE_DIRECTION_LEFT:
                direction = 0;
                break;
            case MODE_DIRECTION_RIGHT:
                direction = 4;
                break;
            case MODE_DIRECTION_UP:
                direction = 6;
                break;
            case MODE_DIRECTION_DOWN:
                direction = 2;
                break;
            case MODE_DIRECTION_HORIZONTAL:
                direction = 8;
                break;
            case MODE_DIRECTION_VERTICAL:
                direction = 1;
                break;
        }

        mode_colors = modes[active_mode].color_mode == MODE_COLORS_PER_LED ? std::vector<RGBColor>(colors) : std::vector<RGBColor>(modes[active_mode].colors);

        aura->UpdateDevice(modes[active_mode].value, mode_colors, direction, color_mode, (15 - modes[active_mode].speed));
    }
}
