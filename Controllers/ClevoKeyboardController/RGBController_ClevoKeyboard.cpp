/*---------------------------------------------------------*\
| RGBController_ClevoKeyboard.cpp                           |
|                                                           |
|   RGBController for Clevo per-key RGB keyboard            |
|   Supports ITE 8291 (PID 0x600B) and ITE 829x (0x8910)    |
|                                                           |
|   Kyle Cascade (kyle@cascade.family)          16 Jan 2026 |
|   Valentin Lobstein (balgogan@protonmail.com) 27 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "RGBController_ClevoKeyboard.h"
#include "KeyboardLayoutManager.h"

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

RGBController_ClevoKeyboard::RGBController_ClevoKeyboard(ClevoKeyboardController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = "CLEVO Keyboard";
    vendor                          = "CLEVO Computers";
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = "CLEVO Laptop Keyboard";
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();
    version                         = controller->GetFirmwareVersion();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = CLEVO_KEYBOARD_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min           = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
    Direct.brightness_max           = controller->GetBrightnessMax();
    Direct.brightness               = controller->GetBrightnessMax();
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    /*---------------------------------------------------------*\
    | ITE 8291 built-in effects (not available on ITE 829x)     |
    \*---------------------------------------------------------*/
    if(controller->GetType() == CLEVO_KB_ITE8291)
    {
        mode Rainbow;
        Rainbow.name                    = "Rainbow";
        Rainbow.value                   = CLEVO_KEYBOARD_MODE_RAINBOW;
        Rainbow.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
        Rainbow.speed_min               = CLEVO_KEYBOARD_SPEED_MAX;
        Rainbow.speed_max               = CLEVO_KEYBOARD_SPEED_MIN;
        Rainbow.speed                   = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Rainbow.brightness_min          = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Rainbow.brightness_max          = controller->GetBrightnessMax();
        Rainbow.brightness              = controller->GetBrightnessMax();
        Rainbow.color_mode              = MODE_COLORS_NONE;
        modes.push_back(Rainbow);

        mode Wave;
        Wave.name                       = "Wave";
        Wave.value                      = CLEVO_KEYBOARD_MODE_WAVE;
        Wave.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
        Wave.speed_min                  = CLEVO_KEYBOARD_SPEED_MAX;
        Wave.speed_max                  = CLEVO_KEYBOARD_SPEED_MIN;
        Wave.speed                      = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Wave.brightness_min             = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Wave.brightness_max             = controller->GetBrightnessMax();
        Wave.brightness                 = controller->GetBrightnessMax();
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
        Breathing.speed                 = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Breathing.brightness_min        = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Breathing.brightness_max        = controller->GetBrightnessMax();
        Breathing.brightness            = controller->GetBrightnessMax();
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
        Reactive.speed                  = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Reactive.brightness_min         = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Reactive.brightness_max         = controller->GetBrightnessMax();
        Reactive.brightness             = controller->GetBrightnessMax();
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
        Ripple.speed                    = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Ripple.brightness_min           = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Ripple.brightness_max           = controller->GetBrightnessMax();
        Ripple.brightness               = controller->GetBrightnessMax();
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
        Marquee.speed                   = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Marquee.brightness_min          = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Marquee.brightness_max          = controller->GetBrightnessMax();
        Marquee.brightness              = controller->GetBrightnessMax();
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
        Raindrop.speed                  = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Raindrop.brightness_min         = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Raindrop.brightness_max         = controller->GetBrightnessMax();
        Raindrop.brightness             = controller->GetBrightnessMax();
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
        Aurora.speed                    = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Aurora.brightness_min           = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Aurora.brightness_max           = controller->GetBrightnessMax();
        Aurora.brightness               = controller->GetBrightnessMax();
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
        Spark.speed                     = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Spark.brightness_min            = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Spark.brightness_max            = controller->GetBrightnessMax();
        Spark.brightness                = controller->GetBrightnessMax();
        Spark.colors_min                = 1;
        Spark.colors_max                = 1;
        Spark.colors.resize(1);
        Spark.color_mode                = MODE_COLORS_MODE_SPECIFIC;
        modes.push_back(Spark);
    }

    /*---------------------------------------------------------*\
    | ITE 829x firmware effects                                 |
    \*---------------------------------------------------------*/
    if(controller->GetType() == CLEVO_KB_ITE829X)
    {
        unsigned char bmax = controller->GetBrightnessMax();

        mode Wave;
        Wave.name           = "Wave";
        Wave.value          = CLEVO_829X_MODE_WAVE;
        Wave.flags          = MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_DIRECTION_DIAG;
        Wave.speed_min      = CLEVO_KEYBOARD_SPEED_MIN;
        Wave.speed_max      = CLEVO_KEYBOARD_SPEED_MAX;
        Wave.speed          = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Wave.brightness_min = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Wave.brightness_max = bmax;
        Wave.brightness     = bmax;
        Wave.direction      = MODE_DIRECTION_LEFT;
        Wave.colors_min     = 1;
        Wave.colors_max     = 1;
        Wave.colors.resize(1, ToRGBColor(255, 0, 0));
        Wave.color_mode     = MODE_COLORS_MODE_SPECIFIC;
        modes.push_back(Wave);

        mode Breathing;
        Breathing.name           = "Breathing";
        Breathing.value          = CLEVO_829X_MODE_BREATH;
        Breathing.flags          = MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Breathing.speed_min      = CLEVO_KEYBOARD_SPEED_MIN;
        Breathing.speed_max      = CLEVO_KEYBOARD_SPEED_MAX;
        Breathing.speed          = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Breathing.brightness_min = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Breathing.brightness_max = bmax;
        Breathing.brightness     = bmax;
        Breathing.colors_min     = 1;
        Breathing.colors_max     = 1;
        Breathing.colors.resize(1, ToRGBColor(255, 0, 0));
        Breathing.color_mode     = MODE_COLORS_MODE_SPECIFIC;
        modes.push_back(Breathing);

        mode Flashing;
        Flashing.name           = "Flashing";
        Flashing.value          = CLEVO_829X_MODE_BLINK;
        Flashing.flags          = MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Flashing.speed_min      = CLEVO_KEYBOARD_SPEED_MIN;
        Flashing.speed_max      = CLEVO_KEYBOARD_SPEED_MAX;
        Flashing.speed          = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Flashing.brightness_min = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Flashing.brightness_max = bmax;
        Flashing.brightness     = bmax;
        Flashing.colors_min     = 1;
        Flashing.colors_max     = 1;
        Flashing.colors.resize(1, ToRGBColor(255, 0, 0));
        Flashing.color_mode     = MODE_COLORS_MODE_SPECIFIC;
        modes.push_back(Flashing);

        mode Random;
        Random.name           = "Random";
        Random.value          = CLEVO_829X_MODE_RANDOM;
        Random.flags          = MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Random.speed_min      = CLEVO_KEYBOARD_SPEED_MIN;
        Random.speed_max      = CLEVO_KEYBOARD_SPEED_MAX;
        Random.speed          = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Random.brightness_min = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Random.brightness_max = bmax;
        Random.brightness     = bmax;
        Random.colors_min     = 1;
        Random.colors_max     = 1;
        Random.colors.resize(1, ToRGBColor(255, 0, 0));
        Random.color_mode     = MODE_COLORS_MODE_SPECIFIC;
        modes.push_back(Random);

        mode Scan;
        Scan.name           = "Scan";
        Scan.value          = CLEVO_829X_MODE_SCAN;
        Scan.flags          = MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
        Scan.speed_min      = CLEVO_KEYBOARD_SPEED_MIN;
        Scan.speed_max      = CLEVO_KEYBOARD_SPEED_MAX;
        Scan.speed          = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Scan.brightness_min = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Scan.brightness_max = bmax;
        Scan.brightness     = bmax;
        Scan.colors_min     = 2;
        Scan.colors_max     = 2;
        Scan.colors.resize(2);
        Scan.colors[0] = ToRGBColor(255, 0, 0);
        Scan.colors[1] = ToRGBColor(0, 0, 255);
        Scan.color_mode     = MODE_COLORS_MODE_SPECIFIC;
        modes.push_back(Scan);

        mode Snake;
        Snake.name           = "Snake";
        Snake.value          = CLEVO_829X_MODE_SNAKE;
        Snake.flags          = MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_DIAG;
        Snake.speed_min      = CLEVO_KEYBOARD_SPEED_MIN;
        Snake.speed_max      = CLEVO_KEYBOARD_SPEED_MAX;
        Snake.speed          = CLEVO_KEYBOARD_SPEED_DEFAULT;
        Snake.brightness_min = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        Snake.brightness_max = bmax;
        Snake.brightness     = bmax;
        Snake.direction      = MODE_DIRECTION_UP_LEFT;
        Snake.colors_min     = 1;
        Snake.colors_max     = 1;
        Snake.colors.resize(1, ToRGBColor(255, 0, 0));
        Snake.color_mode     = MODE_COLORS_MODE_SPECIFIC;
        modes.push_back(Snake);

        mode RainbowCycle;
        RainbowCycle.name           = "Spectrum Cycle";
        RainbowCycle.value          = CLEVO_829X_MODE_RAINBOW_CYCLE;
        RainbowCycle.flags          = MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
        RainbowCycle.speed_min      = CLEVO_KEYBOARD_SPEED_MIN;
        RainbowCycle.speed_max      = CLEVO_KEYBOARD_SPEED_MAX;
        RainbowCycle.speed          = CLEVO_KEYBOARD_SPEED_DEFAULT;
        RainbowCycle.brightness_min = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        RainbowCycle.brightness_max = bmax;
        RainbowCycle.brightness     = bmax;
        RainbowCycle.color_mode     = MODE_COLORS_NONE;
        modes.push_back(RainbowCycle);

        mode RainbowGrad;
        RainbowGrad.name           = "Rainbow Wave";
        RainbowGrad.value          = CLEVO_829X_MODE_RAINBOW_GRAD;
        RainbowGrad.flags          = MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_DIRECTION_DIAG;
        RainbowGrad.speed_min      = CLEVO_KEYBOARD_SPEED_MIN;
        RainbowGrad.speed_max      = CLEVO_KEYBOARD_SPEED_MAX;
        RainbowGrad.speed          = CLEVO_KEYBOARD_SPEED_DEFAULT;
        RainbowGrad.brightness_min = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        RainbowGrad.brightness_max = bmax;
        RainbowGrad.brightness     = bmax;
        RainbowGrad.direction      = MODE_DIRECTION_LEFT;
        RainbowGrad.color_mode     = MODE_COLORS_NONE;
        modes.push_back(RainbowGrad);

        mode FnColor;
        FnColor.name           = "Fn Highlight";
        FnColor.value          = CLEVO_829X_MODE_FN_COLOR;
        FnColor.flags          = MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
        FnColor.brightness_min = CLEVO_KEYBOARD_BRIGHTNESS_MIN;
        FnColor.brightness_max = bmax;
        FnColor.brightness     = bmax;
        FnColor.colors_min     = 1;
        FnColor.colors_max     = 1;
        FnColor.colors.resize(1, ToRGBColor(255, 0, 0));
        FnColor.color_mode     = MODE_COLORS_MODE_SPECIFIC;
        modes.push_back(FnColor);
    }

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
    Shutdown();

    delete controller;
}

void RGBController_ClevoKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Select layout based on device type                        |
    \*---------------------------------------------------------*/
    bool is_829x = (controller->GetType() == CLEVO_KB_ITE829X);

    keyboard_keymap_overlay_values& layout = is_829x
        ? clevo_829x_keyboard_layout
        : clevo_keyboard_layout;

    KeyboardLayoutManager new_kb(KEYBOARD_LAYOUT_ISO_QWERTY,
                                 layout.base_size,
                                 layout.key_values);

    new_kb.ChangeKeys(layout);

    /*---------------------------------------------------------*\
    | Create keyboard zone from KLM                             |
    \*---------------------------------------------------------*/
    zone keyboard_zone;

    keyboard_zone.name              = ZONE_EN_KEYBOARD;
    keyboard_zone.type              = ZONE_TYPE_MATRIX;
    keyboard_zone.leds_count        = new_kb.GetKeyCount();
    keyboard_zone.leds_min          = keyboard_zone.leds_count;
    keyboard_zone.leds_max          = keyboard_zone.leds_count;

    /*---------------------------------------------------------*\
    | Set up the matrix map using KLM dimensions                |
    \*---------------------------------------------------------*/
    keyboard_zone.matrix_map        = new_kb.GetKeyMap(KEYBOARD_MAP_FILL_TYPE_COUNT);

    zones.push_back(keyboard_zone);

    for(unsigned int led_idx = 0; led_idx < keyboard_zone.leds_count; led_idx++)
    {
        led new_led;

        new_led.name    = new_kb.GetKeyNameAt(led_idx);
        new_led.value   = new_kb.GetKeyValueAt(led_idx);

        leds.push_back(new_led);
    }

    SetupColors();

    /*---------------------------------------------------------*\
    | Build buffer map from LED values to color pointers        |
    | 829x LED IDs have gaps (stride 32), need larger map       |
    \*---------------------------------------------------------*/
    null_color = 0x00000000;

    size_t map_size = is_829x
        ? CLEVO_829X_LED_ID(CLEVO_829X_NUM_ROWS - 1, CLEVO_829X_NUM_COLS - 1) + 1
        : (size_t)controller->GetNumLEDs();

    buffer_map.resize(map_size, &null_color);

    for(size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        if(leds[led_idx].value < buffer_map.size())
        {
            buffer_map[leds[led_idx].value] = &colors[led_idx];
        }
    }
}

void RGBController_ClevoKeyboard::DeviceUpdateLEDs()
{
    int  num_rows = controller->GetNumRows();
    int  num_cols = controller->GetNumCols();
    bool is_829x  = (controller->GetType() == CLEVO_KB_ITE829X);

    std::vector<unsigned char> color_data(num_rows * num_cols * 3);

    for(int row = 0; row < num_rows; row++)
    {
        for(int col = 0; col < num_cols; col++)
        {
            int seq    = row * num_cols + col;
            int led_id = is_829x ? CLEVO_829X_LED_ID(row, col) : seq;

            color_data[seq * 3 + 0] = RGBGetRValue(*buffer_map[led_id]);
            color_data[seq * 3 + 1] = RGBGetGValue(*buffer_map[led_id]);
            color_data[seq * 3 + 2] = RGBGetBValue(*buffer_map[led_id]);
        }
    }

    controller->SendColors(color_data.data(), modes[active_mode].brightness, modes[active_mode].speed);
}

void RGBController_ClevoKeyboard::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ClevoKeyboard::DeviceUpdateSingleLED(int /*led*/)
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
    | Handle ITE 829x firmware effects                          |
    \*---------------------------------------------------------*/
    if(controller->GetType() == CLEVO_KB_ITE829X && mode_value != CLEVO_KEYBOARD_MODE_DIRECT)
    {
        controller->SetEffect829x(mode_value, modes[active_mode].colors, modes[active_mode].direction, modes[active_mode].color_mode);
        controller->SetBrightnessSpeed829x(modes[active_mode].brightness, modes[active_mode].speed);
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
    unsigned char behaviour  = (mode_value == CLEVO_KEYBOARD_MODE_WAVE)
                             ? modes[active_mode].direction + 1
                             : 0x00;

    /*---------------------------------------------------------*\
    | Set mode color if applicable                              |
    \*---------------------------------------------------------*/
    if(modes[active_mode].colors.size() > 0)
    {
        controller->SetModeColor(1, modes[active_mode].colors[0]);
    }

    controller->SetMode(mode_value, brightness, speed, behaviour);
}

void RGBController_ClevoKeyboard::DeviceSaveMode()
{
    if(controller->GetType() == CLEVO_KB_ITE829X)
    {
        DeviceUpdateMode();
        controller->SaveToBios829x();
    }
}
