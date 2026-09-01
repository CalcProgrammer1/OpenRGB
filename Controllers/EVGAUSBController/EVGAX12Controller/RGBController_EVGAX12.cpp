/*---------------------------------------------------------*\
| RGBController_EVGAX12.cpp                                 |
|                                                           |
|   RGBController for the EVGA X12 gaming mouse             |
|                                                           |
|   QuickMythril                               08 Aug 2026  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_EVGAX12.h"

/**------------------------------------------------------------------*\
    @name EVGA X12 Gaming Mouse
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectEVGAX12
    @comment Lighting changes are volatile and revert to the onboard
        profile when the mouse is reconnected.
\*-------------------------------------------------------------------*/
RGBController_EVGAX12::RGBController_EVGAX12(EVGAX12Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "EVGA";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "EVGA X12 Gaming Mouse";
    serial                      = controller->GetSerialString();
    location                    = controller->GetDeviceLocation();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = EVGA_X12_MODE_STATIC;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min       = EVGA_X12_BRIGHTNESS_MIN;
    Direct.brightness_max       = EVGA_X12_BRIGHTNESS_MAX;
    Direct.brightness           = EVGA_X12_BRIGHTNESS_MAX;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    direct_mode_index           = (unsigned int)modes.size();
    modes.push_back(Direct);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = EVGA_X12_MODE_OFF;
    Off.color_mode              = MODE_COLORS_NONE;
    off_mode_index              = (unsigned int)modes.size();
    modes.push_back(Off);

    active_mode                 = direct_mode_index;

    SetupZones();

    EVGAX12LightingState current_state = controller->GetLightingState();

    if(current_state.static_valid)
    {
        modes[direct_mode_index].brightness = current_state.brightness[0];

        for(unsigned int led_idx = 0; led_idx < EVGA_X12_LED_COUNT; led_idx++)
        {
            colors[led_idx] = current_state.colors[led_idx];
        }
    }

    if(current_state.modes_valid)
    {
        bool all_off = true;

        for(unsigned int led_idx = 0; led_idx < EVGA_X12_LED_COUNT; led_idx++)
        {
            if(current_state.modes[led_idx] != EVGA_X12_MODE_OFF)
            {
                all_off = false;
                break;
            }
        }

        if(all_off)
        {
            active_mode = off_mode_index;
        }
    }
}

RGBController_EVGAX12::~RGBController_EVGAX12()
{
    Shutdown();

    delete controller;
}

void RGBController_EVGAX12::SetupZones()
{
    const char* zone_names[EVGA_X12_LED_COUNT] =
    {
        "Side",
        "Scroll Wheel",
        "Logo",
    };

    for(unsigned int led_idx = 0; led_idx < EVGA_X12_LED_COUNT; led_idx++)
    {
        zone new_zone;
        new_zone.name         = zone_names[led_idx];
        new_zone.type         = ZONE_TYPE_SINGLE;
        new_zone.leds_min     = 1;
        new_zone.leds_max     = 1;
        new_zone.leds_count   = 1;
        zones.push_back(new_zone);

        led new_led;
        new_led.name          = std::string(zone_names[led_idx]) + " LED";
        new_led.value         = led_idx;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_EVGAX12::DeviceUpdateLEDs()
{
    controller->SetStatic(colors, modes[direct_mode_index].brightness);
}

void RGBController_EVGAX12::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGAX12::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGAX12::DeviceUpdateMode()
{
    if(modes[active_mode].value == EVGA_X12_MODE_OFF)
    {
        controller->SetMode(EVGA_X12_MODE_OFF);
    }
    else
    {
        controller->SetStaticAndActivate(colors, modes[direct_mode_index].brightness);
    }
}
