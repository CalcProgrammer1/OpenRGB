/*---------------------------------------------------------*\
| RGBController_RoccatKova.cpp                              |
|                                                           |
|   RGBController for Roccat Kova                           |
|                                                           |
|   Gustash                                     01 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_RoccatKova.h"

/**------------------------------------------------------------------*\
    @name Roccat Kova
    @category Mouse
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors RoccatControllerDetect
    @comment Color Flow mode is only supported starting at the first
    preset color in the mouse's memory, and color offsets for each LED
    are not supported. You'd need to use Swarm if you intend to use that
    specific feature.
\*-------------------------------------------------------------------*/

RGBController_RoccatKova::RGBController_RoccatKova(RoccatKovaController* controller_ptr)
{
    controller                    = controller_ptr;

    name                          = controller->GetName();
    vendor                        = "Roccat";
    type                          = DEVICE_TYPE_MOUSE;
    description                   = "Roccat Kova Mouse Device";
    serial                        = controller->GetSerial();
    location                      = controller->GetLocation();
    version                       = controller->GetVersion();

    mode Static;
    Static.name                   = "Static";
    Static.value                  = ROCCAT_KOVA_MODE_STATIC;
    Static.flags                  = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode ColorFlow;
    ColorFlow.name                = "Color Flow";
    ColorFlow.value               = ROCCAT_KOVA_MODE_COLOR_FLOW;
    ColorFlow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    ColorFlow.color_mode          = MODE_COLORS_RANDOM;
    ColorFlow.speed_min           = ROCCAT_KOVA_SPEED_MIN;
    ColorFlow.speed_max           = ROCCAT_KOVA_SPEED_MAX;
    modes.push_back(ColorFlow);

    mode Flashing;
    Flashing.name                 = "Flashing";
    Flashing.value                = ROCCAT_KOVA_MODE_FLASHING;
    Flashing.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Flashing.color_mode           = MODE_COLORS_PER_LED;
    Flashing.speed_min            = ROCCAT_KOVA_SPEED_MIN;
    Flashing.speed_max            = ROCCAT_KOVA_SPEED_MAX;
    modes.push_back(Flashing);

    mode Breathing;
    Breathing.name                = "Breathing";
    Breathing.value               = ROCCAT_KOVA_MODE_BREATHING;
    Breathing.flags               = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode          = MODE_COLORS_PER_LED;
    Breathing.speed_min           = ROCCAT_KOVA_SPEED_MIN;
    Breathing.speed_max           = ROCCAT_KOVA_SPEED_MAX;
    modes.push_back(Breathing);

    mode Off;
    Off.name                      = "Off";
    Off.value                     = 0x00;
    Off.flags                     = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode                = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_RoccatKova::~RGBController_RoccatKova()
{
    delete controller;
}

void RGBController_RoccatKova::SetupZones()
{
    zone Mouse;
    Mouse.name       = "Mouse";
    Mouse.type       = ZONE_TYPE_LINEAR;
    Mouse.leds_count = ROCCAT_KOVA_LED_COUNT;
    Mouse.leds_min   = ROCCAT_KOVA_LED_COUNT;
    Mouse.leds_max   = ROCCAT_KOVA_LED_COUNT;
    Mouse.matrix_map = NULL;
    zones.push_back(Mouse);

    led WheelLED;
    WheelLED.name    = "Wheel LED";
    WheelLED.value   = ROCCAT_KOVA_WHEEL_IDX;
    leds.push_back(WheelLED);

    led StripeLED;
    StripeLED.name   = "Stripe LED";
    StripeLED.value  = ROCCAT_KOVA_PIPE_IDX;
    leds.push_back(StripeLED);

    SetupColors();
}

void RGBController_RoccatKova::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RoccatKova::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_RoccatKova::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateMode();
}

void RGBController_RoccatKova::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateMode();
}

void RGBController_RoccatKova::DeviceUpdateMode()
{
    mode &active       = modes[active_mode];
    int mode           = active.value;
    bool is_color_flow = active.color_mode == MODE_COLORS_RANDOM;
    if(active.value == ROCCAT_KOVA_MODE_COLOR_FLOW)
    {
        mode           = ROCCAT_KOVA_MODE_STATIC;
        is_color_flow  = true;
    }

    controller->SetColor(colors[0], colors[1], mode, active.speed, is_color_flow);
}
