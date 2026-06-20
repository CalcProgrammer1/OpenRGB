/*---------------------------------------------------------*\
| RGBController_LogitechG522.cpp                            |
|                                                           |
|   RGBController for Logitech G522 headset                 |
|                                                           |
|   fawmdev                                     03 Dec 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_LogitechG522.h"

/**------------------------------------------------------------------*\
    @name Logitech G522
    @category Headset
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectLogitechG522
    @comment
\*-------------------------------------------------------------------*/

RGBController_LogitechG522::RGBController_LogitechG522(LogitechG522Controller* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "Logitech";
    type                    = DEVICE_TYPE_HEADSET;
    description             = "Logitech G522 Gaming Headset";
    location                = controller->GetDeviceLocation();

    mode Off;
    Off.name                = "Off";
    Off.value               = LOGITECH_G522_MODE_OFF;
    Off.flags               = 0;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = LOGITECH_G522_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

void RGBController_LogitechG522::SetupZones()
{
    zone G522_zone;
    G522_zone.name          = "Headset";
    G522_zone.type          = ZONE_TYPE_SINGLE;
    G522_zone.leds_min      = 1;
    G522_zone.leds_max      = 1;
    G522_zone.leds_count    = 1;
    G522_zone.matrix_map    = NULL;
    zones.push_back(G522_zone);

    led G522_led;
    G522_led.name           = "Headset LED";
    G522_led.value          = 0x00;
    leds.push_back(G522_led);

    SetupColors();
}

void RGBController_LogitechG522::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechG522::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SetColor(red, grn, blu);
}

void RGBController_LogitechG522::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG522::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG522::DeviceUpdateMode()
{
    if(modes[active_mode].value == LOGITECH_G522_MODE_OFF)
    {
        controller->SetOff();
    }
    else
    {
        DeviceUpdateLEDs();
    }
}
