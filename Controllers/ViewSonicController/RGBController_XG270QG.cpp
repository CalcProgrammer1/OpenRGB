/*---------------------------------------------------------*\
| RGBController_XG270QG.cpp                                 |
|                                                           |
|   RGBController for ViewSonic XG270QG                     |
|                                                           |
|   Lanzaa                                      23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include "RGBController_XG270QG.h"

/**------------------------------------------------------------------*\
    @name Viewsonic Monitor
    @category Accessory
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectViewSonic
    @comment
\*-------------------------------------------------------------------*/

RGBController_XG270QG::RGBController_XG270QG(VS_XG270QG_Controller* controller_ptr)
{
    controller  = controller_ptr;

    name        = "ViewSonic Elite XG270QG";
    vendor      = "ViewSonic";
    type        = DEVICE_TYPE_MONITOR;
    description = "ViewSonic Monitor";
    location    = controller->GetLocation();
    serial      = controller->GetSerial();

    mode Off;
    Off.name                = "Off";
    Off.value               = VS_MODE_OFF;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode StaticColor;
    StaticColor.name        = "Static";
    StaticColor.value       = VS_MODE_STATIC;
    StaticColor.flags       = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    StaticColor.colors_min  = 1;
    StaticColor.colors_max  = 1;
    StaticColor.color_mode  = MODE_COLORS_MODE_SPECIFIC;
    StaticColor.colors.resize(1);
    modes.push_back(StaticColor);

    mode Rainbow;
    Rainbow.name            = "Rainbow Wave";
    Rainbow.value           = VS_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Breath;
    Breath.name             = "Breathing";
    Breath.value            = VS_MODE_BREATHING;
    Breath.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breath.colors_min       = 1;
    Breath.colors_max       = 1;
    Breath.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Breath.colors.resize(1);
    modes.push_back(Breath);

    mode Waterfall;
    Waterfall.name          = "Waterfall";
    Waterfall.value         = VS_MODE_WATERFALL;
    Waterfall.flags         = MODE_FLAG_AUTOMATIC_SAVE;
    Waterfall.color_mode    = MODE_COLORS_NONE;
    modes.push_back(Waterfall);

    mode Elite;
    Elite.name              = "Elite";
    Elite.value             = VS_MODE_ELITE;
    Elite.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Elite.colors_min        = 1;
    Elite.colors_max        = 1;
    Elite.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Elite.colors.resize(1);
    modes.push_back(Elite);

    //mode Jazz;
    //Jazz.name       = "Jazz Wave (Audio Reactive)";
    //Jazz.value      = VS_MODE_JAZZ;
    ////Jazz.color_mode = MODE_COLORS_NONE; // might have color
    //Jazz.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    //Jazz.colors_min = 1;
    //Jazz.colors_max = 1;
    //Jazz.color_mode = MODE_COLORS_MODE_SPECIFIC;
    //Jazz.colors.resize(1);
    //modes.push_back(Jazz);

    //mode EliteGlobal;
    //EliteGlobal.name       = "Elite Global (Audio Reactive)";
    //EliteGlobal.value      = VS_MODE_ELITEGLOBAL;
    //EliteGlobal.flags      = MODE_FLAG_AUTOMATIC_SAVE;
    //EliteGlobal.color_mode = MODE_COLORS_NONE;
    //modes.push_back(EliteGlobal);

    RGBController_XG270QG::SetupZones();
}

void RGBController_XG270QG::SetupZones()
{
    zone back_circle;
    back_circle.name        = "Back Circle";
    back_circle.type        = ZONE_TYPE_SINGLE;
    back_circle.leds_min    = 1;
    back_circle.leds_max    = 1;
    back_circle.leds_count  = 1;
    back_circle.matrix_map  = NULL;
    zones.push_back(back_circle);

    zone down;
    down.name               = "Down Stuff";
    down.type               = ZONE_TYPE_SINGLE;
    down.leds_min           = 1;
    down.leds_max           = 1;
    down.leds_count         = 1;
    down.matrix_map         = NULL;
    zones.push_back(down);

    led back;
    back.name               = "Back";
    back.value              = 0x00;
    leds.push_back(back);

    led d;
    d.name                  = "Down";
    d.value                 = 0x01;
    leds.push_back(d);

    SetupColors();
}

void RGBController_XG270QG::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_XG270QG::DeviceUpdateLEDs()
{
    /*---------------------------------------------------------*\
    | This device does not include any per-LED modes            |
    \*---------------------------------------------------------*/
}

void RGBController_XG270QG::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_XG270QG::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_XG270QG::DeviceUpdateMode()
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    if(modes[active_mode].flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        r = RGBGetRValue(modes[active_mode].colors[0]);
        g = RGBGetGValue(modes[active_mode].colors[0]);
        b = RGBGetBValue(modes[active_mode].colors[0]);
    }
    controller->SetMode(modes[active_mode].value, r, g, b);
}
