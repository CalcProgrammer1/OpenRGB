/*---------------------------------------------------------*\
| RGBController_HyperXPulsefireRaid.cpp                     |
|                                                           |
|   RGBController for HyperX Pulsefire Raid                 |
|                                                           |
|   Morgan Guimard (morg)                       06 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_HyperXPulsefireRaid.h"

/**------------------------------------------------------------------*\
    @name HyperX Pulsefire Raid
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectHyperXPulsefireRaidControllers
    @comment
\*-------------------------------------------------------------------*/

using namespace std::chrono_literals;

RGBController_HyperXPulsefireRaid::RGBController_HyperXPulsefireRaid(HyperXPulsefireRaidController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "HyperX";
    type                    = DEVICE_TYPE_MOUSE;
    description             = "HyperX Pulsefire Raid Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0x00;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness       = HYPERX_PULSFIRE_RAID_BRIGHTNESS_MAX;
    Direct.brightness_min   = HYPERX_PULSFIRE_RAID_BRIGHTNESS_MIN;
    Direct.brightness_max   = HYPERX_PULSFIRE_RAID_BRIGHTNESS_MAX;

    modes.push_back(Direct);

    SetupZones();

    /*-----------------------------------------------------*\
    | This devices requires a keepalive thread or it will   |
    | reset to default (flash)                              |
    \*-----------------------------------------------------*/
    keepalive_thread_run = 1;
    keepalive_thread = new std::thread(&RGBController_HyperXPulsefireRaid::KeepaliveThread, this);
}

RGBController_HyperXPulsefireRaid::~RGBController_HyperXPulsefireRaid()
{

}

void RGBController_HyperXPulsefireRaid::SetupZones()
{
    std::string led_names[HYPERX_PULSFIRE_RAID_LEDS_COUNT] =
    {
        "Scroll Wheel", "Logo"
    };

    for(unsigned int i = 0; i < HYPERX_PULSFIRE_RAID_LEDS_COUNT; i++)
    {
        zone new_zone;
        new_zone.name        = led_names[i];
        new_zone.type        = ZONE_TYPE_SINGLE;
        new_zone.leds_min    = 1;
        new_zone.leds_max    = 1;
        new_zone.leds_count  = 1;
        new_zone.matrix_map  = NULL;
        zones.push_back(new_zone);

        led new_led;
        new_led.name         = led_names[i];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_HyperXPulsefireRaid::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_HyperXPulsefireRaid::DeviceUpdateLEDs()
{
    UpdateSingleLED(0);
}

void RGBController_HyperXPulsefireRaid::UpdateZoneLEDs(int zone)
{
    UpdateSingleLED(zone);
}

void RGBController_HyperXPulsefireRaid::UpdateSingleLED(int /*led*/)
{
    last_update_time = std::chrono::steady_clock::now();
    controller->SendColors(colors);
}

void RGBController_HyperXPulsefireRaid::DeviceUpdateMode()
{

}

void RGBController_HyperXPulsefireRaid::DeviceSaveMode()
{

}

void RGBController_HyperXPulsefireRaid::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > 1s)
            {
                UpdateLEDs();
            }
        }

        std::this_thread::sleep_for(10ms);
    }
}
