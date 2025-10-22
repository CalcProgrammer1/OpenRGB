/*---------------------------------------------------------*\
| RGBController_HyperXMicrophoneV2.cpp                      |
|                                                           |
|   RGBController for HyperX QuadCast 2 S Microphone        |
|                                                           |
|   Morgan Guimard (morg)                                   |
|   Logan Phillips (Eclipse)                    23 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

/**------------------------------------------------------------------*\
    @name HyperX Quadcast 2S
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectHyperXMicrophoneV2Controllers
    @comment
\*-------------------------------------------------------------------*/

#include "RGBController_HyperXMicrophoneV2.h"
#include <LogManager.h>

using namespace std::chrono_literals;

RGBController_HyperXMicrophoneV2::RGBController_HyperXMicrophoneV2(HyperXMicrophoneV2Controller* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "HyperX";
    type                    = DEVICE_TYPE_MICROPHONE;
    description             = "HyperX Microphone Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode Direct;
    Direct.name             = "Direct";
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.colors_min       = HYPERX_QUADCAST_2S_TOTAL_LEDS;
    Direct.colors_max       = HYPERX_QUADCAST_2S_TOTAL_LEDS;

    modes.push_back(Direct);

    SetupZones();

    keepalive_thread_run    = 1;
    keepalive_thread        = new std::thread(&RGBController_HyperXMicrophoneV2::KeepaliveThread, this);
};

RGBController_HyperXMicrophoneV2::~RGBController_HyperXMicrophoneV2()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    delete controller;
}

void RGBController_HyperXMicrophoneV2::SetupZones()
{
    zone Mic;

    Mic.name                = "Microphone";
    Mic.type                = ZONE_TYPE_MATRIX;
    Mic.leds_min            = HYPERX_QUADCAST_2S_TOTAL_LEDS;
    Mic.leds_max            = HYPERX_QUADCAST_2S_TOTAL_LEDS;
    Mic.leds_count          = HYPERX_QUADCAST_2S_TOTAL_LEDS;
    Mic.matrix_map          = new matrix_map_type;
    Mic.matrix_map->width   = HYPERX_QUADCAST_2S_MATRIX_WIDTH;
    Mic.matrix_map->height  = HYPERX_QUADCAST_2S_MATRIX_HEIGHT;
    Mic.matrix_map->map     = new unsigned int[HYPERX_QUADCAST_2S_TOTAL_LEDS];

    unsigned int led_mapping[HYPERX_QUADCAST_2S_TOTAL_LEDS] =
    {
        /* front */               /* rear */
        26, 27, 44, 45, 62, 63,   80, 81, 98,  99, 8,  9,
        25, 28, 43, 46, 61, 64,   79, 82, 97, 100, 7, 10,
        24, 29, 42, 47, 60, 65,   78, 83, 96, 101, 6, 11,
        23, 30, 41, 48, 59, 66,   77, 84, 95, 102, 5, 12,
        22, 31, 40, 49, 58, 67,   76, 85, 94, 103, 4, 13,
        21, 32, 39, 50, 57, 68,   75, 86, 93, 104, 3, 14,
        20, 33, 38, 51, 56, 69,   74, 87, 92, 105, 2, 15,
        19, 34, 37, 52, 55, 70,   73, 88, 91, 106, 1, 16,
        18, 35, 36, 53, 54, 71,   72, 89, 90, 107, 0, 17
    };

    for(unsigned int i = 0; i < HYPERX_QUADCAST_2S_TOTAL_LEDS; i ++)
    {
        led l;
        l.name   = "LED " + std::to_string(i);
        l.value  = led_mapping[i];
        leds.push_back(l);

        Mic.matrix_map->map[i] = led_mapping[i];
    }

    zones.push_back(Mic);

    SetupColors();
}

void RGBController_HyperXMicrophoneV2::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXMicrophoneV2::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();
    controller->SendDirect(colors);
}
void RGBController_HyperXMicrophoneV2::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}
void RGBController_HyperXMicrophoneV2::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}
void RGBController_HyperXMicrophoneV2::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXMicrophoneV2::DeviceSaveMode()
{
    LOG_DEBUG("[%s] Saving current direct colors to device", name.c_str());
    controller->SaveColors(colors);
}

void RGBController_HyperXMicrophoneV2::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(!controller->ShouldPauseUpdates() && (std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(1000))
        {
            UpdateLEDs();
        }
        std::this_thread::sleep_for(250ms);
    }
}
