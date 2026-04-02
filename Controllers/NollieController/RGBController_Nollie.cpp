/*---------------------------------------------------------*\
| RGBController_Nollie.cpp                                  |
|                                                           |
|   RGBController for Nollie                                |
|                                                           |
|   Name (cnn1236661)                           25 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <iostream>
#include "RGBController_Nollie.h"

/**------------------------------------------------------------------*\
    @name Nollie Controller
    @category LEDStrip
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectNollieControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_Nollie::RGBController_Nollie(NollieController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetNameString();
    vendor              = "Nollie";
    description         = "Nollie Controller Device";
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

RGBController_Nollie::~RGBController_Nollie()
{
    Shutdown();

    delete controller;
}

void RGBController_Nollie::SetupZones()
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
    zones.resize(controller->GetNumChannels());

    /*-----------------------------------------------------*\
    | Set up zones                                          |
    \*-----------------------------------------------------*/
    for(std::size_t channel_idx = 0; channel_idx < zones.size(); channel_idx++)
    {
        zones[channel_idx].leds_min                 = 0;
        zones[channel_idx].leds_max                 = controller->GetLEDsPerChannel();

        if(first_run)
        {
            zones[channel_idx].flags                = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS;
        }

        if(!(zones[channel_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
        {
            zones[channel_idx].name                 = controller->GetChannelName((unsigned char)channel_idx);
        }

        if(!(zones[channel_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
        {
            zones[channel_idx].leds_count           = 0;
        }

        if(!(zones[channel_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
        {
            zones[channel_idx].type                 = ZONE_TYPE_LINEAR;
        }

        if(!(zones[channel_idx].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
        {
            zones[channel_idx].matrix_map.width     = 0;
            zones[channel_idx].matrix_map.height    = 0;
            zones[channel_idx].matrix_map.map.resize(0);
        }

        for(unsigned int led_ch_idx = 0; led_ch_idx < zones[channel_idx].leds_count; led_ch_idx++)
        {
            led new_led;
            new_led.name                = zones[channel_idx].name + ", LED " + std::to_string(led_ch_idx);
            leds.push_back(new_led);
            leds_channel.push_back(channel_idx);
        }
    }

    SetupColors();
}

void RGBController_Nollie::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        /*-------------------------------------------------*\
        |  Set whether MOS is enabled or not                |
        \*-------------------------------------------------*/
        if(controller->GetUSBVID() == NOLLIE32_VID && NOLLIE32_PID == controller->GetUSBPID())
        {
            if(zone_idx == NOLLIE32_MOS_TRIGGER_CH && zones[zone_idx].leds_count > NOLLIE32_MOS_TRIGGER_LED)
            {
                controller->SetMos(false);
            }
            else if(zone_idx == NOLLIE32_MOS_TRIGGER_CH)
            {
                controller->SetMos(true);
            }
        }

        /*-------------------------------------------------*\
        |  Nollie1 needs to report the number of LEDs       |
        \*-------------------------------------------------*/
        if(controller->GetUSBVID() == NOLLIE1_VID && controller->GetUSBPID() == NOLLIE1_PID)
        {
            controller->InitChLEDs(&zones[zone_idx].leds_count, controller->GetNumChannels());
        }

        SetupZones();
    }
}

void RGBController_Nollie::DeviceUpdateLEDs()
{
    unsigned int DevPid = controller->GetUSBPID();
    if(DevPid == NOLLIE32_PID || DevPid == NOLLIE16_PID
    || DevPid == NOLLIE32_OS2_1_PID || DevPid == NOLLIE16_OS2_1_PID)
    {
        const int* channel_index = controller->GetChannelIndex();
        std::vector<int> ChSort;
        for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
        {
            unsigned int channel = channel_index[zone_idx];
            if(zones[zone_idx].leds_count > 0)
            {
                ChSort.push_back(channel);
            }
            else if(channel == NOLLIE32_FLAG1_CHANNEL || channel == NOLLIE32_FLAG2_CHANNEL)
            {
                ChSort.push_back(channel);
            }
        }
        std::sort(ChSort.begin(), ChSort.end());
        for(std::size_t i = 0; i < ChSort.size(); i++)
        {
            const int* ptr = std::find(channel_index, channel_index + 32, ChSort[i]);
            int zone_idx = (int)(ptr - channel_index);
            controller->SetChannelLEDs(ChSort[i], zones[zone_idx].colors, zones[zone_idx].leds_count);
        }
    }
    else
    {
        for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
        {
            if(zones[zone_idx].leds_count > 0)
            {
                controller->SetChannelLEDs((unsigned char)zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
            }
        }
        controller->SendUpdate();
    }

}

void RGBController_Nollie::DeviceUpdateZoneLEDs(int zone)
{
    controller->SetChannelLEDs(controller->GetChannelIndex()[zone], zones[zone].colors, zones[zone].leds_count);
}

void RGBController_Nollie::DeviceUpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];
    controller->SetChannelLEDs(controller->GetChannelIndex()[channel], zones[channel].colors, zones[channel].leds_count);
}

void RGBController_Nollie::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
