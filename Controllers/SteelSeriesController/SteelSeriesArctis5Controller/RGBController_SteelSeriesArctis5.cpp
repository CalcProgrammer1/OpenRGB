/*---------------------------------------------------------*\
| RGBController_SteelSeriesArctis5.cpp                      |
|                                                           |
|   RGBController for SteelSeries Arctis 5                  |
|                                                           |
|   Morgan Guimard                              04 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_SteelSeriesArctis5.h"

/**------------------------------------------------------------------*\
    @name Steelseries Arctis 5
    @category Headset
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectSteelSeriesArctis5
    @comment
\*-------------------------------------------------------------------*/

RGBController_SteelSeriesArctis5::RGBController_SteelSeriesArctis5(SteelSeriesArctis5Controller* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "SteelSeries";
    type                    = DEVICE_TYPE_HEADSET;
    description             = "SteelSeries Arctis 5 Headset Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0x00;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;

    modes.push_back(Direct);

    SetupZones();
}

RGBController_SteelSeriesArctis5::~RGBController_SteelSeriesArctis5()
{
    delete controller;
}

void RGBController_SteelSeriesArctis5::SetupZones()
{
    const std::string zone_names[2] =
    {
        "Left" , "Right"
    };

    for(const std::string& zone_name : zone_names)
    {
        zone zone;
        zone.name           = zone_name;
        zone.type           = ZONE_TYPE_SINGLE;
        zone.leds_min       = 1;
        zone.leds_max       = 1;
        zone.leds_count     = 1;
        zone.matrix_map     = NULL;
        zones.push_back(zone);

        led mouse_led;
        mouse_led.name      = zone_name;
        leds.push_back(mouse_led);
    }

    SetupColors();
}

void RGBController_SteelSeriesArctis5::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SteelSeriesArctis5::DeviceUpdateLEDs()
{
    for(unsigned int i = 0; i < zones.size(); i++)
    {
        UpdateZoneLEDs(i);
    }
}

void RGBController_SteelSeriesArctis5::UpdateZoneLEDs(int zone)
{
   controller->SetColor(zone, colors[zone]);
}

void RGBController_SteelSeriesArctis5::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_SteelSeriesArctis5::DeviceUpdateMode()
{

}
