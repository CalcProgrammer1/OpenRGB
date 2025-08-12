/*---------------------------------------------------------*\
| RGBController_LenovoMotherboard.cpp                       |
|                                                           |
|   RGBController for Lenovo motherboard                    |
|                                                           |
|   Morgan Guimard (morg)                       26 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_LenovoMotherboard.h"

/**------------------------------------------------------------------*\
    @name LenovoMotherboard mouse
    @category Motherboard
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectLenovoMotherboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_LenovoMotherboard::RGBController_LenovoMotherboard(LenovoMotherboardController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "Lenovo";
    type                        = DEVICE_TYPE_MOTHERBOARD;
    description                 = name;
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Static;
    Static.name                 = "Static";
    Static.value                = LENOVO_MB_STATIC_MODE;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode           = MODE_COLORS_PER_LED;
    Static.brightness_min       = LENOVO_MB_BRIGHTNESS_MIN;
    Static.brightness_max       = LENOVO_MB_BRIGHTNESS_MAX;
    Static.brightness           = LENOVO_MB_BRIGHTNESS_MAX;
    Static.speed                = LENOVO_MB_SPEED_MIN;
    modes.push_back(Static);

    mode Sparkle;
    Sparkle.name                = "Sparkle";
    Sparkle.value               = LENOVO_MB_SPARKLE_MODE;
    Sparkle.flags               = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Sparkle.color_mode          = MODE_COLORS_PER_LED;
    Sparkle.brightness_min      = LENOVO_MB_BRIGHTNESS_MIN;
    Sparkle.brightness_max      = LENOVO_MB_BRIGHTNESS_MAX;
    Sparkle.brightness          = LENOVO_MB_BRIGHTNESS_MAX;
    Sparkle.speed               = LENOVO_MB_SPEED_MIN;
    Sparkle.speed_max           = LENOVO_MB_SPEED_MAX;
    Sparkle.speed_min           = LENOVO_MB_SPEED_MIN;
    modes.push_back(Sparkle);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = LENOVO_MB_BREATHING_MODE;
    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    Breathing.brightness_min    = LENOVO_MB_BRIGHTNESS_MIN;
    Breathing.brightness_max    = LENOVO_MB_BRIGHTNESS_MAX;
    Breathing.brightness        = LENOVO_MB_BRIGHTNESS_MAX;
    Breathing.speed             = LENOVO_MB_SPEED_MIN;
    Breathing.speed_max         = LENOVO_MB_SPEED_MAX;
    Breathing.speed_min         = LENOVO_MB_SPEED_MIN;
    modes.push_back(Breathing);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = LENOVO_MB_WAVE_MODE;
    Wave.flags                  = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.color_mode             = MODE_COLORS_NONE;
    Wave.brightness_min         = LENOVO_MB_BRIGHTNESS_MIN;
    Wave.brightness_max         = LENOVO_MB_BRIGHTNESS_MAX;
    Wave.brightness             = LENOVO_MB_BRIGHTNESS_MAX;
    Wave.speed                  = LENOVO_MB_SPEED_MIN;
    Wave.speed_max              = LENOVO_MB_SPEED_MAX;
    Wave.speed_min              = LENOVO_MB_SPEED_MIN;
    modes.push_back(Wave);

    mode Specter;
    Specter.name                = "Specter";
    Specter.value               = LENOVO_MB_SPECTER_MODE;
    Specter.flags               = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Specter.color_mode          = MODE_COLORS_PER_LED;
    Specter.brightness_min      = LENOVO_MB_BRIGHTNESS_MIN;
    Specter.brightness_max      = LENOVO_MB_BRIGHTNESS_MAX;
    Specter.brightness          = LENOVO_MB_BRIGHTNESS_MAX;
    Specter.speed               = LENOVO_MB_SPEED_MIN;
    Specter.speed_max           = LENOVO_MB_SPEED_MAX;
    Specter.speed_min           = LENOVO_MB_SPEED_MIN;
    modes.push_back(Specter);

    mode Rainbow;
    Rainbow.name                = "Rainbow wave";
    Rainbow.value               = LENOVO_MB_RAINBOW_WAVE_MODE;
    Rainbow.flags               = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.brightness_min      = LENOVO_MB_BRIGHTNESS_MIN;
    Rainbow.brightness_max      = LENOVO_MB_BRIGHTNESS_MAX;
    Rainbow.brightness          = LENOVO_MB_BRIGHTNESS_MAX;
    Rainbow.speed               = LENOVO_MB_SPEED_MIN;
    Rainbow.speed_max           = LENOVO_MB_SPEED_MAX;
    Rainbow.speed_min           = LENOVO_MB_SPEED_MIN;
    modes.push_back(Rainbow);

    mode Random;
    Random.name                 = "Random";
    Random.value                = LENOVO_MB_RANDOM_MODE;
    Random.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Random.color_mode           = MODE_COLORS_NONE;
    Random.brightness_min       = LENOVO_MB_BRIGHTNESS_MIN;
    Random.brightness_max       = LENOVO_MB_BRIGHTNESS_MAX;
    Random.brightness           = LENOVO_MB_BRIGHTNESS_MAX;
    Random.speed                = LENOVO_MB_SPEED_MIN;
    Random.speed_max            = LENOVO_MB_SPEED_MAX;
    Random.speed_min            = LENOVO_MB_SPEED_MIN;
    modes.push_back(Random);

    SetupZones();
}

RGBController_LenovoMotherboard::~RGBController_LenovoMotherboard()
{
    delete controller;
}

void RGBController_LenovoMotherboard::SetupZones()
{
    zone cpu_fan_zone;

    cpu_fan_zone.name       = "CPU FAN Zone";
    cpu_fan_zone.type       = ZONE_TYPE_SINGLE;
    cpu_fan_zone.leds_min   = 1;
    cpu_fan_zone.leds_max   = 1;
    cpu_fan_zone.leds_count = 1;
    cpu_fan_zone.matrix_map = nullptr;

    zones.emplace_back(cpu_fan_zone);

    zone rear_fan_zone;

    rear_fan_zone.name       = "Rear FAN Zone";
    rear_fan_zone.type       = ZONE_TYPE_SINGLE;
    rear_fan_zone.leds_min   = 1;
    rear_fan_zone.leds_max   = 1;
    rear_fan_zone.leds_count = 1;
    rear_fan_zone.matrix_map = nullptr;

    zones.emplace_back(rear_fan_zone);

    leds.resize(LENOVO_MB_NUMBER_OF_LEDS);

    leds[0].name  = "LED 1";
    leds[0].value = LENOVO_MB_ZONE_1_VALUE;

    leds[1].name  = "LED 2";
    leds[1].value = LENOVO_MB_ZONE_2_VALUE;

    SetupColors();
}

void RGBController_LenovoMotherboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LenovoMotherboard::DeviceUpdateLEDs()
{
    for(uint8_t i = 0; i < leds.size(); i++)
    {
        controller->SetMode(
                    leds[i].value,
                    modes[active_mode].value,
                    modes[active_mode].brightness,
                    modes[active_mode].speed,
                    colors[i]
                    );
    }
}

void RGBController_LenovoMotherboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LenovoMotherboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LenovoMotherboard::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
