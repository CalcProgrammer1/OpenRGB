/*---------------------------------------------------------*\
| RGBController_CorsairHydroPlatinum.cpp                    |
|                                                           |
|   RGBController for Corsair Hydro Platinum coolers        |
|                                                           |
|   Kasper                                      28 Mar 2021 |
|   Nikola Jurkovic (jurkovic.nikola)           13 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_CorsairHydroPlatinum.h"

#define NA 0xFFFFFFFF
static unsigned int matrix_map[5][5] =
{
    { NA,  11,  12,  13,  NA },
    { 10,  NA,  1,   NA,  14 },
    { 9,   0,   NA,  2,   15 },
    { 8,   NA,  3,   NA,   4 },
    { NA,  7,   6,   5,   NA }
};

/**------------------------------------------------------------------*\
    @name Corsair Hydro Platinum
    @category Cooler
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairHydroPlatinumControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_CorsairHydroPlatinum::RGBController_CorsairHydroPlatinum(CorsairHydroPlatinumController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetName();
    vendor      = "Corsair";
    description = "Corsair Hydro Platinum Series Device";
    type        = DEVICE_TYPE_COOLER;
    location    = controller->GetLocation();
    version     = controller->GetFirmwareString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_CorsairHydroPlatinum::~RGBController_CorsairHydroPlatinum()
{
    Shutdown();

    delete controller;
}

void RGBController_CorsairHydroPlatinum::SetupZones()
{
    /*-----------------------------------------------------*\
    | Only set LED count on the first run                   |
    \*-----------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-----------------------------------------------------*\
    | Clear any existing color/LED configuration            |
    \*-----------------------------------------------------*/
    leds.clear();
    colors.clear();
    if(controller->HaveRgbFan())
    {
        zones.resize(2);
    }
    else
    {
        zones.resize(1);
    }

    /*-----------------------------------------------------*\
    | Set up zones                                          |
    \*-----------------------------------------------------*/
    zones[0].name                           = "CPU Block";
    zones[0].type                           = ZONE_TYPE_MATRIX;
    zones[0].leds_min                       = 16;
    zones[0].leds_max                       = 16;
    zones[0].leds_count                     = 16;
    zones[0].matrix_map.Set(5, 5, (unsigned int *)&matrix_map);

    /*-----------------------------------------------------*\
    | If the device is RGB fan-capable, set up fan zone.    |
    \*-----------------------------------------------------*/
    if(controller->HaveRgbFan())
    {
        zones[1].leds_min                   = 0;
        zones[1].leds_max                   = 32;

        if(first_run)
        {
            zones[1].flags                  = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                            | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                            | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                            | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP;
        }

        if(!(zones[1].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
        {
            zones[1].name                   = "Corsair Fan Header";
        }

        if(!(zones[1].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
        {
            zones[1].leds_count             = 0;
        }

        if(!(zones[1].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
        {
            zones[1].type                   = ZONE_TYPE_LINEAR;
        }

        if(!(zones[1].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
        {
            zones[1].matrix_map.width       = 0;
            zones[1].matrix_map.height      = 0;
            zones[1].matrix_map.map.resize(0);
        }
    }

    /*-----------------------------------------------------*\
    | Set up LEDs                                           |
    \*-----------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name = zones[zone_idx].name + ", LED " + std::to_string(led_idx);;
            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_CorsairHydroPlatinum::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        SetupZones();
    }
}

void RGBController_CorsairHydroPlatinum::DeviceUpdateLEDs()
{
    controller->SetupColors(colors);
}

void RGBController_CorsairHydroPlatinum::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairHydroPlatinum::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairHydroPlatinum::DeviceUpdateMode()
{

}
