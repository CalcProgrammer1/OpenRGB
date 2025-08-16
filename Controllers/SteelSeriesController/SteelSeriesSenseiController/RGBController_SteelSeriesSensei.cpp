/*---------------------------------------------------------*\
| RGBController_SteelSeriesSensei.cpp                       |
|                                                           |
|   RGBController for SteelSeries Sensei                    |
|                                                           |
|   Based on SteelSeries Rival controller                   |
|   B Horn (bahorn)                             13 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_SteelSeriesSensei.h"

/**------------------------------------------------------------------*\
    @name Steel Series Sensei
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectSteelSeriesSensei
    @comment
\*-------------------------------------------------------------------*/

RGBController_SteelSeriesSensei::RGBController_SteelSeriesSensei(SteelSeriesSenseiController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "SteelSeries";
    type                    = DEVICE_TYPE_MOUSE;
    description             = "SteelSeries Sensei Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = STEELSERIES_SENSEI_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = STEELSERIES_SENSEI_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    Breathing.speed_min     = STEELSERIES_SENSEI_EFFECT_BREATHING_MIN;
    Breathing.speed_max     = STEELSERIES_SENSEI_EFFECT_BREATHING_MAX;
    Breathing.speed         = STEELSERIES_SENSEI_EFFECT_BREATHING_MID;
    modes.push_back(Breathing);

    mode Rainbow;
    Rainbow.name            = "Rainbow";
    Rainbow.value           = STEELSERIES_SENSEI_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_HAS_SPEED;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    Rainbow.speed_min       = STEELSERIES_SENSEI_EFFECT_RAINBOW_MIN;
    Rainbow.speed_max       = STEELSERIES_SENSEI_EFFECT_RAINBOW_MAX;
    Rainbow.speed           = STEELSERIES_SENSEI_EFFECT_RAINBOW_MID;
    modes.push_back(Rainbow);

    SetupZones();
}

RGBController_SteelSeriesSensei::~RGBController_SteelSeriesSensei()
{
    delete controller;
}

void RGBController_SteelSeriesSensei::SetupZones()
{
    zone logo_zone;
    logo_zone.name          = "Logo";
    logo_zone.type          = ZONE_TYPE_SINGLE;
    logo_zone.leds_min      = 1;
    logo_zone.leds_max      = 1;
    logo_zone.leds_count    = 1;
    logo_zone.matrix_map    = NULL;
    zones.push_back(logo_zone);

    led logo_led;
    logo_led.name = "Logo";
    leds.push_back(logo_led);

    zone wheel_zone;
    wheel_zone.name         = "Scroll Wheel";
    wheel_zone.type         = ZONE_TYPE_SINGLE;
    wheel_zone.leds_min     = 1;
    wheel_zone.leds_max     = 1;
    wheel_zone.leds_count   = 1;
    wheel_zone.matrix_map   = NULL;
    zones.push_back(wheel_zone);

    led wheel_led;
    wheel_led.name = "Scroll Wheel";
    leds.push_back(wheel_led);

    SetupColors();
}

void RGBController_SteelSeriesSensei::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SteelSeriesSensei::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
    UpdateZoneLEDs(1);
}

void RGBController_SteelSeriesSensei::UpdateZoneLEDs(int zone)
{
    RGBColor      color = colors[zone];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    switch(modes[active_mode].value)
    {
        case STEELSERIES_SENSEI_MODE_DIRECT:
            controller->SetColor(zone, red, grn, blu);
            break;

        case STEELSERIES_SENSEI_MODE_BREATHING:
        case STEELSERIES_SENSEI_MODE_RAINBOW:
            controller->SetLightEffect(zone, modes[active_mode].value, modes[active_mode].speed, red, grn, blu);
            break;
    }
}

void RGBController_SteelSeriesSensei::UpdateSingleLED(int led)
{
    /*---------------------------------------------------------*\
    | Each zone only has a single LED, so we can use the LED ID |
    | to reference the existing zone code.                      |
    \*---------------------------------------------------------*/
    UpdateZoneLEDs(led);
}

void RGBController_SteelSeriesSensei::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | Strictly, the device actually does support different modes|
    | for the different zones, but we don't support that.       |
    \*---------------------------------------------------------*/
    DeviceUpdateLEDs();
}
