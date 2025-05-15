/*---------------------------------------------------------*\
| RGBController_AlienwareMonitor.cpp                        |
|                                                           |
|   RGBController for Alienware monitors                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AlienwareMonitor.h"

/**------------------------------------------------------------------*\
    @name Alienware Monitor
    @category Accessory
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectAlienwareMonitorControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AlienwareMonitor::RGBController_AlienwareMonitor(AlienwareMonitorController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = "Alienware Monitor";
    description             = "Alienware Monitor";
    vendor                  = "Alienware";
    type                    = DEVICE_TYPE_MONITOR;
    location                = controller->GetLocation();
    serial                  = controller->GetSerialString();

    mode Direct;
    Direct.name             = "Direct";
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    active_mode             = 0;

    SetupZones();
}

RGBController_AlienwareMonitor::~RGBController_AlienwareMonitor()
{
    delete controller;
}

void RGBController_AlienwareMonitor::SetupZones()
{
    zone Logo;
    Logo.name               = "Logo";
    Logo.type               = ZONE_TYPE_SINGLE;
    Logo.leds_min           = 1;
    Logo.leds_max           = 1;
    Logo.leds_count         = 1;
    Logo.matrix_map         = NULL;
    zones.push_back(Logo);

    led Logo_LED;
    Logo_LED.name           = "Logo";
    Logo_LED.value          = 0x01;
    leds.push_back(Logo_LED);

    zone Number;
    Number.name             = "Number";
    Number.type             = ZONE_TYPE_SINGLE;
    Number.leds_min         = 1;
    Number.leds_max         = 1;
    Number.leds_count       = 1;
    Number.matrix_map       = NULL;
    zones.push_back(Number);

    led Number_LED;
    Number_LED.name         = "Number";
    Number_LED.value        = 0x02;
    leds.push_back(Number_LED);

    zone PowerButton;
    PowerButton.name        = "Power Button";
    PowerButton.type        = ZONE_TYPE_SINGLE;
    PowerButton.leds_min    = 1;
    PowerButton.leds_max    = 1;
    PowerButton.leds_count  = 1;
    PowerButton.matrix_map  = NULL;
    zones.push_back(PowerButton);

    led PowerButton_LED;
    PowerButton_LED.name    = "Power Button";
    PowerButton_LED.value   = 0x08;
    leds.push_back(PowerButton_LED);

    SetupColors();
}

void RGBController_AlienwareMonitor::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AlienwareMonitor::DeviceUpdateLEDs()
{
    /*-----------------------------------------------------*\
    | If all three colors are the same value, speed up the  |
    | direct mode by using the ALL target (0x0B) instead of |
    | setting each LED individually.                        |
    \*-----------------------------------------------------*/
    if((colors[0] == colors[1]) && (colors[1] == colors[2]))
    {
        unsigned char red = RGBGetRValue(colors[0]);
        unsigned char grn = RGBGetGValue(colors[0]);
        unsigned char blu = RGBGetBValue(colors[0]);
        controller->SendColor(0x0B, red, grn, blu);
    }
    else
    {
        for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
        {
            UpdateSingleLED(led_idx);
        }
    }
}

void RGBController_AlienwareMonitor::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AlienwareMonitor::UpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);
    controller->SendColor(leds[led].value, red, grn, blu);
}

void RGBController_AlienwareMonitor::DeviceUpdateMode()
{

}
