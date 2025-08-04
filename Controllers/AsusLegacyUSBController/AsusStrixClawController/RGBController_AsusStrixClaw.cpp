/*---------------------------------------------------------*\
| RGBController_AsusStrixClaw.cpp                           |
|                                                           |
|   RGBController for ASUS Strix Claw mouse                 |
|                                                           |
|   Mola19                                      06 Aug 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AsusStrixClaw.h"

/**------------------------------------------------------------------*\
    @name Asus Strix Claw
    @category Mouse
    @type USB
    @save :robot:
    @direct :x:
    @effects :tools:
    @detectors DetectAsusStrixClaw
    @comment
\*-------------------------------------------------------------------*/

RGBController_StrixClaw::RGBController_StrixClaw(StrixClawController* controller_ptr)
{
    controller   = controller_ptr;

    name         = controller->GetDeviceName();
    vendor       = "ASUS";
    type         = DEVICE_TYPE_MOUSE;
    description  = "ASUS Legacy Mouse Device";
    version      = controller->GetVersion();
    location     = controller->GetDeviceLocation();
    serial       = controller->GetSerialString();

    mode Off;
    Off.name        = "Off";
    Off.value       = 0;
    Off.flags       = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode  = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode On;
    On.name         = "On";
    On.value        = 1;
    On.flags        = MODE_FLAG_AUTOMATIC_SAVE;
    On.color_mode   = MODE_COLORS_NONE;
    modes.push_back(On);

    SetupZones();
}

RGBController_StrixClaw::~RGBController_StrixClaw()
{
    delete controller;
}

void RGBController_StrixClaw::SetupZones()
{
    zone scroll_wheel_zone;

    scroll_wheel_zone.name         = "Scroll Wheel";
    scroll_wheel_zone.type         = ZONE_TYPE_SINGLE;
    scroll_wheel_zone.leds_min     = 1;
    scroll_wheel_zone.leds_max     = 1;
    scroll_wheel_zone.leds_count   = 1;
    scroll_wheel_zone.matrix_map   = NULL;

    zones.push_back(scroll_wheel_zone);

    led scroll_wheel_led;

    scroll_wheel_led.name          = "Scroll Wheel LED";
    scroll_wheel_led.value         = 1;

    leds.push_back(scroll_wheel_led);

    zone logo_zone;

    logo_zone.name         = "Logo";
    logo_zone.type         = ZONE_TYPE_SINGLE;
    logo_zone.leds_min     = 1;
    logo_zone.leds_max     = 1;
    logo_zone.leds_count   = 1;
    logo_zone.matrix_map   = NULL;

    zones.push_back(logo_zone);

    led logo_led;

    logo_led.name          = "Logo LED";
    logo_led.value         = 1;

    leds.push_back(logo_led);

    SetupColors();
}

void RGBController_StrixClaw::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_StrixClaw::DeviceUpdateLEDs()
{

}

void RGBController_StrixClaw::UpdateZoneLEDs(int /*zone*/)
{

}

void RGBController_StrixClaw::UpdateSingleLED(int /*led*/)
{

}

void RGBController_StrixClaw::DeviceUpdateMode()
{
    if(modes[active_mode].value == 0)
    {
        controller->SetScrollWheelLED(false);
        controller->SetLogoLED(0);

    }
    else if(modes[active_mode].value == 1)
    {
        controller->SetScrollWheelLED(true);
        controller->SetLogoLED(255);
    }
}

