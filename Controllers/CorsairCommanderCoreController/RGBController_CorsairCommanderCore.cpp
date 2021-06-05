/*-----------------------------------------*\
|  RGBController_CorsairCapellix.cpp        |
|                                           |
|  Generic RGB Interface for Corsair        |
|  Commander Core                           |
|                                           |
|  Jeff P.                                  |
\*-----------------------------------------*/

#include "RGBController_CorsairCommanderCore.h"

RGBController_CorsairCommanderCore::RGBController_CorsairCommanderCore(CorsairCommanderCoreController* corsair_ptr)
{
    corsair     = corsair_ptr;

    vendor      = "Corsair";
    description = "Corsair Commander Core";
    type        = DEVICE_TYPE_COOLER;
    location    = corsair->GetLocationString();

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
    delete corsair;
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
    zones[0].type                   = ZONE_TYPE_LINEAR;
    zones[0].leds_min               = 0;
    zones[0].leds_max               = 29;

    for(unsigned int i = 1; i < (CORSAIR_COMMANDER_CORE_NUM_CHANNELS + 1); i++)
    {
        zones[i].name               = "RGB Port " + std::to_string(i);
        zones[i].type               = ZONE_TYPE_LINEAR;
        zones[i].leds_min           = 0;
        zones[i].leds_max           = 34;

        if(first_run)
        {
            zones[i].leds_count = 0;
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
            corsair->SetDirectColor(colors, zones);
            break;
    }
}
