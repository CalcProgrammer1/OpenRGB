/*---------------------------------------------------------*\
| RGBController_ThermaltakeRiing.cpp                        |
|                                                           |
|   RGBController for Thermaltake Riing                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_ThermaltakeRiing.h"

/**------------------------------------------------------------------*\
    @name Thermaltake Riing
    @category Cooler
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectThermaltakeRiingControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_ThermaltakeRiing::RGBController_ThermaltakeRiing(ThermaltakeRiingController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Thermaltake Riing";
    vendor      = "Thermaltake";
    type        = DEVICE_TYPE_COOLER;
    description = "Thermaltake Riing Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();
    version     = controller->GetFirmwareVersion();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = THERMALTAKE_MODE_PER_LED;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.speed_min  = 0;
    Direct.speed_max  = 0;
    Direct.speed      = 0;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = THERMALTAKE_MODE_FULL;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.speed_min  = 0;
    Static.speed_max  = 0;
    Static.speed      = 0;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Flow;
    Flow.name       = "Flow";
    Flow.value      = THERMALTAKE_MODE_FLOW;
    Flow.flags      = MODE_FLAG_HAS_SPEED;
    Flow.speed_min  = THERMALTAKE_SPEED_SLOW;
    Flow.speed_max  = THERMALTAKE_SPEED_EXTREME;
    Flow.speed      = THERMALTAKE_SPEED_NORMAL;
    Flow.color_mode = MODE_COLORS_NONE;
    modes.push_back(Flow);

    mode Spectrum;
    Spectrum.name       = "Spectrum";
    Spectrum.value      = THERMALTAKE_MODE_SPECTRUM;
    Spectrum.flags      = MODE_FLAG_HAS_SPEED;
    Spectrum.speed_min  = THERMALTAKE_SPEED_SLOW;
    Spectrum.speed_max  = THERMALTAKE_SPEED_EXTREME;
    Spectrum.speed      = THERMALTAKE_SPEED_NORMAL;
    Spectrum.color_mode = MODE_COLORS_NONE;
    modes.push_back(Spectrum);

    mode Ripple;
    Ripple.name       = "Ripple";
    Ripple.value      = THERMALTAKE_MODE_RIPPLE;
    Ripple.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Ripple.speed_min  = THERMALTAKE_SPEED_SLOW;
    Ripple.speed_max  = THERMALTAKE_SPEED_EXTREME;
    Ripple.speed      = THERMALTAKE_SPEED_NORMAL;
    Ripple.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Ripple);

    mode Blink;
    Blink.name       = "Blink";
    Blink.value      = THERMALTAKE_MODE_BLINK;
    Blink.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Blink.speed_min  = THERMALTAKE_SPEED_SLOW;
    Blink.speed_max  = THERMALTAKE_SPEED_EXTREME;
    Blink.speed      = THERMALTAKE_SPEED_NORMAL;
    Blink.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Blink);

    mode Pulse;
    Pulse.name       = "Pulse";
    Pulse.value      = THERMALTAKE_MODE_PULSE;
    Pulse.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Pulse.speed_min  = THERMALTAKE_SPEED_SLOW;
    Pulse.speed_max  = THERMALTAKE_SPEED_EXTREME;
    Pulse.speed      = THERMALTAKE_SPEED_NORMAL;
    Pulse.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Pulse);

    mode Wave;
    Wave.name       = "Wave";
    Wave.value      = THERMALTAKE_MODE_WAVE;
    Wave.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Wave.speed_min  = THERMALTAKE_SPEED_SLOW;
    Wave.speed_max  = THERMALTAKE_SPEED_EXTREME;
    Wave.speed      = THERMALTAKE_SPEED_NORMAL;
    Wave.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Wave);

    SetupZones();
}

RGBController_ThermaltakeRiing::~RGBController_ThermaltakeRiing()
{
    delete controller;
}

void RGBController_ThermaltakeRiing::SetupZones()
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
    zones.resize(THERMALTAKE_NUM_CHANNELS);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    for (unsigned int channel_idx = 0; channel_idx < THERMALTAKE_NUM_CHANNELS; channel_idx++)
    {
        char ch_idx_string[2];
        snprintf(ch_idx_string, 2, "%d", channel_idx + 1);

        zones[channel_idx].name     = "Riing Channel ";
        zones[channel_idx].name.append(ch_idx_string);
        zones[channel_idx].type     = ZONE_TYPE_LINEAR;

        /*-------------------------------------------------*\
        | The maximum number of colors that would fit in the|
        | Riing protocol is 20                              |
        \*-------------------------------------------------*/
        zones[channel_idx].leds_min   = 0;
        zones[channel_idx].leds_max   = 20;

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

void RGBController_ThermaltakeRiing::ResizeZone(int zone, int new_size)
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

void RGBController_ThermaltakeRiing::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        controller->SetChannelLEDs((unsigned char)zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}

void RGBController_ThermaltakeRiing::UpdateZoneLEDs(int zone)
{
    controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
}

void RGBController_ThermaltakeRiing::UpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];

    controller->SetChannelLEDs(channel, zones[channel].colors, zones[channel].leds_count);
}

void RGBController_ThermaltakeRiing::DeviceUpdateMode()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        controller->SetMode(modes[active_mode].value, modes[active_mode].speed);
        controller->SetChannelLEDs((unsigned char)zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}
