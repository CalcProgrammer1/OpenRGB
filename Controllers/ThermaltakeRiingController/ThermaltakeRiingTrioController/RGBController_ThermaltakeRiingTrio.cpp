/*---------------------------------------------------------*\
| RGBController_ThermaltakeRiingTrio.cpp                    |
|                                                           |
|   RGBController for Thermaltake Riing Trio                |
|                                                           |
|   Sam B (4rcheria)                            24 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_ThermaltakeRiingTrio.h"

/**------------------------------------------------------------------*\
    @name Thermaltake Riing Trio
    @category Cooler
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectThermaltakeRiingTrioControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_ThermaltakeRiingTrio::RGBController_ThermaltakeRiingTrio(ThermaltakeRiingTrioController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Thermaltake Trio";
    vendor      = "Thermaltake";
    type        = DEVICE_TYPE_COOLER;
    description = "Thermaltake Riing Trio Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerial();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = THERMALTAKE_TRIO_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.speed_min  = 0;
    Direct.speed_max  = 0;
    Direct.speed      = 0;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_ThermaltakeRiingTrio::~RGBController_ThermaltakeRiingTrio()
{
    delete controller;
}

void RGBController_ThermaltakeRiingTrio::SetupZones()
{
    /*-------------------------------------------------*\
    | Only set LED count on the first run               |
    \*-------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(THERMALTAKE_TRIO_NUM_CHANNELS);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    for (unsigned int channel_idx = 0; channel_idx < THERMALTAKE_TRIO_NUM_CHANNELS; channel_idx++)
    {
        char ch_idx_string[2];
        snprintf(ch_idx_string, 2, "%d", channel_idx + 1);

        zones[channel_idx].name     = "Riing Channel ";
        zones[channel_idx].name.append(ch_idx_string);
        zones[channel_idx].type     = ZONE_TYPE_LINEAR;

        /*-------------------------------------------------*\
        | The maximum number of colors that would fit in the|
        | Riing Trio protocol is 54                         |
        \*-------------------------------------------------*/
        zones[channel_idx].leds_min   = 0;
        zones[channel_idx].leds_max   = 54;

        if(first_run)
        {
            zones[channel_idx].leds_count = 0;
        }

        zones[channel_idx].matrix_map = NULL;

        for (unsigned int led_ch_idx = 0; led_ch_idx < zones[channel_idx].leds_count; led_ch_idx++)
        {
            char led_idx_string[3];
            snprintf(led_idx_string, 3, "%d", led_ch_idx + 1);

            led new_led;
            new_led.name = "Riing Channel ";
            new_led.name.append(ch_idx_string);
            new_led.name.append(", LED ");
            new_led.name.append(led_idx_string);

            leds.push_back(new_led);
            leds_channel.push_back(channel_idx);
        }
    }

    SetupColors();
}

void RGBController_ThermaltakeRiingTrio::ResizeZone(int zone, int new_size)
{
    if((size_t) zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

        SetupZones();
    }
}

void RGBController_ThermaltakeRiingTrio::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        controller->SetChannelLEDs((unsigned char)zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}

void RGBController_ThermaltakeRiingTrio::UpdateZoneLEDs(int zone)
{
    controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
}

void RGBController_ThermaltakeRiingTrio::UpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];

    controller->SetChannelLEDs(channel, zones[channel].colors, zones[channel].leds_count);
}

void RGBController_ThermaltakeRiingTrio::DeviceUpdateMode()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        controller->SetMode(modes[active_mode].value, modes[active_mode].speed);
        controller->SetChannelLEDs((unsigned char)zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}
