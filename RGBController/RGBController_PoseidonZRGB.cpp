/*-----------------------------------------*\
|  RGBController_PoseidonZRGB.cpp           |
|                                           |
|  Generic RGB Interface for Thermaltake    |
|  Poseidon Z RGB Keyboard                  |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#include "RGBController_PoseidonZRGB.h"

static const char* zone_names[] =
{
    "Keyboard"
};

static const unsigned int zone_sizes[] =
{
    104
};

static const char* led_names[] =
{
    "Key: Escape",
    "Key: `",
    "Key: Tab",
    "Key: Caps Lock",
    "Key: Left Shift",
    "Key: Left Control",
    "Key: Left Arrow",
    "Key: Insert",
    "Key: F1",
    "Key: 1",
    "Key: Q",
    "Key: A",
    "Key: Windows",
    "Key: Down Arrow",
    "Key: Delete",
    "Key: F2",
    "Key: 2",
    "Key: W",
    "Key: S",
    "Key: Left Alt",
    "Key: Right Arrow",
    "Key: Home",
    "Key: F3",
    "Key: 3",
    "Key: E",
    "Key: D",
    "Key: Z",
    "Key: Up Arrow",
    "Key: End",
    "Key: F4",
    "Key: 4",
    "Key: R",
    "Key: F",
    "Key: X",
    "Key: Space",
    "Key: Number Pad 4",
    "Key: Page Up",
    "Key: F5",
    "Key: 5",
    "Key: T",
    "Key: G",
    "Key: C",
    "Key: Number Pad 1",
    "Key: Page Down",
    "Key: F6",
    "Key: 6",
    "Key: Y",
    "Key: H",
    "Key: V",
    "Key: Number Pad 2",
    "Key: Num Lock",
    "Key: F7",
    "Key: 7",
    "Key: U",
    "Key: J",
    "Key: B",
    "Key: Number Pad 0",
    "Key: Number Pad 7",
    "Key: F8",
    "Key: 8",
    "Key: I",
    "Key: K",
    "Key: N",
    "Key: Number Pad 6",
    "Key: Number Pad /",
    "Key: F9",
    "Key: 9",
    "Key: O",
    "Key: L",
    "Key: M",
    "Key: Right Alt",
    "Key: Number Pad 3",
    "Key: Number Pad 8",
    "Key: F10",
    "Key: 0",
    "Key: P",
    "Key: ;",
    "Key: ,",
    "Key: Fn",
    "Key: Number Pad .",
    "Key: Number Pad *",
    "Key: F11",
    "Key: -",
    "Key: [",
    "Key: '",
    "Key: .",
    "Key: Number Pad +",
    "Key: Number Pad 9",
    "Key: F12",
    "Key: =",
    "Key: ]",
    "Key: /",
    "Key: Context",
    "Key: Number Pad -",
    "Key: Print Screen",
    "Key: \\",
    "Key: Enter",
    "Key: Right Control",
    "Key: Number Pad Enter",
    "Key: Number Pad 5",
    "Key: Scroll Lock",
    "Key: Right Shift",
    "Key: Pause/Break",
    "Key: Backspace"
};

RGBController_PoseidonZRGB::RGBController_PoseidonZRGB(PoseidonZRGBController* poseidon_ptr)
{
    poseidon = poseidon_ptr;

    name        = "Thermaltake Poseidon Z RGB";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Thermaltake Poseidon Z RGB Device";

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = POSEIDONZ_MODE_STATIC;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = POSEIDONZ_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Wave;
    Wave.name       = "Wave";
    Wave.value      = POSEIDONZ_MODE_WAVE;
    Wave.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.speed_min  = POSEIDONZ_SPEED_SLOW;
    Wave.speed_max  = POSEIDONZ_SPEED_FAST;
    Wave.color_mode = MODE_COLORS_NONE;
    Wave.speed      = POSEIDONZ_SPEED_FAST;
    Wave.direction  = MODE_DIRECTION_LEFT;
    modes.push_back(Wave);

    mode Ripple;
    Ripple.name       = "Ripple";
    Ripple.value      = POSEIDONZ_MODE_RIPPLE;
    Ripple.flags      = 0;
    Ripple.color_mode = MODE_COLORS_NONE;
    modes.push_back(Ripple);

    mode Reactive;
    Reactive.name       = "Reactive";
    Reactive.value      = POSEIDONZ_MODE_REACTIVE;
    Reactive.flags      = 0;
    Reactive.color_mode = MODE_COLORS_NONE;
    modes.push_back(Reactive);

    SetupZones();
}

RGBController_PoseidonZRGB::~RGBController_PoseidonZRGB()
{

}

void RGBController_PoseidonZRGB::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        zone new_zone;
        new_zone.name           = zone_names[zone_idx];
        new_zone.leds_min       = zone_sizes[zone_idx];
        new_zone.leds_max       = zone_sizes[zone_idx];
        new_zone.leds_count     = zone_sizes[zone_idx];
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

void RGBController_PoseidonZRGB::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PoseidonZRGB::UpdateLEDs()
{
    if(active_mode == 0)
    {
        poseidon->SetLEDsDirect(colors);
    }
    else
    {
        poseidon->SetLEDs(colors);
    }
}

void RGBController_PoseidonZRGB::UpdateZoneLEDs(int /*zone*/)
{
    UpdateLEDs();
}

void RGBController_PoseidonZRGB::UpdateSingleLED(int /*led*/)
{
    UpdateLEDs();
}

void RGBController_PoseidonZRGB::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_PoseidonZRGB::UpdateMode()
{
    poseidon->SetMode(modes[active_mode].value, modes[active_mode].direction, modes[active_mode].speed);
}
