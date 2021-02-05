/*-----------------------------------------*\
|  RGBController_Espurna.cpp                |
|                                           |
|  Generic RGB Interface for Espurna        |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/20/2019   |
\*-----------------------------------------*/

#include "RGBController_Espurna.h"

RGBController_Espurna::RGBController_Espurna(EspurnaController* espurna_ptr)
{
    espurna = espurna_ptr;

    name        = "Espurna";
    type        = DEVICE_TYPE_LIGHT;
    description = "Espurna Device";
    location    = espurna->GetLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_Espurna::~RGBController_Espurna()
{
    delete espurna;
}

void RGBController_Espurna::SetupZones()
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

void RGBController_Espurna::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Espurna::DeviceUpdateLEDs()
{
    espurna->SetLEDs(colors);
}

void RGBController_Espurna::UpdateZoneLEDs(int /*zone*/)
{
    espurna->SetLEDs(colors);
}

void RGBController_Espurna::UpdateSingleLED(int /*led*/)
{
    espurna->SetLEDs(colors);
}

void RGBController_Espurna::SetCustomMode()
{

}

void RGBController_Espurna::DeviceUpdateMode()
{

}
