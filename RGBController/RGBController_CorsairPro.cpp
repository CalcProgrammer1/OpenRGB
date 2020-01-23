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
    return(active_mode);
}

void RGBController_CorsairPro::SetMode(int mode)
{
    unsigned int corsair_direction;

    active_mode = mode;

    switch(modes[active_mode].direction)
    {
        case MODE_DIRECTION_LEFT:
            corsair_direction = CORSAIR_PRO_DIRECTION_LEFT;
            break;
        case MODE_DIRECTION_RIGHT:
            corsair_direction = CORSAIR_PRO_DIRECTION_RIGHT;
            break;
        case MODE_DIRECTION_UP:
            corsair_direction = CORSAIR_PRO_DIRECTION_UP;
            break;
        case MODE_DIRECTION_DOWN:
            corsair_direction = CORSAIR_PRO_DIRECTION_DOWN;
            break;
        case MODE_DIRECTION_HORIZONTAL:
            corsair_direction = CORSAIR_PRO_DIRECTION_HORIZONTAL;
            break;
        case MODE_DIRECTION_VERTICAL:
            corsair_direction = CORSAIR_PRO_DIRECTION_VERTICAL;
            break;
    }

    corsair->SetEffect(modes[active_mode].value,
                       modes[active_mode].speed,
                       corsair_direction,
                       modes[active_mode].random,
                       RGBGetRValue(colors[0]),
                       RGBGetGValue(colors[0]),
                       RGBGetBValue(colors[0]),
                       RGBGetRValue(colors[1]),
                       RGBGetGValue(colors[1]),
                       RGBGetBValue(colors[1]));
}

void RGBController_CorsairPro::SetCustomMode()
{
    active_mode = 9;
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
    ColorShift.name      = "Color Shift";
    ColorShift.value     = CORSAIR_PRO_MODE_COLOR_SHIFT;
    ColorShift.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    ColorShift.speed_min = CORSAIR_PRO_SPEED_SLOW;
    ColorShift.speed_max = CORSAIR_PRO_SPEED_FAST;
    ColorShift.random    = false;
    ColorShift.speed     = CORSAIR_PRO_SPEED_SLOW;
    modes.push_back(ColorShift);

    mode ColorPulse;
    ColorPulse.name      = "Color Pulse";
    ColorPulse.value     = CORSAIR_PRO_MODE_COLOR_PULSE;
    ColorPulse.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    ColorPulse.speed_min = CORSAIR_PRO_SPEED_SLOW;
    ColorPulse.speed_max = CORSAIR_PRO_SPEED_FAST;
    ColorPulse.random    = false;
    ColorPulse.speed     = CORSAIR_PRO_SPEED_SLOW;
    modes.push_back(ColorPulse);

    mode RainbowWave;
    RainbowWave.name      = "Rainbow Wave";
    RainbowWave.value     = CORSAIR_PRO_MODE_RAINBOW_WAVE;
    RainbowWave.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    RainbowWave.speed_min = CORSAIR_PRO_SPEED_SLOW;
    RainbowWave.speed_max = CORSAIR_PRO_SPEED_FAST;
    RainbowWave.random    = false;
    RainbowWave.speed     = CORSAIR_PRO_SPEED_SLOW;
    RainbowWave.direction = MODE_DIRECTION_LEFT;
    modes.push_back(RainbowWave);

    mode ColorWave;
    ColorWave.name      = "Color Wave";
    ColorWave.value     = CORSAIR_PRO_MODE_COLOR_WAVE;
    ColorWave.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    ColorWave.speed_min = CORSAIR_PRO_SPEED_SLOW;
    ColorWave.speed_max = CORSAIR_PRO_SPEED_FAST;
    ColorWave.random    = false;
    ColorWave.speed     = CORSAIR_PRO_SPEED_SLOW;
    ColorWave.direction = MODE_DIRECTION_LEFT;
    modes.push_back(ColorWave); 
    
    mode Visor;
    Visor.name      = "Visor";
    Visor.value     = CORSAIR_PRO_MODE_VISOR;
    Visor.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_HV | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    Visor.speed_min = CORSAIR_PRO_SPEED_SLOW;
    Visor.speed_max = CORSAIR_PRO_SPEED_FAST;
    Visor.random    = false;
    Visor.speed     = CORSAIR_PRO_SPEED_SLOW;
    Visor.direction = MODE_DIRECTION_VERTICAL;
    modes.push_back(Visor);

    mode Rain;
    Rain.name      = "Rain";
    Rain.value     = CORSAIR_PRO_MODE_RAIN;
    Rain.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    Rain.speed_min = CORSAIR_PRO_SPEED_SLOW;
    Rain.speed_max = CORSAIR_PRO_SPEED_FAST;
    Rain.random    = false;
    Rain.speed     = CORSAIR_PRO_SPEED_SLOW;
    Rain.direction = MODE_DIRECTION_LEFT;
    modes.push_back(Rain);

    mode Marquee;
    Marquee.name      = "Marquee";
    Marquee.value     = CORSAIR_PRO_MODE_MARQUEE;
    Marquee.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR;
    Marquee.speed_min = CORSAIR_PRO_SPEED_SLOW;
    Marquee.speed_max = CORSAIR_PRO_SPEED_FAST;
    Marquee.random    = false;
    Marquee.speed     = CORSAIR_PRO_SPEED_SLOW;
    modes.push_back(Marquee);
    
    mode Rainbow;
    Rainbow.name      = "Rainbow";
    Rainbow.value     = CORSAIR_PRO_MODE_RAINBOW;
    Rainbow.flags     = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min = CORSAIR_PRO_SPEED_SLOW;
    Rainbow.speed_max = CORSAIR_PRO_SPEED_FAST;
    Rainbow.random    = false;
    Rainbow.speed     = CORSAIR_PRO_SPEED_SLOW;
    modes.push_back(Rainbow);
    
    mode Sequential;
    Sequential.name      = "Sequential";
    Sequential.value     = CORSAIR_PRO_MODE_SEQUENTIAL;
    Sequential.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_COLOR | MODE_FLAG_RANDOM_COLOR;
    Sequential.speed_min = CORSAIR_PRO_SPEED_SLOW;
    Sequential.speed_max = CORSAIR_PRO_SPEED_FAST;
    Sequential.random    = false;
    Sequential.speed     = CORSAIR_PRO_SPEED_SLOW;
    Sequential.direction = MODE_DIRECTION_DOWN;
    modes.push_back(Sequential);

    mode Static;
    Static.name      = "Static";
    Static.value     = CORSAIR_PRO_MODE_STATIC;
    Static.flags     = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    Static.speed_min = 0;
    Static.speed_max = 0;
    Static.random    = false;
    Static.speed     = 0;
    modes.push_back(Static);

    active_mode = 9;

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
