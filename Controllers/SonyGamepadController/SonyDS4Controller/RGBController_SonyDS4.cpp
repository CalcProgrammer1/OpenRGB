/*---------------------------------------------------------*\
| RGBController_SonyDS4.cpp                                 |
|                                                           |
|   RGBController for Sony Dualshock 4                      |
|                                                           |
|   Pol Rius (alpemwarrior)                     24 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "RGBController_SonyDS4.h"

/**------------------------------------------------------------------*\
    @name Sony Dual Shock 4 controller
    @category Gamepad
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectSonyDS4Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_SonyDS4::RGBController_SonyDS4(SonyDS4Controller* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Sony DualShock 4";
    vendor      = "Sony";
    type        = DEVICE_TYPE_GAMEPAD;
    description = "Sony DualShock 4 Device";
    location    = controller->GetLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.value      = 0;
    Direct.name       = "Direct";
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_SonyDS4::~RGBController_SonyDS4()
{
    delete controller;
}

void RGBController_SonyDS4::SetupZones()
{
    /*---------------------------------------------------------*\
    | This device only has one LED, so create a single zone and |
    | LED for it                                                |
    \*---------------------------------------------------------*/
    zone* new_zone = new zone();
    led*  new_led  = new led();

    new_zone->name          = "Controller Zone";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 1;
    new_zone->leds_count    = 1;
    new_zone->matrix_map    = NULL;

    new_led->name           = "Controller LED";

    /*---------------------------------------------------------*\
    | Push the zone and LED on to device vectors                |
    \*---------------------------------------------------------*/
    leds.push_back(*new_led);
    zones.push_back(*new_zone);
    SetupColors();
}

void RGBController_SonyDS4::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SonyDS4::DeviceUpdateLEDs()
{
    unsigned char red =     char(RGBGetRValue(colors[0]));
    unsigned char green =   char(RGBGetGValue(colors[0]));
    unsigned char blue =    char(RGBGetBValue(colors[0]));
    controller->SetColors(red, green, blue);
}

void RGBController_SonyDS4::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SonyDS4::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SonyDS4::DeviceUpdateMode()
{
}
