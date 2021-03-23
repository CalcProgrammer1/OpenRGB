/*-----------------------------------------*\
|  RGBController_EVisionKeyboard.cpp        |
|                                           |
|  Generic RGB Interface for EVision RGB    |
|  Keyboard                                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/25/2020   |
\*-----------------------------------------*/

#include "RGBController_EVisionKeyboard.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   0,  NA,  1,    2,   3,   4,  NA,   5,   6,   7,   8,  NA,   9,  10,  11,  12,  14,  15,  16,  NA,  NA,  NA,  NA },
      {  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  NA,  32,  33,  34,  NA,  35,  36,  37,  38,  39,  40,  41 },
      {  42,  NA,  43,  44,  45,  46,  NA,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62 },
      {  63,  NA,  64,  65,  66,  67,  NA,  68,  69,  70,  71,  72,  73,  74,  76,  NA,  NA,  NA,  NA,  80,  81,  82,  NA },
      {  84,  NA,  86,  87,  88,  89,  NA,  90,  NA,  91,  92,  93,  94,  95,  97,  NA,  NA,  99,  NA, 101, 102, 103, 104 },
      { 105, 106, 107,  NA,  NA,  NA,  NA, 108,  NA,  NA,  NA,  NA, 109, 110, 111, 113, 119, 120, 121, 123,  NA, 124,  NA } };

RGBController_EVisionKeyboard::RGBController_EVisionKeyboard(EVisionKeyboardController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "EVision Keyboard Device";
    vendor      = "EVision";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "EVision Keyboard Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Custom;
    Custom.name       = "Custom";
    Custom.value      = EVISION_KB_MODE_CUSTOM;
    Custom.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Custom.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode ColorWave;
    ColorWave.name       = "Color Wave";
    ColorWave.value      = EVISION_KB_MODE_COLOR_WAVE_LONG;
    ColorWave.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ColorWave.speed_min  = EVISION_KB_SPEED_SLOWEST;
    ColorWave.speed_max  = EVISION_KB_SPEED_FASTEST;
    ColorWave.speed      = EVISION_KB_SPEED_NORMAL;
    ColorWave.colors_min = 1;
    ColorWave.colors_max = 1;
    ColorWave.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ColorWave.colors.resize(1);
    modes.push_back(ColorWave);

    mode ColorWheel;
    ColorWheel.name       = "Color Wheel";
    ColorWheel.value      = EVISION_KB_MODE_COLOR_WHEEL;
    ColorWheel.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ColorWheel.speed_min  = EVISION_KB_SPEED_SLOWEST;
    ColorWheel.speed_max  = EVISION_KB_SPEED_FASTEST;
    ColorWheel.speed      = EVISION_KB_SPEED_NORMAL;
    ColorWheel.colors_min = 1;
    ColorWheel.colors_max = 1;
    ColorWheel.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ColorWheel.colors.resize(1);
    modes.push_back(ColorWheel);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Spectrum Cycle";
    SpectrumCycle.value      = EVISION_KB_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags      = MODE_FLAG_HAS_SPEED;
    SpectrumCycle.speed_min  = EVISION_KB_SPEED_SLOWEST;
    SpectrumCycle.speed_max  = EVISION_KB_SPEED_FASTEST;
    SpectrumCycle.speed      = EVISION_KB_SPEED_NORMAL;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = EVISION_KB_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Breathing.speed_min  = EVISION_KB_SPEED_SLOWEST;
    Breathing.speed_max  = EVISION_KB_SPEED_FASTEST;
    Breathing.speed      = EVISION_KB_SPEED_NORMAL;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Hurricane;
    Hurricane.name       = "Hurricane";
    Hurricane.value      = EVISION_KB_MODE_HURRICANE;
    Hurricane.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Hurricane.speed_min  = EVISION_KB_SPEED_SLOWEST;
    Hurricane.speed_max  = EVISION_KB_SPEED_FASTEST;
    Hurricane.speed      = EVISION_KB_SPEED_NORMAL;
    Hurricane.colors_min = 1;
    Hurricane.colors_max = 1;
    Hurricane.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Hurricane.colors.resize(1);
    modes.push_back(Hurricane);

    mode Accumulate;
    Accumulate.name       = "Accumulate";
    Accumulate.value      = EVISION_KB_MODE_ACCUMULATE;
    Accumulate.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Accumulate.speed_min  = EVISION_KB_SPEED_SLOWEST;
    Accumulate.speed_max  = EVISION_KB_SPEED_FASTEST;
    Accumulate.speed      = EVISION_KB_SPEED_NORMAL;
    Accumulate.colors_min = 1;
    Accumulate.colors_max = 1;
    Accumulate.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Accumulate.colors.resize(1);
    modes.push_back(Accumulate);

    mode Starlight;
    Starlight.name       = "Starlight";
    Starlight.value      = EVISION_KB_MODE_STARLIGHT_FAST;
    Starlight.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Starlight.speed_min  = EVISION_KB_SPEED_SLOWEST;
    Starlight.speed_max  = EVISION_KB_SPEED_FASTEST;
    Starlight.speed      = EVISION_KB_SPEED_NORMAL;
    Starlight.colors_min = 1;
    Starlight.colors_max = 1;
    Starlight.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Starlight.colors.resize(1);
    modes.push_back(Starlight);

    mode Visor;
    Visor.name       = "Visor";
    Visor.value      = EVISION_KB_MODE_VISOR;
    Visor.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Visor.speed_min  = EVISION_KB_SPEED_SLOWEST;
    Visor.speed_max  = EVISION_KB_SPEED_FASTEST;
    Visor.speed      = EVISION_KB_SPEED_NORMAL;
    Visor.colors_min = 1;
    Visor.colors_max = 1;
    Visor.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Visor.colors.resize(1);
    modes.push_back(Visor);

    mode Static;
    Static.name       = "Static";
    Static.value      = EVISION_KB_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode RainbowCircle;
    RainbowCircle.name       = "Rainbow Circle";
    RainbowCircle.value      = EVISION_KB_MODE_RAINBOW_WAVE_CIRCLE;
    RainbowCircle.flags      = 0;
    RainbowCircle.color_mode = MODE_COLORS_NONE;
    modes.push_back(RainbowCircle);

    mode Blooming;
    Blooming.name       = "Blooming";
    Blooming.value      = EVISION_KB_MODE_BLOOMING;
    Blooming.flags      = MODE_FLAG_HAS_SPEED;
    Blooming.speed_min  = EVISION_KB_SPEED_SLOWEST;
    Blooming.speed_max  = EVISION_KB_SPEED_FASTEST;
    Blooming.speed      = EVISION_KB_SPEED_NORMAL;
    Blooming.color_mode = MODE_COLORS_NONE;
    modes.push_back(Blooming);
    
    mode Reactive;
    Reactive.name       = "Reactive";
    Reactive.value      = EVISION_KB_MODE_REACTIVE;
    Reactive.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Reactive.speed_min  = EVISION_KB_SPEED_SLOWEST;
    Reactive.speed_max  = EVISION_KB_SPEED_FASTEST;
    Reactive.speed      = EVISION_KB_SPEED_NORMAL;
    Reactive.colors_min = 1;
    Reactive.colors_max = 1;
    Reactive.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Reactive.colors.resize(1);
    modes.push_back(Reactive);

    mode ReactiveRipple;
    ReactiveRipple.name       = "Reactive Ripple";
    ReactiveRipple.value      = EVISION_KB_MODE_REACTIVE_RIPPLE;
    ReactiveRipple.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ReactiveRipple.speed_min  = EVISION_KB_SPEED_SLOWEST;
    ReactiveRipple.speed_max  = EVISION_KB_SPEED_FASTEST;
    ReactiveRipple.speed      = EVISION_KB_SPEED_NORMAL;
    ReactiveRipple.colors_min = 1;
    ReactiveRipple.colors_max = 1;
    ReactiveRipple.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ReactiveRipple.colors.resize(1);
    modes.push_back(ReactiveRipple);

    mode ReactiveLine;
    ReactiveLine.name       = "Reactive Line";
    ReactiveLine.value      = EVISION_KB_MODE_REACTIVE_LINE;
    ReactiveLine.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ReactiveLine.speed_min  = EVISION_KB_SPEED_SLOWEST;
    ReactiveLine.speed_max  = EVISION_KB_SPEED_FASTEST;
    ReactiveLine.speed      = EVISION_KB_SPEED_NORMAL;
    ReactiveLine.colors_min = 1;
    ReactiveLine.colors_max = 1;
    ReactiveLine.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ReactiveLine.colors.resize(1);
    modes.push_back(ReactiveLine);

    SetupZones();
}

RGBController_EVisionKeyboard::~RGBController_EVisionKeyboard()
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

void RGBController_EVisionKeyboard::SetupZones()
{
    zone new_zone;

    new_zone.name               = "Keyboard";
    new_zone.type               = ZONE_TYPE_MATRIX;
    new_zone.leds_min           = 126;
    new_zone.leds_max           = 126;
    new_zone.leds_count         = 126;
    new_zone.matrix_map         = new matrix_map_type;
    new_zone.matrix_map->height = 6;
    new_zone.matrix_map->width  = 23;
    new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
    
    zones.push_back(new_zone);

    for(int led_idx = 0; led_idx < 126; led_idx++)
    {
        led new_led;

        new_led.name = "Keyboard LED ";
        new_led.name.append(std::to_string(led_idx));

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_EVisionKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_EVisionKeyboard::DeviceUpdateLEDs()
{
    unsigned char color_data[7*0x36];

    for(int led_idx = 0; led_idx < 126; led_idx++)
    {
        color_data[(3 * led_idx) + 0] = RGBGetRValue(colors[led_idx]);
        color_data[(3 * led_idx) + 1] = RGBGetGValue(colors[led_idx]);
        color_data[(3 * led_idx) + 2] = RGBGetBValue(colors[led_idx]);
    }

    controller->SetKeyboardColors
                    (
                    color_data,
                    0x36 * 7
                    );
}

void RGBController_EVisionKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVisionKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVisionKeyboard::SetCustomMode()
{

}

void RGBController_EVisionKeyboard::DeviceUpdateMode()
{
    unsigned char red    = 0x00;
    unsigned char grn    = 0x00;
    unsigned char blu    = 0x00;
    unsigned char random = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);

    if(modes[active_mode].colors.size() > 0)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    controller->SendKeyboardModeEx
                    (
                    modes[active_mode].value,
                    EVISION_KB_BRIGHTNESS_HIGHEST,
                    modes[active_mode].speed,
                    0,
                    random,
                    red,
                    grn,
                    blu
                    );
}
