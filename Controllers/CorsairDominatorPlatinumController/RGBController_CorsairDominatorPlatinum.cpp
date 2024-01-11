/*-------------------------------------------------*\
|  RGBController_CorsairDominatorPlatinum.cpp       |
|                                                   |
|  Corsair Vengeance Pro RGB driver                 |
|                                                   |
|  Erik Gilling (konkers)       9/25/2020           |
\*-------------------------------------------------*/

#include "RGBController_CorsairDominatorPlatinum.h"

/**------------------------------------------------------------------*\
    @name Corsair Dominator Platinum
    @category RAM
    @type SMBus
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCorsairDominatorPlatinumControllers
    @comment
        The Corsair Dominator controller chip can be found on several
        Corsair memory sticks which have different LED counts. This can be controlled
        by editing the Part Number in OpenRGB.json with values in the below table.

        | Part Number | LED Count |
        | :---------: | --------: |
        | CMG         |  6        |
        | CMH         | 10        |
        | CMN         | 10        |
        | CMT         | 12        |
\*-------------------------------------------------------------------*/

RGBController_CorsairDominatorPlatinum::RGBController_CorsairDominatorPlatinum(CorsairDominatorPlatinumController* controller_ptr)
{
    controller  = controller_ptr;

    vendor      = "Corsair";
    type        = DEVICE_TYPE_DRAM;
    description = "Corsair RAM RGB Device";
    location    = controller->GetDeviceLocation();

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
    delete controller;
}

void RGBController_CorsairDominatorPlatinum::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name       = "Corsair RAM Zone";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = controller->GetLEDCount();
    new_zone.leds_max   = controller->GetLEDCount();
    new_zone.leds_count = controller->GetLEDCount();
    new_zone.matrix_map = NULL;
    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name   = "Corsair RAM LED ";
        new_led.name.append(std::to_string(led_idx));
        leds.push_back(new_led);
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

        controller->SetLEDColor(led, red, grn, blu);
    }

    controller->ApplyColors();
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

    controller->SetLEDColor(led, red, grn, blu);
    controller->ApplyColors();
}

void RGBController_CorsairDominatorPlatinum::DeviceUpdateMode()
{

}
