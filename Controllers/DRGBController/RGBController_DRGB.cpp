/*---------------------------------------------------------*\
| RGBController_DRGB.cpp                                    |
|                                                           |
|   Driver for DRGBmods                                     |
|                                                           |
|   Zhi Yan                                     25 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    controller  = controller_ptr;

    name        = "DRGB Controller";
    vendor      = "DRGB";
    description = "DRGB Controller Device";
    type        = DEVICE_TYPE_LEDSTRIP;
    location    = controller->GetLocationString();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_DRGB::~RGBController_DRGB()
{
    delete controller;
}

void RGBController_DRGB::SetupZones()
{
    /*-------------------------------------------------*\
    | Only set LED count on the first run               |
    \*-------------------------------------------------*/
    bool first_run = false;
    if(zones.size() == 0)
    {
        first_run = true;
    }
    leds.clear();
    colors.clear();

    unsigned int NUM_CHANNELS = 0;
    unsigned int NUM_Channel_led   = 0;
    switch(controller->GetDevicePID())
    {
        case DRGB_LED_V4_PID:
            NUM_CHANNELS    = 8;
            NUM_Channel_led = 256;
            Version         = 4;
            break;
        case DRGB_ULTRA_V4F_PID:
            NUM_CHANNELS    = 16;
            NUM_Channel_led = 256;
            Version         = 4;
            break;
        case DRGB_CORE_V4F_PID:
            NUM_CHANNELS    = 32;
            NUM_Channel_led = 256;
            Version         = 4;
            break;
        case DRGB_SIG_V4F_PID:
            NUM_CHANNELS    = 36;
            NUM_Channel_led = 256;
            Version         = 4;
            break;

        case DRGB_LED_V3_PID:
            NUM_CHANNELS    = 8;
            NUM_Channel_led = 256;
            Version         = 3;
            break;
        case DRGB_Ultra_V3_PID:
            NUM_CHANNELS    = 16;
            NUM_Channel_led = 256;
            Version         = 3;
            break;
        case DRGB_CORE_V3_PID:
            NUM_CHANNELS    = 30;
            NUM_Channel_led = 256;
            Version         = 3;
            break;

        case DRGB_LED_PID:
            NUM_CHANNELS    = 8;
            NUM_Channel_led = 256;
            Version         = 2;
            break;
        case DRGB_ULTRA_PID:
            NUM_CHANNELS    = 16;
            NUM_Channel_led = 256;
            Version         = 2;
            break;
        case DRGB_SIG_AB_PID:
            NUM_CHANNELS    = 16;
            NUM_Channel_led = 256;
            Version         = 2;
            break;
        case DRGB_SIG_CD_PID:
            NUM_CHANNELS    = 6;
            NUM_Channel_led = 256;
            Version         = 2;
            break;
        case DRGB_Strimer_PID:
            NUM_CHANNELS    = 6;
            NUM_Channel_led = 256;
            Version         = 2;
            break;
    }

    zones.resize(NUM_CHANNELS);

    for(unsigned int channel_idx = 0; channel_idx < NUM_CHANNELS; channel_idx++)
    {
        char ch_idx_string[2];
        if(NUM_CHANNELS == 6)
        {
            if(channel_idx==0)
            {
                snprintf(ch_idx_string, 2, "%d", channel_idx+1 );
                zones[channel_idx].name     = "Strimer ATX";
            }
            else if(channel_idx<3)
            {
                snprintf(ch_idx_string, 2, "%d", channel_idx );
                zones[channel_idx].name     = "Channel C";
            }
            else if(channel_idx==3)
            {
                snprintf(ch_idx_string, 2, "%d", channel_idx-2 );
                zones[channel_idx].name     = "Strimer GPU";
            }
            else if(channel_idx<6)
            {
                snprintf(ch_idx_string, 2, "%d", channel_idx -3);
                zones[channel_idx].name     = "Channel D";
            }
        }
        else if(channel_idx<8)
        {
            snprintf(ch_idx_string, 2, "%d", channel_idx + 1);
            zones[channel_idx].name     = "Channel A";
        }
        else if(channel_idx<16)
        {
            snprintf(ch_idx_string, 2, "%d", channel_idx -7);
            zones[channel_idx].name     = "Channel B";
        }
        else if(NUM_CHANNELS == 30)
        {
            if(channel_idx<24)
            {
                snprintf(ch_idx_string, 2, "%d", channel_idx -15);
                zones[channel_idx].name     = "Channel C";
            }
            else if(channel_idx<30)
            {
                snprintf(ch_idx_string, 2, "%d", channel_idx -23);
                zones[channel_idx].name     = "Channel D";
            }
        }
        else if(channel_idx<22)
        {
            snprintf(ch_idx_string, 2, "%d", channel_idx -15);
            zones[channel_idx].name     = "Channel C";
        }
        else if(channel_idx<28)
        {
            snprintf(ch_idx_string, 2, "%d", channel_idx -21);
            zones[channel_idx].name     = "Channel D";
        }
        else if(channel_idx<36)
        {
            snprintf(ch_idx_string, 2, "%d", channel_idx -27);
            zones[channel_idx].name     = "Channel E";
        }
        zones[channel_idx].name.append(ch_idx_string);
        zones[channel_idx].type         = ZONE_TYPE_LINEAR;
        zones[channel_idx].leds_min     = 0;
        zones[channel_idx].leds_max     = NUM_Channel_led;

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

    /*-----------------------------------------------------*\
    |  Turn off hardware effects                            |
    \*-----------------------------------------------------*/

    if(Version > 2)
    {
        unsigned char   HWLData[64] = {0};
        HWLData[0]  = 0x35;
        HWLData[4]  = 0x00;
        HWLData[5]  = 0x01;
        HWLData[6]  = 0x01;
        HWLData[7]  = 0x00;
        HWLData[8]  = 0x00;
        HWLData[9]  = 0xFF;
        HWLData[10] = 0x00;
        HWLData[11] = 0x00;
        HWLData[12] = 0x00;
        HWLData[13] = 0xFF;
        HWLData[14] = 0x00;
        HWLData[20] = 0xF0;
        HWLData[21] = 0x00;

        if(Version == 4)
        {
            controller->SendPacketFS(&HWLData[0], 1,0);
        }
        else if(Version == 3)
        {
            controller->SendPacketFS(&HWLData[0], 1,0);
        }
    }
}

void RGBController_DRGB::ResizeZone(int zone, int new_size)
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

void RGBController_DRGB::DeviceUpdateLEDs()
{
    switch(Version)
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
            controller->SendPacketFS(&RGBData[0], col_packets,1);
            break;
        }

        case 2:
            for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
            {
                unsigned char   RGBData[256*3]  = {0};
                unsigned char   ArrayData[64]   = {0};
                unsigned char   LEDnum          = zones[zone_idx].leds_count;
                for(unsigned int i=0; i<LEDnum;i++)
                {
                    unsigned int RGBcolors = zones[zone_idx].colors[i];
                    RGBData[i * 3]      = RGBcolors & 0xFF;
                    RGBData[i * 3 +1]   = (RGBcolors >> 8) & 0xFF;
                    RGBData[i * 3 +2]   = (RGBcolors >> 16) & 0xFF;
                }
                unsigned char   NumPackets      = LEDnum / DRGB_V2_PACKAGE_SIZE + ((LEDnum % DRGB_V2_PACKAGE_SIZE) > 0);
                for (unsigned int CurrPacket = 1 ; CurrPacket <= NumPackets; CurrPacket++)
                {
                    ArrayData[0] = CurrPacket;
                    ArrayData[1] = NumPackets;
                    ArrayData[2] = zone_idx;
                    ArrayData[3] = 0xBB;
                    for(unsigned int i=0; i<60;i++)
                    {
                        ArrayData[4+i] = RGBData[(CurrPacket -1)*60 + i];
                    }
                    controller->SendPacketFS(&ArrayData[0], 1,0);
                }
            }
            break;
    }
}

void RGBController_DRGB::UpdateZoneLEDs(int zone)
{
    controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
}

void RGBController_DRGB::UpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];
    controller->SetChannelLEDs(channel, zones[channel].colors, zones[channel].leds_count);
}

void RGBController_DRGB::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
