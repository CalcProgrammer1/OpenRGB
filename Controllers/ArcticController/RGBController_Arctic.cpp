/*---------------------------------------------------------*\
| RGBController_Arctic.cpp                                  |
|                                                           |
|   RGBController for Arctic devices                        |
|                                                           |
|   Armin Wolf (Wer-Wolf)                       09 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <math.h>
#include "RGBController_Arctic.h"

using namespace std::chrono_literals;

#define ARCTIC_NUM_CHANNELS     4
#define ARCTIC_SLEEP_THRESHOLD  100ms
#define ARCTIC_KEEPALIVE_PERIOD 500ms  /* Device requires at least 1s */

/**------------------------------------------------------------------*\
    @name Arctic RGB Controller Devices
    @category LEDStrip
    @type Serial
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectArcticControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_Arctic::RGBController_Arctic(ArcticController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Arctic RGB Controller";
    vendor      = "Arctic";
    description = "Arctic 4-Channel RGB Controller";
    location    = controller->GetLocation();
    type        = DEVICE_TYPE_LEDSTRIP;

    mode DirectMode;
    DirectMode.name         = "Direct";
    DirectMode.value        = 0;
    DirectMode.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    DirectMode.color_mode   = MODE_COLORS_PER_LED;

    modes.push_back(DirectMode);

    SetupZones();

    keepalive_thread_run = true;
    keepalive_thread = std::thread(&RGBController_Arctic::KeepaliveThreadFunction, this);
}

RGBController_Arctic::~RGBController_Arctic()
{
    keepalive_thread_run = false;
    keepalive_thread.join();
    delete controller;
}

void RGBController_Arctic::SetupZones()
{
    for(int channel = 0; channel < ARCTIC_NUM_CHANNELS; channel++)
    {
        zone LedZone;
        LedZone.name        = "LED Strip " + std::to_string(channel);
        LedZone.type        = ZONE_TYPE_SINGLE;
        LedZone.leds_count  = 1;
        LedZone.leds_min    = 1;
        LedZone.leds_max    = 1;
        LedZone.matrix_map  = nullptr;

        led Led;
        Led.name    = LedZone.name + " LED";
        Led.value   = channel;

        zones.push_back(LedZone);
        leds.push_back(Led);
    }

    SetupColors();
}

void RGBController_Arctic::ResizeZone(int /* zone */, int /* new_size */)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Arctic::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    controller->SetChannels(colors);
}

void RGBController_Arctic::UpdateZoneLEDs(int /* zone */)
{
    DeviceUpdateLEDs();
}

void RGBController_Arctic::UpdateSingleLED(int /* led */)
{
    DeviceUpdateLEDs();
}

void RGBController_Arctic::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | This device does not support mode updates                 |
    \*---------------------------------------------------------*/
}

void RGBController_Arctic::KeepaliveThreadFunction()
{
    std::chrono::nanoseconds sleep_time;

    while(keepalive_thread_run.load())
    {
        sleep_time = ARCTIC_KEEPALIVE_PERIOD - (std::chrono::steady_clock::now() - last_update_time);
        if(sleep_time <= ARCTIC_SLEEP_THRESHOLD)
        {
            UpdateLEDs();   // Already protected thru a device update thread
            std::this_thread::sleep_for(ARCTIC_KEEPALIVE_PERIOD);
        }
        else
        {
            std::this_thread::sleep_for(sleep_time);
        }
    }
}
