/*---------------------------------------------------------*\
| RGBController_SRGBmodsPico.cpp                            |
|                                                           |
|   RGBController for SRGBmods Raspberry Pi Pico LED        |
|   Controller                                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_SRGBmodsPico.h"

/**------------------------------------------------------------------*\
    @name SRGBmods Raspberry Pi Pico LED Controller
    @category LEDStrip
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectSRGBmodsControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_SRGBmodsPico::RGBController_SRGBmodsPico(SRGBmodsPicoController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetNameString();
    vendor              = "SRGBmods.net";
    description         = "SRGBmods Pico LED Controller Device";
    type                = DEVICE_TYPE_LEDSTRIP;
    location            = controller->GetLocationString();
    serial              = controller->GetSerialString();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0xFFFF;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_SRGBmodsPico::~RGBController_SRGBmodsPico()
{
    Shutdown();

    delete controller;
}

void RGBController_SRGBmodsPico::SetupZones()
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
    zones.resize(SRGBMODS_PICO_NUM_CHANNELS);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    for(unsigned int channel_idx = 0; channel_idx < SRGBMODS_PICO_NUM_CHANNELS; channel_idx++)
    {
        char ch_idx_string[2];
        snprintf(ch_idx_string, 2, "%d", channel_idx + 1);

        zones[channel_idx].name     = "Channel ";
        zones[channel_idx].name.append(ch_idx_string);
        zones[channel_idx].type     = ZONE_TYPE_LINEAR;

        /*-------------------------------------------------*\
        | The maximum number of LEDs per channel is 512     |
        | according to https://srgbmods.net/picoled/        |
        \*-------------------------------------------------*/
        zones[channel_idx].leds_min   = 0;
        zones[channel_idx].leds_max   = 512;

        if(first_run)
        {
            zones[channel_idx].leds_count = 0;
        }

        for(unsigned int led_ch_idx = 0; led_ch_idx < zones[channel_idx].leds_count; led_ch_idx++)
        {
            char led_idx_string[4];
            snprintf(led_idx_string, 4, "%d", led_ch_idx + 1);

            led new_led;
            new_led.name = "LED ";
            new_led.name.append(led_idx_string);

            leds.push_back(new_led);
            leds_channel.push_back(channel_idx);
        }
    }

    SetupColors();
}

void RGBController_SRGBmodsPico::DeviceResizeZone(int zone, int new_size)
{
    if((size_t)zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

        SetupZones();
    }
}

void RGBController_SRGBmodsPico::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        if(zones[zone_idx].leds_count > 0)
        {
            controller->SetChannelLEDs((unsigned char)zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
        }
    }
}

void RGBController_SRGBmodsPico::DeviceUpdateZoneLEDs(int zone)
{
    controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
}

void RGBController_SRGBmodsPico::DeviceUpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];

    controller->SetChannelLEDs(channel, zones[channel].colors, zones[channel].leds_count);
}

void RGBController_SRGBmodsPico::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
