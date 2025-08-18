/*---------------------------------------------------------*\
| RGBController_Nollie.cpp                                  |
|                                                           |
|   RGBController for Nollie                                |
|                                                           |
|   Name (cnn1236661)                           25 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    delete controller;
}

void RGBController_Nollie::SetupZones()
{
    bool first_run = false;
    unsigned int channels_num = 0;
    unsigned int ch_led_num   = 0;
    if(zones.size() == 0)
    {
        first_run = true;
    }
    leds.clear();
    colors.clear();
    switch(controller->GetUSBPID())
    {
        case NOLLIE32_PID:
            channels_num  = NOLLIE32_CHANNELS_NUM;
            ch_led_num    = NOLLIE_HS_CH_LED_NUM;
            channel_index = ch32;
            break;
        case NOLLIE16_PID:
            channels_num  = NOLLIE16_CHANNELS_NUM;
            ch_led_num    = NOLLIE_HS_CH_LED_NUM;
            channel_index = ch16;
            if (controller->GetUSBVID() == NOLLIERGBOS_2_VID)
                channel_index = n16;
            break;
        case NOLLIE28_12_PID:
            channels_num  = NOLLIE28_12_CHANNELS_NUM;
            ch_led_num    = NOLLIE_12_CH_LED_NUM;
            break;
        case NOLLIE8_PID:
            channels_num  = NOLLIE8_CHANNELS_NUM;
            ch_led_num    = NOLLIE_8_CH_LED_NUM;
            break;
        case NOLLIE1_PID:
            channels_num  = NOLLIE1_CHANNELS_NUM;
            ch_led_num    = NOLLIE_1_CH_LED_NUM;
            break;
        default:
            channels_num  = NOLLIE8_CHANNELS_NUM;
            ch_led_num    = NOLLIE_FS_CH_LED_NUM;
            break;
    }
    zones.resize(channels_num);
    for(unsigned int channel_idx = 0; channel_idx < channels_num; channel_idx++)
    {
        if(channel_idx > 27 )
        {
            char ch_idx_string[4];
            snprintf(ch_idx_string, 4, "%d", channel_idx + 1 - 28);
            zones[channel_idx].name     = "Channel EXT ";
            zones[channel_idx].name.append(ch_idx_string);
        }
        else if(channel_idx > 21 )
        {
            char ch_idx_string[4];
            snprintf(ch_idx_string, 4, "%d", channel_idx + 1 - 22);
            zones[channel_idx].name     = "Channel GPU ";
            zones[channel_idx].name.append(ch_idx_string);
        }
        else if(channel_idx > 15 )
        {
            char ch_idx_string[4];
            snprintf(ch_idx_string, 4, "%d", channel_idx + 1 - 16);
            zones[channel_idx].name     = "Channel ATX ";
            zones[channel_idx].name.append(ch_idx_string);
        }
        else
        {
            char ch_idx_string[4];
            snprintf(ch_idx_string, 4, "%d", channel_idx + 1);
            zones[channel_idx].name     = "Channel ";
            zones[channel_idx].name.append(ch_idx_string);
        }
        zones[channel_idx].type     = ZONE_TYPE_LINEAR;
        zones[channel_idx].leds_min   = 0;
        zones[channel_idx].leds_max   = ch_led_num;

        if(first_run)
        {
            zones[channel_idx].leds_count = 0;
        }

        zones[channel_idx].matrix_map = NULL;

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

void RGBController_Nollie::ResizeZone(int zone, int new_size)
{
    /*-----------------------------------------------------*\
    |  Set whether MOS is enabled or not                    |
    \*-----------------------------------------------------*/
    if(controller->GetUSBVID() == NOLLIE32_VID && NOLLIE32_PID == controller->GetUSBPID())
    {
        if(zone == NOLLIE32_MOS_TRIGGER_CH && new_size > NOLLIE32_MOS_TRIGGER_LED)
        {
            controller->SetMos(false);
        }
        else if(zone == NOLLIE32_MOS_TRIGGER_CH)
        {
            controller->SetMos(true);
        }
    }

    /*-----------------------------------------------------*\
    |  Nollie1 needs to report the number of LEDs           |
    \*-----------------------------------------------------*/
    if(controller->GetUSBVID() == NOLLIE1_VID && controller->GetUSBPID() == NOLLIE1_PID)
    {
        controller->InitChLEDs(&new_size,NOLLIE1_CHANNELS_NUM);
    }

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

void RGBController_Nollie::DeviceUpdateLEDs()
{
    unsigned int DevPid = controller->GetUSBPID();
    if(DevPid == NOLLIE32_PID || DevPid == NOLLIE16_PID)
    {
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
            int* ptr = std::find(channel_index, channel_index + 32, ChSort[i]);
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

void RGBController_Nollie::UpdateZoneLEDs(int zone)
{
    controller->SetChannelLEDs(channel_index[zone], zones[zone].colors, zones[zone].leds_count);
}

void RGBController_Nollie::UpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];
    controller->SetChannelLEDs(channel_index[channel], zones[channel].colors, zones[channel].leds_count);
}

void RGBController_Nollie::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
