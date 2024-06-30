/*---------------------------------------------------------*\
| RGBController_WushiL50USB.cpp                             |
|                                                           |
|   RGBController for Wushi L50                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_WushiL50USB.h"

RGBController_WushiL50USB::RGBController_WushiL50USB(WushiL50USBController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->getName();
    type                    = DEVICE_TYPE_ACCESSORY;
    description             = "Wushi L50 device";
    vendor                  = "Wushi";
    location                = controller->getLocation();
    serial                  = controller->GetSerialString();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = WUSHI_L50_EFFECT_STATIC;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness_min   = 1;
    Direct.brightness_max   = 2;
    Direct.brightness       = 2;
    modes.push_back(Direct);

    mode Breath;
    Breath.name             = "Breathing";
    Breath.value            = WUSHI_L50_EFFECT_BREATH;
    Breath.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breath.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Breath.speed_min        = 1;
    Breath.speed_max        = 4;
    Breath.speed            = 3;
    Breath.colors_min       = 1;
    Breath.colors_max       = 1;
    Breath.colors.resize(1);
    modes.push_back(Breath);

    mode Wave;
    Wave.name               = "Rainbow Wave";
    Wave.value              = WUSHI_L50_EFFECT_WAVE;
    Wave.flags              = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_SPEED;
    Wave.color_mode         = MODE_COLORS_RANDOM;
    Wave.speed_min          = 1;
    Wave.speed_max          = 4;
    Wave.speed              = 3;
    modes.push_back(Wave);

    mode Smooth;
    Smooth.name             = "Spectrum Cycle";
    Smooth.value            = WUSHI_L50_EFFECT_SMOOTH;
    Smooth.flags            = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED;
    Smooth.color_mode       = MODE_COLORS_RANDOM;
    Smooth.speed_min        = 1;
    Smooth.speed_max        = 4;
    Smooth.speed            = 3;
    modes.push_back(Smooth);

    mode Race;
    Race.name               = "Race Cycle";
    Race.value              = WUSHI_L50_EFFECT_RACE;
    Race.flags              = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_SPEED;
    Race.color_mode         = MODE_COLORS_RANDOM;
    Race.speed_min          = 1;
    Race.speed_max          = 4;
    Race.speed              = 3;
    modes.push_back(Race);

    mode Stack;
    Stack.name              = "Stacking";
    Stack.value             = WUSHI_L50_EFFECT_STACK;
    Stack.flags             = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Stack.color_mode        = MODE_COLORS_RANDOM;
    Stack.brightness_min    = 1;
    Stack.brightness_max    = 2;
    Stack.brightness        = 2;
    Stack.speed_min         = 1;
    Stack.speed_max         = 4;
    Stack.speed             = 3;
    modes.push_back(Stack);

    SetupZones();
}

RGBController_WushiL50USB::~RGBController_WushiL50USB()
{
    delete controller;
}

void RGBController_WushiL50USB::SetupZones()
{
    zone new_zone;
    new_zone.name       = "Dock";
    new_zone.type       = ZONE_TYPE_SINGLE;
    new_zone.leds_count = WUSHI_L50_NUM_LEDS;
    new_zone.leds_max   = new_zone.leds_count;
    new_zone.leds_min   = new_zone.leds_count;
    new_zone.matrix_map = NULL;

    zones.push_back(new_zone);

    for(unsigned int led_idx = 0; led_idx < WUSHI_L50_NUM_LEDS; led_idx++ )
    {
        led new_led;
        new_led.name    = "Dock Zone ";
        new_led.name.append(std::to_string(led_idx + 1));

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_WushiL50USB::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_WushiL50USB::DeviceUpdateLEDs()
{
    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        state.SetColors(colors);
    }

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        state.zone0_rgb[0]      = RGBGetRValue(modes[active_mode].colors[0]);
        state.zone0_rgb[1]      = RGBGetGValue(modes[active_mode].colors[0]);
        state.zone0_rgb[2]      = RGBGetBValue(modes[active_mode].colors[0]);
    }

    controller->setMode(&state);
}

void RGBController_WushiL50USB::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WushiL50USB::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WushiL50USB::DeviceUpdateMode()
{
    state.Reset();

    state.effect = modes[active_mode].value;

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        state.SetColors(colors);
    }

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        state.zone0_rgb[0]      = RGBGetRValue(modes[active_mode].colors[0]);
        state.zone0_rgb[1]      = RGBGetGValue(modes[active_mode].colors[0]);
        state.zone0_rgb[2]      = RGBGetBValue(modes[active_mode].colors[0]);
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_DIRECTION_LR)
    {
        state.wave_ltr          = modes[active_mode].direction ? 0 : 1;
        state.wave_rtl          = modes[active_mode].direction ? 1 : 0;
    }
    if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
    {
        state.speed             = modes[active_mode].speed;
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        state.brightness        = modes[active_mode].brightness;
    }

    controller->setMode(&state);
}

void RGBController_WushiL50USB::DeviceSaveMode()
{
    /*---------------------------------------------------------*\
    | This device does not support saving or multiple modes     |
    \*---------------------------------------------------------*/
}
