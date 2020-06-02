/*-----------------------------------------*\
|  RGBController_GloriousModelO.cpp         |
|                                           |
|  Definitions and types for Glorious       |
|  and other Mice                           |
|                                           |
|  Niels Westphal (crashniels) 20/5/2020    |
\*-----------------------------------------*/

#include "RGBController_GloriousModelO.h"

RGBController_GloriousModelO::RGBController_GloriousModelO(GloriousModelOController* gmo_ptr)
{
    gmo = gmo_ptr;

    name        = gmo->GetDeviceName();
    type        = DEVICE_TYPE_MOUSE;
    description = "Glorious Device";

    mode Static;
    Static.name       = "Static";
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    SetupZones();
}

void RGBController_GloriousModelO::SetupZones()
{
    /*---------------------------------------------------------*\
    | Create a single zone                                      |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name       = "Glorious Zone";
    new_zone.type       = ZONE_TYPE_SINGLE;
    new_zone.leds_min   = gmo->GetLEDCount();
    new_zone.leds_max   = gmo->GetLEDCount();
    new_zone.leds_count = gmo->GetLEDCount();
    new_zone.matrix_map = NULL;
    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led* new_led = new led();
        new_led->name = "Glorious LED";
        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_GloriousModelO::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_GloriousModelO::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    gmo->SetLEDColor(red, grn, blu);
}

void RGBController_GloriousModelO::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GloriousModelO::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}
void RGBController_GloriousModelO::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_GloriousModelO::UpdateMode()
{
    gmo->SetMode(modes[active_mode].value);
}



