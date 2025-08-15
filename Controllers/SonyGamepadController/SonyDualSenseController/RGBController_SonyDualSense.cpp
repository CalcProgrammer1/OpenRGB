/*---------------------------------------------------------*\
| RGBController_SonyDualSense.cpp                           |
|                                                           |
|   RGBController for Sony DualSense                        |
|                                                           |
|   Flora Aubry                                 01 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <iostream>
#include "RGBController_SonyDualSense.h"

/**------------------------------------------------------------------*\
    @name Sony Dual Sense controller
    @category Gamepad
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectSonyDualSenseControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_SonyDualSense::RGBController_SonyDualSense(SonyDualSenseController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetName();

    if(controller->IsBluetooth())
    {
        name.append(" (BT)");
    }

    vendor                      = "Sony";
    type                        = DEVICE_TYPE_GAMEPAD;
    description                 = "Sony DualSense Device";
    location                    = controller->GetLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.value                = SONY_DUALSENSE_DIRECT_MODE_VALUE;
    Direct.name                 = "Direct";
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    Direct.brightness_min       = SONY_DUALSENSE_BRIGHTNESS_MIN;
    Direct.brightness_max       = SONY_DUALSENSE_BRIGHTNESS_MAX;
    Direct.brightness           = SONY_DUALSENSE_DEFAULT_BRIGHTNESS;
    modes.push_back(Direct);

    mode Micoff;
    Micoff.value                = SONY_DUALSENSE_MIC_OFF_MODE_VALUE;
    Micoff.name                 = "Mic Off (Direct)";
    Micoff.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Micoff.color_mode           = MODE_COLORS_PER_LED;
    Micoff.brightness_min       = SONY_DUALSENSE_BRIGHTNESS_MIN;
    Micoff.brightness_max       = SONY_DUALSENSE_BRIGHTNESS_MAX;
    Micoff.brightness           = SONY_DUALSENSE_DEFAULT_BRIGHTNESS;
    modes.push_back(Micoff);

    mode Micpulse;
    Micpulse.value              = SONY_DUALSENSE_MIC_PULSE_MODE_VALUE;
    Micpulse.name               = "Mic Pulse (Direct)";
    Micpulse.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Micpulse.color_mode         = MODE_COLORS_PER_LED;
    Micpulse.brightness_min     = SONY_DUALSENSE_BRIGHTNESS_MIN;
    Micpulse.brightness_max     = SONY_DUALSENSE_BRIGHTNESS_MAX;
    Micpulse.brightness         = SONY_DUALSENSE_DEFAULT_BRIGHTNESS;
    modes.push_back(Micpulse);

    SetupZones();
}

RGBController_SonyDualSense::~RGBController_SonyDualSense()
{
    delete controller;
}

void RGBController_SonyDualSense::SetupZones()
{
    /*---------------------------------------------------------*\
    | This device only has one LED, so create a single zone and |
    | LED for it                                                |
    \*---------------------------------------------------------*/
    zone lightbar;
    lightbar.name            = "Lightbar";
    lightbar.type            = ZONE_TYPE_SINGLE;
    lightbar.leds_min        = SONY_DUALSENSE_LIGHTBAR_LED_COUNT;
    lightbar.leds_max        = SONY_DUALSENSE_LIGHTBAR_LED_COUNT;
    lightbar.leds_count      = SONY_DUALSENSE_LIGHTBAR_LED_COUNT;
    lightbar.matrix_map      = NULL;
    zones.push_back(lightbar);

    zone playerleds;
    playerleds.name          = "Player LEDs";
    playerleds.type          = ZONE_TYPE_LINEAR;
    playerleds.leds_min      = SONY_DUALSENSE_PLAYER_LED_COUNT;
    playerleds.leds_max      = SONY_DUALSENSE_PLAYER_LED_COUNT;
    playerleds.leds_count    = SONY_DUALSENSE_PLAYER_LED_COUNT;
    playerleds.matrix_map    = NULL;
    zones.push_back(playerleds);

    leds.resize(SONY_DUALSENSE_LIGHTBAR_LED_COUNT + SONY_DUALSENSE_PLAYER_LED_COUNT);

    leds[0].name = "LED 1";

    for(unsigned int i = 0 ; i < SONY_DUALSENSE_PLAYER_LED_COUNT; i++)
    {
        leds[i + 1].name = "Player " + std::to_string(i + 1);
    }

    SetupColors();
}

void RGBController_SonyDualSense::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SonyDualSense::DeviceUpdateLEDs()
{
    controller->SetColors(colors, modes[active_mode].brightness, modes[active_mode].value);
}

void RGBController_SonyDualSense::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SonyDualSense::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SonyDualSense::DeviceUpdateMode()
{
}
