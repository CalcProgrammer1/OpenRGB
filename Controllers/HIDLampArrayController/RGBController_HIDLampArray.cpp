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

#include <cstring>
#include <set>
#include "RGBController_HIDLampArray.h"

/**------------------------------------------------------------------*\
    @name HID LampArray Controllers
    @category Keyboard
    @type USB
    @detectors DetectHIDLampArrayControllers
\*-------------------------------------------------------------------*/

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
    new_zone.name       = "LampArray";
    new_zone.type       = ZONE_TYPE_MATRIX;
    new_zone.leds_count = controller->GetLampCount();
    new_zone.leds_min   = new_zone.leds_count;
    new_zone.leds_max   = new_zone.leds_count;

    std::set<unsigned int>  columns;
    std::set<unsigned int>  rows;

    for(std::size_t lamp_idx = 0; lamp_idx < controller->GetLamps().size(); lamp_idx++)
    {
        rows.insert(controller->GetLamps()[lamp_idx].PositionYInMillimeters);
        columns.insert(controller->GetLamps()[lamp_idx].PositionXInMillimeters);
    }

    new_zone.matrix_map.height  = rows.size();
    new_zone.matrix_map.width   = columns.size();
    new_zone.matrix_map.map.resize(rows.size() * columns.size());

    memset(new_zone.matrix_map.map.data(), 0xFF, (new_zone.matrix_map.map.size() * sizeof(unsigned int)));

    for(std::size_t lamp_idx = 0; lamp_idx < controller->GetLamps().size(); lamp_idx++)
    {
        //FIXME this assumes that matrix_size is big enough which is only guaranteed when no key possition is doubled
        size_t idx      = std::distance(columns.begin(), columns.find(controller->GetLamps()[lamp_idx].PositionXInMillimeters));
        size_t idy      = std::distance(rows.begin(), rows.find(controller->GetLamps()[lamp_idx].PositionYInMillimeters));

        new_zone.matrix_map.map[idx + idy * new_zone.matrix_map.width] = lamp_idx;
    }

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
