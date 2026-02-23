/*---------------------------------------------------------*\
| RGBController_CorsairCCommanderCore.cpp                   |
|                                                           |
|   RGBController for Corsair Commander Core                |
|                                                           |
|   Jeff P.                                                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_CorsairCommanderCore.h"

/**------------------------------------------------------------------*\
    @name Corsair Commander Core
    @category Cooler
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairCommanderCoreControllers
    @comment
\*-------------------------------------------------------------------*/

#define NA 0xFFFFFFFF
static unsigned int matrix_map29[7][7] =
{
    { 28,  NA,  27,  NA,  26,  NA,  25 },
    { NA,  16,  NA,  15,  NA,  14,  NA },
    { 17,  NA,   0,   5,   3,  NA,  24 },
    { NA,   9,   4,   8,   6,  13,  NA },
    { 18,  NA,   1,   7,   2,  NA,  23 },
    { NA,  10,  NA,  11,  NA,  12,  NA },
    { 19,  NA,  20,  NA,  21,  NA,  22 },
};

static unsigned int matrix_map24[11][11] =
{
    { NA,  NA,  NA,  NA,  NA,   6,  NA,  NA,  NA,  NA,  NA },
    { NA,  NA,  NA,   4,   5,  NA,   7,   8,  NA,  NA,  NA },
    { NA,  NA,   3,  NA,  NA,  NA,  NA,  NA,   9,  NA,  NA },
    { NA,   2,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  10,  NA },
    { NA,   1,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  11,  NA },
    {  0,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  12 },
    { NA,  23,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  13,  NA },
    { NA,  22,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  14,  NA },
    { NA,  NA,  21,  NA,  NA,  NA,  NA,  NA,  15,  NA,  NA },
    { NA,  NA,  NA,  20,  19,  NA,  17,  16,  NA,  NA,  NA },
    { NA,  NA,  NA,  NA,  NA,  18,  NA,  NA,  NA,  NA,  NA }
};

RGBController_CorsairCommanderCore::RGBController_CorsairCommanderCore(CorsairCommanderCoreController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetNameString();
    vendor              = "Corsair";
    description         = "Corsair Commander Core Device";
    version             = controller->GetFirmwareString();
    type                = DEVICE_TYPE_COOLER;
    location            = controller->GetLocationString();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_CorsairCommanderCore::~RGBController_CorsairCommanderCore()
{
    Shutdown();

    delete controller;
}

void RGBController_CorsairCommanderCore::SetupZones()
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
    zones.resize(7);

    /*-----------------------------------------------------*\
    | Set up zones                                          |
    \*-----------------------------------------------------*/
    std::vector<unsigned short int> led_count = controller->GetLedCounts();

    if(controller->GetPidInt() == CORSAIR_COMMANDER_CORE_XT_PID)
    {
        zones[0].leds_min                           = 0;
        zones[0].leds_max                           = 204;

        if(first_run)
        {
            zones[0].flags                          = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP;
        }

        if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
        {
            zones[0].name                           = "Corsair RGB Header";
        }

        if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
        {
            zones[0].leds_count                     = 0;
        }

        if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
        {
            zones[0].type                           = ZONE_TYPE_LINEAR;
        }

        if(!(zones[0].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
        {
            zones[0].matrix_map.width               = 0;
            zones[0].matrix_map.height              = 0;
            zones[0].matrix_map.map.resize(0);
        }
    }
    else
    {
        zones[0].name                               = "Pump";
        zones[0].type                               = ZONE_TYPE_MATRIX;
        zones[0].leds_min                           = led_count.at(0);
        zones[0].leds_max                           = led_count.at(0);
        zones[0].leds_count                         = led_count.at(0);

        if(led_count.at(0) == 24)
        {
            zones[0].matrix_map.Set(11, 11, (unsigned int *)&matrix_map24);
        }
        else
        {
            zones[0].matrix_map.Set(7, 7, (unsigned int *)matrix_map29);
        }
    }

    for(unsigned int zone_idx = 1; zone_idx < (CORSAIR_COMMANDER_CORE_NUM_CHANNELS + 1); zone_idx++)
    {
        zones[zone_idx].leds_min                    = 0;
        zones[zone_idx].leds_max                    = 34;

        if(first_run)
        {
            zones[zone_idx].flags                   = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
        {
            zones[zone_idx].name                    = "Corsair Fan Header " + std::to_string(zone_idx);
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
        {
            zones[zone_idx].leds_count              = 0;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
        {
            zones[zone_idx].type                    = ZONE_TYPE_LINEAR;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
        {
            zones[zone_idx].matrix_map.width        = 0;
            zones[zone_idx].matrix_map.height       = 0;
            zones[zone_idx].matrix_map.map.resize(0);
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
            new_led.name            = zones[zone_idx].name + ", LED " + std::to_string(led_idx+1);

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_CorsairCommanderCore::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        if(zone_idx == 0 && controller->GetPidInt() == CORSAIR_COMMANDER_CORE_XT_PID)
        {
            if(zones[zone_idx].leds_count > 0)
            {
                controller->SetFanMode(true);
                controller->SetLedAmount(zones[zone_idx].leds_count);
            }
            else
            {
                controller->SetFanMode(false);
            }
        }

        SetupZones();
    }
}

void RGBController_CorsairCommanderCore::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_CorsairCommanderCore::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairCommanderCore::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairCommanderCore::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case CORSAIR_COMMANDER_CORE_MODE_DIRECT:
            controller->SetDirectColor(colors, zones);
            break;
    }
}
