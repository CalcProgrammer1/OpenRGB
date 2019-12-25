/*-----------------------------------------*\
|  RGBController_MSI3Zone.cpp               |
|                                           |
|  Generic RGB Interface for MSI/Steelseries|
|  3-Zone Keyboard                          |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#include "RGBController_MSI3Zone.h"

RGBController_MSI3Zone::RGBController_MSI3Zone(MSI3ZoneController* msi_ptr)
{
    msi = msi_ptr;

    name = "MSI 3-Zone Keyboard";
    type = DEVICE_TYPE_KEYBOARD;

    mode led_mode;
    led_mode.name = "Custom";
    modes.push_back(led_mode);

    led left_led;
    left_led.name = "Keyboard Left";
    leds.push_back(left_led);
    colors.push_back(0x00000000);

    led mid_led;
    mid_led.name = "Keyboard Middle";
    leds.push_back(mid_led);
    colors.push_back(0x00000000);

    led right_led;
    right_led.name = "Keyboard Right";
    leds.push_back(right_led);
    colors.push_back(0x00000000);

    zone keyboard_zone;
    keyboard_zone.name = "Keyboard";
    keyboard_zone.type = ZONE_TYPE_LINEAR;
    std::vector<int> keyboard_zone_map;
    keyboard_zone_map.push_back(0);
    keyboard_zone_map.push_back(1);
    keyboard_zone_map.push_back(2);
    keyboard_zone.map.push_back(keyboard_zone_map);
    zones.push_back(keyboard_zone);

    led aux_led;
    aux_led.name = "Aux";
    leds.push_back(aux_led);
    colors.push_back(0x00000000);

    zone aux_zone;
    aux_zone.name = "Aux";
    aux_zone.type = ZONE_TYPE_SINGLE;
    std::vector<int> aux_zone_map;
    aux_zone_map.push_back(3);
    aux_zone.map.push_back(aux_zone_map);
    zones.push_back(aux_zone);
}

RGBController_MSI3Zone::~RGBController_MSI3Zone()
{

}

int RGBController_MSI3Zone::GetMode()
{
    return 0;
}

void RGBController_MSI3Zone::SetMode(int mode)
{

}

void RGBController_MSI3Zone::SetCustomMode()
{

}

void RGBController_MSI3Zone::SetAllLEDs(RGBColor color)
{
    for(int i = 0; i < colors.size(); i++)
    {
        colors[i] = color;
    }

    UpdateLEDs();
}

void RGBController_MSI3Zone::SetAllZoneLEDs(int zone, RGBColor color)
{
    for (int x = 0; x < zones[zone].map.size(); x++)
    {
        for (int y = 0; y < zones[zone].map[x].size(); y++)
        {
            colors[zones[zone].map[x][y]] = color;
        }
    }

    UpdateLEDs();
}

void RGBController_MSI3Zone::SetLED(int led, RGBColor color)
{
    colors[led] = color;

    UpdateLEDs();
}

void RGBController_MSI3Zone::UpdateLEDs()
{
    msi->SetLEDs(colors);
}