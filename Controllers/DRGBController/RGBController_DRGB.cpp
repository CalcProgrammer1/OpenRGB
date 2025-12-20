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
    controller  = controller_ptr;

    name        = controller->GetNameString();
    vendor      = "DRGB";
    description = "DRGB Controller Device";
    type        = DEVICE_TYPE_LEDSTRIP;
    version     = controller->GetFirmwareString();
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
            NUM_Channel_led = 512;
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
        case DRGB_AG_04_V4F_PID:
            NUM_CHANNELS    = 4;
            NUM_Channel_led = 256;
            Version         = 4;
            break;
        case DRGB_AG_16_V4F_PID:
            NUM_CHANNELS    = 16;
            NUM_Channel_led = 256;
            Version         = 4;
            break;
        case DRGB_AG_16_F12_PID:
            NUM_CHANNELS    = 16;
            NUM_Channel_led = 256;
            Version         = 4;
            break;

        case DRGB_L8_V5_PID:
            NUM_CHANNELS    = 8;
            NUM_Channel_led = 512;
            Version         = 4;
            break;
        case DRGB_U16_V5_PID:
            NUM_CHANNELS    = 16;
            NUM_Channel_led = 256;
            Version         = 4;
            break;
        case DRGB_U16_V5F_PID:
            NUM_CHANNELS    = 16;
            NUM_Channel_led = 256;
            Version         = 4;
            break;
        case DRGB_C16_V5_PID:
            NUM_CHANNELS    = 32;
            NUM_Channel_led = 256;
            Version         = 4;
            break;
        case DRGB_C16_V5F_PID:
            NUM_CHANNELS    = 32;
            NUM_Channel_led = 256;
            Version         = 4;
            break;
        case DRGB_S16_V5F_PID:
            NUM_CHANNELS    = 32;
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
        case DRGB_E8_F_PID:
            NUM_CHANNELS    = 8;
            NUM_Channel_led = 132;
            Version         = 1;
            break;
        case DRGB_E8_PID:
            NUM_CHANNELS    = 8;
            NUM_Channel_led = 132;
            Version         = 1;
            break;
        case DRGB_E16_PID:
            NUM_CHANNELS    = 16;
            NUM_Channel_led = 132;
            Version         = 1;
            break;
        case DM_10_PID:
            NUM_CHANNELS    = 10;
            NUM_Channel_led = 132;
            Version         = 1;
            break;
        case JPU_12_PID:
            NUM_CHANNELS    = 12;
            NUM_Channel_led = 60;
            Version         = 1;
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

        case YICO_8_PID:
            NUM_CHANNELS    = 8;
            NUM_Channel_led = 256;
            Version         = 3;
            break;
        case YICO_08_PID:
            NUM_CHANNELS    = 8;
            NUM_Channel_led = 256;
            Version         = 3;
            break;
        case YICO_08_1_PID:
            NUM_CHANNELS    = 8;
            NUM_Channel_led = 132;
            Version         = 3;
            break;
        case YICO_14_PID:
            NUM_CHANNELS    = 14;
            NUM_Channel_led = 132;
            Version         = 1;
            break;
        case YICO_16_PID:
            NUM_CHANNELS    = 16;
            NUM_Channel_led = 256;
            Version         = 4;
            break;
    }

    zones.resize(NUM_CHANNELS);

    for(unsigned int channel_idx = 0; channel_idx < NUM_CHANNELS; channel_idx++)
    {
        char ch_idx_string[4];
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
        else if(NUM_CHANNELS == 10 || NUM_CHANNELS == 12)
        {
            snprintf(ch_idx_string, 4, "%d", channel_idx+1 );
            zones[channel_idx].name     = "Channel ";
        }
        else if(NUM_CHANNELS == 14)
        {
            if(channel_idx<4)
            {
                snprintf(ch_idx_string, 3, "%d", channel_idx+1 );
                zones[channel_idx].name     = "LCD ";
            }
            else if(channel_idx<6)
            {
                snprintf(ch_idx_string, 3, "%d", channel_idx+1 );
                zones[channel_idx].name     = "LED ";
            }
            else if(channel_idx<16)
            {
                snprintf(ch_idx_string, 3, "%d", channel_idx-5 );
                zones[channel_idx].name     = "ARGB ";
            }
        }
        else if(channel_idx<8)
        {
            snprintf(ch_idx_string, 3, "%d", channel_idx + 1);
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
