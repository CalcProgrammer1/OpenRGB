/*-----------------------------------------*\
|  RGBController_LEDStrip.cpp               |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  E1.31 Streaming ACN interface            |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/18/2019  |
\*-----------------------------------------*/

#include "RGBController_E131.h"


RGBController_E131::RGBController_E131(std::vector<E131Device> devices)
{
    name = "E1.31 Streaming ACN Device";

    mode led_mode;
    led_mode.name = "Custom";
    modes.push_back(led_mode);

    for (int i = 0; i < devices.size(); i++)
    {
    	for (int led_idx = 0; led_idx < devices[i].num_leds; led_idx++)
    	{
       		colors.push_back(0x00000000);
        	led new_led;
        	new_led.name = "E1.31 LED";
        	leds.push_back(new_led);
		}
		
	    zone led_zone;
    	led_zone.name = devices[i].name;
    	led_zone.type = devices[i].type;
    	std::vector<int> led_zone_map;
    	for (int led_idx = 0; led_idx < devices[i].num_leds; led_idx++)
    	{
      	  	led_zone_map.push_back(led_idx);
    	}
    	led_zone.map.push_back(led_zone_map);
    	zones.push_back(led_zone);
	}
}

int RGBController_E131::GetMode()
{
    return 0;
}

void RGBController_E131::SetMode(int mode)
{

}

void RGBController_E131::SetCustomMode()
{

}

void RGBController_E131::SetAllLEDs(RGBColor color)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i] = color;
    }
}

void RGBController_E131::SetAllZoneLEDs(int zone, RGBColor color)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i] = color;
    }
}

void RGBController_E131::SetLED(int led, RGBColor color)
{
    colors[led] = color;
}

void RGBController_E131::UpdateLEDs()
{
}