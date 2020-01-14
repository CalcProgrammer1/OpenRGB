/*-----------------------------------------*\
|  RGBController_CorsairPro.cpp             |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Corsair Vengeance Pro RGB driver         |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/30/2019   |
\*-----------------------------------------*/

#include "RGBController_CorsairPro.h"

int RGBController_CorsairPro::GetMode()
{
    int dev_mode = corsair->GetEffect();

    for(int mode = 0; mode < modes.size(); mode++)
    {
        if(modes[mode].value == dev_mode)
        {
            return(mode);
        }
    }

    return(0);
}

void RGBController_CorsairPro::SetMode(int mode)
{
    corsair->SetEffect(modes[mode].value);
}

void RGBController_CorsairPro::SetCustomMode()
{
    corsair->SetCustom();
}

void RGBController_CorsairPro::UpdateLEDs()
{
    for (std::size_t led = 0; led < colors.size(); led++)
    {
        RGBColor      color = colors[led];
        unsigned char red   = RGBGetRValue(color);
        unsigned char grn   = RGBGetGValue(color);
        unsigned char blu   = RGBGetBValue(color);
        corsair->SetLEDColor(led, red, grn, blu);
    }

    corsair->ApplyColors();
}

void RGBController_CorsairPro::UpdateZoneLEDs(int zone)
{
    UpdateLEDs();
}

void RGBController_CorsairPro::UpdateSingleLED(int led)
{
    RGBColor      color = colors[led];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    corsair->SetLEDColor(led, red, grn, blu);
    corsair->ApplyColors();
}

RGBController_CorsairPro::RGBController_CorsairPro(CorsairProController* corsair_ptr)
{
    corsair = corsair_ptr;

    name = corsair->GetDeviceName();
    location = corsair->GetDeviceLocation();

    type = DEVICE_TYPE_DRAM;

    mode ColorShift;
    ColorShift.name  = "Color Shift";
    ColorShift.value = CORSAIR_PRO_MODE_COLOR_SHIFT;
    ColorShift.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    modes.push_back(ColorShift);

    mode ColorPulse;
    ColorPulse.name  = "Color Pulse";
    ColorPulse.value = CORSAIR_PRO_MODE_COLOR_PULSE;
    ColorPulse.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    modes.push_back(ColorPulse);

    mode RainbowWave;
    RainbowWave.name  = "Rainbow Wave";
    RainbowWave.value = CORSAIR_PRO_MODE_RAINBOW_WAVE;
    RainbowWave.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    modes.push_back(RainbowWave);

    mode ColorWave;
    ColorWave.name  = "Color Wave";
    ColorWave.value = CORSAIR_PRO_MODE_COLOR_WAVE;
    ColorWave.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    modes.push_back(ColorWave); 
    
    mode Visor;
    Visor.name  = "Visor";
    Visor.value = CORSAIR_PRO_MODE_VISOR;
    Visor.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_HV | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    modes.push_back(Visor);

    mode Rain;
    Rain.name  = "Rain";
    Rain.value = CORSAIR_PRO_MODE_RAIN;
    Rain.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    modes.push_back(Rain);

    mode Marquee;
    Marquee.name  = "Marquee";
    Marquee.value = CORSAIR_PRO_MODE_MARQUEE;
    Marquee.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR;
    modes.push_back(Marquee);
    
    mode Rainbow;
    Rainbow.name  = "Rainbow";
    Rainbow.value = CORSAIR_PRO_MODE_RAINBOW;
    Rainbow.flags = MODE_FLAG_HAS_SPEED;
    modes.push_back(Rainbow);
    
    mode Sequential;
    Sequential.name  = "Sequential";
    Sequential.value = CORSAIR_PRO_MODE_SEQUENTIAL;
    Sequential.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    modes.push_back(Sequential);

    mode Static;
    Static.name = "Static";
    Static.value = CORSAIR_PRO_MODE_STATIC;
    Static.flags = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    modes.push_back(Static);

    colors.resize(corsair->GetLEDCount());

    for (unsigned int i = 0; i < corsair->GetLEDCount(); i++)
    {
        led* new_led = new led();

        new_led->name = "Corsair Pro LED";

        leds.push_back(*new_led);
    }

    zone new_zone;

    new_zone.name = "Corsair Pro Zone";
    new_zone.type = ZONE_TYPE_LINEAR;

    std::vector<int> zone_row;

    for (unsigned int i = 0; i < corsair->GetLEDCount(); i++)
    {
        zone_row.push_back(i);
    }

    new_zone.map.push_back(zone_row);

    zones.push_back(new_zone);
}
