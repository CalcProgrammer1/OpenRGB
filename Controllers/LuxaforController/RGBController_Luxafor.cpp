/*---------------------------------------------------------*\
| RGBController_Luxafor.cpp                                 |
|                                                           |
|   RGBController for Luxafor devices                       |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      05 Sep 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_Luxafor.h"

RGBController_Luxafor::RGBController_Luxafor(LuxaforController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    type                        = DEVICE_TYPE_ACCESSORY;
    vendor                      = "Luxafor";
    description                 = "Luxafor Device";
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = LUXAFOR_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    // mode Fade;
    // Fade.name           = "Fade";
    // Fade.value          = LUXAFOR_MODE_FADE;
    // Fade.flags          = MODE_FLAG_HAS_PER_LED_COLOR;
    // Fade.color_mode     = MODE_COLORS_PER_LED;
    // modes.push_back(Fade);

    // mode Strobe;
    // Strobe.name         = "Strobe";
    // Strobe.value        = LUXAFOR_MODE_STROBE;
    // Strobe.flags        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    // Strobe.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    // Strobe.colors_min   = 1;
    // Strobe.colors_max   = 1;
    // Strobe.colors.resize(1);
    // modes.push_back(Strobe);

    // mode Wave;
    // Wave.name           = "Wave";
    // Wave.value          = LUXAFOR_MODE_WAVE;
    // Wave.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    // Wave.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    // Wave.colors_min     = 1;
    // Wave.colors_max     = 1;
    // Wave.colors.resize(1);
    // modes.push_back(Wave);

    mode TrafficLights;
    TrafficLights.name          = "Traffic Lights";
    TrafficLights.value         = LUXAFOR_MODE_PATTERN_TRAFFIC_LIGHTS;
    TrafficLights.flags         = 0;
    TrafficLights.color_mode    = MODE_COLORS_NONE;
    modes.push_back(TrafficLights);

    mode Pattern2;
    Pattern2.name               = "Pattern 2";
    Pattern2.value              = LUXAFOR_MODE_PATTERN_2;
    Pattern2.flags              = 0;
    Pattern2.color_mode         = MODE_COLORS_NONE;
    modes.push_back(Pattern2);

    mode Pattern3;
    Pattern3.name               = "Pattern 3";
    Pattern3.value              = LUXAFOR_MODE_PATTERN_3;
    Pattern3.flags              = 0;
    Pattern3.color_mode         = MODE_COLORS_NONE;
    modes.push_back(Pattern3);

    mode Pattern4;
    Pattern4.name               = "Pattern 4";
    Pattern4.value              = LUXAFOR_MODE_PATTERN_4;
    Pattern4.flags              = 0;
    Pattern4.color_mode         = MODE_COLORS_NONE;
    modes.push_back(Pattern4);

    mode Police;
    Police.name                 = "Police";
    Police.value                = LUXAFOR_MODE_PATTERN_POLICE;
    Police.flags                = 0;
    Police.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Police);

    mode Pattern6;
    Pattern6.name               = "Pattern 6";
    Pattern6.value              = LUXAFOR_MODE_PATTERN_6;
    Pattern6.flags              = 0;
    Pattern6.color_mode         = MODE_COLORS_NONE;
    modes.push_back(Pattern6);

    mode Pattern7;
    Pattern7.name               = "Pattern 7";
    Pattern7.value              = LUXAFOR_MODE_PATTERN_7;
    Pattern7.flags              = 0;
    Pattern7.color_mode         = MODE_COLORS_NONE;
    modes.push_back(Pattern7);

    mode Pattern8;
    Pattern8.name               = "Pattern 8";
    Pattern8.value              = LUXAFOR_MODE_PATTERN_8;
    Pattern8.flags              = 0;
    Pattern8.color_mode         = MODE_COLORS_NONE;
    modes.push_back(Pattern8);

    SetupZones();
}

RGBController_Luxafor::~RGBController_Luxafor()
{

}

void RGBController_Luxafor::SetupZones()
{
    /*-----------------------------------------------------*\
    | The Luxafor Flag has 2 zones                          |
    |   * Flag (3 LEDs)                                     |
    |   * Rear (3 LEDs)                                     |
    | The LED index starts at 1. Sending 255 for the LED ID |
    | sets all LEDs at once.                                |
    \*-----------------------------------------------------*/
    unsigned int led_value  = LUXAFOR_LED_FIRST;

    zone flag_zone;
    flag_zone.name          = "Flag";
    flag_zone.type          = ZONE_TYPE_SINGLE;
    flag_zone.leds_min      = 3;
    flag_zone.leds_max      = 3;
    flag_zone.leds_count    = 3;
    flag_zone.matrix_map    = NULL;
    zones.push_back(flag_zone);

    for(std::size_t led_idx = 0; led_idx < flag_zone.leds_count; led_idx++)
    {
        led luxafor_led;
        luxafor_led.name        = "Flag LED";
        luxafor_led.value       = led_value;
        leds.push_back(luxafor_led);

        led_value++;
    }

    zone rear_zone;
    rear_zone.name          = "Rear";
    rear_zone.type          = ZONE_TYPE_SINGLE;
    rear_zone.leds_min      = 3;
    rear_zone.leds_max      = 3;
    rear_zone.leds_count    = 3;
    rear_zone.matrix_map    = NULL;
    zones.push_back(rear_zone);

    for(std::size_t led_idx = 0; led_idx < rear_zone.leds_count; led_idx++)
    {
        led luxafor_led;
        luxafor_led.name        = "Rear LED";
        luxafor_led.value       = led_value;
        leds.push_back(luxafor_led);

        led_value++;
    }

    SetupColors();
}

void RGBController_Luxafor::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*-----------------------------------------------------*\
    | This device does not support resizing zones           |
    \*-----------------------------------------------------*/
}

void RGBController_Luxafor::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        UpdateZoneLEDs((int)zone_idx);
    }
}

void RGBController_Luxafor::UpdateZoneLEDs(int zone)
{
    for(unsigned int led_idx = 0; led_idx < zones[zone].leds_count; led_idx++)
    {
        UpdateSingleLED((int)(zones[zone].start_idx + led_idx));
    }
}

void RGBController_Luxafor::UpdateSingleLED(int led)
{
    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        controller->SendPacket((modes[active_mode].value & 0xFF), leds[led].value, red, grn, blu, 0);
    }
}

void RGBController_Luxafor::DeviceUpdateMode()
{
    switch(modes[active_mode].color_mode)
    {
        case MODE_COLORS_PER_LED:
            DeviceUpdateLEDs();
            break;

        case MODE_COLORS_MODE_SPECIFIC:
            {
            unsigned char red = RGBGetRValue(colors[modes[active_mode].colors[0]]);
            unsigned char grn = RGBGetGValue(colors[modes[active_mode].colors[0]]);
            unsigned char blu = RGBGetBValue(colors[modes[active_mode].colors[0]]);

            controller->SendPacket((modes[active_mode].value & 0xFF), LUXAFOR_LED_ALL, red, grn, blu, 0);
            }
            break;

        case MODE_COLORS_NONE:
            controller->SendPacket((modes[active_mode].value & 0xFF), LUXAFOR_LED_ALL, 0, 0, 0, (modes[active_mode].value >> 8));
            break;
    }
}

void RGBController_Luxafor::DeviceSaveMode()
{
    /*-----------------------------------------------------*\
    | This device does not support saving                   |
    \*-----------------------------------------------------*/
}
