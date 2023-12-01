/*-----------------------------------------*\
|  RGBController_Govee.cpp                  |
|                                           |
|  Generic RGB Interface for Govee          |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/27/2023  |
\*-----------------------------------------*/

#include "RGBController_Govee.h"

RGBController_Govee::RGBController_Govee(GoveeController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Govee";
    vendor      = "Govee";
    type        = DEVICE_TYPE_DRAM;
    description = "Govee Device";
    location    = controller->GetLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_Govee::~RGBController_Govee()
{

}

void RGBController_Govee::SetupZones()
{
    zone strip;
    strip.name          = "Govee Strip";
    strip.type          = ZONE_TYPE_SINGLE;
    strip.leds_count    = 1;
    strip.leds_min      = 1;
    strip.leds_max      = 1;
    strip.matrix_map    = NULL;
    zones.push_back(strip);

    led strip_led;
    strip_led.name      = "Govee Strip";
    leds.push_back(strip_led);

    SetupColors();
}

void RGBController_Govee::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Govee::DeviceUpdateLEDs()
{
    controller->SetColor(RGBGetRValue(colors[0]), RGBGetGValue(colors[0]), RGBGetBValue(colors[0]), 255);
}

void RGBController_Govee::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Govee::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Govee::DeviceUpdateMode()
{

}