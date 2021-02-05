/*-------------------------------------------------*\
|  RGBController_CorsairDominatorPlatinum.cpp       |
|                                                   |
|  Corsair Vengeance Pro RGB driver                 |
|                                                   |
|  Erik Gilling (konkers)       9/25/2020           |
\*-------------------------------------------------*/

#include "RGBController_CorsairDominatorPlatinum.h"

RGBController_CorsairDominatorPlatinum::RGBController_CorsairDominatorPlatinum(CorsairDominatorPlatinumController* corsair_ptr)
{
    corsair = corsair_ptr;

    name        = corsair->GetDeviceName();
    vendor      = "Corsair";
    type        = DEVICE_TYPE_DRAM;
    description = "Corsair Dominator Platinum RGB Device";
    location    = corsair->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.speed_min  = 0;
    Direct.speed_max  = 0;
    Direct.speed      = 0;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    active_mode = 0;
}

RGBController_CorsairDominatorPlatinum::~RGBController_CorsairDominatorPlatinum()
{
    delete corsair;
}

void RGBController_CorsairDominatorPlatinum::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name       = "Corsair Platinum Zone";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = corsair->GetLEDCount();
    new_zone.leds_max   = corsair->GetLEDCount();
    new_zone.leds_count = corsair->GetLEDCount();
    new_zone.matrix_map = NULL;
    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led *new_led  = new led();
        new_led->name = "Corsair Platinum LED ";
        new_led->name.append(std::to_string(led_idx));
        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_CorsairDominatorPlatinum::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairDominatorPlatinum::DeviceUpdateLEDs()
{
    for(std::size_t led = 0; led < colors.size(); led++)
    {
        RGBColor color    = colors[led];
        unsigned char red = RGBGetRValue(color);
        unsigned char grn = RGBGetGValue(color);
        unsigned char blu = RGBGetBValue(color);
        corsair->SetLEDColor(led, red, grn, blu);
    }

    corsair->ApplyColors();
}

void RGBController_CorsairDominatorPlatinum::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairDominatorPlatinum::UpdateSingleLED(int led)
{
    RGBColor color    = colors[led];
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    corsair->SetLEDColor(led, red, grn, blu);
    corsair->ApplyColors();
}

void RGBController_CorsairDominatorPlatinum::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_CorsairDominatorPlatinum::DeviceUpdateMode()
{

}
