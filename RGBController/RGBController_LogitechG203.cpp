/*-----------------------------------------*\
|  RGBController_LogitechG203.cpp           |
|                                           |
|  Generic RGB Interface for Logitech G203  |
|  Prodigy RGB Mouse                        |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/17/2020   |
\*-----------------------------------------*/

#include "RGBController_LogitechG203.h"

RGBController_LogitechG203::RGBController_LogitechG203(LogitechG203Controller* logitech_ptr)
{
    logitech = logitech_ptr;

    name        = "Logitech Mouse Device";
    type        = DEVICE_TYPE_MOUSE;
    description = "Logitech Mouse Device";

    mode Static;
    Static.name       = "Static";
    Static.value      = LOGITECH_G203_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);
    
    mode Off;
    Off.name       = "Off";
    Off.value      = LOGITECH_G203_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Cycle;
    Cycle.name       = "Cycle";
    Cycle.value      = LOGITECH_G203_MODE_CYCLE;
    Cycle.flags      = MODE_FLAG_HAS_SPEED;
    Cycle.color_mode = MODE_COLORS_NONE;
    Cycle.speed_min  = LOGITECH_G203_SPEED_SLOWEST;
    Cycle.speed_max  = LOGITECH_G203_SPEED_FASTEST;
    Cycle.speed      = LOGITECH_G203_SPEED_NORMAL;
    modes.push_back(Cycle);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = LOGITECH_G203_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    Breathing.speed_min  = LOGITECH_G203_SPEED_SLOWEST;
    Breathing.speed_max  = LOGITECH_G203_SPEED_FASTEST;
    Breathing.speed      = LOGITECH_G203_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
}

void RGBController_LogitechG203::SetupZones()
{
    zone g203_zone;
    g203_zone.name           = "Mouse Zone";
    g203_zone.type           = ZONE_TYPE_SINGLE;
    g203_zone.leds_min       = 1;
    g203_zone.leds_max       = 1;
    g203_zone.leds_count     = 1;
    g203_zone.matrix_map     = NULL;
    zones.push_back(g203_zone);

    led g203_led;
    g203_led.name = "Mouse LED";
    leds.push_back(g203_led);

    SetupColors();
}

void RGBController_LogitechG203::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechG203::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    logitech->SendMouseMode(modes[active_mode].value, modes[active_mode].speed, red, grn, blu);
}

void RGBController_LogitechG203::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG203::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG203::SetCustomMode()
{

}

void RGBController_LogitechG203::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
