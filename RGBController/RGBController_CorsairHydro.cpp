/*-----------------------------------------*\
|  RGBController_CorsairHydro.cpp           |
|                                           |
|  Generic RGB Interface for Corsair        |
|  Hydro Series                             |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/18/2020   |
\*-----------------------------------------*/

#include "RGBController_CorsairHydro.h"

RGBController_CorsairHydro::RGBController_CorsairHydro(CorsairHydroController* corsair_ptr)
{
    corsair = corsair_ptr;

    description = "Corsair Hydro Series Device";
    version     = corsair->GetFirmwareString();
    type        = DEVICE_TYPE_COOLER;

    mode Direct;
    Direct.name = "Direct";
    Direct.value = 0;
    Direct.flags = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

void RGBController_CorsairHydro::SetupZones()
{
    zone new_zone;
    
    new_zone.name       = "Pump Zone";
    new_zone.type       = ZONE_TYPE_SINGLE;
    new_zone.leds_min   = 1;
    new_zone.leds_max   = 1;
    new_zone.leds_count = 1;
    new_zone.matrix_map = NULL;
    zones.push_back(new_zone);

    led new_led;

    new_led.name        = "Pump LED";
    leds.push_back(new_led);

    SetupColors();
}

void RGBController_CorsairHydro::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_CorsairHydro::DeviceUpdateLEDs()
{
    corsair->SetFixed(RGBGetRValue(colors[0]), RGBGetGValue(colors[0]), RGBGetBValue(colors[0]));
}

void RGBController_CorsairHydro::UpdateZoneLEDs(int zone)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairHydro::UpdateSingleLED(int led)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairHydro::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_CorsairHydro::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}