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

    vendor      = "Corsair";
    description = "Corsair Hydro Series Device";
    version     = corsair->GetFirmwareString();
    type        = DEVICE_TYPE_COOLER;
    location    = corsair->GetLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Blinking;
    Blinking.name       = "Blinking";
    Blinking.value      = 1;
    Blinking.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Blinking.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Blinking.speed_min  = 0x0F;
    Blinking.speed_max  = 0x05;
    Blinking.speed      = 0x0A;
    Blinking.colors_min = 2;
    Blinking.colors_max = 2;
    Blinking.colors.resize(2);
    modes.push_back(Blinking);

    mode ColorShift;
    ColorShift.name       = "Color Shift";
    ColorShift.value      = 2;
    ColorShift.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    ColorShift.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ColorShift.speed_min  = 0x46;
    ColorShift.speed_max  = 0x0F;
    ColorShift.speed      = 0x28;
    ColorShift.colors_min = 2;
    ColorShift.colors_max = 2;
    ColorShift.colors.resize(2);
    modes.push_back(ColorShift);

    mode Pulsing;
    Pulsing.name       = "Pulsing";
    Pulsing.value      = 3;
    Pulsing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Pulsing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Pulsing.speed_min  = 0x50;
    Pulsing.speed_max  = 0x1E;
    Pulsing.speed      = 0x37;
    Pulsing.colors_min = 2;
    Pulsing.colors_max = 2;
    Pulsing.colors.resize(2);
    modes.push_back(Pulsing);

    SetupZones();
}

RGBController_CorsairHydro::~RGBController_CorsairHydro()
{
    delete corsair;
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
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairHydro::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_CorsairHydro::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairHydro::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CorsairHydro::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_CorsairHydro::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case 0:
            corsair->SetFixed(colors);
            break;

        case 1:
            corsair->SetBlink(modes[active_mode].colors, modes[active_mode].speed);
            break;

        case 2:
            corsair->SetShift(modes[active_mode].colors, modes[active_mode].speed);
            break;

        case 3:
            corsair->SetPulse(modes[active_mode].colors, modes[active_mode].speed);
            break;
    }
}
