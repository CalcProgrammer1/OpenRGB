/*-----------------------------------------*\
|  RGBController_LogitechGProWireless.cpp   |
|                                           |
|  Generic RGB Interface for                |
|  Logitech G Pro Wireless Gaming Mouse     |
|                                           |
|  TheRogueZeta   8/5/2020                  |
\*-----------------------------------------*/

#include "RGBController_LogitechGProWireless.h"

RGBController_LogitechGProWireless::RGBController_LogitechGProWireless(LogitechGProWirelessController* logitech_ptr)
{
    logitech = logitech_ptr;

    name                        = "Logitech G Pro Wireless Gaming Mouse";
    vendor                      = "Logitech";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "Logitech G Pro Wireless Gaming Mouse";
    location                    = logitech->GetDeviceLocation();
    serial                      = logitech->GetSerialString();

    mode Off;
    Off.name                    = "Off";
    Off.value                   = LOGITECH_G_PRO_WIRELESS_MODE_OFF;
    Off.flags                   = 0;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = LOGITECH_G_PRO_WIRELESS_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Cycle;
    Cycle.name                  = "Cycle";
    Cycle.value                 = LOGITECH_G_PRO_WIRELESS_MODE_CYCLE;
    Cycle.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Cycle.color_mode            = MODE_COLORS_NONE;
    Cycle.speed_min             = LOGITECH_G_PRO_WIRELESS_SPEED_SLOWEST;
    Cycle.speed_max             = LOGITECH_G_PRO_WIRELESS_SPEED_FASTEST;
    Cycle.speed                 = LOGITECH_G_PRO_WIRELESS_SPEED_NORMAL;
    modes.push_back(Cycle);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = LOGITECH_G_PRO_WIRELESS_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    Breathing.speed_min         = LOGITECH_G_PRO_WIRELESS_SPEED_SLOWEST;
    Breathing.speed_max         = LOGITECH_G_PRO_WIRELESS_SPEED_FASTEST;
    Breathing.speed             = LOGITECH_G_PRO_WIRELESS_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_LogitechGProWireless::~RGBController_LogitechGProWireless()
{
    delete logitech;
}

void RGBController_LogitechGProWireless::SetupZones()
{
    zone GProWireless_primary_zone;
    GProWireless_primary_zone.name          = "DPI";
    GProWireless_primary_zone.type          = ZONE_TYPE_SINGLE;
    GProWireless_primary_zone.leds_min      = 1;
    GProWireless_primary_zone.leds_max      = 1;
    GProWireless_primary_zone.leds_count    = 1;
    GProWireless_primary_zone.matrix_map    = NULL;
    zones.push_back(GProWireless_primary_zone);

    led GProWireless_primary_led;
    GProWireless_primary_led.name = "DPI";
    leds.push_back(GProWireless_primary_led);

    zone GProWireless_logo_zone;
    GProWireless_logo_zone.name             = "Logo";
    GProWireless_logo_zone.type             = ZONE_TYPE_SINGLE;
    GProWireless_logo_zone.leds_min         = 1;
    GProWireless_logo_zone.leds_max         = 1;
    GProWireless_logo_zone.leds_count       = 1;
    GProWireless_logo_zone.matrix_map       = NULL;
    zones.push_back(GProWireless_logo_zone);

    led GProWireless_logo_led;
    GProWireless_logo_led.name = "Logo";
    leds.push_back(GProWireless_logo_led);

    SetupColors();
}

void RGBController_LogitechGProWireless::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechGProWireless::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
    UpdateZoneLEDs(1);
}

void RGBController_LogitechGProWireless::UpdateZoneLEDs(int zone)
{
    unsigned char red = RGBGetRValue(colors[zone]);
    unsigned char grn = RGBGetGValue(colors[zone]);
    unsigned char blu = RGBGetBValue(colors[zone]);

    logitech->SendMouseMode(modes[active_mode].value, modes[active_mode].speed, zone, red, grn, blu);
}

void RGBController_LogitechGProWireless::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_LogitechGProWireless::SetCustomMode()
{
    active_mode = 1;
}

void RGBController_LogitechGProWireless::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
