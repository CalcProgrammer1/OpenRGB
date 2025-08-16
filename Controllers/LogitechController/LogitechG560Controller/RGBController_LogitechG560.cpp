/*---------------------------------------------------------*\
| RGBController_LogitechG560.cpp                            |
|                                                           |
|   RGBController for Logitech G560                         |
|                                                           |
|   Cheerpipe                                   28 Oct 2020 |
|   based on TheRogueZeta                       31 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_LogitechG560.h"

/**------------------------------------------------------------------*\
    @name Logitech G560
    @category Speaker
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectLogitechG560
    @comment
\*-------------------------------------------------------------------*/

RGBController_LogitechG560::RGBController_LogitechG560(LogitechG560Controller* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "Logitech";
    type                    = DEVICE_TYPE_SPEAKER;
    description             = "Logitech G560 Lightsync Speaker";
    location                = controller->GetDeviceLocation();

    mode Off;
    Off.name                = "Off";
    Off.value               = LOGITECH_G560_MODE_OFF;
    Off.flags               = 0;
    Off.color_mode          = MODE_COLORS_PER_LED;
    modes.push_back(Off);

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = LOGITECH_G560_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);
    SetupZones();
}

void RGBController_LogitechG560::SetupZones()
{
    zone G560_left_front;
    G560_left_front.name            = "Left Front";
    G560_left_front.type            = ZONE_TYPE_SINGLE;
    G560_left_front.leds_min        = 1;
    G560_left_front.leds_max        = 1;
    G560_left_front.leds_count      = 1;
    G560_left_front.matrix_map      = NULL;
    zones.push_back(G560_left_front);

    led G560_left_front_led;
    G560_left_front_led.name        = "Left Front";
    G560_left_front_led.value       = 0x00;
    leds.push_back(G560_left_front_led);


    zone G560_right_front;
    G560_right_front.name           = "Right Front";
    G560_right_front.type           = ZONE_TYPE_SINGLE;
    G560_right_front.leds_min       = 1;
    G560_right_front.leds_max       = 1;
    G560_right_front.leds_count     = 1;
    G560_right_front.matrix_map     = NULL;
    zones.push_back(G560_right_front);

    led G560_right_front_led;
    G560_right_front_led.name       = "Right Front";
    G560_right_front_led.value      = 0x01;
    leds.push_back(G560_right_front_led);


    zone G560_left_rear;
    G560_left_rear.name             = "Left Rear";
    G560_left_rear.type             = ZONE_TYPE_SINGLE;
    G560_left_rear.leds_min         = 1;
    G560_left_rear.leds_max         = 1;
    G560_left_rear.leds_count       = 1;
    G560_left_rear.matrix_map       = NULL;
    zones.push_back(G560_left_rear);

    led G560_left_read_led;
    G560_left_read_led.name         = "Left Rear";
    G560_left_read_led.value        = 0x02;
    leds.push_back(G560_left_read_led);


    zone G560_right_rear;
    G560_right_rear.name            = "Right Rear";
    G560_right_rear.type            = ZONE_TYPE_SINGLE;
    G560_right_rear.leds_min        = 1;
    G560_right_rear.leds_max        = 1;
    G560_right_rear.leds_count      = 1;
    G560_right_rear.matrix_map      = NULL;
    zones.push_back(G560_right_rear);

    led G560_right_rear_led;
    G560_right_rear_led.name        = "Right Rear";
    G560_right_rear_led.value       = 0x03;
    leds.push_back(G560_right_rear_led);

    SetupColors();
}

void RGBController_LogitechG560::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechG560::DeviceUpdateLEDs()
{
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        unsigned char red = RGBGetRValue(colors[led_idx]);
        unsigned char grn = RGBGetGValue(colors[led_idx]);
        unsigned char blu = RGBGetBValue(colors[led_idx]);

        controller->SendSpeakerMode((unsigned char)leds[led_idx].value, modes[active_mode].value, red, grn, blu);
    }
}

void RGBController_LogitechG560::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG560::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG560::DeviceUpdateMode()
{
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        if(modes[active_mode].value == LOGITECH_G560_MODE_OFF)
        {
            controller->SetOffMode(leds[led_idx].value);
        }
        else
        {
            /*---------------------------------------------------------*\
            | Required to "reset" RGB controller and start receiving    |
            | color in direct mode                                      |
            \*---------------------------------------------------------*/
            controller->SetDirectMode(leds[led_idx].value);
        }

    }
    DeviceUpdateLEDs();
}
