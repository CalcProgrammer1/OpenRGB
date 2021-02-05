/*-----------------------------------------*\
|  RGBController_CorsairVengeance.cpp       |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Corsair Vengeance RGB driver             |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/13/2019   |
\*-----------------------------------------*/

#include "RGBController_CorsairVengeance.h"

RGBController_CorsairVengeance::RGBController_CorsairVengeance(CorsairVengeanceController* corsair_ptr)
{
    corsair = corsair_ptr;

    name        = corsair->GetDeviceName();
    vendor      = "Corsair";
    type        = DEVICE_TYPE_DRAM;
    description = "Corsair Vengeance RGB Device";
    location    = corsair->GetDeviceLocation();

    mode Static;
    Static.name       = "Static";
    Static.value      = CORSAIR_VENGEANCE_RGB_MODE_SINGLE;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Fade;
    Fade.name       = "Fade";
    Fade.value      = CORSAIR_VENGEANCE_RGB_MODE_FADE;
    Fade.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Fade.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Fade);

    mode Pulse;
    Pulse.name       = "Pulse";
    Pulse.value      = CORSAIR_VENGEANCE_RGB_MODE_PULSE;
    Pulse.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Pulse.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Pulse);

    SetupZones();
}

RGBController_CorsairVengeance::~RGBController_CorsairVengeance()
{
    delete corsair;
}

void RGBController_CorsairVengeance::SetupZones()
{
    /*---------------------------------------------------------*\
    | Create a single zone                                      |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name       = "Corsair Zone";
    new_zone.type       = ZONE_TYPE_SINGLE;
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
        led* new_led = new led();
        new_led->name = "Corsair LED";
        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_CorsairVengeance::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairVengeance::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    corsair->SetLEDColor(red, grn, blu);
}

void RGBController_CorsairVengeance::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairVengeance::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairVengeance::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_CorsairVengeance::DeviceUpdateMode()
{
    corsair->SetMode(modes[active_mode].value);
}
