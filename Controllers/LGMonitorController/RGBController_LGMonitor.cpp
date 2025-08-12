/*---------------------------------------------------------*\
| RGBController_LGMonitor.cpp                               |
|                                                           |
|   RGBController for LG monitor                            |
|                                                           |
|   Morgan Guimard (morg)                       11 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_LGMonitor.h"

using namespace std::chrono_literals;

/**------------------------------------------------------------------*\
    @name LGMonitor
    @category Accessory
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectLGMonitorControllers
    @comment
\*-------------------------------------------------------------------*/
RGBController_LGMonitor::RGBController_LGMonitor(LGMonitorController* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetNameString();
    vendor                              = "LG";
    type                                = DEVICE_TYPE_MONITOR;
    description                         = "LG Monitor";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Direct;
    Direct.name                         = "Direct";
    Direct.value                        = LG_MONITOR_DIRECT_MODE_VALUE;
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                         = "Static";
    Static.value                        = LG_MONITOR_STATIC_SLOT_1_MODE_VALUE;
    Static.flags                        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode                   = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    Static.colors_min                   = 1;
    Static.colors_max                   = 1;
    Static.brightness_min               = 1;
    Static.brightness_max               = 12;
    Static.brightness                   = 12;
    modes.push_back(Static);

    mode SpectrumCycle;
    SpectrumCycle.name                  = "Spectrum Cycle";
    SpectrumCycle.value                 = LG_MONITOR_SPECTRUM_CYCLE_MODE_VALUE;
    SpectrumCycle.flags                 = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumCycle.color_mode            = MODE_COLORS_NONE;
    SpectrumCycle.brightness_min        = 1;
    SpectrumCycle.brightness_max        = 12;
    SpectrumCycle.brightness            = 12;
    modes.push_back(SpectrumCycle);

    mode RainbowWave;
    RainbowWave.name                    = "Rainbow Wave";
    RainbowWave.value                   = LG_MONITOR_RAINBOW_MODE_VALUE;
    RainbowWave.flags                   = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    RainbowWave.color_mode              = MODE_COLORS_NONE;
    RainbowWave.brightness_min          = 1;
    RainbowWave.brightness_max          = 12;
    RainbowWave.brightness              = 12;
    modes.push_back(RainbowWave);

    mode Off;
    Off.name                            = "Off";
    Off.value                           = LG_MONITOR_OFF_MODE_VALUE;
    Off.flags                           = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();

    keepalive_thread_run    = 1;
    keepalive_thread        = new std::thread(&RGBController_LGMonitor::KeepaliveThread, this);
}

RGBController_LGMonitor::~RGBController_LGMonitor()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    delete controller;
}

void RGBController_LGMonitor::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Screen";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = 48;
    new_zone.leds_max   = 48;
    new_zone.leds_count = 48;
    new_zone.matrix_map = nullptr;

    zones.emplace_back(new_zone);

    for(unsigned int i = 0 ; i < 48; i ++)
    {
        led new_led;
        new_led.name = "LED " + std::to_string(i + 1);
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_LGMonitor::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LGMonitor::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();
    controller->SetDirect(colors);
}

void RGBController_LGMonitor::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LGMonitor::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LGMonitor::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode].value, modes[active_mode].brightness, modes[active_mode].colors);
}

void RGBController_LGMonitor::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if((modes[active_mode].value == LG_MONITOR_DIRECT_MODE_VALUE) && (std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(500))
        {
            UpdateLEDs();
        }

        std::this_thread::sleep_for(15ms);
    }
}
