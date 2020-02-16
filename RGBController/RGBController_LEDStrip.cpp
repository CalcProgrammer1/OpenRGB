/*-----------------------------------------*\
|  RGBController_LEDStrip.cpp               |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  KeyboardVisualizer LED strip interface   |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/20/2019   |
\*-----------------------------------------*/

#include "RGBController_LEDStrip.h"


RGBController_LEDStrip::RGBController_LEDStrip(LEDStripController* ledstrip_ptr)
{
    strip = ledstrip_ptr;

    name = "LED Strip";

    type = DEVICE_TYPE_LEDSTRIP;

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    colors.resize(strip->num_leds);
    
    for (int i = 0; i < strip->num_leds; i++)
    {
        char id_buf[16];
        snprintf(id_buf, 16, "%d", i);

        led new_led;
        new_led.name = "LED ";
        new_led.name.append(id_buf);
        
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

void RGBController_LEDStrip::UpdateLEDs()
{
    strip->SetLEDs(colors);
}

void RGBController_LEDStrip::UpdateZoneLEDs(int zone)
{
    strip->SetLEDs(colors);
}

void RGBController_LEDStrip::UpdateSingleLED(int led)
{
    strip->SetLEDs(colors);
}

void RGBController_LEDStrip::SetCustomMode()
{

}

void RGBController_LEDStrip::UpdateMode()
{

}
