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
    controller  = controller_ptr;

    name        = controller->GetNameString();
    vendor      = "Corsair";
    description = "Corsair Commander Core Device";
    version     = controller->GetFirmwareString();
    type        = DEVICE_TYPE_COOLER;
    location    = controller->GetLocationString();
    SetupZones();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);
}

RGBController_CorsairCommanderCore::~RGBController_CorsairCommanderCore()
{
    delete controller;
}

void RGBController_CorsairCommanderCore::SetupZones()
{
    std::atomic<bool> first_run;
    first_run = 0;

    if(zones.size() == 0)
    {
        first_run = 1;
    }

    std::vector<unsigned short int> led_count = controller->GetLedCounts();
    zones.resize(7);
    if(controller->GetPidInt() == CORSAIR_COMMANDER_CORE_XT_PID)
    {
        zones[0].name                   = "External RGB Port";
        zones[0].type                   = ZONE_TYPE_LINEAR;
        zones[0].leds_min               = zones[0].leds_min;
        zones[0].leds_max               = 204;
        zones[0].leds_count             = zones[0].leds_count;
    }
    else
    {
        zones[0].name                   = "Pump";
        zones[0].type                   = ZONE_TYPE_MATRIX;
        zones[0].leds_min               = led_count.at(0);
        zones[0].leds_max               = led_count.at(0);
        zones[0].leds_count             = led_count.at(0);
        zones[0].matrix_map             = new matrix_map_type;
        if(led_count.at(0) == 24)
        {
            zones[0].matrix_map->height     = 11;
            zones[0].matrix_map->width      = 11;
            zones[0].matrix_map->map        = (unsigned int *)&matrix_map24;
        }
        else
        {
            zones[0].matrix_map->height     = 7;
            zones[0].matrix_map->width      = 7;
            zones[0].matrix_map->map        = (unsigned int *)&matrix_map29;
        }
    }

    for(unsigned int i = 1; i < (CORSAIR_COMMANDER_CORE_NUM_CHANNELS + 1); i++)
    {
        zones[i].name               = "RGB Port " + std::to_string(i);
        zones[i].type               = ZONE_TYPE_LINEAR;
        zones[i].leds_min           = 0;
        zones[i].leds_max           = 34;

        if(first_run)
        {
            zones[i].leds_count     = (led_count.size() > i) ? led_count.at(i) : 0;
        }
    }

    leds.clear();
    colors.clear();

    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name            = zones[zone_idx].name + " LED " + std::to_string(led_idx+1);

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_CorsairCommanderCore::ResizeZone(int zone, int new_size)
{
    if((size_t) zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;
        if(zone == 0 && controller->GetPidInt() == CORSAIR_COMMANDER_CORE_XT_PID)
        {
            if(new_size > 0)
            {
                controller->SetFanMode(true);
                controller->SetLedAmount(new_size);
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

void RGBController_CorsairCommanderCore::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairCommanderCore::UpdateSingleLED(int /*led*/)
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
