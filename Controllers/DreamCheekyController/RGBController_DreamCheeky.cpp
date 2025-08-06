/*---------------------------------------------------------*\
| RGBController_DreamCheeky.cpp                             |
|                                                           |
|   RGBController for Dream Cheeky devices                  |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      06 Sep 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_DreamCheeky.h"

RGBController_DreamCheeky::RGBController_DreamCheeky(DreamCheekyController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetNameString();
    type                = DEVICE_TYPE_ACCESSORY;
    vendor              = "Dream Cheeky";
    description         = "Dream Cheeky Device";
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

RGBController_DreamCheeky::~RGBController_DreamCheeky()
{

}

void RGBController_DreamCheeky::SetupZones()
{
    zone mail_zone;
    mail_zone.name          = "LED";
    mail_zone.type          = ZONE_TYPE_SINGLE;
    mail_zone.leds_min      = 1;
    mail_zone.leds_max      = 1;
    mail_zone.leds_count    = 1;
    mail_zone.matrix_map    = NULL;
    zones.push_back(mail_zone);

    led mail_led;
    mail_led.name           = "LED";
    leds.push_back(mail_led);

    SetupColors();
}

void RGBController_DreamCheeky::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*-----------------------------------------------------*\
    | This device does not support resizing zones           |
    \*-----------------------------------------------------*/
}

void RGBController_DreamCheeky::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SetColor(red, grn, blu);
}

void RGBController_DreamCheeky::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_DreamCheeky::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_DreamCheeky::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_DreamCheeky::DeviceSaveMode()
{
    /*-----------------------------------------------------*\
    | This device does not support saving                   |
    \*-----------------------------------------------------*/
}
