/*-----------------------------------------*\
|  RGBController_PhilipsHue.cpp             |
|                                           |
|  Generic RGB Interface for Philips Hue    |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/15/2020   |
\*-----------------------------------------*/

#include "RGBController_PhilipsHue.h"

RGBController_PhilipsHue::RGBController_PhilipsHue(PhilipsHueController* light_ptr)
{
    light = light_ptr;

    name        = light->GetManufacturer() + " " + light->GetName();
    type        = DEVICE_TYPE_LIGHT;
    version     = light->GetVersion();
    description = "Philips Hue Device";
    serial      = light->GetUniqueID();
    location    = light->GetLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

void RGBController_PhilipsHue::SetupZones()
{
    zone led_zone;
    led_zone.name       = "RGB Light";
    led_zone.type       = ZONE_TYPE_SINGLE;
    led_zone.leds_min   = 1;
    led_zone.leds_max   = 1;
    led_zone.leds_count = 1;
    led_zone.matrix_map = NULL;
    zones.push_back(led_zone);

    led new_led;
    new_led.name = "RGB Light";

    leds.push_back(new_led);

    SetupColors();
}

void RGBController_PhilipsHue::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PhilipsHue::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    light->SetColor(red, grn, blu);
}

void RGBController_PhilipsHue::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PhilipsHue::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PhilipsHue::SetCustomMode()
{

}

void RGBController_PhilipsHue::DeviceUpdateMode()
{

}
