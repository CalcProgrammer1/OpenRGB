/*-----------------------------------------*\
|  RGBController_DuckyKeyboard.cpp          |
|                                           |
|  Generic RGB Interface for Ducky RGB      |
|  keyboard devices                         |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/4/2020    |
\*-----------------------------------------*/

#include "RGBController_DuckyKeyboard.h"

RGBController_DuckyKeyboard::RGBController_DuckyKeyboard(DuckyKeyboardController* ducky_ptr)
{
    ducky = ducky_ptr;

    name        = "Ducky Keyboard Device";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Ducky Keyboard Device";

    mode Custom;
    Custom.name       = "Custom";
    Custom.value      = 0xFFFF;
    Custom.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Custom.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    SetupZones();
}

RGBController_DuckyKeyboard::~RGBController_DuckyKeyboard()
{

}

void RGBController_DuckyKeyboard::SetupZones()
{
    zone new_zone;

    new_zone.name               = "Keyboard";
    new_zone.type               = ZONE_TYPE_MATRIX;
    new_zone.leds_min           = 155;
    new_zone.leds_max           = 155;
    new_zone.leds_count         = 155;
    new_zone.matrix_map         = NULL;//new matrix_map_type;
    //new_zone.matrix_map->height = 6;
    //new_zone.matrix_map->width  = 23;
    //new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
    
    zones.push_back(new_zone);

    for(int led_idx = 0; led_idx < 155; led_idx++)
    {
        led new_led;

        new_led.name = "Keyboard LED ";
        new_led.name.append(std::to_string(led_idx));

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_DuckyKeyboard::ResizeZone(int zone, int new_size)
{
    
}
    
void RGBController_DuckyKeyboard::DeviceUpdateLEDs()
{
    unsigned char colordata[155*3];
    
    for(int color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        colordata[(color_idx*3)+0] = RGBGetRValue(colors[color_idx]);
        colordata[(color_idx*3)+1] = RGBGetGValue(colors[color_idx]);
        colordata[(color_idx*3)+2] = RGBGetBValue(colors[color_idx]);
    }

    ducky->SendColors(colordata, sizeof(colordata));
}

void RGBController_DuckyKeyboard::UpdateZoneLEDs(int zone)
{
    DeviceUpdateLEDs();
}

void RGBController_DuckyKeyboard::UpdateSingleLED(int led)
{
    DeviceUpdateLEDs();
}

void RGBController_DuckyKeyboard::SetCustomMode()
{

}

void RGBController_DuckyKeyboard::UpdateMode()
{

}