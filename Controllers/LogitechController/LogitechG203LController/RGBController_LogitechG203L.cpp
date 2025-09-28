/*---------------------------------------------------------*\
| RGBController_LogitechG203L.cpp                           |
|                                                           |
|   Driver for Logitech G203L                               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_LogitechG203L.h"

/**------------------------------------------------------------------*\
    @name Logitech G203L
    @category Mouse
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectLogitechMouseG203L
    @comment
\*-------------------------------------------------------------------*/

RGBController_LogitechG203L::RGBController_LogitechG203L(LogitechG203LController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "Logitech";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "Logitech Mouse Device";
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = LOGITECH_G203L_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = LOGITECH_G203L_MODE_OFF;
    Off.flags                   = 0;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = LOGITECH_G203L_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Cycle;
    Cycle.name                  = "Cycle";
    Cycle.value                 = LOGITECH_G203L_MODE_CYCLE;
    Cycle.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Cycle.color_mode            = MODE_COLORS_NONE;
    Cycle.speed_min             = 0x4E20;
    Cycle.speed_max             = 0x03E8;
    Cycle.brightness            = 20;
    Cycle.brightness_min        = 0;
    Cycle.brightness_max        = 20;
    modes.push_back(Cycle);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = LOGITECH_G203L_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed_min         = 0x4E20;
    Breathing.speed_max         = 0x03E8;
    Breathing.brightness        = 20;
    Breathing.brightness_min    = 0;
    Breathing.brightness_max    = 20;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = LOGITECH_G203L_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.color_mode             = MODE_COLORS_NONE;
    Wave.speed_min              = 0x4E20;
    Wave.speed_max              = 0x03E8;
    Wave.brightness             = 20;
    Wave.brightness_min         = 0;
    Wave.brightness_max         = 20;
    modes.push_back(Wave);

    mode Colormixing;
    Colormixing.name            = "Colormixing";
    Colormixing.value           = LOGITECH_G203L_MODE_COLORMIXING;
    Colormixing.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Colormixing.color_mode      = MODE_COLORS_NONE;
    Colormixing.speed_min       = 0x4E20;
    Colormixing.speed_max       = 0x03E8;
    Colormixing.brightness      = 20;
    Colormixing.brightness_min  = 0;
    Colormixing.brightness_max  = 20;
    modes.push_back(Colormixing);

    SetupZones();
}

RGBController_LogitechG203L::~RGBController_LogitechG203L()
{
    delete controller;
}

void RGBController_LogitechG203L::SetupZones()
{
    zone g203L_zone;
    g203L_zone.name         = "Mouse Zone";
    g203L_zone.type         = ZONE_TYPE_LINEAR;
    g203L_zone.leds_min     = 3;
    g203L_zone.leds_max     = 3;
    g203L_zone.leds_count   = 3;
    g203L_zone.matrix_map   = NULL;
    zones.push_back(g203L_zone);

    led g203L_led_l;
    g203L_led_l.name        = "Mouse Left";
    g203L_led_l.value       = 1;
    leds.push_back(g203L_led_l);

    led g203L_led_c;
    g203L_led_c.name        = "Mouse Center";
    g203L_led_c.value       = 2;
    leds.push_back(g203L_led_c);

    led g203L_led_r;
    g203L_led_r.name        = "Mouse Right";
    g203L_led_r.value       = 3;
    leds.push_back(g203L_led_r);

    SetupColors();
}

void RGBController_LogitechG203L::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechG203L::DeviceUpdateLEDs()
{
    controller->SetDevice(colors);
    controller->SetDevice(colors); //dirty workaround for color lag
}

void RGBController_LogitechG203L::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG203L::UpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);

    controller->SetSingleLED(leds[led].value, red, grn, blu);
    controller->SetSingleLED(leds[led].value, red, grn, blu); //dirty workaround for color lag
}

void RGBController_LogitechG203L::DeviceUpdateMode()
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;
    unsigned char dir = 0;

    if(modes[active_mode].color_mode & MODE_COLORS_MODE_SPECIFIC)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_DIRECTION_LR)
    {
        dir = (unsigned char)modes[active_mode].direction;
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        //dunno where brightness is
    }

    if(modes[active_mode].value == LOGITECH_G203L_MODE_DIRECT)
    {
        controller->SetDevice(colors);
    }
    else
    {
        controller->SetMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, dir, red, grn, blu);
    }
}
