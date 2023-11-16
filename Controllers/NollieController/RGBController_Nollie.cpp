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

#include "RGBController_Nollie.h"
#include <algorithm>
#include <iostream>

int channel_index[32] = {0};
int ch32[32]     = {5, 4, 3, 2, 1, 0, 15, 14, 26, 27, 28, 29, 30, 31, 8, 9, 19, 18, 17, 16, 7, 6, 25, 24, 23, 22, 21, 20, 13, 12, 11, 10};
int ch16[32]     = {19, 18, 17, 16, 24, 25, 26, 27, 20, 21, 22, 23, 31, 30, 29, 28, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
RGBController_Nollie::RGBController_Nollie(NollieController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Nollie Device";
    vendor      = "Nollie";
    description = "Nollie Controller Device";
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

RGBController_Nollie::~RGBController_Nollie()
{
    delete controller;
}

void RGBController_Nollie::SetupZones()
{
    bool first_run = false;
    unsigned int channels_num = 0;
    if(zones.size() == 0)
    {
        first_run = true;
    }
    leds.clear();
    colors.clear();
    switch(controller->GetUSBPID())
    {
        case NOLLIE32_PID:
            channels_num     = 32;
            memcpy(channel_index,ch32,sizeof(ch32));
            break;
        default:
            channels_num     = 16;
            memcpy(channel_index,ch16,sizeof(ch16));
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
        zones[channel_idx].leds_max   = NOLLIE_CHANNELS_LED_NUM;

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
    |  Set whether MOS is enabled or not                   |
    \*-----------------------------------------------------*/
    if(zone == 26 && new_size >20)
    {
        controller->SetMos(false);
    }
    else if(zone == 26)
    {
        controller->SetMos(true);
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
    std::vector<int> ChSort;
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        unsigned int channel = channel_index[zone_idx];
        if(zones[zone_idx].leds_count > 0)
        {
            ChSort.push_back(channel);
        }
        else if(channel == 15 || channel == 31)
        {
            ChSort.push_back(channel);
        }
    }
    std::sort(ChSort.begin(), ChSort.end());
    for (std::size_t i = 0; i < ChSort.size(); i++)
    {
        int* ptr = std::find(channel_index, channel_index + 32, ChSort[i]);
        int zone_idx = ptr - channel_index;
        controller->SetChannelLEDs(ChSort[i], zones[zone_idx].colors, zones[zone_idx].leds_count);
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
