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
    /*-----------------------------------------------------*\
    | Only set LED count on the first run                   |
    \*-----------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-----------------------------------------------------*\
    | Clear any existing color/LED configuration            |
    \*-----------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(SRGBMODS_PICO_NUM_CHANNELS);

    /*-----------------------------------------------------*\
    | Set zones and leds                                    |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        /*-------------------------------------------------*\
        | The maximum number of LEDs per channel is 512     |
        | according to https://srgbmods.net/picoled/        |
        \*-------------------------------------------------*/
        zones[zone_idx].leds_min                    = 0;
        zones[zone_idx].leds_max                    = 512;

        if(first_run)
        {
            zones[zone_idx].flags                   = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
        {
            zones[zone_idx].name                    = "Addressable RGB Header ";
            zones[zone_idx].name.append(std::to_string(zone_idx + 1));
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
        {
            zones[zone_idx].leds_count              = 0;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
        {
            zones[zone_idx].type                    = ZONE_TYPE_LINEAR;
        }

        if(!(zones[zone_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
        {
            zones[zone_idx].matrix_map.width        = 0;
            zones[zone_idx].matrix_map.height       = 0;
            zones[zone_idx].matrix_map.map.resize(0);
        }

        for(unsigned int led_ch_idx = 0; led_ch_idx < zones[zone_idx].leds_count; led_ch_idx++)
        {
            led new_led;
            new_led.name                            = zones[zone_idx].name + ", LED " + std::to_string(led_ch_idx + 1);

            leds.push_back(new_led);
            leds_channel.push_back(zone_idx);
        }
    }

    SetupColors();
}

void RGBController_SRGBmodsPico::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
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
