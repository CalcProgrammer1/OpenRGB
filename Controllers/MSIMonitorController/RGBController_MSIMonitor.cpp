/*---------------------------------------------------------*\
| RGBController_MSIMonitor.cpp                              |
|                                                           |
|   RGBController for MSI monitor (gaming controller)       |
|                                                           |
|   Andy Herbert                              2026 June 1   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_MSIMonitor.h"

using namespace std::chrono_literals;

/**------------------------------------------------------------------*\
    @name MSIMonitor
    @category Accessory
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectMSIMonitorController
    @comment Developed with MSI MAG272CQR
\*-------------------------------------------------------------------*/
RGBController_MSIMonitor::RGBController_MSIMonitor(MSIMonitorController* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetNameString();
    vendor                              = "MSI";
    type                                = DEVICE_TYPE_MONITOR;
    description                         = "MSI Monitor (Gaming Controller)";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Direct;
    Direct.name                         = "Direct";
    Direct.value                        = MSI_MONITOR_STATIC_MODE_VALUE;
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                         = "Static";
    Static.value                        = MSI_MONITOR_STATIC_MODE_VALUE;
    Static.flags                        = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Static);


    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = MSI_MONITOR_BREATHING_MODE_VALUE;
    Breathing.flags                     = MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.color_mode                = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name                       = "Flashing";
    Flashing.value                      = MSI_MONITOR_FLASHING_MODE_VALUE;
    Flashing.flags                      = MODE_FLAG_HAS_PER_LED_COLOR;
    Flashing.color_mode                 = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    mode Lightning;
    Lightning.name                      = "Lightning";
    Lightning.value                     = MSI_MONITOR_LIGHTNING_MODE_VALUE;
    Lightning.flags                     = MODE_FLAG_HAS_PER_LED_COLOR;
    Lightning.color_mode                = MODE_COLORS_PER_LED;
    modes.push_back(Lightning);

    mode Marquee;
    Marquee.name                        = "Marquee";
    Marquee.value                       = MSI_MONITOR_MARQUEE_MODE_VALUE;
    Marquee.flags                       = MODE_FLAG_AUTOMATIC_SAVE;
    Marquee.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Marquee);

    mode Meteor;
    Meteor.name                         = "Meteor";
    Meteor.value                        = MSI_MONITOR_METEOR_MODE_VALUE;
    Meteor.flags                        = MODE_FLAG_AUTOMATIC_SAVE;
    Meteor.color_mode                   = MODE_COLORS_NONE;
    modes.push_back(Meteor);

    mode Rainbow;
    Rainbow.name                        = "Rainbow";
    Rainbow.value                       = MSI_MONITOR_RAINBOW_MODE_VALUE;
    Rainbow.flags                       = MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Random;
    Random.name                         = "Random";
    Random.value                        = MSI_MONITOR_RANDOM_MODE_VALUE;
    Random.flags                        = MODE_FLAG_AUTOMATIC_SAVE;
    Random.color_mode                   = MODE_COLORS_RANDOM;
    modes.push_back(Random);

    mode Off;
    Off.name                            = "Off";
    Off.value                           = MSI_MONITOR_OFF_MODE_VALUE;
    Off.flags                           = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_MSIMonitor::~RGBController_MSIMonitor()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    delete controller;
}

void RGBController_MSIMonitor::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Rear";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = 9;
    new_zone.leds_max   = 9;
    new_zone.leds_count = 9;
    new_zone.matrix_map = nullptr;

    zones.emplace_back(new_zone);

    for(unsigned int i = 0 ; i < 9; i ++)
    {
        led new_led;
        new_led.name = "LED " + std::to_string(i + 1);
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_MSIMonitor::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_MSIMonitor::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();
    controller->Set(modes[active_mode].value, colors, active_mode == 0 ? 0x00 : 0x01);
}

void RGBController_MSIMonitor::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSIMonitor::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSIMonitor::DeviceUpdateMode()
{
    controller->Set(modes[active_mode].value, colors, 0x01);
}
