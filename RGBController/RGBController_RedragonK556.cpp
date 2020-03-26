/*-----------------------------------------*\
|  RGBController_RedragonK556.cpp           |
|                                           |
|  Generic RGB Interface for Redragon K556  |
|  Devarajas RGB Keyboard                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/25/2020   |
\*-----------------------------------------*/

#include "RGBController_RedragonK556.h"

RGBController_RedragonK556::RGBController_RedragonK556(RedragonK556Controller* redragon_ptr)
{
    redragon = redragon_ptr;

    name = "Redragon K556 Devarajas";
    type = DEVICE_TYPE_KEYBOARD;
    description = "Redragon K556 Device";

    mode Custom;
    Custom.name       = "Custom";
    Custom.value      = REDRAGON_K556_MODE_CUSTOM;
    Custom.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Custom.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    SetupZones();
}

void RGBController_RedragonK556::SetupZones()
{
    zone new_zone;

    new_zone.name = "Keyboard";
    new_zone.type = ZONE_TYPE_MATRIX;
    new_zone.leds_min = 126;
    new_zone.leds_max = 126;
    new_zone.leds_count = 126;

    zones.push_back(new_zone);

    for(int led_idx = 0; led_idx < 126; led_idx++)
    {
        led new_led;

        new_led.name = "Keyboard LED ";
        new_led.name.append(std::to_string(led_idx));

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_RedragonK556::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RedragonK556::UpdateLEDs()
{
    unsigned char color_data[7*0x36];

    for(int led_idx = 0; led_idx < 126; led_idx++)
    {
        color_data[(3 * led_idx) + 0] = RGBGetRValue(colors[led_idx]);
        color_data[(3 * led_idx) + 1] = RGBGetGValue(colors[led_idx]);
        color_data[(3 * led_idx) + 2] = RGBGetBValue(colors[led_idx]);
    }
    
    redragon->SendKeyboardBegin();
    redragon->SendKeyboardMode(20);

    redragon->SetKeyboardColors
                (
                color_data,
                0x36 * 7
                );
    
    redragon->SendKeyboardEnd();
}

void RGBController_RedragonK556::UpdateZoneLEDs(int /*zone*/)
{
    UpdateLEDs();
}

void RGBController_RedragonK556::UpdateSingleLED(int /*led*/)
{
    UpdateLEDs();
}

void RGBController_RedragonK556::SetCustomMode()
{

}

void RGBController_RedragonK556::UpdateMode()
{
    UpdateLEDs();
}
