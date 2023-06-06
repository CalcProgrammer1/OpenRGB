/*-------------------------------------*\
|  RGBController_WushiL50USB.cpp        |
|                                       |
|  interface for Wushi L50 Devices      |
\*-------------------------------------*/

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
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness_min   = 1;
    Direct.brightness_max   = 2;
    modes.push_back(Direct);

    mode Breath;
    Breath.name             = "Breathing";
    Breath.flags            = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED;//MODE_FLAG_HAS_PER_LED_COLOR
    Breath.color_mode       = MODE_COLORS_RANDOM;//;MODE_COLORS_PER_LED;//
    Breath.brightness_min   = 1;
    Breath.brightness_max   = 2;
    Breath.speed_min        = 1;
    Breath.speed_max        = 4;
    modes.push_back(Breath);

    mode Wave;
    Wave.name               = "Rainbow Wave";
    Wave.flags              = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.color_mode         = MODE_COLORS_RANDOM;
    Wave.speed_min          = 1;
    Wave.speed_max          = 4;
    modes.push_back(Wave);

    mode Smooth;
    Smooth.name             = "Spectrum Cycle";
    Smooth.flags            = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Smooth.color_mode       = MODE_COLORS_RANDOM;
    Smooth.brightness_min   = 1;
    Smooth.brightness_max   = 2;
    Smooth.speed_min        = 1;
    Smooth.speed_max        = 4;
    modes.push_back(Smooth);

    mode Race;
    Race.name               = "Race Cycle";
    Race.flags              = MODE_FLAG_HAS_RANDOM_COLOR |MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_SPEED;
    Race.color_mode         = MODE_COLORS_RANDOM;
    Race.brightness_min     = 1;
    Race.brightness_max     = 2;
    Race.speed_min          = 1;
    Race.speed_max          = 4;
    modes.push_back(Race);

    mode Stack;
    Stack.name              = "Stacking";
    Stack.flags             = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Stack.color_mode        = MODE_COLORS_RANDOM;
    Stack.brightness_min    = 1;
    Stack.brightness_max    = 2;
    Stack.speed_min         = 1;
    Stack.speed_max         = 4;
    modes.push_back(Stack);

    mode Off;
    Off.name                = "Off";
    Off.flags               = MODE_FLAG_HAS_RANDOM_COLOR;
    Off.color_mode          = MODE_COLORS_RANDOM;
    modes.push_back(Off);

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
    new_zone.type       = ZONE_TYPE_LINEAR;
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
    state.SetColors(colors);
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
    state.SetColors(colors);

    switch(active_mode)
    {
        case 0:
            state.effect        = WUSHI_L50_EFFECT_STATIC;
            break;

        case 1:
            state.effect        = WUSHI_L50_EFFECT_BREATH;
            break;

        case 2:
            state.effect        = WUSHI_L50_EFFECT_WAVE;
            state.wave_ltr      = modes[active_mode].direction ? 0 : 1;
            state.wave_rtl      = modes[active_mode].direction ? 1 : 0;
            break;

        case 3:
            state.effect        = WUSHI_L50_EFFECT_SMOOTH;
            break;

        case 4:
            state.effect        = WUSHI_L50_EFFECT_RACE;
            state.wave_ltr      = modes[active_mode].direction ? 0 : 1;
            state.wave_rtl      = modes[active_mode].direction ? 1 : 0;
            break;

        case 5:
            state.effect        = WUSHI_L50_EFFECT_STACK;
            state.wave_ltr      = modes[active_mode].direction ? 0 : 1;
            state.wave_rtl      = modes[active_mode].direction ? 1 : 0;
            break;

        case 6:
            state.effect        = WUSHI_L50_EFFECT_STATIC;
            state.zone0_rgb[0]  = 0;
            state.zone0_rgb[1]  = 0;
            state.zone0_rgb[2]  = 0;
            state.zone1_rgb[0]  = 0;
            state.zone1_rgb[1]  = 0;
            state.zone1_rgb[2]  = 0;
            state.zone2_rgb[0]  = 0;
            state.zone2_rgb[1]  = 0;
            state.zone2_rgb[2]  = 0;
            state.zone3_rgb[0]  = 0;
            state.zone3_rgb[1]  = 0;
            state.zone3_rgb[2]  = 0;
            break;
    }

    /*---------------------------------------------------------*\
    | Mode number from 0, but in mode from 1                    |
    \*---------------------------------------------------------*/
    if(active_mode != (WUSHI_L50_EFFECT_STATIC - 1))
    {
        state.speed             = modes[active_mode].speed;
    }

    state.brightness            = modes[active_mode].brightness;

    controller->setMode(&state);
}

void RGBController_WushiL50USB::DeviceSaveMode()
{
    /*---------------------------------------------------------*\
    | This device does not support saving or multiple modes     |
    \*---------------------------------------------------------*/
}
