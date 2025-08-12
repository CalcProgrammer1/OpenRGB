/*---------------------------------------------------------*\
| RGBController_Lenovo4ZoneUSB.cpp                          |
|                                                           |
|   Device list for Lenovo 4-Zone devices                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "Lenovo4ZoneUSBController.h"
#include "LenovoDevices4Zone.h"
#include "RGBController_Lenovo4ZoneUSB.h"
#include "LogManager.h"

/**------------------------------------------------------------------*\
    @name Lenovo 4 Zone USB
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectLenovo4ZoneUSBControllers
    @comment Tested on Lenovo Legion 5 2021
\*-------------------------------------------------------------------*/


#define LENOVO_4_ZONE_NUM_LEDS 4

RGBController_Lenovo4ZoneUSB::RGBController_Lenovo4ZoneUSB(Lenovo4ZoneUSBController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->getName();
    type                    = DEVICE_TYPE_LAPTOP;
    vendor                  = "Lenovo";
    description             = "Lenovo 4-Zone device";

    mode Direct;
    Direct.name             = "Direct";
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness_min   = 1;
    Direct.brightness_max   = 2;

    modes.push_back(Direct);

    mode Breath;
    Breath.name             = "Breathing";
    Breath.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breath.color_mode       = MODE_COLORS_PER_LED;
    Breath.brightness_min   = 1;
    Breath.brightness_max   = 2;
    Breath.speed_min        = 1;
    Breath.speed_max        = 4;

    modes.push_back(Breath);

    mode Wave;
    Wave.name               = "Rainbow Wave";
    Wave.flags              = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.color_mode         = MODE_COLORS_RANDOM;
    Wave.brightness_min     = 1;
    Wave.brightness_max     = 2;
    Wave.speed_min          = 1;
    Wave.speed_max          = 4;
    Wave.direction          = MODE_DIRECTION_LEFT | MODE_DIRECTION_RIGHT;
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

    SetupZones();
}

RGBController_Lenovo4ZoneUSB::~RGBController_Lenovo4ZoneUSB()
{
    delete controller;
}

void RGBController_Lenovo4ZoneUSB::SetupZones()
{
    zone new_zone;
    new_zone.name       = ZONE_EN_KEYBOARD;
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_count = LENOVO_4_ZONE_NUM_LEDS;
    new_zone.leds_max   = new_zone.leds_count;
    new_zone.leds_min   = new_zone.leds_count;

    new_zone.matrix_map = NULL;


    zones.push_back(new_zone);

    for(unsigned int led_idx = 0; led_idx < LENOVO_4_ZONE_NUM_LEDS; led_idx++ )
    {
        led new_led;
        new_led.name  = lenovo_4_zone_leds[led_idx].name;
        new_led.value = lenovo_4_zone_leds[led_idx].led_num;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_Lenovo4ZoneUSB::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Lenovo4ZoneUSB::UpdateSingleLED(int /*led*/)
{
}

void RGBController_Lenovo4ZoneUSB::UpdateZoneLEDs(int /*zone*/)
{
}

void RGBController_Lenovo4ZoneUSB::DeviceUpdateLEDs()
{
    state.SetColors(colors);
    controller->setMode(state);
}

void RGBController_Lenovo4ZoneUSB::DeviceUpdateMode()
{
    state.Reset();
    state.SetColors(colors);

    switch (active_mode)
    {
    case 0:
        state.effect = LENOVO_4_ZONE_EFFECT_STATIC;
        break;
    case 1:
        state.effect = LENOVO_4_ZONE_EFFECT_BREATH;
        break;
    case 2:
        state.effect = LENOVO_4_ZONE_EFFECT_WAVE;
        state.wave_ltr = modes[active_mode].direction?0:1;
        state.wave_rtl = modes[active_mode].direction?1:0;
        break;
    case 3:
        state.effect = LENOVO_4_ZONE_EFFECT_SMOOTH;
        break;
    }

    if(active_mode != (LENOVO_4_ZONE_EFFECT_STATIC - 1)) // mode number from 0, but in mode from 1
    {
        state.speed = modes[active_mode].speed;
    }
    state.brightness = modes[active_mode].brightness;

    controller->setMode(state);
}

void RGBController_Lenovo4ZoneUSB::DeviceSaveMode()
{
    /*---------------------------------------------------------*\
    | This device does not support saving or multiple modes     |
    \*---------------------------------------------------------*/
}
