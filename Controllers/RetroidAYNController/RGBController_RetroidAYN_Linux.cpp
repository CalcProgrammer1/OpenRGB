/*---------------------------------------------------------*\
| RGBController_RetroidAYN_Linux.cpp                        |
|                                                           |
|   RGBController for Retroid and AYN handheld RGB LEDs     |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      08 Nov 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_RetroidAYN_Linux.h"

/**------------------------------------------------------------------*\
    @name Retroid/AYN RGB LEDs
    @category LEDStrip
    @type
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectRetroidAYNControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RetroidAYN::RGBController_RetroidAYN(RetroidAYNController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetName();
    vendor                      = "Retroid/AYN";
    type                        = DEVICE_TYPE_GAMEPAD;
    description                 = "Retroid/AYN RGB LED Device";
    location                    = controller->GetLocation();

    mode direct_mode;
    direct_mode.name            = "Direct";
    direct_mode.value           = 0;
    direct_mode.flags           = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    direct_mode.color_mode      = MODE_COLORS_PER_LED;
    direct_mode.speed_min       = 0;
    direct_mode.speed_max       = 0;
    direct_mode.speed           = 0;
    direct_mode.brightness_min  = 0;
    direct_mode.brightness_max  = controller->GetMaxBrightness(0);
    direct_mode.brightness      = controller->GetBrightness(0);
    modes.push_back(direct_mode);

    SetupZones();

    for(std::size_t led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        colors[led_idx] = controller->GetLEDColor((unsigned int)led_idx);
    }
}

RGBController_RetroidAYN::~RGBController_RetroidAYN()
{
    Shutdown();

    delete controller;
}

void RGBController_RetroidAYN::SetupZones()
{
    int led_count   = (int)controller->GetLEDCount();
    int left_count  = led_count / 2;
    int right_count = led_count - left_count;

    /*-----------------------------------------------------*\
    | Create Left Stick zone (l1-l4)                        |
    \*-----------------------------------------------------*/
    zone left_zone;
    left_zone.name       = "Left Stick";
    left_zone.type       = ZONE_TYPE_LINEAR;
    left_zone.leds_min   = (unsigned int)left_count;
    left_zone.leds_max   = (unsigned int)left_count;
    left_zone.leds_count = (unsigned int)left_count;
    zones.push_back(left_zone);

    /*-----------------------------------------------------*\
    | Create Right Stick zone (r1-r4)                       |
    \*-----------------------------------------------------*/
    zone right_zone;
    right_zone.name       = "Right Stick";
    right_zone.type       = ZONE_TYPE_LINEAR;
    right_zone.leds_min   = (unsigned int)right_count;
    right_zone.leds_max   = (unsigned int)right_count;
    right_zone.leds_count = (unsigned int)right_count;
    zones.push_back(right_zone);

    /*-----------------------------------------------------*\
    | Create LED entries for each LED in the controller     |
    | LEDs 0-3 are Left Stick, LEDs 4-7 are Right Stick     |
    \*-----------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        led new_led;
        new_led.name    = "LED " + std::to_string(led_idx + 1);

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_RetroidAYN::DeviceUpdateLEDs()
{
    for(std::size_t led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        controller->SetLEDColor((unsigned int)led_idx, colors[led_idx]);
    }
}

void RGBController_RetroidAYN::DeviceUpdateZoneLEDs(int zone)
{
    for(unsigned int led_idx = zones[zone].start_idx; led_idx < zones[zone].start_idx + zones[zone].leds_count; led_idx++)
    {
        DeviceUpdateSingleLED(led_idx);
    }
}

void RGBController_RetroidAYN::DeviceUpdateSingleLED(int led)
{
    controller->SetLEDColor(led, colors[led]);
}

void RGBController_RetroidAYN::DeviceUpdateMode()
{
    for(std::size_t led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        controller->SetBrightness((unsigned int)led_idx, modes[active_mode].brightness);
    }
}
