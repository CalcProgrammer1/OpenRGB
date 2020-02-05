/*-----------------------------------------*\
|  RGBController_HyperXKeyboard.cpp         |
|                                           |
|  Generic RGB Interface for HyperX RGB     |
|  Keyboard                                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/2/2020    |
\*-----------------------------------------*/

#include "RGBController_HyperXKeyboard.h"

static const char* zone_names[] =
{
    "Keyboard",
    "RGB Strip",
    "Media Keys"
};

static const unsigned int zone_sizes[] =
{
    104,
    18,
    4
};

static const char* led_names[] =
{
    "Key: Escape",
    "Key: `",
    "Key: Tab",
    "Key: Caps Lock",
    "Key: Left Shift",
    "Key: Left Control",
    "Key: F12",
    "Key: =",
    "Key: F9",
    "Key: 9",
    "Key: O",
    "Key: L",
    "Key: ,",
    "Key: Context",
    "Key: Left Arrow",
    "Key: F1",
    "Key: 1",
    "Key: Q",
    "Key: A",
    "Key: Left Windows",
    "Key: Print Screen",
    "Key: F10",
    "Key: 0",
    "Key: P",
    "Key: ;",
    "Key: .",
    "Key: Enter",
    "Key: Down Arrow",
    "Key: F2",
    "Key: 2",
    "Key: W",
    "Key: S",
    "Key: Z",
    "Key: Left Alt",
    "Key: Scroll Lock",
    "Key: Backspace",
    "Key: F11",
    "Key: -",
    "Key: [",
    "Key: '",
    "Key: /",
    "Key: Right Arrow",
    "Key: F3",
    "Key: 3",
    "Key: E",
    "Key: D",
    "Key: X",
    "Key: Pause/Break",
    "Key: Delete",
    "Key: Number Pad 7",
    "Key: Num Lock",
    "Key: Number Pad 6",
    "Key: F4",
    "Key: 4",
    "Key: R",
    "Key: F",
    "Key: C",
    "Key: Space",
    "Key: Insert",
    "Key: End",
    "Key: Number Pad 8",
    "Key: Number Pad /",
    "Key: Number Pad 1",
    "Key: F5",
    "Key: 5",
    "Key: T",
    "Key: G",
    "Key: V",
    "Key: Home",
    "Key: Page Down",
    "Key: Number Pad 9",
    "Key: Number Pad *",
    "Key: Number Pad 2",
    "Key: F6",
    "Key: 6",
    "Key: Y",
    "Key: H",
    "Key: B",
    "Key: Page Up",
    "Key: Right Shift",
    "Key: Number Pad -",
    "Key: Number Pad 3",
    "Key: F7",
    "Key: 7",
    "Key: U",
    "Key: J",
    "Key: N",
    "Key: Right Alt",
    "Key: ]",
    "Key: Right Control",
    "Key: Number Pad 4",
    "Key: Number Pad +",
    "Key: Number Pad 0",
    "Key: F8",
    "Key: 8",
    "Key: I",
    "Key: K",
    "Key: M",
    "Key: Right Windows",
    "Key: \\",
    "Key: Up Arrow",
    "Key: Number Pad 5",
    "Key: Number Pad Enter",
    "Key: Number Pad .",
    "RGB Strip 1",
    "RGB Strip 2",
    "RGB Strip 3",
    "RGB Strip 4",
    "RGB Strip 5",
    "RGB Strip 6",
    "RGB Strip 7",
    "RGB Strip 8",
    "RGB Strip 9",
    "RGB Strip 10",
    "RGB Strip 11",
    "RGB Strip 12",
    "RGB Strip 13",
    "RGB Strip 14",
    "RGB Strip 15",
    "RGB Strip 16",
    "RGB Strip 17",
    "RGB Strip 18",
    "Media Previous",
    "Media Play/Pause",
    "Media Next",
    "Media Mute"
};

RGBController_HyperXKeyboard::RGBController_HyperXKeyboard(HyperXKeyboardController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name = "HyperX RGB Keyboard";
    type = DEVICE_TYPE_KEYBOARD;

    mode Direct;
    Direct.name   = "Direct";
    Direct.value  = HYPERX_MODE_STATIC;
    Direct.flags  = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    Direct.random = false;
    modes.push_back(Direct);

    mode Static;
    Static.name   = "Static";
    Static.value  = HYPERX_MODE_STATIC;
    Static.flags  = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    Static.random = false;
    modes.push_back(Static);

    mode Wave;
    Wave.name      = "Wave";
    Wave.value     = HYPERX_MODE_WAVE;
    Wave.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.speed_min = 0x00;
    Wave.speed_max = 0x09;
    Wave.random    = false;
    Wave.speed     = 0x09;
    Wave.direction = MODE_DIRECTION_LEFT;
    modes.push_back(Wave);

    mode Breathing;
    Breathing.name = "Breathing";
    Breathing.value = HYPERX_MODE_BREATHING;
    Breathing.flags = MODE_FLAG_HAS_SPEED;
    Breathing.speed_min = 0x00;
    Breathing.speed_max = 0x09;
    Breathing.random = false;
    Breathing.speed = 0x09;
    modes.push_back(Breathing);

    colors.resize(126);

    unsigned int led_idx = 0;
    for(unsigned int zone_idx = 0; zone_idx < 3; zone_idx++)
    {
        zone new_zone;
        new_zone.name.append(zone_names[zone_idx]);

        std::vector<int> new_zone_map;

        for(unsigned int led_count = 0; led_count < zone_sizes[zone_idx]; led_count++)
        {
            led new_led;
            new_led.name.append(led_names[led_idx]);
            leds.push_back(new_led);

            new_zone_map.push_back(led_idx);

            led_idx++;
        }

        new_zone.map.push_back(new_zone_map);

        zones.push_back(new_zone);
    }
    for(int i = 0; i < 126; i++)
    {

    }
}

RGBController_HyperXKeyboard::~RGBController_HyperXKeyboard()
{

}

void RGBController_HyperXKeyboard::UpdateLEDs()
{
    if(active_mode == 0)
    {
        hyperx->SetLEDsDirect(colors);
    }
    else
    {
        hyperx->SetLEDs(colors);
    }
}

void RGBController_HyperXKeyboard::UpdateZoneLEDs(int zone)
{
    UpdateLEDs();
}

void RGBController_HyperXKeyboard::UpdateSingleLED(int led)
{
    UpdateLEDs();
}

void RGBController_HyperXKeyboard::SetCustomMode()
{
    SetMode(0);
}

void RGBController_HyperXKeyboard::UpdateMode()
{
    hyperx->SetMode(modes[active_mode].value, modes[active_mode].direction, modes[active_mode].speed);
}
