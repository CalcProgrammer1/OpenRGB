/*-----------------------------------------*\
|  RGBController_HuePlus.cpp               |
|                                           |
|  Generic RGB Interface for NZXT Hue+      |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/20/2019   |
\*-----------------------------------------*/

#include "RGBController_HuePlus.h"


RGBController_HuePlus::RGBController_HuePlus(HuePlusController* hueplus_ptr)
{
    strip = hueplus_ptr;

    name = "LED Strip";

    type = DEVICE_TYPE_LEDSTRIP;

    mode led_mode;
    led_mode.name = "Custom";
    modes.push_back(led_mode);

    for (int i = 0; i < strip->num_leds; i++)
    {
        colors.push_back(0x00000000);
        led new_led;
        new_led.name = "LED Strip";
        leds.push_back(new_led);
    }

    zone led_zone;
    led_zone.name = "LED Strip";
    std::vector<int> led_zone_map;
    for (int i = 0; i < strip->num_leds; i++)
    {
        led_zone_map.push_back(i);
    }
    led_zone.map.push_back(led_zone_map);
    zones.push_back(led_zone);
}

int RGBController_HuePlus::GetMode()
{
    return 0;
}

void RGBController_HuePlus::SetMode(int mode)
{

}

void RGBController_HuePlus::SetCustomMode()
{

}

void RGBController_HuePlus::SetAllLEDs(RGBColor color)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i] = color;
    }

    strip->SetLEDs(colors);
}

void RGBController_HuePlus::SetAllZoneLEDs(int zone, RGBColor color)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i] = color;
    }

    strip->SetLEDs(colors);
}

void RGBController_HuePlus::SetLED(int led, RGBColor color)
{
    colors[led] = color;

    strip->SetLEDs(colors);
}

void RGBController_HuePlus::UpdateLEDs()
{
    strip->SetLEDs(colors);
}
