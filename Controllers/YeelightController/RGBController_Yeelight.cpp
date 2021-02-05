/*-----------------------------------------*\
|  RGBController_Yeelight.cpp               |
|                                           |
|  Generic RGB Interface for Yeelight       |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/18/2021   |
\*-----------------------------------------*/

#include "RGBController_Yeelight.h"

RGBController_Yeelight::RGBController_Yeelight(YeelightController* light_ptr)
{
    light = light_ptr;

    name        = light->GetName();
    vendor      = light->GetManufacturer();
    type        = DEVICE_TYPE_LIGHT;
    version     = light->GetVersion();
    description = "Yeelight Device";
    serial      = light->GetUniqueID();
    location    = light->GetLocation();

    /*---------------------------------------------*\
    | Yeelight standard control does not support    |
    | fast refreshing.  We need to implement music  |
    | mode, so for now name the mode "Static"       |
    \*---------------------------------------------*/
    mode Direct;
    Direct.name       = "Static";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_Yeelight::~RGBController_Yeelight()
{
    delete light;
}

void RGBController_Yeelight::SetupZones()
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

void RGBController_Yeelight::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Yeelight::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    light->SetColor(red, grn, blu);
}

void RGBController_Yeelight::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Yeelight::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Yeelight::SetCustomMode()
{

}

void RGBController_Yeelight::DeviceUpdateMode()
{

}
