/*-----------------------------------------*\
|  RGBController_AorusATC800.cpp            |
|                                           |
|  Generic RGB Interface Aorus ATC800 CPU   |
|  Cooler                                   |
|                                           |
|  Felipe Cavalcanti 08/13/2020             |
\*-----------------------------------------*/

#include "RGBController_AorusATC800.h"

RGBController_AorusATC800::RGBController_AorusATC800(ATC800Controller* cooler_ptr)
{
    cooler = cooler_ptr;

    name        = "Aorus ATC800 CPU Cooler";
    vendor      = "Gigabyte";
    type        = DEVICE_TYPE_COOLER;
    description = "Aorus ATC800 CPU Cooler";
    location    = cooler->GetDeviceLocation();
    serial      = cooler->GetSerialString();

    mode Static;
    Static.name       = "Static";
    Static.value      = AORUS_ATC800_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Off;
    Off.name       = "Off";
    Off.value      = AORUS_ATC800_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Flashing;
    Flashing.name       = "Flashing";
    Flashing.value      = AORUS_ATC800_MODE_FLASHING;
    Flashing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Flashing.color_mode = MODE_COLORS_PER_LED;
    Flashing.speed_min  = AORUS_ATC800_SPEED_SLOWEST;
    Flashing.speed_max  = AORUS_ATC800_SPEED_FASTEST;
    Flashing.speed      = AORUS_ATC800_SPEED_NORMAL;
    modes.push_back(Flashing);

    mode DoubleFlashing;
    DoubleFlashing.name       = "Double Flashing";
    DoubleFlashing.value      = AORUS_ATC800_MODE_DOUBLE_FLASH;
    DoubleFlashing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DoubleFlashing.color_mode = MODE_COLORS_PER_LED;
    DoubleFlashing.speed_min  = AORUS_ATC800_SPEED_SLOWEST;
    DoubleFlashing.speed_max  = AORUS_ATC800_SPEED_FASTEST;
    DoubleFlashing.speed      = AORUS_ATC800_SPEED_NORMAL;
    modes.push_back(DoubleFlashing);

    mode Pulsing;
    Pulsing.name       = "Pulsing";
    Pulsing.value      = AORUS_ATC800_MODE_PULSE;
    Pulsing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Pulsing.color_mode = MODE_COLORS_PER_LED;
    Pulsing.speed_min  = AORUS_ATC800_SPEED_SLOWEST;
    Pulsing.speed_max  = AORUS_ATC800_SPEED_FASTEST;
    Pulsing.speed      = AORUS_ATC800_SPEED_NORMAL;
    modes.push_back(Pulsing);

    SetupZones();
}

RGBController_AorusATC800::~RGBController_AorusATC800()
{
    delete cooler;
}

void RGBController_AorusATC800::SetupZones()
{
    zone atc800_cpu_fans_zone;
    atc800_cpu_fans_zone.name           = "Fan";
    atc800_cpu_fans_zone.type           = ZONE_TYPE_SINGLE;
    atc800_cpu_fans_zone.leds_min       = 1;
    atc800_cpu_fans_zone.leds_max       = 1;
    atc800_cpu_fans_zone.leds_count     = 1;
    atc800_cpu_fans_zone.matrix_map     = NULL;
    zones.push_back(atc800_cpu_fans_zone);

    led atc800_fan_led;
    atc800_fan_led.name = "Fan";
    leds.push_back(atc800_fan_led);

    zone atc800_top_zone;
    atc800_top_zone.name           = "Top";
    atc800_top_zone.type           = ZONE_TYPE_SINGLE;
    atc800_top_zone.leds_min       = 1;
    atc800_top_zone.leds_max       = 1;
    atc800_top_zone.leds_count     = 1;
    atc800_top_zone.matrix_map     = NULL;
    zones.push_back(atc800_top_zone);

    led atc800_top_led;
    atc800_top_led.name = "Top";
    leds.push_back(atc800_top_led);

    SetupColors();
}

void RGBController_AorusATC800::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AorusATC800::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
    UpdateZoneLEDs(1);
}

void RGBController_AorusATC800::UpdateZoneLEDs(int zone)
{
    unsigned char mode = modes[active_mode].value;
    unsigned char red  = RGBGetRValue(colors[zone]);
    unsigned char grn  = RGBGetGValue(colors[zone]);
    unsigned char blu  = RGBGetBValue(colors[zone]);

    if (mode == AORUS_ATC800_MODE_OFF)
    {
        mode = 1;
        red  = 0;
        grn  = 0;
        blu  = 0;
    }

    cooler->SendCoolerMode(mode, modes[active_mode].speed, zone, red, grn, blu);
}

void RGBController_AorusATC800::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_AorusATC800::SetCustomMode()
{

}

void RGBController_AorusATC800::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
