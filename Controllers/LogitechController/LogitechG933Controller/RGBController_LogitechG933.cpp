/*---------------------------------------------------------*\
| RGBController_LogitechG933.cpp                            |
|                                                           |
|   RGBController for Logitech G933                         |
|                                                           |
|   Edbgon                                      21 Jun 2021 |
|   Based on TheRogueZeta                       31 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_LogitechG933.h"

/**------------------------------------------------------------------*\
    @name Logitech G933
    @category Headset
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectLogitechG933
    @comment
\*-------------------------------------------------------------------*/

RGBController_LogitechG933::RGBController_LogitechG933(LogitechG933Controller* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "Logitech";
    type                    = DEVICE_TYPE_HEADSET;
    description             = "Logitech G933 Lightsync Headset";
    location                = controller->GetDeviceLocation();

    mode Off;
    Off.name                = "Off";
    Off.value               = LOGITECH_G933_MODE_OFF;
    Off.flags               = 0;
    Off.color_mode          = MODE_COLORS_PER_LED;
    modes.push_back(Off);

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = LOGITECH_G933_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_LogitechG933::~RGBController_LogitechG933()
{
    Shutdown();

    delete controller;
}

void RGBController_LogitechG933::SetupZones()
{
    zone G933_logo;
    G933_logo.name          = "Logo";
    G933_logo.type          = ZONE_TYPE_SINGLE;
    G933_logo.leds_min      = 1;
    G933_logo.leds_max      = 1;
    G933_logo.leds_count    = 1;
    zones.push_back(G933_logo);

    led G933_logo_led;
    G933_logo_led.name      = "Logo";
    G933_logo_led.value     = 0x00;
    leds.push_back(G933_logo_led);

    zone G933_strip;
    G933_strip.name         = "LED Strip";
    G933_strip.type         = ZONE_TYPE_SINGLE;
    G933_strip.leds_min     = 1;
    G933_strip.leds_max     = 1;
    G933_strip.leds_count   = 1;
    zones.push_back(G933_strip);

    led G933_strip_led;
    G933_strip_led.name     = "Led Strip";
    G933_strip_led.value    = 0x01;
    leds.push_back(G933_strip_led);

    SetupColors();
}

void RGBController_LogitechG933::DeviceUpdateLEDs()
{
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        unsigned char red = RGBGetRValue(colors[led_idx]);
        unsigned char grn = RGBGetGValue(colors[led_idx]);
        unsigned char blu = RGBGetBValue(colors[led_idx]);

        controller->SendHeadsetMode((unsigned char)leds[led_idx].value, modes[active_mode].value, red, grn, blu);
    }
}

void RGBController_LogitechG933::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG933::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG933::DeviceUpdateMode()
{
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        if(modes[active_mode].value == LOGITECH_G933_MODE_OFF)
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
