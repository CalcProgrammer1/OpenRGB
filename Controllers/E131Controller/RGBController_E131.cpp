/*---------------------------------------------------------*\
| RGBController_E131.cpp                                    |
|                                                           |
|   RGBController for E1.31 devices                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                18 Oct 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <e131.h>
#include <math.h>
#include "RGBController_E131.h"

using namespace std::chrono_literals;

/**------------------------------------------------------------------*\
    @name E1.31 Devices
    @category LEDStrip
    @type E1.31
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectE131Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_E131::RGBController_E131(std::vector<E131Device> device_list)
{
    bool multicast = false;

    devices = device_list;

    name        = "E1.31 Device Group";
    type        = DEVICE_TYPE_LEDSTRIP;
    description = "E1.31 Streaming ACN Device";
    location    = "E1.31: ";

    /*-----------------------------------------*\
    | If this controller only represents a      |
    | single device, use the device name for the|
    | controller name                           |
    \*-----------------------------------------*/
    if(devices.size() == 1)
    {
        name    = devices[0].name;
    }
    else if(devices[0].ip != "")
    {
        name += " (" + devices[0].ip + ")";
    }

    /*-----------------------------------------*\
    | Append the destination address to the     |
    | location field                            |
    \*-----------------------------------------*/
    if(devices[0].ip != "")
    {
        location += "Unicast " + devices[0].ip + ", ";
    }
    else
    {
        location   += "Multicast, ";
        multicast = true;
    }

    /*-----------------------------------------*\
    | Calculate universe list                   |
    | Use this to fill in the location field    |
    \*-----------------------------------------*/
    std::vector<unsigned int> universe_list;

    for(unsigned int device_idx = 0; device_idx < devices.size(); device_idx++)
    {
        float universe_size = (float)devices[device_idx].universe_size;
        unsigned int total_universes = (unsigned int)ceil( ( ( devices[device_idx].num_leds * 3 ) + devices[device_idx].start_channel ) / universe_size );

        for(unsigned int univ_idx = 0; univ_idx < total_universes; univ_idx++)
        {
            bool found = false;

            for(unsigned int univ_list_idx = 0; univ_list_idx < universe_list.size(); univ_list_idx++)
            {
                if((devices[device_idx].start_universe + univ_idx) == universe_list[univ_list_idx])
                {
                    found = true;
                    break;
                }
            }

            if(!found)
            {
                universe_list.push_back(devices[device_idx].start_universe + univ_idx);
            }
        }
    }

    /*-----------------------------------------*\
    | Append "Universe" and make plural if there|
    | are multiple universes in use             |
    \*-----------------------------------------*/
    location   += "Universe";

    if(universe_list.size() > 1)
    {
        location += "s ";
    }
    else
    {
        location += " ";
    }

    /*-----------------------------------------*\
    | Append comma separated list of universes  |
    \*-----------------------------------------*/
    for(unsigned int univ_list_idx = 0; univ_list_idx < universe_list.size(); univ_list_idx++)
    {
        location += std::to_string(universe_list[univ_list_idx]);

        if(univ_list_idx < (universe_list.size() - 1))
        {
            location += ", ";
        }
    }

    /*-----------------------------------------*\
    | Set up modes                              |
    \*-----------------------------------------*/
    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    /*-----------------------------------------*\
    | Create E1.31 socket                       |
    \*-----------------------------------------*/
    sockfd = e131_socket();

    keepalive_delay = 0ms;

    SetupZones();

    for (std::size_t device_idx = 0; device_idx < devices.size(); device_idx++)
    {
        /*-----------------------------------------*\
        | Update keepalive delay                    |
        \*-----------------------------------------*/
        if(devices[device_idx].keepalive_time > 0)
        {
            if(keepalive_delay.count() == 0 || keepalive_delay.count() > devices[device_idx].keepalive_time)
            {
                keepalive_delay = std::chrono::milliseconds(devices[device_idx].keepalive_time);
            }
        }

        /*-----------------------------------------*\
        | Add Universes                             |
        \*-----------------------------------------*/
        float universe_size = (float)devices[device_idx].universe_size;
        unsigned int total_universes = (unsigned int)ceil( ( ( devices[device_idx].num_leds * 3 ) + devices[device_idx].start_channel ) / universe_size );

        for (unsigned int univ_idx = 0; univ_idx < total_universes; univ_idx++)
        {
            unsigned int universe = devices[device_idx].start_universe + univ_idx;
            bool universe_exists = false;

            for (std::size_t pkt_idx = 0; pkt_idx < packets.size(); pkt_idx++)
            {
                if(universes[pkt_idx] == universe)
                {
                    universe_exists = true;
                }
            }

            if(!universe_exists)
            {
                e131_packet_t   packet;
                e131_addr_t     dest_addr;

                e131_pkt_init(&packet, (uint16_t)universe, (uint16_t)universe_size);

                if(multicast)
                {
                    e131_multicast_dest(&dest_addr, universe, E131_DEFAULT_PORT);
                }
                else
                {
                    e131_unicast_dest(&dest_addr, devices[0].ip.c_str(), E131_DEFAULT_PORT);
                }

                packets.push_back(packet);
                universes.push_back(universe);
                dest_addrs.push_back(dest_addr);
            }
        }

        /*-----------------------------------------*\
        | Generate matrix maps                      |
        \*-----------------------------------------*/
        if(devices[device_idx].type == ZONE_TYPE_MATRIX)
        {
            unsigned int led_idx = 0;
            matrix_map_type * new_map = new matrix_map_type;

            new_map->width = devices[device_idx].matrix_width;
            new_map->height = devices[device_idx].matrix_height;
            new_map->map = new unsigned int[devices[device_idx].matrix_width * devices[device_idx].matrix_height];

            switch(devices[device_idx].matrix_order)
            {
                case E131_MATRIX_ORDER_HORIZONTAL_TOP_LEFT:
                    for(unsigned int y = 0; y < new_map->height; y++)
                    {
                        for(unsigned int x = 0; x < new_map->width; x++)
                        {
                            new_map->map[(y * new_map->width) + x] = led_idx;
                            led_idx++;
                        }
                    }
                    break;
                case E131_MATRIX_ORDER_HORIZONTAL_TOP_RIGHT:
                    for(unsigned int y = 0; y < new_map->height; y++)
                    {
                        for(int x = new_map->width - 1; x >= 0; x--)
                        {
                            new_map->map[(y * new_map->width) + x] = led_idx;
                            led_idx++;
                        }
                    }
                    break;
                case E131_MATRIX_ORDER_HORIZONTAL_BOTTOM_LEFT:
                    for(int y = new_map->height; y >= 0; y--)
                    {
                        for(unsigned int x = 0; x < new_map->width; x++)
                        {
                            new_map->map[(y * new_map->width) + x] = led_idx;
                            led_idx++;
                        }
                    }
                    break;
                case E131_MATRIX_ORDER_HORIZONTAL_BOTTOM_RIGHT:
                    for(int y = new_map->height; y >= 0; y--)
                    {
                        for(int x = new_map->width - 1; x >= 0; x--)
                        {
                            new_map->map[(y * new_map->width) + x] = led_idx;
                            led_idx++;
                        }
                    }
                    break;
                case E131_MATRIX_ORDER_VERTICAL_TOP_LEFT:
                    for(unsigned int x = 0; x < new_map->width; x++)
                    {
                        for(unsigned int y = 0; y < new_map->height; y++)
                        {
                            new_map->map[(y * new_map->width) + x] = led_idx;
                            led_idx++;
                        }
                    }
                    break;
                case E131_MATRIX_ORDER_VERTICAL_TOP_RIGHT:
                    for(int x = new_map->width - 1; x >= 0; x--)
                    {
                        for(unsigned int y = 0; y < new_map->height; y++)
                        {
                            new_map->map[(y * new_map->width) + x] = led_idx;
                            led_idx++;
                        }
                    }
                    break;
                case E131_MATRIX_ORDER_VERTICAL_BOTTOM_LEFT:
                    for(unsigned int x = 0; x < new_map->width; x++)
                    {
                        for(int y = new_map->height - 1; y >= 0; y--)
                        {
                            new_map->map[(y * new_map->width) + x] = led_idx;
                            led_idx++;
                        }
                    }
                    break;
                case E131_MATRIX_ORDER_VERTICAL_BOTTOM_RIGHT:
                    for(int x = new_map->width - 1; x >= 0; x--)
                    {
                        for(int y = new_map->height - 1; y >= 0; y--)
                        {
                            new_map->map[(y * new_map->width) + x] = led_idx;
                            led_idx++;
                        }
                    }
                    break;
            }
            zones[device_idx].matrix_map = new_map;
        }
    }

    if(keepalive_delay.count() > 0)
    {
        keepalive_thread_run = 1;
        keepalive_thread = new std::thread(&RGBController_E131::KeepaliveThreadFunction, this);
    }
    else
    {
        keepalive_thread_run = 0;
        keepalive_thread = nullptr;
    }
}

RGBController_E131::~RGBController_E131()
{
    if(keepalive_thread != nullptr)
    {
        keepalive_thread_run = 0;
        keepalive_thread->join();
        delete keepalive_thread;
    }

    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            if(zones[zone_index].matrix_map->map != NULL)
            {
                delete zones[zone_index].matrix_map->map;
            }

            delete zones[zone_index].matrix_map;
        }
    }
}

void RGBController_E131::SetupZones()
{
    /*-----------------------------------------*\
    | Add Zones                                 |
    \*-----------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < devices.size(); zone_idx++)
    {
        zone led_zone;
        led_zone.name           = devices[zone_idx].name;
        led_zone.type           = devices[zone_idx].type;
        led_zone.leds_min       = devices[zone_idx].num_leds;
        led_zone.leds_max       = devices[zone_idx].num_leds;
        led_zone.leds_count     = devices[zone_idx].num_leds;
        led_zone.matrix_map     = NULL;

        zones.push_back(led_zone);
    }

    /*-----------------------------------------*\
    | Add LEDs                                  |
    \*-----------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(std::size_t led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;

            new_led.name = zones[zone_idx].name + " LED ";
            new_led.name.append(std::to_string(led_idx));

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_E131::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_E131::DeviceUpdateLEDs()
{
    int color_idx = 0;

    last_update_time = std::chrono::steady_clock::now();

    for(std::size_t device_idx = 0; device_idx < devices.size(); device_idx++)
    {
        float universe_size = (float)devices[device_idx].universe_size;
        unsigned int total_universes = (unsigned int)ceil( ( ( devices[device_idx].num_leds * 3 ) + devices[device_idx].start_channel ) / universe_size );
        unsigned int channel_idx = devices[device_idx].start_channel;
        unsigned int led_idx = 0;
        unsigned int rgb_idx = 0;
        bool         done = false;

        for (unsigned int univ_idx = 0; univ_idx < total_universes; univ_idx++)
        {
            unsigned int universe = devices[device_idx].start_universe + univ_idx;

            for(std::size_t packet_idx = 0; packet_idx < packets.size(); packet_idx++)
            {
                if(!done && (universes[packet_idx] == universe))
                {
                    while(!done && (channel_idx <= universe_size))
                    {
                        switch(rgb_idx)
                        {
                            case 0:
                                packets[packet_idx].dmp.prop_val[channel_idx] = RGBGetRValue( colors[color_idx] );
                                rgb_idx = 1;
                                break;
                            case 1:
                                packets[packet_idx].dmp.prop_val[channel_idx] = RGBGetGValue( colors[color_idx] );
                                rgb_idx = 2;
                                break;
                            case 2:
                                packets[packet_idx].dmp.prop_val[channel_idx] = RGBGetBValue( colors[color_idx] );
                                rgb_idx = 0;
                                led_idx++;
                                color_idx++;
                                break;
                        }

                        if(led_idx >= devices[device_idx].num_leds)
                        {
                            done = true;
                        }

                        channel_idx++;
                    }
                }
            }

            channel_idx = 1;
        }
    }

    for(std::size_t packet_idx = 0; packet_idx < packets.size(); packet_idx++)
    {
        e131_send(sockfd, &packets[packet_idx], &dest_addrs[packet_idx]);
        packets[packet_idx].frame.seq_number++;
    }
}

void RGBController_E131::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_E131::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_E131::DeviceUpdateMode()
{

}

void RGBController_E131::KeepaliveThreadFunction()
{
    while(keepalive_thread_run.load())
    {
        if((std::chrono::steady_clock::now() - last_update_time) > ( keepalive_delay * 0.95f ) )
        {
            UpdateLEDs();
        }
        std::this_thread::sleep_for(keepalive_delay / 2);
    }
}
