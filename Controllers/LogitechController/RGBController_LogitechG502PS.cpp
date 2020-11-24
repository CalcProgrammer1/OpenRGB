/*-----------------------------------------*\
|  RGBController_LogitechG403.h             |
|                                           |
|  Generic RGB Interface for Logitech G502  |
|  Proteus Sprectrum Mouse                  |
|                                           |
|  kernzerfall 07/28/2020                   |
\*-----------------------------------------*/

#include "RGBController_LogitechG502PS.h"

RGBController_LogitechG502PS::RGBController_LogitechG502PS(LogitechG502PSController* logitech_ptr)
{
    logitech = logitech_ptr;

    name                            = "Logitech Mouse Device";
    type                            = DEVICE_TYPE_MOUSE;
    description                     = "Logitech Mouse Device";
    location                        = logitech->GetDeviceLocation();

    mode Static;
    Static.name                     = "Static";
    Static.value                    = LOGITECH_G502_PS_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = LOGITECH_G502_PS_MODE_OFF;
    Off.flags                       = 0;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Cycle;
    Cycle.name                      = "Cycle";
    Cycle.value                     = LOGITECH_G502_PS_MODE_CYCLE;
    Cycle.flags                     = MODE_FLAG_HAS_SPEED;
    Cycle.color_mode                = MODE_COLORS_NONE;
    Cycle.speed_min                 = LOGITECH_G502_PS_SPEED_SLOWEST;
    Cycle.speed_max                 = LOGITECH_G502_PS_SPEED_FASTEST;
    Cycle.speed                     = LOGITECH_G502_PS_SPEED_NORMAL;
    modes.push_back(Cycle);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = LOGITECH_G502_PS_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.speed_min             = LOGITECH_G502_PS_SPEED_SLOWEST;
    Breathing.speed_max             = LOGITECH_G502_PS_SPEED_FASTEST;
    Breathing.speed                 = LOGITECH_G502_PS_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
}

void RGBController_LogitechG502PS::SetupZones()
{
    zone G502_PS_side_zone;
    G502_PS_side_zone.name          = "DPI";
    G502_PS_side_zone.type          = ZONE_TYPE_SINGLE;
    G502_PS_side_zone.leds_min      = 1;
    G502_PS_side_zone.leds_max      = 1;
    G502_PS_side_zone.leds_count    = 1;
    G502_PS_side_zone.matrix_map    = NULL;
    zones.push_back(G502_PS_side_zone);

    led G502_PS_side_led;
    G502_PS_side_led.name = "DPI";
    leds.push_back(G502_PS_side_led);

    zone G502_logo_zone;
    G502_logo_zone.name             = "Logo";
    G502_logo_zone.type             = ZONE_TYPE_SINGLE;
    G502_logo_zone.leds_min         = 1;
    G502_logo_zone.leds_max         = 1;
    G502_logo_zone.leds_count       = 1;
    G502_logo_zone.matrix_map       = NULL;
    zones.push_back(G502_logo_zone);

    led G502_logo_led;
    G502_logo_led.name = "Logo";
    leds.push_back(G502_logo_led);

    SetupColors();
}

void RGBController_LogitechG502PS::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechG502PS::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
    UpdateZoneLEDs(1);
}

void RGBController_LogitechG502PS::UpdateZoneLEDs(int zone)
{
    unsigned char red = RGBGetRValue(colors[zone]);
    unsigned char grn = RGBGetGValue(colors[zone]);
    unsigned char blu = RGBGetBValue(colors[zone]);

    logitech->SendMouseMode(modes[active_mode].value, modes[active_mode].speed, zone, red, grn, blu);
}

void RGBController_LogitechG502PS::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_LogitechG502PS::SetCustomMode()
{

}

void RGBController_LogitechG502PS::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
