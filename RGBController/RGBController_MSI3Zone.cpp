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

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

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

void RGBController_MSI3Zone::UpdateLEDs()
{
    msi->SetLEDs(colors);
}

void RGBController_MSI3Zone::UpdateZoneLEDs(int zone)
{
    msi->SetLEDs(colors);
}

void RGBController_MSI3Zone::UpdateSingleLED(int led)
{
    msi->SetLEDs(colors);
}

void RGBController_MSI3Zone::SetCustomMode()
{

}

void RGBController_MSI3Zone::UpdateMode()
{

}
