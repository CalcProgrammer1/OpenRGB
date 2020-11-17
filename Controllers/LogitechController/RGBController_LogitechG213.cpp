/*-----------------------------------------*\
|  RGBController_LogitechG213.cpp           |
|                                           |
|  Generic RGB Interface for Logitech G213  |
|                                           |
|  Eric Samuelson (edbgon) 10/06/2020       |
\*-----------------------------------------*/

#include "RGBController_LogitechG213.h"

static const char* led_names[] =
{
    "Left Area",
    "Middle Area",
    "Right Area",
    "Arrow and Homekeys",
    "Numpad",
};

static const unsigned char led_values[] =
{
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
};

#define LOGITECH_G213_ZONES (sizeof(led_values) / sizeof(led_values[ 0 ]))

RGBController_LogitechG213::RGBController_LogitechG213(LogitechG213Controller* logitech_ptr)
{
    logitechG213 = logitech_ptr;

    name        = "Logitech G213 Keyboard Device";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Logitech G213 Keyboard Device";

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = 0xFFFF;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_LogitechG213::~RGBController_LogitechG213()
{

}

void RGBController_LogitechG213::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name                   = "Keyboard";
    new_zone.type                   = ZONE_TYPE_LINEAR;
    new_zone.leds_min               = 5;
    new_zone.leds_max               = 5;
    new_zone.leds_count             = 5;

    new_zone.matrix_map             = NULL;

    zones.push_back(new_zone);

    for(unsigned int led_idx = 0; led_idx < LOGITECH_G213_ZONES; led_idx++)
    {
        led new_led;
        new_led.name = led_names[led_idx];
        new_led.value = led_values[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_LogitechG213::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechG213::DeviceUpdateLEDs()
{
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        logitechG213->SetDirect((unsigned char)leds[led_idx].value, RGBGetRValue(colors[led_idx]), RGBGetGValue(colors[led_idx]), RGBGetBValue(colors[led_idx]));
    }
}

void RGBController_LogitechG213::UpdateZoneLEDs(int zone)
{
    logitechG213->SetDirect((unsigned char) zone, RGBGetRValue(zones[zone].colors[0]), RGBGetGValue(zones[zone].colors[0]), RGBGetBValue(zones[zone].colors[0]));
}

void RGBController_LogitechG213::UpdateSingleLED(int led)
{
    logitechG213->SetDirect(leds[led].value, RGBGetRValue(colors[led]), RGBGetGValue(colors[led]), RGBGetBValue(colors[led]));
}

void RGBController_LogitechG213::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_LogitechG213::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | Direct mode does not send a mode packet                   |
    | Call UpdateLEDs to send direct packet                     |
    \*---------------------------------------------------------*/
}
