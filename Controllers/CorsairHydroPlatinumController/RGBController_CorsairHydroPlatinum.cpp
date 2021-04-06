/*-------------------------------------------------------------------*\
|  RGBController_CorsairHydroPlatinum.cpp                             |
|                                                                     |
|  Driver for Corsair Hydro Platinum AIO Coolers                      |
|                                                                     |
|  Kasper         28th March 2021                                     |
|                                                                     |
\*-------------------------------------------------------------------*/

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

RGBController_CorsairHydroPlatinum::RGBController_CorsairHydroPlatinum(CorsairHydroPlatinumController* corsair_ptr)
{
    corsair = corsair_ptr;

    name        = "Corsair Hydro Platinum Series Device";
    vendor      = "Corsair";
    description = "Corsair Hydro Platinum Series Device";
    type        = DEVICE_TYPE_COOLER;
    location    = corsair->GetLocation();
    version     = corsair->GetFirmwareString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    Init_Controller();
    SetupZones();
}

void RGBController_CorsairHydroPlatinum::Init_Controller()
{
    zone cpu_block_zone;
    cpu_block_zone.name               = "CPU Block";
    cpu_block_zone.type               = ZONE_TYPE_MATRIX;
    cpu_block_zone.leds_min           = 16;
    cpu_block_zone.leds_max           = 16;
    cpu_block_zone.leds_count         = 16;
    cpu_block_zone.matrix_map         = new matrix_map_type;
    cpu_block_zone.matrix_map->height = 5;
    cpu_block_zone.matrix_map->width  = 5;
    cpu_block_zone.matrix_map->map    = (unsigned int *)&matrix_map;
    zones.push_back(cpu_block_zone);

    zone fans_zone;
    fans_zone.name       = "Fans";
    fans_zone.type       = ZONE_TYPE_LINEAR;
    fans_zone.leds_min   = 0;
    fans_zone.leds_max   = 32;
    fans_zone.leds_count = 0;
    fans_zone.matrix_map = NULL;
    zones.push_back(fans_zone);
}

void RGBController_CorsairHydroPlatinum::SetupZones()
{
    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    for (unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for (unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name = zones[zone_idx].name + " " + std::to_string(led_idx);;
            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_CorsairHydroPlatinum::ResizeZone(int zone, int new_size)
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

void RGBController_CorsairHydroPlatinum::DeviceUpdateLEDs()
{
    corsair->SetupColors(colors);
}

void RGBController_CorsairHydroPlatinum::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairHydroPlatinum::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairHydroPlatinum::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_CorsairHydroPlatinum::DeviceUpdateMode()
{

}
