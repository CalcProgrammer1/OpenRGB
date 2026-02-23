/*---------------------------------------------------------*\
| RGBController_DRGB.cpp                                    |
|                                                           |
|   Driver for DRGBmods                                     |
|                                                           |
|   Zhi Yan                                     25 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <iostream>
#include "RGBController_DRGB.h"

/**------------------------------------------------------------------*\
    @name DRGB Controller
    @category LEDStrip
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectDRGBControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_DRGB::RGBController_DRGB(DRGBController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetNameString();
    vendor              = "DRGB";
    description         = "DRGB Controller Device";
    type                = DEVICE_TYPE_LEDSTRIP;
    version             = controller->GetFirmwareString();
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

RGBController_DRGB::~RGBController_DRGB()
{
    Shutdown();

    delete controller;
}

void RGBController_DRGB::SetupZones()
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
                                                    | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP;
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

void RGBController_DRGB::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        SetupZones();
    }
}

void RGBController_DRGB::DeviceUpdateLEDs()
{
    switch(controller->GetVersion())
    {
        case 4:
        {
            unsigned int    led_index = 0;
            unsigned char   RGBData[8192*3 + 72] = {0};
            for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
            {
                unsigned char   LEDnum      = zones[zone_idx].leds_count;
                unsigned int    HighCount   = (LEDnum & 0xFFFF)>>8;
                unsigned int    LowCount    = LEDnum & 0xFF;
                RGBData[zone_idx * 2 ]      = HighCount;
                RGBData[zone_idx * 2 + 1]   = LowCount;
                for(unsigned int i=0; i<LEDnum;i++)
                {
                    unsigned int RGBcolors = zones[zone_idx].colors[i];
                    RGBData[led_index * 3 +72] = RGBcolors & 0xFF;
                    RGBData[led_index * 3 +73] = (RGBcolors >> 8) & 0xFF;
                    RGBData[led_index * 3 +74] = (RGBcolors >> 16) & 0xFF;
                    led_index++;
                }
                if(led_index>8192)
                {
                    break;
                }
            }
            unsigned int    col_packets = 1 ;
            if(led_index > DRGB_V4_ONE_PACKAGE_SIZE)
            {
                col_packets = 1 + ((led_index - DRGB_V4_ONE_PACKAGE_SIZE) / DRGB_V4_PACKAGE_SIZE) + (((led_index - DRGB_V4_ONE_PACKAGE_SIZE) % DRGB_V4_PACKAGE_SIZE) > 0);
            }
            controller->SendPacket(&RGBData[0], col_packets,led_index);
            break;
        }

        case 3:
        {
            unsigned int    led_index = 0;
            unsigned char   RGBData[1801*3]     = {0};
            unsigned char   ArrayData[64]       = {0};
            ArrayData[0] = 0x60;
            ArrayData[1] = 0xBB;
            for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
            {
                unsigned char   LEDnum      = zones[zone_idx].leds_count;
                unsigned int    HighCount   = (LEDnum & 0xFFFF)>>8;
                unsigned int    LowCount    = LEDnum & 0xFF;
                ArrayData[zone_idx * 2 + 2] = HighCount;
                ArrayData[zone_idx * 2 + 3] = LowCount;
                for(unsigned int i=0; i<LEDnum;i++)
                {
                    unsigned int RGBcolors = zones[zone_idx].colors[i];
                    RGBData[led_index * 3]      = RGBcolors & 0xFF;
                    RGBData[led_index * 3 +1]   = (RGBcolors >> 8) & 0xFF;
                    RGBData[led_index * 3 +2]   = (RGBcolors >> 16) & 0xFF;
                    led_index++;
                }
                if(led_index>1800)
                {
                    break;
                }
            }
            unsigned int    col_packets     = (led_index / DRGB_V3_PACKAGE_SIZE) + ((led_index % DRGB_V3_PACKAGE_SIZE) > 0);
            controller->SendPacketFS(&ArrayData[0], 1,0);
            controller->SendPacketFS(&RGBData[0], col_packets,0x64);
            break;
        }

        case 2:
            for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
            {
                unsigned char   RGBData[256*3]  = {0};
                unsigned char   ArrayData[64]   = {0};
                unsigned char   LEDnum          = zones[zone_idx].leds_count;
                for(unsigned int i = 0; i < LEDnum; i++)
                {
                    unsigned int RGBcolors = zones[zone_idx].colors[i];
                    RGBData[i * 3]      = RGBcolors & 0xFF;
                    RGBData[i * 3 +1]   = (RGBcolors >> 8) & 0xFF;
                    RGBData[i * 3 +2]   = (RGBcolors >> 16) & 0xFF;
                }

                unsigned char   NumPackets      = LEDnum / DRGB_V2_PACKAGE_SIZE + ((LEDnum % DRGB_V2_PACKAGE_SIZE) > 0);
                for(unsigned char CurrPacket = 1; CurrPacket <= NumPackets; CurrPacket++)
                {
                    ArrayData[0] = CurrPacket;
                    ArrayData[1] = NumPackets;
                    ArrayData[2] = (unsigned char)zone_idx;
                    ArrayData[3] = 0xBB;
                    for(unsigned int i=0; i<60;i++)
                    {
                        ArrayData[4+i] = RGBData[(CurrPacket -1)*60 + i];
                    }
                    controller->SendPacketFS(&ArrayData[0], 1,0);
                }
            }
            break;

        case 1:
        {
            unsigned int    led_index = 0;
            unsigned char   RGBData[1801*3]     = {0};
            unsigned char   ArrayData[64]       = {0};
            ArrayData[0] = 0x46;
            ArrayData[1] = 0xBB;
            for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
            {
                unsigned char   LEDnum      = zones[zone_idx].leds_count;
                unsigned int    HighCount   = (LEDnum & 0xFFFF)>>8;
                unsigned int    LowCount    = LEDnum & 0xFF;
                ArrayData[zone_idx * 2 + 2] = HighCount;
                ArrayData[zone_idx * 2 + 3] = LowCount;
                for(unsigned int i=0; i<LEDnum;i++)
                {
                    unsigned int RGBcolors = zones[zone_idx].colors[i];
                    RGBData[led_index * 3]      = RGBcolors & 0xFF;
                    RGBData[led_index * 3 +1]   = (RGBcolors >> 8) & 0xFF;
                    RGBData[led_index * 3 +2]   = (RGBcolors >> 16) & 0xFF;
                    led_index++;
                }
                if(led_index>1800)
                {
                    break;
                }
            }
            unsigned int    col_packets     = (led_index / DRGB_V3_PACKAGE_SIZE) + ((led_index % DRGB_V3_PACKAGE_SIZE) > 0);
            controller->SendPacketFS(&ArrayData[0], 1,0);
            controller->SendPacketFS(&RGBData[0], col_packets,0x47);
            break;
        }
    }
}

void RGBController_DRGB::DeviceUpdateZoneLEDs(int zone)
{
    controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
}

void RGBController_DRGB::DeviceUpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];
    controller->SetChannelLEDs(channel, zones[channel].colors, zones[channel].leds_count);
}

void RGBController_DRGB::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
