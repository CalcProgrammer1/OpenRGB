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

    name                = controller->GetDeviceName();
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
    delete controller;
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

void RGBController_HIDLampArray::DeviceUpdateLEDs()
{
    unsigned int leds_to_go = leds.size();

    while(leds_to_go > 0)
    {
        unsigned int leds_this_frame = LAMP_MULTI_UPDATE_LAMP_COUNT;
        unsigned short LampIds[LAMP_MULTI_UPDATE_LAMP_COUNT];
        LampArrayColor UpdateColors[LAMP_MULTI_UPDATE_LAMP_COUNT];
        unsigned char LampUpdateFlags = 0;

        if(leds_to_go < leds_this_frame)
        {
            leds_to_go = leds_this_frame;
        }

        for(unsigned int led_frame_idx = 0; led_frame_idx < leds_this_frame; led_frame_idx++)
        {
            unsigned short led_idx = (leds.size() - leds_to_go) + led_frame_idx;
            LampIds[led_frame_idx] = led_idx;
            UpdateColors[led_frame_idx].RedChannel = RGBGetRValue(colors[led_idx]);
            UpdateColors[led_frame_idx].GreenChannel = RGBGetGValue(colors[led_idx]);
            UpdateColors[led_frame_idx].BlueChannel = RGBGetBValue(colors[led_idx]);
            UpdateColors[led_frame_idx].IntensityChannel = 255;
        }

        leds_to_go -= leds_this_frame;

        if(leds_to_go <= 0)
        {
            LampUpdateFlags = LAMP_UPDATE_FLAG_UPDATE_COMPLETE;
        }

        controller->SetLampMultiUpdateReport(leds_this_frame, LampUpdateFlags, LampIds, UpdateColors);
    }
}

void RGBController_HIDLampArray::DeviceUpdateZoneLEDs(int /*zone*/)
{

}

void RGBController_HIDLampArray::DeviceUpdateSingleLED(int /*led*/)
{

}

void RGBController_HIDLampArray::DeviceUpdateMode()
{

}
