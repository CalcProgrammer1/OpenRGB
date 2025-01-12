/*---------------------------------------------------------*\
| RGBController_AOCHeadset.cpp                              |
|                                                           |
|   RGBController for AOC headset                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                12 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AOCHeadset.h"

/**------------------------------------------------------------------*\
    @name AOC Headset
    @category Headset
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAOCHeadsetControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AOCHeadset::RGBController_AOCHeadset(AOCHeadsetController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = "AOC Headset Device";
    vendor                          = "AOC";
    type                            = DEVICE_TYPE_HEADSET;
    description                     = "AOC Headset Device";
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    mode direct;
    direct.name = "Direct";
    direct.value = 0;
    direct.color_mode = MODE_COLORS_PER_LED;
    direct.flags = MODE_FLAG_HAS_PER_LED_COLOR;
    modes.push_back(direct);

    SetupZones();
};

RGBController_AOCHeadset::~RGBController_AOCHeadset()
{
    delete controller;
}

void RGBController_AOCHeadset::SetupZones()
{
    zone headset_zone;
    headset_zone.name          = "Headset";
    headset_zone.type          = ZONE_TYPE_SINGLE;
    headset_zone.leds_min      = 1;
    headset_zone.leds_max      = 1;
    headset_zone.leds_count    = 1;
    headset_zone.matrix_map    = NULL;
    zones.push_back(headset_zone);

    led headset_led;
    headset_led.name           = "Headset";
    leds.push_back(headset_led);

    SetupColors();
}

void RGBController_AOCHeadset::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AOCHeadset::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_AOCHeadset::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCHeadset::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCHeadset::DeviceUpdateMode()
{
    controller->SendDirect(&colors[0]);
}
