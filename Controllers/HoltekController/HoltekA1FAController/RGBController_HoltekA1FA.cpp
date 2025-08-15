/*---------------------------------------------------------*\
| RGBController_HoltekA1FA.cpp                              |
|                                                           |
|   RGBController for Holtek mousemat                       |
|                                                           |
|   Edoardo Ridolfi (edo2313)                   26 Dec 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_HoltekA1FA.h"

/**------------------------------------------------------------------*\
    @name Holtek A1FA
    @category Mouse
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectHoltekMousemats
    @comment
\*-------------------------------------------------------------------*/

RGBController_HoltekA1FA::RGBController_HoltekA1FA(HoltekA1FAController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "Holtek";
    type                    = DEVICE_TYPE_MOUSEMAT;
    description             = "Holtek Mousemat Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode Static;
    Static.name             = "Static";
    Static.value            = HOLTEK_A1FA_MODE_STATIC;
    Static.speed            = HOLTEK_A1FA_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode       = MODE_COLORS_PER_LED;
    Static.colors_min       = 1;
    Static.colors_max       = 7;
    Static.colors.resize(7);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = HOLTEK_A1FA_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    Breathing.speed_min     = HOLTEK_A1FA_SPEED_SLOWEST;
    Breathing.speed_max     = HOLTEK_A1FA_SPEED_FASTEST;
    Breathing.speed         = HOLTEK_A1FA_SPEED_NORMAL;
    Breathing.colors_min    = 1;
    Breathing.colors_max    = 7;
    Breathing.colors.resize(7);
    modes.push_back(Breathing);

    mode Neon;
    Neon.name               = "Neon";
    Neon.value              = HOLTEK_A1FA_MODE_NEON;
    Neon.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Neon.color_mode         = MODE_COLORS_NONE;
    Neon.speed_min          = HOLTEK_A1FA_SPEED_SLOWEST;
    Neon.speed_max          = HOLTEK_A1FA_SPEED_FASTEST;
    Neon.speed              = HOLTEK_A1FA_SPEED_NORMAL;
    modes.push_back(Neon);

    mode Rainbow;
    Rainbow.name            = "Rainbow";
    Rainbow.value           = HOLTEK_A1FA_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    Rainbow.speed_min       = HOLTEK_A1FA_SPEED_SLOWEST;
    Rainbow.speed_max       = HOLTEK_A1FA_SPEED_FASTEST;
    Rainbow.speed           = HOLTEK_A1FA_SPEED_NORMAL;
    modes.push_back(Rainbow);

    SetupZones();
}

RGBController_HoltekA1FA::~RGBController_HoltekA1FA()
{
    delete controller;
}

void RGBController_HoltekA1FA::SetupZones()
{
    zone mouse_zone;
    mouse_zone.name         = "Mousemat";
    mouse_zone.type         = ZONE_TYPE_SINGLE;
    mouse_zone.leds_min     = 1;
    mouse_zone.leds_max     = 1;
    mouse_zone.leds_count   = 1;
    mouse_zone.matrix_map   = NULL;
    zones.push_back(mouse_zone);

    led mouse_led;
    mouse_led.name          = "Mousemat";
    leds.push_back(mouse_led);

    SetupColors();
}

void RGBController_HoltekA1FA::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HoltekA1FA::DeviceUpdateLEDs()
{
    unsigned char mode          = modes[active_mode].value;
    unsigned char brightness    = 0x20;  /*When brightness support is added, change this */
    unsigned char speed         = modes[active_mode].speed;
    unsigned char preset        = (modes[active_mode].color_mode == MODE_COLORS_RANDOM) ? 0x70 : 0x00;
    unsigned char red           = RGBGetRValue(colors[0]);
    unsigned char green         = RGBGetGValue(colors[0]);
    unsigned char blue          = RGBGetBValue(colors[0]);

    controller->SendData(mode, brightness, speed, preset, red, green, blue);
}

void RGBController_HoltekA1FA::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HoltekA1FA::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HoltekA1FA::DeviceUpdateMode()
{
    if((active_mode < HOLTEK_A1FA_MODE_NEON) && (previous_mode < HOLTEK_A1FA_MODE_NEON))
    {
        //If we're switching from and to static and breathing then sync the mode colors
        for(unsigned int i = 0; i < modes[active_mode].colors_max; i++)
        {
           modes[active_mode].colors[i] = modes[previous_mode].colors[i];
        }
    }

    previous_mode = active_mode;

    DeviceUpdateLEDs();
}
