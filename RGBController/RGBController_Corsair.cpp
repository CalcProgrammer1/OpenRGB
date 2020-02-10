/*-----------------------------------------*\
|  RGBController_Corsair.cpp                |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Corsair Vengeance RGB driver             |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/13/2019   |
\*-----------------------------------------*/

#include "RGBController_Corsair.h"

void RGBController_Corsair::UpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    corsair->SetLEDColor(red, grn, blu);
}

void RGBController_Corsair::UpdateZoneLEDs(int zone)
{
    UpdateLEDs();
}

void RGBController_Corsair::UpdateSingleLED(int led)
{
    UpdateLEDs();
}

RGBController_Corsair::RGBController_Corsair(CorsairController* corsair_ptr)
{
    corsair = corsair_ptr;

    name = corsair->GetDeviceName();
    location = corsair->GetDeviceLocation();

    type = DEVICE_TYPE_DRAM;

    mode Static;
    Static.name  = "Static";
    Static.value = CORSAIR_VENGEANCE_RGB_MODE_SINGLE;
    Static.flags = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    modes.push_back(Static);

    mode Fade;
    Fade.name  = "Fade";
    Fade.value = CORSAIR_VENGEANCE_RGB_MODE_FADE;
    Fade.flags = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    modes.push_back(Fade);

    mode Pulse;
    Pulse.name  = "Pulse";
    Pulse.value = CORSAIR_VENGEANCE_RGB_MODE_PULSE;
    Pulse.flags = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    modes.push_back(Pulse);

    for (unsigned int i = 0; i < corsair->GetLEDCount(); i++)
    {
        led* new_led = new led();

        new_led->name = "Corsair LED";

        leds.push_back(*new_led);
        colors.push_back(0x00000000);
    }

    zone new_zone;

    new_zone.name = "Corsair Zone";
    new_zone.type = ZONE_TYPE_SINGLE;

    std::vector<int> zone_row;

    for (unsigned int i = 0; i < corsair->GetLEDCount(); i++)
    {
        zone_row.push_back(i);
    }

    new_zone.map.push_back(zone_row);

    zones.push_back(new_zone);
}

void RGBController_Corsair::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_Corsair::UpdateMode()
{
    corsair->SetMode(modes[active_mode].value);
}