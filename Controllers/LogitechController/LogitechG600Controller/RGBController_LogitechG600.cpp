/*---------------------------------------------------------*\
| RGBController_LogitechG600.cpp                            |
|                                                           |
|   RGBController for Logitech G600 Gaming Mouse            |
|                                                           |
|   Austin B     (austinleroy)                11 Sep 2025   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_LogitechG600.h"

/**------------------------------------------------------------------*\
    @name Logitech G600
    @category Mouse
    @type USB
    @save :o:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectLogitechMouseG600
    @comment
\*-------------------------------------------------------------------*/

RGBController_LogitechG600::RGBController_LogitechG600(LogitechG600Controller* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetNameString();
    vendor                          = "Logitech";
    type                            = DEVICE_TYPE_MOUSE;
    description                     = "Logitech Mouse Device";
    serial                          = controller->GetSerialString();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = LOGITECH_G600_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);
    
    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = LOGITECH_G600_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.speed_min             = LOGITECH_G600_SPEED_SLOWEST;
    Breathing.speed_max             = LOGITECH_G600_SPEED_FASTEST;
    Breathing.speed                 = LOGITECH_G600_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode Cycle;
    Cycle.name                      = "Spectrum Cycle";
    Cycle.value                     = LOGITECH_G600_MODE_CYCLE;
    Cycle.flags                     = MODE_FLAG_HAS_SPEED;
    Cycle.color_mode                = MODE_COLORS_NONE;
    Cycle.speed_min                 = LOGITECH_G600_SPEED_SLOWEST;
    Cycle.speed_max                 = LOGITECH_G600_SPEED_FASTEST;
    Cycle.speed                     = LOGITECH_G600_SPEED_NORMAL;
    modes.push_back(Cycle);

    SetupZones();
}

RGBController_LogitechG600::~RGBController_LogitechG600()
{
    delete controller;
}

void RGBController_LogitechG600::SetupZones()
{
    zone side_lights_zone;
    side_lights_zone.name       = "Side Lights";
    side_lights_zone.type       = ZONE_TYPE_SINGLE;
    side_lights_zone.leds_min   = 1;
    side_lights_zone.leds_max   = 1;
    side_lights_zone.leds_count = 1;
    side_lights_zone.matrix_map = NULL;
    zones.push_back(side_lights_zone);

    // Set up LED
    led g600_led;
    g600_led.name  = "All";
    leds.push_back(g600_led);

    SetupColors();
}

void RGBController_LogitechG600::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | Currently does not support resizing zones                 |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechG600::DeviceUpdateLEDs()
{
    controller->SetMode(modes[active_mode].value, modes[active_mode].speed, GetLED(0));
}

void RGBController_LogitechG600::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG600::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechG600::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode].value, modes[active_mode].speed, GetLED(0));
}
