/*-----------------------------------------*\
|  RGBController_LogitechG403.cpp           |
|                                           |
|  Generic RGB Interface for Logitech G403  |
|  Prodigy RGB Mouse                        |
|                                           |
|  Martin Hartl (inlar) 5/19/2020           |
\*-----------------------------------------*/

#include "RGBController_LogitechG403.h"

RGBController_LogitechG403::RGBController_LogitechG403(LogitechG403Controller* logitech_ptr)
{
    logitech = logitech_ptr;

    name        = "Logitech Mouse Device";
    vendor      = "Logitech";
    type        = DEVICE_TYPE_MOUSE;
    description = "Logitech Mouse Device";
    location    = logitech->GetDeviceLocation();

    mode Static;
    Static.name                     = "Static";
    Static.value                    = LOGITECH_G403_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = LOGITECH_G403_MODE_OFF;
    Off.flags                       = 0;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Cycle;
    Cycle.name                      = "Cycle";
    Cycle.value                     = LOGITECH_G403_MODE_CYCLE;
    Cycle.flags                     = MODE_FLAG_HAS_SPEED;
    Cycle.color_mode                = MODE_COLORS_NONE;
    Cycle.speed_min                 = LOGITECH_G403_SPEED_SLOWEST;
    Cycle.speed_max                 = LOGITECH_G403_SPEED_FASTEST;
    Cycle.speed                     = LOGITECH_G403_SPEED_NORMAL;
    modes.push_back(Cycle);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = LOGITECH_G403_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.speed_min             = LOGITECH_G403_SPEED_SLOWEST;
    Breathing.speed_max             = LOGITECH_G403_SPEED_FASTEST;
    Breathing.speed                 = LOGITECH_G403_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
}

void RGBController_LogitechG403::SetupZones()
{
    zone G403_wheel_zone;
    G403_wheel_zone.name           = "Scroll Wheel";
    G403_wheel_zone.type           = ZONE_TYPE_SINGLE;
    G403_wheel_zone.leds_min       = 1;
    G403_wheel_zone.leds_max       = 1;
    G403_wheel_zone.leds_count     = 1;
    G403_wheel_zone.matrix_map     = NULL;
    zones.push_back(G403_wheel_zone);

    led G403_wheel_led;
    G403_wheel_led.name = "Scroll Wheel";
    leds.push_back(G403_wheel_led);

    zone G403_logo_zone;
    G403_logo_zone.name           = "Logo";
    G403_logo_zone.type           = ZONE_TYPE_SINGLE;
    G403_logo_zone.leds_min       = 1;
    G403_logo_zone.leds_max       = 1;
    G403_logo_zone.leds_count     = 1;
    G403_logo_zone.matrix_map     = NULL;
    zones.push_back(G403_logo_zone);

    led G403_logo_led;
    G403_logo_led.name = "Logo";
    leds.push_back(G403_logo_led);

    SetupColors();
}

void RGBController_LogitechG403::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechG403::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
    UpdateZoneLEDs(1);
}

void RGBController_LogitechG403::UpdateZoneLEDs(int zone)
{
    unsigned char red = RGBGetRValue(colors[zone]);
    unsigned char grn = RGBGetGValue(colors[zone]);
    unsigned char blu = RGBGetBValue(colors[zone]);

    logitech->SendMouseMode(modes[active_mode].value, modes[active_mode].speed, zone, red, grn, blu);
}

void RGBController_LogitechG403::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_LogitechG403::SetCustomMode()
{

}

void RGBController_LogitechG403::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
