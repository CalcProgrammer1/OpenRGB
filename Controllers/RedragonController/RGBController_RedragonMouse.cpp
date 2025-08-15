/*---------------------------------------------------------*\
| RGBController_RedragonMouse.cpp                           |
|                                                           |
|   RGBController for Redragon mouse                        |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Mar 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_RedragonMouse.h"

/**------------------------------------------------------------------*\
    @name Redragon Mice
    @category Mouse
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectRedragonMice
    @comment
\*-------------------------------------------------------------------*/

RGBController_RedragonMouse::RGBController_RedragonMouse(RedragonMouseController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "Redragon";
    type                    = DEVICE_TYPE_MOUSE;
    description             = "Redragon Mouse Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode Static;
    Static.name             = "Static";
    Static.value            = REDRAGON_MOUSE_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Wave;
    Wave.name               = "Wave";
    Wave.value              = REDRAGON_MOUSE_MODE_WAVE;
    Wave.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(Wave);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = REDRAGON_MOUSE_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Rainbow;
    Rainbow.name            = "Rainbow";
    Rainbow.value           = REDRAGON_MOUSE_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Flashing;
    Flashing.name           = "Flashing";
    Flashing.value          = REDRAGON_MOUSE_MODE_FLASHING;
    Flashing.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Flashing.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    SetupZones();
}

RGBController_RedragonMouse::~RGBController_RedragonMouse()
{
    delete controller;
}

void RGBController_RedragonMouse::SetupZones()
{
    zone mouse_zone;
    mouse_zone.name         = "Mouse";
    mouse_zone.type         = ZONE_TYPE_SINGLE;
    mouse_zone.leds_min     = REDRAGON_MOUSE_LED_COUNT;
    mouse_zone.leds_max     = REDRAGON_MOUSE_LED_COUNT;
    mouse_zone.leds_count   = REDRAGON_MOUSE_LED_COUNT;
    mouse_zone.matrix_map   = NULL;
    zones.push_back(mouse_zone);

    led mouse_led;
    mouse_led.name          = "Mouse";
    leds.push_back(mouse_led);

    SetupColors();
}

void RGBController_RedragonMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RedragonMouse::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SendMouseColor(red, grn, blu);
    controller->SendMouseApply();
}

void RGBController_RedragonMouse::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RedragonMouse::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RedragonMouse::DeviceUpdateMode()
{
    bool random       = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    if((modes[active_mode].value == REDRAGON_MOUSE_MODE_BREATHING) && random)
    {
        controller->SendMouseMode(REDRAGON_MOUSE_MODE_RANDOM_BREATHING, 0, red, grn, blu);
    }
    else
    {
        controller->SendMouseMode(modes[active_mode].value, 0, red, grn, blu);
    }

    controller->SendMouseApply();
}
