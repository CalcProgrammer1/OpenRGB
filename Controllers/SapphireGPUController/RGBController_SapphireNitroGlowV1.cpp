/*-----------------------------------------*\
|  RGBController_SapphireNitroGlowV1.cpp    |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Sapphire Nitro Glow V1 GPU Driver        |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/15/2020   |
\*-----------------------------------------*/

#include "RGBController_SapphireNitroGlowV1.h"

RGBController_SapphireNitroGlowV1::RGBController_SapphireNitroGlowV1(SapphireNitroGlowV1Controller* sapphire_ptr)
{
    sapphire = sapphire_ptr;

    name        = "Sapphire Nitro Glow V1 Device";
    vendor      = "Sapphire";
    description = "Sapphire Nitro Glow V1 Device";
    location    = sapphire->GetDeviceLocation();

    type = DEVICE_TYPE_GPU;

    mode Static;
    Static.name       = "Static";
    Static.value      = 0x04;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Rainbow;
    Rainbow.name       = "Rainbow";
    Rainbow.value      = 0x01;
    Rainbow.flags      = 0;
    Rainbow.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    SetupZones();

    // Initialize active mode
    active_mode = 0;
}

void RGBController_SapphireNitroGlowV1::SetupZones()
{
    /*---------------------------------------------------------*\
    | This device only has one LED, so create a single zone and |
    | LED for it                                                |
    \*---------------------------------------------------------*/
    zone* new_zone = new zone();
    led*  new_led  = new led();

    new_zone->name          = "GPU Zone";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 1;
    new_zone->leds_count    = 1;
    new_zone->matrix_map    = NULL;

    new_led->name           = "GPU LED";

    /*---------------------------------------------------------*\
    | Push the zone and LED on to device vectors                |
    \*---------------------------------------------------------*/
    leds.push_back(*new_led);
    zones.push_back(*new_zone);

    SetupColors();
}

void RGBController_SapphireNitroGlowV1::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SapphireNitroGlowV1::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    sapphire->SetColor(red, grn, blu);
}

void RGBController_SapphireNitroGlowV1::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SapphireNitroGlowV1::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SapphireNitroGlowV1::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_SapphireNitroGlowV1::DeviceUpdateMode()
{
    sapphire->SetMode((unsigned char)modes[(unsigned int)active_mode].value, (unsigned char)modes[(unsigned int)active_mode].speed);
}