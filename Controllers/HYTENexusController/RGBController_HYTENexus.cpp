/*---------------------------------------------------------*\
| RGBController_HYTENexus.cpp                               |
|                                                           |
|   RGBController for HYTE Nexus                            |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_HYTENexus.h"

using namespace std::chrono_literals;

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int thicc_q60_matrix_map[9][5] =
{
    {  33,  32,  NA,  17,   0 },
    {  34,  31,  NA,  16,   1 },
    {  35,  30,  NA,  15,   2 },
    {  36,  29,  18,  14,   3 },
    {  37,  28,  19,  13,   4 },
    {  38,  27,  20,  12,   5 },
    {  39,  26,  21,  11,   6 },
    {  40,  25,  22,  10,   7 },
    {  41,  24,  23,   9,   8 },
};

RGBController_HYTENexus::RGBController_HYTENexus(HYTENexusController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetName();
    vendor              = "HYTE";
    description         = "HYTE Nexus Device";
    type                = DEVICE_TYPE_LEDSTRIP;
    location            = controller->GetLocation();
    version             = controller->GetFirmwareVersion();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_HYTENexus::~RGBController_HYTENexus()
{
    delete controller;
}

void RGBController_HYTENexus::SetupZones()
{
    for(unsigned int channel = 0; channel < controller->num_channels; channel++)
    {
        unsigned int channel_leds = 0;
        unsigned int logo_leds    = 0;

        for(unsigned int device = 0; device < controller->channels[channel].num_devices; device++)
        {
            channel_leds += controller->channels[channel].devices[device].led_count;
        }

        if(controller->channels[channel].has_4_led_logo == true || controller->channels[channel].has_6_led_logo == true)
        {
            if(controller->channels[channel].has_4_led_logo == true)
            {
                logo_leds = 4;
            }
            else if(controller->channels[channel].has_6_led_logo == true)
            {
                logo_leds = 6;
            }

            channel_leds += logo_leds;
        }

        if(controller->channels[channel].has_lcd_leds == true)
        {
            channel_leds += 42;
        }

        zone channel_zone;

        channel_zone.name       = "Channel " + std::to_string(channel);
        channel_zone.type       = ZONE_TYPE_LINEAR;
        channel_zone.leds_min   = channel_leds;
        channel_zone.leds_max   = channel_leds;
        channel_zone.leds_count = channel_leds;
        channel_zone.matrix_map = NULL;

        for(unsigned int led_idx = 0; led_idx < channel_leds; led_idx++)
        {
            led channel_led;

            channel_led.name    = "Channel " + std::to_string(channel) + " LED " + std::to_string(led_idx);
            channel_led.value   = channel;

            leds.push_back(channel_led);
        }

        unsigned int start_idx = 0;

        if(controller->channels[channel].has_4_led_logo == true || controller->channels[channel].has_6_led_logo == true)
        {
            segment logo_segment;

            logo_segment.name           = "Logo";
            logo_segment.leds_count     = logo_leds;
            logo_segment.start_idx      = start_idx;
            logo_segment.type           = ZONE_TYPE_SINGLE;

            channel_zone.segments.push_back(logo_segment);

            start_idx                  += logo_segment.leds_count;
        }

        if(controller->channels[channel].has_lcd_leds == true)
        {
            segment lcd_leds_segment;

            lcd_leds_segment.name           = "LCD LED Matrix";
            lcd_leds_segment.leds_count     = 42;
            lcd_leds_segment.start_idx      = start_idx;
            lcd_leds_segment.type           = ZONE_TYPE_MATRIX;

            channel_zone.type               = ZONE_TYPE_MATRIX;
            channel_zone.matrix_map         = new matrix_map_type;
            channel_zone.matrix_map->height = 9;
            channel_zone.matrix_map->width  = 5;
            channel_zone.matrix_map->map    = (unsigned int *)&thicc_q60_matrix_map;

            channel_zone.segments.push_back(lcd_leds_segment);

            start_idx                  += lcd_leds_segment.leds_count;
        }

        for(unsigned int device = 0; device < controller->channels[channel].num_devices; device++)
        {
            if(controller->channels[channel].devices[device].led_count > 0)
            {
                segment device_segment;

                device_segment.name         = controller->GetDeviceName(controller->channels[channel].devices[device].device_type);
                device_segment.leds_count   = controller->channels[channel].devices[device].led_count;
                device_segment.start_idx    = start_idx;
                device_segment.type         = ZONE_TYPE_LINEAR;

                channel_zone.segments.push_back(device_segment);

                start_idx                  += device_segment.leds_count;
            }
        }

        zones.push_back(channel_zone);
    }

    SetupColors();
}

void RGBController_HYTENexus::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HYTENexus::DeviceUpdateLEDs()
{
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        UpdateZoneLEDs(zone_idx);
    }
}

void RGBController_HYTENexus::UpdateZoneLEDs(int zone)
{
    controller->LEDStreaming(zone, zones[zone].leds_count, zones[zone].colors);
}

void RGBController_HYTENexus::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(leds[led].value);
}

void RGBController_HYTENexus::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
