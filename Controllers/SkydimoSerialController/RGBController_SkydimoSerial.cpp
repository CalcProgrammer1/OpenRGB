/*---------------------------------------------------------*\
| RGBController_SkydimoSerial.cpp                           |
|                                                           |
|   RGBController for Skydimo serial devices                |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  30 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_SkydimoSerial.h"

/**------------------------------------------------------------------*\
    @name Skydimo Serial
    @category LEDStrip
    @type Serial
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectSkydimoSerialControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_SkydimoSerial::RGBController_SkydimoSerial(SkydimoSerialController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetName();
    vendor              = "Skydimo";
    type                = DEVICE_TYPE_LEDSTRIP;
    description         = "Skydimo Serial Device";
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

RGBController_SkydimoSerial::~RGBController_SkydimoSerial()
{
    Shutdown();

    controller->SetBlack();
    delete controller;
}

void RGBController_SkydimoSerial::SetupZones()
{
    bool first_run = zones.empty();

    leds.clear();
    colors.clear();
    zones.resize(1);

    zones[0].leds_min = controller->GetMinLEDCount();
    zones[0].leds_max = controller->GetMaxLEDCount();

    if(first_run)
    {
        if(controller->IsResizable())
        {
            zones[0].flags = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                           | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                           | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                           | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP
                           | ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS;
        }
    }

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
    {
        zones[0].name = "Output 1";
    }

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
    {
        zones[0].leds_count = controller->GetLEDCount();
    }

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
    {
        zones[0].type = controller->GetMatrixMap().empty() ? ZONE_TYPE_LINEAR : ZONE_TYPE_MATRIX;
    }

    if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
    {
        zones[0].matrix_map.height = controller->GetMatrixHeight();
        zones[0].matrix_map.width  = controller->GetMatrixWidth();
        zones[0].matrix_map.map    = controller->GetMatrixMap();
    }

    for(unsigned int led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name = zones[0].name + ", LED " + std::to_string(led_idx + 1);

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_SkydimoSerial::DeviceConfigureZone(int zone_idx)
{
    unsigned int zone_index = (unsigned int)zone_idx;

    if(zone_index < zones.size())
    {
        if(zones[zone_index].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE)
        {
            controller->SetLEDCount(zones[zone_index].leds_count);
        }
        else
        {
            controller->SetLEDCount(controller->GetMinLEDCount());
        }

        zones[zone_index].leds_count = controller->GetLEDCount();
        SetupZones();
    }
}

void RGBController_SkydimoSerial::DeviceUpdateLEDs()
{
    controller->SetLEDs(colors);
}

void RGBController_SkydimoSerial::DeviceUpdateZoneLEDs(int)
{
    DeviceUpdateLEDs();
}

void RGBController_SkydimoSerial::DeviceUpdateSingleLED(int)
{
    DeviceUpdateLEDs();
}

void RGBController_SkydimoSerial::DeviceUpdateMode()
{
}
