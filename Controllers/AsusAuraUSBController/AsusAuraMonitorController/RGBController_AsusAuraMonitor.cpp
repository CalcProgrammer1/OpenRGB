/*---------------------------------------------------------*\
| RGBController_AsusAuraMonitor.cpp                         |
|                                                           |
|   RGBController for ASUS Aura monitor                     |
|                                                           |
|   Mola19                                      08 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AsusAuraMonitor.h"

/**------------------------------------------------------------------*\
    @name Asus Aura Monitor
    @category Accessory
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectAsusAuraUSBMonitor
    @comment
\*-------------------------------------------------------------------*/

RGBController_AuraMonitor::RGBController_AuraMonitor(AuraMonitorController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetNameString();
    vendor              = "ASUS";
    type                = DEVICE_TYPE_MONITOR;
    description         = "ASUS Aura Monitor Device";
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

RGBController_AuraMonitor::~RGBController_AuraMonitor()
{
    delete controller;
}

void RGBController_AuraMonitor::SetupZones()
{
    zone underglow_zone;

    underglow_zone.name         = "Backlight";
    underglow_zone.type         = ZONE_TYPE_LINEAR;
    underglow_zone.leds_min     = 3;
    underglow_zone.leds_max     = 3;
    underglow_zone.leds_count   = 3;
    underglow_zone.matrix_map   = NULL;

    zones.push_back(underglow_zone);

    for(unsigned int i = 0; i < 3; i++)
    {
        led underglow_led;

        underglow_led.name      = "Backlight LED " + std::to_string(i + 1);

        leds.push_back(underglow_led);
    }

    SetupColors();
}

void RGBController_AuraMonitor::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AuraMonitor::DeviceUpdateLEDs()
{
    controller->BeginUpdate();

    for (int i = 0; i < 3; i++)
    {
        unsigned char red   = RGBGetRValue(colors[i]);
        unsigned char green = RGBGetGValue(colors[i]);
        unsigned char blue  = RGBGetBValue(colors[i]);

        controller->UpdateLed(i, red, green, blue);
    }

    controller->ApplyChanges();
}

void RGBController_AuraMonitor::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraMonitor::UpdateSingleLED(int led)
{
    controller->BeginUpdate();

    unsigned char red   = RGBGetRValue(colors[led]);
    unsigned char green = RGBGetGValue(colors[led]);
    unsigned char blue  = RGBGetBValue(colors[led]);

    controller->UpdateLed(led, red, green, blue);

    controller->ApplyChanges();
}

void RGBController_AuraMonitor::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | This device does not support Mode changing                |
    \*---------------------------------------------------------*/
}

void RGBController_AuraMonitor::DeviceSaveMode()
{
    /*---------------------------------------------------------*\
    | This device does not support Mode saving                  |
    \*---------------------------------------------------------*/
}
