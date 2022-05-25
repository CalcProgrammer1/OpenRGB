/*-----------------------------------------*\
|  RGBController_CorsairCapellix.cpp        |
|                                           |
|  Generic RGB Interface for Corsair        |
|  Commander Core                           |
|                                           |
|  Jeff P.                                  |
\*-----------------------------------------*/

#include "RGBController_CorsairCommanderCore.h"

/**------------------------------------------------------------------*\
    @name Corsair Commander Core
    @category Cooler
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairCapellixHIDControllers
    @comment
\*-------------------------------------------------------------------*/

#define NA 0xFFFFFFFF
static unsigned int matrix_map[7][7] =
{
    { 28,  NA,  27,  NA,  26,  NA,  25 },
    { NA,  16,  NA,  15,  NA,  14,  NA },
    { 17,  NA,   0,   5,   3,  NA,  24 },
    { NA,   9,   4,   8,   6,  13,  NA },
    { 18,  NA,   1,   7,   2,  NA,  23 },
    { NA,  10,  NA,  11,  NA,  12,  NA },
    { 19,  NA,  20,  NA,  21,  NA,  22 },
};

RGBController_CorsairCommanderCore::RGBController_CorsairCommanderCore(CorsairCommanderCoreController* controller_ptr)
{
    controller  = controller_ptr;

    vendor      = "Corsair";
    description = "Corsair Commander Core";
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

    zones.resize(7);
    zones[0].name                   = "Pump";
    zones[0].type                   = ZONE_TYPE_MATRIX;
    zones[0].leds_min               = 29;
    zones[0].leds_max               = 29;
    zones[0].leds_count             = 29;
    zones[0].matrix_map             = new matrix_map_type;
    zones[0].matrix_map->height     = 7;
    zones[0].matrix_map->width      = 7;
    zones[0].matrix_map->map        = (unsigned int *)&matrix_map;

    for(unsigned int i = 1; i < (CORSAIR_COMMANDER_CORE_NUM_CHANNELS + 1); i++)
    {
        zones[i].name               = "RGB Port " + std::to_string(i);
        zones[i].type               = ZONE_TYPE_LINEAR;
        zones[i].leds_min           = 0;
        zones[i].leds_max           = 34;

        if(first_run)
        {
            zones[i].leds_count     = 0;
        }
    }

    leds.clear();
    colors.clear();

    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for (unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
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

void RGBController_CorsairCommanderCore::SetCustomMode()
{
    active_mode = 0;
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
