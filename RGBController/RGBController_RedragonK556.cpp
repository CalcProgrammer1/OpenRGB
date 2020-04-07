/*-----------------------------------------*\
|  RGBController_RedragonK556.cpp           |
|                                           |
|  Generic RGB Interface for Redragon K556  |
|  Devarajas RGB Keyboard                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/25/2020   |
\*-----------------------------------------*/

#include "RGBController_RedragonK556.h"

RGBController_RedragonK556::RGBController_RedragonK556(RedragonK556Controller* redragon_ptr)
{
    redragon = redragon_ptr;

    name        = "Redragon K556 Devarajas";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Redragon K556 Device";

    mode Custom;
    Custom.name       = "Custom";
    Custom.value      = REDRAGON_K556_MODE_CUSTOM;
    Custom.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Custom.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode ColorWave;
    ColorWave.name       = "Color Wave";
    ColorWave.value      = REDRAGON_K556_MODE_COLOR_WAVE_LONG;
    ColorWave.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ColorWave.speed_min  = REDRAGON_K556_SPEED_SLOWEST;
    ColorWave.speed_max  = REDRAGON_K556_SPEED_FASTEST;
    ColorWave.speed      = REDRAGON_K556_SPEED_NORMAL;
    ColorWave.colors_min = 1;
    ColorWave.colors_max = 1;
    ColorWave.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ColorWave.colors.resize(1);
    modes.push_back(ColorWave);

    mode ColorWheel;
    ColorWheel.name       = "Color Wheel";
    ColorWheel.value      = REDRAGON_K556_MODE_COLOR_WHEEL;
    ColorWheel.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ColorWheel.speed_min  = REDRAGON_K556_SPEED_SLOWEST;
    ColorWheel.speed_max  = REDRAGON_K556_SPEED_FASTEST;
    ColorWheel.speed      = REDRAGON_K556_SPEED_NORMAL;
    ColorWheel.colors_min = 1;
    ColorWheel.colors_max = 1;
    ColorWheel.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ColorWheel.colors.resize(1);
    modes.push_back(ColorWheel);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Spectrum Cycle";
    SpectrumCycle.value      = REDRAGON_K556_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags      = MODE_FLAG_HAS_SPEED;
    SpectrumCycle.speed_min  = REDRAGON_K556_SPEED_SLOWEST;
    SpectrumCycle.speed_max  = REDRAGON_K556_SPEED_FASTEST;
    SpectrumCycle.speed      = REDRAGON_K556_SPEED_NORMAL;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = REDRAGON_K556_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Breathing.speed_min  = REDRAGON_K556_SPEED_SLOWEST;
    Breathing.speed_max  = REDRAGON_K556_SPEED_FASTEST;
    Breathing.speed      = REDRAGON_K556_SPEED_NORMAL;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Hurricane;
    Hurricane.name       = "Hurricane";
    Hurricane.value      = REDRAGON_K556_MODE_HURRICANE;
    Hurricane.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Hurricane.speed_min  = REDRAGON_K556_SPEED_SLOWEST;
    Hurricane.speed_max  = REDRAGON_K556_SPEED_FASTEST;
    Hurricane.speed      = REDRAGON_K556_SPEED_NORMAL;
    Hurricane.colors_min = 1;
    Hurricane.colors_max = 1;
    Hurricane.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Hurricane.colors.resize(1);
    modes.push_back(Hurricane);

    mode Accumulate;
    Accumulate.name       = "Accumulate";
    Accumulate.value      = REDRAGON_K556_MODE_ACCUMULATE;
    Accumulate.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Accumulate.speed_min  = REDRAGON_K556_SPEED_SLOWEST;
    Accumulate.speed_max  = REDRAGON_K556_SPEED_FASTEST;
    Accumulate.speed      = REDRAGON_K556_SPEED_NORMAL;
    Accumulate.colors_min = 1;
    Accumulate.colors_max = 1;
    Accumulate.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Accumulate.colors.resize(1);
    modes.push_back(Accumulate);

    mode Starlight;
    Starlight.name       = "Starlight";
    Starlight.value      = REDRAGON_K556_MODE_STARLIGHT_FAST;
    Starlight.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Starlight.speed_min  = REDRAGON_K556_SPEED_SLOWEST;
    Starlight.speed_max  = REDRAGON_K556_SPEED_FASTEST;
    Starlight.speed      = REDRAGON_K556_SPEED_NORMAL;
    Starlight.colors_min = 1;
    Starlight.colors_max = 1;
    Starlight.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Starlight.colors.resize(1);
    modes.push_back(Starlight);

    mode Visor;
    Visor.name       = "Visor";
    Visor.value      = REDRAGON_K556_MODE_VISOR;
    Visor.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Visor.speed_min  = REDRAGON_K556_SPEED_SLOWEST;
    Visor.speed_max  = REDRAGON_K556_SPEED_FASTEST;
    Visor.speed      = REDRAGON_K556_SPEED_NORMAL;
    Visor.colors_min = 1;
    Visor.colors_max = 1;
    Visor.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Visor.colors.resize(1);
    modes.push_back(Visor);

    mode Static;
    Static.name       = "Static";
    Static.value      = REDRAGON_K556_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode RainbowCircle;
    RainbowCircle.name       = "Rainbow Circle";
    RainbowCircle.value      = REDRAGON_K556_MODE_RAINBOW_WAVE_CIRCLE;
    RainbowCircle.flags      = 0;
    RainbowCircle.color_mode = MODE_COLORS_NONE;
    modes.push_back(RainbowCircle);

    mode Blooming;
    Blooming.name       = "Blooming";
    Blooming.value      = REDRAGON_K556_MODE_BLOOMING;
    Blooming.flags      = MODE_FLAG_HAS_SPEED;
    Blooming.speed_min  = REDRAGON_K556_SPEED_SLOWEST;
    Blooming.speed_max  = REDRAGON_K556_SPEED_FASTEST;
    Blooming.speed      = REDRAGON_K556_SPEED_NORMAL;
    Blooming.color_mode = MODE_COLORS_NONE;
    modes.push_back(Blooming);
    
    mode Reactive;
    Reactive.name       = "Reactive";
    Reactive.value      = REDRAGON_K556_MODE_REACTIVE;
    Reactive.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Reactive.speed_min  = REDRAGON_K556_SPEED_SLOWEST;
    Reactive.speed_max  = REDRAGON_K556_SPEED_FASTEST;
    Reactive.speed      = REDRAGON_K556_SPEED_NORMAL;
    Reactive.colors_min = 1;
    Reactive.colors_max = 1;
    Reactive.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Reactive.colors.resize(1);
    modes.push_back(Reactive);

    mode ReactiveRipple;
    ReactiveRipple.name       = "Reactive Ripple";
    ReactiveRipple.value      = REDRAGON_K556_MODE_REACTIVE_RIPPLE;
    ReactiveRipple.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ReactiveRipple.speed_min  = REDRAGON_K556_SPEED_SLOWEST;
    ReactiveRipple.speed_max  = REDRAGON_K556_SPEED_FASTEST;
    ReactiveRipple.speed      = REDRAGON_K556_SPEED_NORMAL;
    ReactiveRipple.colors_min = 1;
    ReactiveRipple.colors_max = 1;
    ReactiveRipple.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ReactiveRipple.colors.resize(1);
    modes.push_back(ReactiveRipple);

    mode ReactiveLine;
    ReactiveLine.name       = "Reactive Line";
    ReactiveLine.value      = REDRAGON_K556_MODE_REACTIVE_LINE;
    ReactiveLine.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    ReactiveLine.speed_min  = REDRAGON_K556_SPEED_SLOWEST;
    ReactiveLine.speed_max  = REDRAGON_K556_SPEED_FASTEST;
    ReactiveLine.speed      = REDRAGON_K556_SPEED_NORMAL;
    ReactiveLine.colors_min = 1;
    ReactiveLine.colors_max = 1;
    ReactiveLine.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ReactiveLine.colors.resize(1);
    modes.push_back(ReactiveLine);

    SetupZones();
}

void RGBController_RedragonK556::SetupZones()
{
    zone new_zone;

    new_zone.name = "Keyboard";
    new_zone.type = ZONE_TYPE_MATRIX;
    new_zone.leds_min = 126;
    new_zone.leds_max = 126;
    new_zone.leds_count = 126;

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

void RGBController_RedragonK556::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RedragonK556::UpdateLEDs()
{
    unsigned char color_data[7*0x36];

    for(int led_idx = 0; led_idx < 126; led_idx++)
    {
        color_data[(3 * led_idx) + 0] = RGBGetRValue(colors[led_idx]);
        color_data[(3 * led_idx) + 1] = RGBGetGValue(colors[led_idx]);
        color_data[(3 * led_idx) + 2] = RGBGetBValue(colors[led_idx]);
    }

    redragon->SetKeyboardColors
                (
                color_data,
                0x36 * 7
                );
}

void RGBController_RedragonK556::UpdateZoneLEDs(int /*zone*/)
{
    UpdateLEDs();
}

void RGBController_RedragonK556::UpdateSingleLED(int /*led*/)
{
    UpdateLEDs();
}

void RGBController_RedragonK556::SetCustomMode()
{

}

void RGBController_RedragonK556::UpdateMode()
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

    redragon->SendKeyboardModeEx
                (
                modes[active_mode].value,
                REDRAGON_K556_BRIGHTNESS_HIGHEST,
                modes[active_mode].speed,
                0,
                random,
                red,
                grn,
                blu
                );
}
