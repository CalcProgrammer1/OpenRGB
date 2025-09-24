/*---------------------------------------------------------*\
| RGBController_SayoDevice.cpp                              |
|                                                           |
|   Controller for Sayo Devices                             |
|                                                           |
|   Richard Harris                              24 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_SayoDevice.h"
#include "SayoDeviceController.h"
#include "LogManager.h"

/**--------------------------------------------------------------------*\
    @name SayoDevice E1
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectSayoDeviceController
    @comment
\*---------------------------------------------------------------------*/

RGBController_SayoDevice::RGBController_SayoDevice(SayoDeviceController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "SayoDevice";
    description                     = "SayoDevice E1 Knob";
    type                            = DEVICE_TYPE_KEYBOARD;
    location                        = controller->GetDeviceLocation();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = SAYO_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    /*-----------------------------------------------------*\
    | Breathing - pulses a single color                     |
    \*-----------------------------------------------------*/
    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = SAYO_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_RANDOM_COLOR;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.speed_min             = 0;
    Breathing.speed_max             = 3;
    Breathing.speed                 = 1;
    modes.push_back(Breathing);

    /*-----------------------------------------------------*\
    | Wave - fades through multiple colors                  |
    \*-----------------------------------------------------*/
    mode Wave;
    Wave.name                       = "Wave";
    Wave.value                      = SAYO_MODE_WAVE;
    Wave.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_RANDOM_COLOR;
    Wave.color_mode                 = MODE_COLORS_NONE;
    Wave.speed_min                  = 0;
    Wave.speed_max                  = 3;
    Wave.speed                      = 1;
    modes.push_back(Wave);

    /*-----------------------------------------------------*\
    | Switch - alternates multiple colors                   |
    \*-----------------------------------------------------*/
    mode Switch;
    Switch.name                     = "Switch";
    Switch.value                    = SAYO_MODE_SWITCH;
    Switch.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_RANDOM_COLOR;
    Switch.color_mode               = MODE_COLORS_NONE;
    Switch.speed_min                = 0;
    Switch.speed_max                = 3;
    Switch.speed                    = 1;
    modes.push_back(Switch);

    /*-----------------------------------------------------*\
    | Blink - blinks on and off                             |
    \*-----------------------------------------------------*/
    mode Blink;
    Blink.name                      = "Blink";
    Blink.value                     = SAYO_MODE_BLINK;
    Blink.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_RANDOM_COLOR;
    Blink.color_mode                = MODE_COLORS_PER_LED;
    Blink.speed_min                 = 0;
    Blink.speed_max                 = 3;
    Blink.speed                     = 1;
    modes.push_back(Blink);

    SetupZones();
};

RGBController_SayoDevice::~RGBController_SayoDevice()
{
    delete controller;
}

void RGBController_SayoDevice::SetupZones()
{
    const int led_count = 1;

    zone zone;
    zone.name       = "Underglow";
    zone.type       = ZONE_TYPE_SINGLE;
    zone.leds_count = led_count;
    zone.leds_min   = led_count;
    zone.leds_max   = led_count;

    zones.clear();
    zones.push_back(zone);

    leds.clear();
    leds.resize(led_count);
    for(int i = 0; i < led_count; i++)
    {
        led& new_led = leds[i];
        new_led.name                = "LED " + std::to_string(i + 1);
        new_led.value               = i;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_SayoDevice::DeviceUpdateLEDs()
{
    unsigned int hw_mode = (modes[active_mode].value == SAYO_MODE_DIRECT) ? SAYO_MODE_STATIC : modes[active_mode].value;
    bool         random  = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    unsigned int speed   = 3 - modes[active_mode].speed;

    controller->SetMode(hw_mode, speed, colors[0], random);
}

void RGBController_SayoDevice::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SayoDevice::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SayoDevice::DeviceUpdateMode()
{
    /*-----------------------------------------------------*\
    | Mode and color are always set together.               |
    \*-----------------------------------------------------*/
    DeviceUpdateLEDs();
}

void RGBController_SayoDevice::DeviceSaveMode()
{
    controller->Save();
}
