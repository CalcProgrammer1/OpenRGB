/*---------------------------------------------------------*\
| RGBController_PatriotViperMouse.cpp                       |
|                                                           |
|   RGBController for Patriot Viper Mouse                   |
|                                                           |
|   mi4code                                     07 Apr 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_PatriotViperMouse.h"

/**------------------------------------------------------------------*\
    @name Patriot Viper V550
    @category Mouse
    @type USB
    @save :o:
    @direct :white_check_mark:
    @effects :o:
    @detectors DetectPatriotViperMouseControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PatriotViperMouse::RGBController_PatriotViperMouse(PatriotViperMouseController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetName();
    vendor                          = "Patriot";
    type                            = DEVICE_TYPE_MOUSE;
    description                     = "Patriot Viper Mouse";
    location                        = controller->GetLocation();
    serial                          = controller->GetSerial();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = 1;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
};

RGBController_PatriotViperMouse::~RGBController_PatriotViperMouse()
{
    delete controller;
}

void RGBController_PatriotViperMouse::SetupZones()
{
    zone left_zone;
    left_zone.name                  = "Left";
    left_zone.type                  = ZONE_TYPE_LINEAR;
    left_zone.leds_min              = 3;
    left_zone.leds_max              = 3;
    left_zone.leds_count            = 3;
    left_zone.matrix_map            = NULL;
    zones.push_back(left_zone);

    zone right_zone;
    right_zone.name                 = "Right";
    right_zone.type                 = ZONE_TYPE_LINEAR;
    right_zone.leds_min             = 3;
    right_zone.leds_max             = 3;
    right_zone.leds_count           = 3;
    right_zone.matrix_map           = NULL;
    zones.push_back(right_zone);

    zone wheel_zone;
    wheel_zone.name                 = "Mousewheel";
    wheel_zone.type                 = ZONE_TYPE_SINGLE;
    wheel_zone.leds_min             = 1;
    wheel_zone.leds_max             = 1;
    wheel_zone.leds_count           = 1;
    wheel_zone.matrix_map           = NULL;
    zones.push_back(wheel_zone);

    for(unsigned char i = 0x00; i < 0x07; i++)
    {
        led new_led;
        new_led.name                   = "LED " + std::to_string(i+1);
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_PatriotViperMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PatriotViperMouse::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_PatriotViperMouse::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PatriotViperMouse::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PatriotViperMouse::DeviceUpdateMode()
{
    /*-----------------*\
    | Direct mode       |
    \*-----------------*/
    if(modes[active_mode].value == 1)
    {
        controller->SetRGB(colors);
    }
}
