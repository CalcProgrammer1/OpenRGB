/*---------------------------------------------------------*\
| RGBController_NZXTMouse.cpp                               |
|                                                           |
|   RGBController for NZXT Mouse                            |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      16 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_NZXTMouse.h"

/**------------------------------------------------------------------*\
    @name NZXT Mouse
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :tools:
    @detectors DetectNZXTMouseControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_NZXTMouse::RGBController_NZXTMouse(NZXTMouseController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetName();
    vendor              = "NZXT";
    type                = DEVICE_TYPE_MOUSE;
    description         = "NZXT Mouse Device";
    version             = controller->GetFirmwareVersion();
    location            = controller->GetLocation();
    serial              = controller->GetSerialString();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0xFFFF;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_NZXTMouse::~RGBController_NZXTMouse()
{

}

void RGBController_NZXTMouse::SetupZones()
{
    zone left;

    left.name           = "Left";
    left.type           = ZONE_TYPE_LINEAR;
    left.leds_min       = 3;
    left.leds_max       = 3;
    left.leds_count     = 3;
    left.matrix_map     = NULL;

    zones.push_back( left );

    for(unsigned int led_idx = 0; led_idx < left.leds_count; led_idx++)
    {
        led left_led;
        left_led.name   = "Left LED " + std::to_string(led_idx);

        leds.push_back(left_led);
    }

    zone right;

    right.name          = "Right";
    right.type          = ZONE_TYPE_LINEAR;
    right.leds_min      = 3;
    right.leds_max      = 3;
    right.leds_count    = 3;
    right.matrix_map    = NULL;

    zones.push_back( right );

    for(unsigned int led_idx = 0; led_idx < right.leds_count; led_idx++)
    {
        led right_led;
        right_led.name   = "Right LED " + std::to_string(led_idx);

        leds.push_back(right_led);
    }

    SetupColors();
}

void RGBController_NZXTMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_NZXTMouse::DeviceUpdateLEDs()
{
controller->SetLEDs(&colors[0]);
}

void RGBController_NZXTMouse::UpdateZoneLEDs(int /*zone*/)
{
DeviceUpdateLEDs();
}

void RGBController_NZXTMouse::UpdateSingleLED(int /*led*/)
{
DeviceUpdateLEDs();
}

void RGBController_NZXTMouse::DeviceUpdateMode()
{
DeviceUpdateLEDs();
}
