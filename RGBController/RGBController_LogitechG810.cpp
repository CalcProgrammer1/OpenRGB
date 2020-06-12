/*-----------------------------------------*\
|  RGBController_LogitechG810.cpp           |
|                                           |
|  Generic RGB Interface for Logitech G810  |
|  Orion Spectrum Keyboard                  |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/12/2020   |
\*-----------------------------------------*/

#include "RGBController_LogitechG810.h"

RGBController_LogitechG810::RGBController_LogitechG810(LogitechG810Controller* logitech_ptr)
{
    logitech = logitech_ptr;

    name        = "Logitech Keyboard Device";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Logitech Keyboard Device";

    mode Static;
    Static.name       = "Static";
    Static.value      = LOGITECH_G810_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);
    
    mode Off;
    Off.name       = "Off";
    Off.value      = LOGITECH_G810_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Cycle;
    Cycle.name       = "Cycle";
    Cycle.value      = LOGITECH_G810_MODE_CYCLE;
    Cycle.flags      = MODE_FLAG_HAS_SPEED;
    Cycle.color_mode = MODE_COLORS_NONE;
    Cycle.speed_min  = LOGITECH_G810_SPEED_SLOWEST;
    Cycle.speed_max  = LOGITECH_G810_SPEED_FASTEST;
    Cycle.speed      = LOGITECH_G810_SPEED_NORMAL;
    modes.push_back(Cycle);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = LOGITECH_G810_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.speed_min  = LOGITECH_G810_SPEED_SLOWEST;
    Breathing.speed_max  = LOGITECH_G810_SPEED_FASTEST;
    Breathing.speed      = LOGITECH_G810_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
}

void RGBController_LogitechG810::SetupZones()
{
    zone g810_zone;
    g810_zone.name           = "Keyboard Zone";
    g810_zone.type           = ZONE_TYPE_SINGLE;
    g810_zone.leds_min       = 1;
    g810_zone.leds_max       = 1;
    g810_zone.leds_count     = 1;
    g810_zone.matrix_map     = NULL;
    zones.push_back(g810_zone);

    led g810_led;
    g810_led.name = "Keyboard LED";
    leds.push_back(g810_led);

    SetupColors();
}

void RGBController_LogitechG810::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechG810::DeviceUpdateLEDs()
{

}

void RGBController_LogitechG810::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG810::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG810::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_LogitechG810::UpdateMode()
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    logitech->SetMode(modes[active_mode].value, modes[active_mode].speed, red, grn, blu);
}
