/*---------------------------------------------------------*\
| RGBController_SteelSeriesApex3.cpp                        |
|                                                           |
|   RGBController for SteelSeries Apex 3                    |
|                                                           |
|   Chris M (Dr_No)                             23 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_SteelSeriesApex3.h"

/**------------------------------------------------------------------*\
    @name Steel Series Apex Tri Zone Keyboards
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectSteelSeriesApexTZone
    @comment
\*-------------------------------------------------------------------*/

RGBController_SteelSeriesApex3::RGBController_SteelSeriesApex3(SteelSeriesApex3Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "SteelSeries";
    type                        = DEVICE_TYPE_KEYBOARD;
    description                 = "SteelSeries Apex 3 Device";
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode direct;
    direct.name                 = "Direct";
    direct.value                = static_cast<int>(APEX3_MODES::DIRECT);
    direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    if(controller->SupportsSave())
    {
        direct.flags           |= MODE_FLAG_MANUAL_SAVE;
    }
    direct.color_mode           = MODE_COLORS_PER_LED;
    direct.brightness_min       = STEELSERIES_APEX3_BRIGHTNESS_MIN;
    direct.brightness_max       = controller->GetMaxBrightness();
    direct.brightness           = direct.brightness_max;
    modes.push_back(direct);

    if(controller->SupportsRainbowWave())
    {
        mode rainbow;
        rainbow.name            = "Rainbow Wave";
        rainbow.value           = static_cast<int>(APEX3_MODES::RAINBOW_WAVE);
        rainbow.flags           = MODE_FLAG_HAS_BRIGHTNESS;
        rainbow.color_mode      = MODE_COLORS_NONE;
        rainbow.brightness_min  = STEELSERIES_APEX3_BRIGHTNESS_MIN;
        rainbow.brightness_max  = controller->GetMaxBrightness();
        rainbow.brightness      = rainbow.brightness_max;
        modes.push_back(rainbow);
    }

    SetupZones();
}

RGBController_SteelSeriesApex3::~RGBController_SteelSeriesApex3()
{
    delete controller;
}

void RGBController_SteelSeriesApex3::DeviceSaveMode()
{
    controller->Save();
}

void RGBController_SteelSeriesApex3::SetupZones()
{
    uint8_t led_count       = controller->GetLedCount();

    zone curr_zone;
    curr_zone.name          = "Keyboard";
    curr_zone.type          = ZONE_TYPE_LINEAR;
    curr_zone.leds_min      = led_count;
    curr_zone.leds_max      = led_count;
    curr_zone.leds_count    = led_count;
    curr_zone.matrix_map    = NULL;
    zones.push_back(curr_zone);

    for(size_t i = 0; i < curr_zone.leds_count; i++)
    {
        led zone_led;
        zone_led.name = "LED " + std::to_string(i);
        leds.push_back(zone_led);
    }

    SetupColors();
}

void RGBController_SteelSeriesApex3::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SteelSeriesApex3::DeviceUpdateLEDs()
{
    controller->SetColor(colors, modes[active_mode].value, modes[active_mode].brightness);
}

void RGBController_SteelSeriesApex3::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesApex3::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesApex3::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_FLAG_HAS_PER_LED_COLOR)
    {
        DeviceUpdateLEDs();
    }
    else
    {
        controller->SetColor(modes[active_mode].colors, modes[active_mode].value, modes[active_mode].brightness);
    }
}
