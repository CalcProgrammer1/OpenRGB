/*---------------------------------------------------------*\
| RGBController_SteelSeriesRival.cpp                        |
|                                                           |
|   RGBController for SteelSeries Rival                     |
|                                                           |
|   B Horn (bahorn)                             13 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_SteelSeriesRival.h"

typedef struct
{
    const char* name;
    const int   value;
} steelseries_rival_led_info;

static const steelseries_rival_led_info rival_650_leds[]=
{
    {"Left 1",          0x12},
    {"Left 2",          0x14},
    {"Left 3",          0x16},
    {"Right 1",         0x13},
    {"Right 2",         0x15},
    {"Right 3",         0x17},
};

static const steelseries_rival_led_info rival_600_leds[]=
{
    {"Left top",        0x02},
    {"Left mid",        0x04},
    {"Left bottom",     0x06},
    {"Right top",       0x03},
    {"Right mid",       0x05},
    {"Right bottom",    0x07},
};

/**------------------------------------------------------------------*\
    @name Steel Series Rival
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectSteelSeriesRival100,DetectSteelSeriesRival300,DetectSteelSeriesRival600,DetectSteelSeriesRival650,DetectSteelSeriesRival700
    @comment
\*-------------------------------------------------------------------*/

RGBController_SteelSeriesRival::RGBController_SteelSeriesRival(SteelSeriesRivalController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetDeviceName();
    vendor              = "SteelSeries";
    type                = DEVICE_TYPE_MOUSE;
    description         = "SteelSeries Rival Device";
    location            = controller->GetDeviceLocation();
    serial              = controller->GetSerialString();
    version             = controller->GetFirmwareVersion();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = STEELSERIES_RIVAL_DIRECT;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Pulsate;
    Pulsate.name        = "Pulsate";
    Pulsate.value       = STEELSERIES_RIVAL_PULSATE;
    Pulsate.flags       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Pulsate.color_mode  = MODE_COLORS_PER_LED;
    Pulsate.speed_min   = STEELSERIES_RIVAL_EFFECT_PULSATE_MIN;
    Pulsate.speed_max   = STEELSERIES_RIVAL_EFFECT_PULSATE_MAX;
    Pulsate.speed       = STEELSERIES_RIVAL_EFFECT_PULSATE_MID;
    modes.push_back(Pulsate);

    SetupZones();
}

RGBController_SteelSeriesRival::~RGBController_SteelSeriesRival()
{
    delete controller;
}

void RGBController_SteelSeriesRival::SetupZones()
{
    /* Rival 100 Series only has one Zone */
    zone logo_zone;
    logo_zone.name          = "Logo";
    logo_zone.type          = ZONE_TYPE_SINGLE;
    logo_zone.leds_min      = 1;
    logo_zone.leds_max      = 1;
    logo_zone.leds_count    = 1;
    logo_zone.matrix_map    = NULL;
    zones.push_back(logo_zone);

    led logo_led;
    logo_led.name           = "Logo";
    logo_led.value          = 0;
    leds.push_back(logo_led);

    /* Rival 300 and 700 extend this by adding Scroll Wheel LED + Zone */
    if(controller->GetMouseType() == RIVAL_300 ||
       controller->GetMouseType() == RIVAL_700)
    {
        zone wheel_zone;
        wheel_zone.name         = "Scroll Wheel";
        wheel_zone.type         = ZONE_TYPE_SINGLE;
        wheel_zone.leds_min     = 1;
        wheel_zone.leds_max     = 1;
        wheel_zone.leds_count   = 1;
        wheel_zone.matrix_map   = NULL;
        zones.push_back(wheel_zone);

        led wheel_led;
        wheel_led.name          = "Scroll Wheel";
        wheel_led.value         = 1;
        leds.push_back(wheel_led);
    }
    /* Rival 650 extends this by Scroll Wheel LED + Zone and additional lights LEDs + Zone */
    else if(controller->GetMouseType() == RIVAL_650)
    {
        leds[0].value           = 0x11;

        zone wheel_zone;
        wheel_zone.name         = "Scroll Wheel";
        wheel_zone.type         = ZONE_TYPE_SINGLE;
        wheel_zone.leds_min     = 1;
        wheel_zone.leds_max     = 1;
        wheel_zone.leds_count   = 1;
        wheel_zone.matrix_map   = NULL;
        zones.push_back(wheel_zone);

        led wheel_led;
        wheel_led.name          = "Scroll Wheel";
        wheel_led.value         = 0x10;
        leds.push_back(wheel_led);

        zone mouse_zone;
        mouse_zone.name         = "Mouse";
        mouse_zone.type         = ZONE_TYPE_LINEAR;
        mouse_zone.leds_min     = 6;
        mouse_zone.leds_max     = 6;
        mouse_zone.leds_count   = 6;
        mouse_zone.matrix_map   = NULL;
        zones.push_back(mouse_zone);

        for(const steelseries_rival_led_info led_info: rival_650_leds)
        {
            led mouse_led;
            mouse_led.name      = led_info.name;
            mouse_led.value     = led_info.value;
            leds.push_back(mouse_led);
        }
    }
    /* Rival 600 is simular to Rival 650 */
    else if(controller->GetMouseType() == RIVAL_600)
    {
        leds[0].value           = 0x01;

        zone wheel_zone;
        wheel_zone.name         = "Scroll Wheel";
        wheel_zone.type         = ZONE_TYPE_SINGLE;
        wheel_zone.leds_min     = 1;
        wheel_zone.leds_max     = 1;
        wheel_zone.leds_count   = 1;
        wheel_zone.matrix_map   = NULL;
        zones.push_back(wheel_zone);

        led wheel_led;
        wheel_led.name          = "Scroll Wheel";
        wheel_led.value         = 0x00;
        leds.push_back(wheel_led);

        zone mouse_zone;
        mouse_zone.name         = "Mouse";
        mouse_zone.type         = ZONE_TYPE_LINEAR;
        mouse_zone.leds_min     = 6;
        mouse_zone.leds_max     = 6;
        mouse_zone.leds_count   = 6;
        mouse_zone.matrix_map   = NULL;
        zones.push_back(mouse_zone);

        for(const steelseries_rival_led_info led_info: rival_600_leds)
        {
            led mouse_led;
            mouse_led.name      = led_info.name;
            mouse_led.value     = led_info.value;
            leds.push_back(mouse_led);
        }
    }

    SetupColors();
}

void RGBController_SteelSeriesRival::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SteelSeriesRival::DeviceUpdateLEDs()
{
    for(unsigned int i = 0; i < leds.size(); i++)
    {
        unsigned char red = RGBGetRValue(colors[i]);
        unsigned char grn = RGBGetGValue(colors[i]);
        unsigned char blu = RGBGetBValue(colors[i]);
        controller->SetColor(leds[i].value, red, grn, blu);
    }
}

void RGBController_SteelSeriesRival::UpdateZoneLEDs(int zone)
{
    for(unsigned int i = 0; i < zones[zone].leds_count; i++)
    {
        unsigned char red = RGBGetRValue(zones[zone].colors[i]);
        unsigned char grn = RGBGetGValue(zones[zone].colors[i]);
        unsigned char blu = RGBGetBValue(zones[zone].colors[i]);
        controller->SetColor(zones[zone].leds[i].value, red, grn, blu);
    }
}

void RGBController_SteelSeriesRival::UpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);
    controller->SetColor(leds[led].value, red, grn, blu);
}

void RGBController_SteelSeriesRival::DeviceUpdateMode()
{
    /* Strictly, the device actually does support different modes for the
     * different zones, but we don't support that. */
    //steelseries_type mouse_type = rival->GetMouseType();
    switch (modes[active_mode].value)
    {
        case STEELSERIES_RIVAL_DIRECT:
            controller->SetLightEffectAll(STEELSERIES_RIVAL_EFFECT_DIRECT);
            break;

        case STEELSERIES_RIVAL_PULSATE:
            controller->SetLightEffectAll(modes[active_mode].speed);
            break;
    }

    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesRival::DeviceSaveMode()
{
    DeviceUpdateMode();
    controller->SaveMode();
}
