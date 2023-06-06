#include "WushiL50USBController.h"
#include "WushiDevicesL50.h"
#include "RGBController_WushiL50USB.h"
#include "LogManager.h"

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <iomanip>

#define WUSHI_L50_NUM_LEDS 4

RGBController_WushiL50USB::RGBController_WushiL50USB(WushiL50USBController* controller_ptr)
{
    controller        = controller_ptr;

    name    = controller->getName();
    type    = DEVICE_TYPE_KEYBOARD;
    vendor  = "Wushi";

    location = controller->getLocation();
    version  = controller->GetFirmwareVersion();
    serial   = controller->GetSerialString();

    description = "Wushi L50 device";

    mode Direct;
    Direct.name       = "Direct";
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode = MODE_COLORS_PER_LED;
    Direct.brightness_min = 1;
    Direct.brightness_max = 2;

    modes.push_back(Direct);

    mode Breath;
    Breath.name       = "Breathing";
    Breath.flags      =  MODE_FLAG_HAS_RANDOM_COLOR| MODE_FLAG_HAS_SPEED;//MODE_FLAG_HAS_PER_LED_COLOR
    Breath.color_mode = MODE_COLORS_RANDOM;//;MODE_COLORS_PER_LED;//
    Breath.brightness_min = 1;
    Breath.brightness_max = 2;
    Breath.speed_min      = 1;
    Breath.speed_max      = 4;

    modes.push_back(Breath);

    mode Wave;
    Wave.name       = "Rainbow Wave";
    Wave.flags      = MODE_FLAG_HAS_RANDOM_COLOR  |MODE_FLAG_HAS_SPEED |MODE_FLAG_HAS_DIRECTION_LR;
    Wave.color_mode = MODE_COLORS_RANDOM;
    Wave.speed_min      = 1;
    Wave.speed_max      = 4;
    Wave.direction = MODE_DIRECTION_LEFT | MODE_DIRECTION_RIGHT;
    modes.push_back(Wave);

    mode Smooth;
    Smooth.name       = "Spectrum Cycle";
    Smooth.flags      = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Smooth.color_mode = MODE_COLORS_RANDOM;
    Smooth.brightness_min = 1;
    Smooth.brightness_max = 2;
    Smooth.speed_min      = 1;
    Smooth.speed_max      = 4;
    modes.push_back(Smooth);  //添加mode列表

    mode Race;
    Race.name       = "Race Cycle";
    Race.flags      = MODE_FLAG_HAS_RANDOM_COLOR |MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_SPEED;
    Race.color_mode = MODE_COLORS_RANDOM;
    Race.brightness_min = 1;
    Race.brightness_max = 2;
    Race.speed_min      = 1;
    Race.speed_max      = 4;
    Race.direction = MODE_DIRECTION_LEFT | MODE_DIRECTION_RIGHT;
    modes.push_back(Race);  //添加mode列表

    mode Stack;
    Stack.name       = "Stacking";
    Stack.flags      = MODE_FLAG_HAS_RANDOM_COLOR |MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_SPEED| MODE_FLAG_HAS_BRIGHTNESS ;
    Stack.color_mode = MODE_COLORS_RANDOM;
    Stack.brightness_min = 1;
    Stack.brightness_max = 2;
    Stack.speed_min      = 1;
    Stack.direction = MODE_DIRECTION_LEFT | MODE_DIRECTION_RIGHT;
    Stack.speed_max      = 4;
    modes.push_back(Stack);  //添加mode列表

    mode Off;
    Off.name     = "Off";
    Off.flags      = MODE_FLAG_HAS_RANDOM_COLOR;
    Off.color_mode = MODE_COLORS_RANDOM;
    modes.push_back(Off);  //添加mode列表

    SetupZones();

    // Reset colors to white
    for(unsigned int led_idx = 0; led_idx < WUSHI_L50_NUM_LEDS; led_idx++ )
    {
        colors[led_idx] = 0xFFFFFFFF;
    }
}

RGBController_WushiL50USB::~RGBController_WushiL50USB()
{
    controller->setDeviceHardwareMode();

    delete controller;
}

void RGBController_WushiL50USB::SetupZones()
{
    zone new_zone;
    new_zone.name       = WUSHI_L50.name;
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_count = WUSHI_L50_NUM_LEDS;
    new_zone.leds_max   = new_zone.leds_count;
    new_zone.leds_min   = new_zone.leds_count;

    new_zone.matrix_map = NULL;


    zones.push_back(new_zone);

    for(unsigned int led_idx = 0; led_idx < WUSHI_L50_NUM_LEDS; led_idx++ )
    {
        led new_led; //添加LEDZONE列表
        new_led.name  = WUSHI_L50_leds[led_idx].name;
        new_led.value = WUSHI_L50_leds[led_idx].led_num;
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

void RGBController_WushiL50USB::UpdateSingleLED(int /*led*/)
{
}

void RGBController_WushiL50USB::UpdateZoneLEDs(int /*zone*/)
{
}

void RGBController_WushiL50USB::DeviceUpdateLEDs()
{
    state.SetColors(colors);
    controller->setMode(state);
}

void RGBController_WushiL50USB::DeviceUpdateMode()
{
    state.Reset();
    state.SetColors(colors);

    switch (active_mode)
    {
    case 0:
        state.effect = WUSHI_L50_EFFECT_STATIC;
        break;
    case 1:
        state.effect = WUSHI_L50_EFFECT_BREATH;
        break;
    case 2:
        state.effect = WUSHI_L50_EFFECT_WAVE;
        state.wave_ltr = modes[active_mode].direction?0:1;
        state.wave_rtl = modes[active_mode].direction?1:0;
        break;
    case 3:
        state.effect = WUSHI_L50_EFFECT_SMOOTH;
        break;
    case 4:
        state.effect = WUSHI_L50_EFFECT_RACE;
        state.wave_ltr = modes[active_mode].direction?0:1;
        state.wave_rtl = modes[active_mode].direction?1:0;
        break;
    case 5:
        state.effect = WUSHI_L50_EFFECT_STACK;
        state.wave_ltr = modes[active_mode].direction?0:1;
        state.wave_rtl = modes[active_mode].direction?1:0;
        break;
    case 6:
        state.effect = WUSHI_L50_EFFECT_STATIC;
        state.zone0_rgb[0] = 0;state.zone0_rgb[1] = 0;state.zone0_rgb[2] = 0;
        state.zone1_rgb[0] = 0;state.zone1_rgb[1] = 0;state.zone1_rgb[2] = 0;
        state.zone2_rgb[0] = 0;state.zone2_rgb[1] = 0;state.zone2_rgb[2] = 0;
        state.zone3_rgb[0] = 0;state.zone3_rgb[1] = 0;state.zone3_rgb[2] = 0;
        break;
    }

    if(active_mode != (WUSHI_L50_EFFECT_STATIC - 1)) // mode number from 0, but in mode from 1
    {
        state.speed = modes[active_mode].speed;
    }
    state.brightness = modes[active_mode].brightness;

    controller->setMode(state);
}

void RGBController_WushiL50USB::DeviceSaveMode()
{
    /*---------------------------------------------------------*\
    | This device does not support saving or multiple modes     |
    \*---------------------------------------------------------*/
}
