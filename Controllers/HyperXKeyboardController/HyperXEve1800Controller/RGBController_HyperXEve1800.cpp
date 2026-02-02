/*---------------------------------------------------------*\
| RGBController_HyperXEve1800.cpp                          |
|                                                           |
|   RGBController for HyperX Eve 1800 keyboard              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_HyperXEve1800.h"

using namespace std::chrono_literals;

#define HYPERX_EVE_1800_BRIGHTNESS_MIN 0x00
#define HYPERX_EVE_1800_BRIGHTNESS_MAX 0xFF
#define HYPERX_EVE_1800_ZONE_COUNT 10

/**------------------------------------------------------------------*\
    @name HyperX Eve 1800
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectHyperXEve1800
    @comment
\*-------------------------------------------------------------------*/

RGBController_HyperXEve1800::RGBController_HyperXEve1800(HyperXEve1800Controller* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetNameString();
    vendor              = "HyperX";
    type                = DEVICE_TYPE_KEYBOARD;
    description         = "HyperX Eve 1800 Keyboard Device";
    location            = controller->GetDeviceLocation();
    serial              = controller->GetSerialString();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0xFFFF;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min = HYPERX_EVE_1800_BRIGHTNESS_MIN;
    Direct.brightness_max = HYPERX_EVE_1800_BRIGHTNESS_MAX;
    Direct.brightness   = HYPERX_EVE_1800_BRIGHTNESS_MAX;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    keepalive_thread_run = 1;
    keepalive_thread = new std::thread(&RGBController_HyperXEve1800::KeepaliveThread, this);
}

RGBController_HyperXEve1800::~RGBController_HyperXEve1800()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    Shutdown();

    delete controller;
}

void RGBController_HyperXEve1800::SetupZones()
{
    zone new_zone;
    new_zone.name                   = "Keyboard";
    new_zone.type                   = ZONE_TYPE_LINEAR;
    new_zone.leds_min               = HYPERX_EVE_1800_ZONE_COUNT;
    new_zone.leds_max               = HYPERX_EVE_1800_ZONE_COUNT;
    new_zone.leds_count             = HYPERX_EVE_1800_ZONE_COUNT;
    zones.push_back(new_zone);

    for(unsigned int led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name                = "Zone ";
        new_led.name.append(std::to_string(led_idx + 1));
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_HyperXEve1800::DeviceUpdateLEDs()
{
    controller->SetLEDsDirect(colors);
    last_update_time = std::chrono::steady_clock::now();
}

void RGBController_HyperXEve1800::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXEve1800::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXEve1800::DeviceUpdateMode()
{
    controller->SetBrightness(modes[active_mode].brightness);
    DeviceUpdateLEDs();
}

void RGBController_HyperXEve1800::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(50))
            {
                UpdateLEDs();
            }
        }
        std::this_thread::sleep_for(10ms);
    }
}
