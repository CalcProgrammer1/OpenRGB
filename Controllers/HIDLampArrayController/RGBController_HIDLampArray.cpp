/*---------------------------------------------------------*\
| RGBController_HIDLampArray.cpp                            |
|                                                           |
|   RGBController for HID LampArray Devices                 |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      26 Mar 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_HIDLampArray.h"

RGBController_HIDLampArray::RGBController_HIDLampArray(HIDLampArrayController* controller_ptr)
{
    controller          = controller_ptr;

    name                = "HID LampArray Device";
    vendor              = "Generic";
    type                = DEVICE_TYPE_MOUSEMAT;
    description         = "HID LampArray Device";
    location            = controller->GetDeviceLocation();
    serial              = controller->GetSerialString();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_HIDLampArray::~RGBController_HIDLampArray()
{

}

void RGBController_HIDLampArray::SetupZones()
{
    zone new_zone;
    new_zone.name       = "Test Zone";
    new_zone.type       = ZONE_TYPE_SINGLE;
    new_zone.leds_count = controller->GetLampCount();
    new_zone.leds_min   = new_zone.leds_count;
    new_zone.leds_max   = new_zone.leds_count;
    new_zone.matrix_map = NULL;
    zones.push_back(new_zone);

    for(unsigned int led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
    {
        led new_led;
        new_led.name    = "Test LED";
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_HIDLampArray::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_HIDLampArray::DeviceUpdateLEDs()
{

}

void RGBController_HIDLampArray::UpdateZoneLEDs(int /*zone*/)
{

}

void RGBController_HIDLampArray::UpdateSingleLED(int /*led*/)
{

}

void RGBController_HIDLampArray::DeviceUpdateMode()
{

}
