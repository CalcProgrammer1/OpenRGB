/*---------------------------------------------------------*\
| RGBController_SkydimoHID.cpp                              |
|                                                           |
|   RGBController for Skydimo HID devices                   |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  09 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_SkydimoHID.h"

/**------------------------------------------------------------------*\
    @name Skydimo SK0902
    @category HeadsetStand
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectSkydimoHIDControllers
    @comment
\*-------------------------------------------------------------------*/

static const unsigned int skydimo_hid_led_positions[SKYDIMO_HID_LED_COUNT][2] =
{
    {0, 6}, {0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0},
    {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6},
    {2, 6}, {2, 5}, {2, 4}, {2, 3}, {2, 2}, {2, 1}, {2, 0},
    {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6},
    {4, 6}, {4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0},
    {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6},
    {6, 6}, {6, 5}, {6, 4}, {6, 3}, {6, 2}, {6, 1}, {6, 0}
};

RGBController_SkydimoHID::RGBController_SkydimoHID(SkydimoHIDController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetDeviceName();
    vendor              = "Skydimo";
    type                = DEVICE_TYPE_HEADSET_STAND;
    description         = "Skydimo HID Device";
    serial              = controller->GetSerial();
    location            = controller->GetLocation();

    mode direct_mode;
    direct_mode.name        = "Direct";
    direct_mode.value       = 0;
    direct_mode.flags       = MODE_FLAG_HAS_PER_LED_COLOR;
    direct_mode.color_mode  = MODE_COLORS_PER_LED;
    modes.push_back(direct_mode);

    SetupZones();
}

RGBController_SkydimoHID::~RGBController_SkydimoHID()
{
    Shutdown();

    delete controller;
}

void RGBController_SkydimoHID::SetupZones()
{
    zone led_zone;
    led_zone.name       = "Matrix";
    led_zone.type       = ZONE_TYPE_MATRIX;
    led_zone.leds_min   = SKYDIMO_HID_LED_COUNT;
    led_zone.leds_max   = SKYDIMO_HID_LED_COUNT;
    led_zone.leds_count = SKYDIMO_HID_LED_COUNT;

    led_zone.matrix_map.height    = SKYDIMO_HID_MATRIX_SIZE;
    led_zone.matrix_map.width     = SKYDIMO_HID_MATRIX_SIZE;
    led_zone.matrix_map.map.assign(SKYDIMO_HID_MATRIX_SIZE * SKYDIMO_HID_MATRIX_SIZE, SKYDIMO_HID_MATRIX_NA);

    /*-----------------------------------------------------*\
    | Construct the matrix map                              |
    \*-----------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < SKYDIMO_HID_LED_COUNT; led_idx++)
    {
        unsigned int column = skydimo_hid_led_positions[led_idx][0];
        unsigned int row    = skydimo_hid_led_positions[led_idx][1];

        led_zone.matrix_map.map[(row * SKYDIMO_HID_MATRIX_SIZE) + column] = led_idx;
    }

    zones.push_back(led_zone);

    for(unsigned int led_idx = 0; led_idx < SKYDIMO_HID_LED_COUNT; led_idx++)
    {
        led new_led;
        new_led.name = "LED " + std::to_string(led_idx + 1);

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_SkydimoHID::DeviceUpdateLEDs()
{
    controller->SetLEDs(colors);
}

void RGBController_SkydimoHID::DeviceUpdateZoneLEDs(int)
{
    DeviceUpdateLEDs();
}

void RGBController_SkydimoHID::DeviceUpdateSingleLED(int)
{
    DeviceUpdateLEDs();
}

void RGBController_SkydimoHID::DeviceUpdateMode()
{
}
