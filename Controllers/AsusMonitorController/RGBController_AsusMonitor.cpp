/*---------------------------------------------------------*\
| RGBController_AsusMonitor.cpp                             |
|                                                           |
|   RGBController for Asus monitors                         |
|                                                           |
|   Morgan Guimard (morg)                       19 oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_AsusMonitor.h"

/**------------------------------------------------------------------*\
    @name Asus monitors
    @category Monitor
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectAsusMonitorControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AsusMonitor::RGBController_AsusMonitor(AsusMonitorController* controller_ptr)
{
    controller                          = controller_ptr;
    name                                = controller->GetNameString();
    vendor                              = "ASUS";
    type                                = DEVICE_TYPE_MONITOR;
    description                         = "ASUS monitor";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();
    number_of_leds                      = controller->GetNumberOfLEDs();

    controller->SendInit();

    mode Direct;
    Direct.name                         = "Direct";
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_AsusMonitor::~RGBController_AsusMonitor()
{
    Shutdown();

    delete controller;
}

void RGBController_AsusMonitor::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Monitor";
    new_zone.type       = ZONE_TYPE_LINEAR;

    new_zone.leds_min   = number_of_leds;
    new_zone.leds_max   = number_of_leds;
    new_zone.leds_count = number_of_leds;

    zones.emplace_back(new_zone);

    leds.resize(new_zone.leds_count);

    for(unsigned int i = 0; i < number_of_leds; i++)
    {
        leds[i].name = "LED " + std::to_string(i);
    }

    SetupColors();
}

void RGBController_AsusMonitor::DeviceUpdateLEDs()
{
    controller->SetDirect(colors);
}

void RGBController_AsusMonitor::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AsusMonitor::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AsusMonitor::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
