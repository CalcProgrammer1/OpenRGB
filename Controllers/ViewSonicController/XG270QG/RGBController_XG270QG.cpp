/*---------------------------------------------------------*\
| RGBController_XG270QG.cpp                                 |
|                                                           |
|   RGBController for ViewSonic XG270QG                     |
|                                                           |
|   Lanzaa                                      23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
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
    controller              = controller_ptr;

    name                    = controller->GetName();
    vendor                  = "ViewSonic";
    type                    = DEVICE_TYPE_MONITOR;
    description             = "ViewSonic Monitor Device";
    location                = controller->GetLocation();
    serial                  = controller->GetSerial();

    mode Off;
    Off.name                = "Off";
    Off.value               = VS_XG270QG_Controller::VS_MODE_OFF;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Custom;
    Custom.name             = "Custom";
    Custom.value            = VS_XG270QG_Controller::VS_MODE_STATIC;
    Custom.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Custom.colors_min       = 1;
    Custom.colors_max       = 1;
    Custom.color_mode       = MODE_COLORS_PER_LED;
    Custom.colors.resize(1);
    modes.push_back(Custom);

    mode Rainbow;
    Rainbow.name            = "Rainbow Wave";
    Rainbow.value           = VS_XG270QG_Controller::VS_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Breath;
    Breath.name             = "Breathing";
    Breath.value            = VS_XG270QG_Controller::VS_MODE_BREATHING;
    Breath.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breath.colors_min       = 1;
    Breath.colors_max       = 1;
    Breath.color_mode       = MODE_COLORS_PER_LED;
    Breath.colors.resize(1);
    modes.push_back(Breath);

    mode Waterfall;
    Waterfall.name          = "Waterfall";
    Waterfall.value         = VS_XG270QG_Controller::VS_MODE_WATERFALL;
    Waterfall.flags         = MODE_FLAG_AUTOMATIC_SAVE;
    Waterfall.color_mode    = MODE_COLORS_NONE;
    modes.push_back(Waterfall);

    mode Elite;
    Elite.name              = "Elite";
    Elite.value             = VS_XG270QG_Controller::VS_MODE_ELITE;
    Elite.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Elite.colors_min        = 1;
    Elite.colors_max        = 1;
    Elite.color_mode        = MODE_COLORS_PER_LED;
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
    zone base;
    base.name               = "Base";
    base.type               = ZONE_TYPE_SINGLE;
    base.leds_min           = 1;
    base.leds_max           = 1;
    base.leds_count         = 1;
    base.matrix_map         = NULL;
    zones.push_back(base);

    zone rear;
    rear.name               = "Rear";
    rear.type               = ZONE_TYPE_SINGLE;
    rear.leds_min           = 1;
    rear.leds_max           = 1;
    rear.leds_count         = 1;
    rear.matrix_map         = NULL;
    zones.push_back(rear);

    led d;
    d.name                  = "Base";
    d.value                 = 0x00;
    leds.push_back(d);

    led back;
    back.name               = "Rear";
    back.value              = 0x01;
    leds.push_back(back);

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
    DeviceUpdateMode();
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
    uint8_t r1 = 0;
    uint8_t g1 = 0;
    uint8_t b1 = 0;
    uint8_t r2 = 0;
    uint8_t g2 = 0;
    uint8_t b2 = 0;

    if(modes[active_mode].flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        r1 = r2 = RGBGetRValue(modes[active_mode].colors[0]);
        g1 = g2 = RGBGetGValue(modes[active_mode].colors[0]);
        b1 = b2 = RGBGetBValue(modes[active_mode].colors[0]);
    }
    else if (modes[active_mode].flags & MODE_FLAG_HAS_PER_LED_COLOR)
    {
        r1 = RGBGetRValue(colors[0]);
        g1 = RGBGetGValue(colors[0]);
        b1 = RGBGetBValue(colors[0]);

        r2 = RGBGetRValue(colors[1]);
        g2 = RGBGetGValue(colors[1]);
        b2 = RGBGetBValue(colors[1]);
    }
    controller->SetMode(modes[active_mode].value, r1, g1, b1, modes[active_mode].value, r2, g2, b2);
}
