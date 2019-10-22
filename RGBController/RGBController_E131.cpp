/*-----------------------------------------*\
|  RGBController_LEDStrip.cpp               |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  E1.31 Streaming ACN interface            |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/18/2019  |
\*-----------------------------------------*/

#include "RGBController_E131.h"
#include <e131.h>
#include <math.h>

RGBController_E131::RGBController_E131(std::vector<E131Device> device_list)
{
    name = "E1.31 Streaming ACN Device";

    devices = device_list;

    mode led_mode;
    led_mode.name = "Custom";
    modes.push_back(led_mode);

    sockfd = e131_socket();

    for (int i = 0; i < devices.size(); i++)
    {
		/*-----------------------------------------*\
		| Add LEDs                                  |
        \*-----------------------------------------*/
    	for (int led_idx = 0; led_idx < devices[i].num_leds; led_idx++)
    	{
       		colors.push_back(0x00000000);
        	led new_led;
        	new_led.name = "E1.31 LED";
        	leds.push_back(new_led);
		}
		
        /*-----------------------------------------*\
        | Add Zones                                 |
        \*-----------------------------------------*/
	    zone led_zone;
    	led_zone.name = devices[i].name;
    	led_zone.type = devices[i].type;
    	std::vector<int> led_zone_map;
    	for (int led_idx = 0; led_idx < devices[i].num_leds; led_idx++)
    	{
      	  	led_zone_map.push_back(led_idx);
    	}
    	led_zone.map.push_back(led_zone_map);
    	zones.push_back(led_zone);

        /*-----------------------------------------*\
        | Add Universes                             |
        \*-----------------------------------------*/
        unsigned int total_universes = ceil( ( ( devices[i].num_leds * 3 ) + devices[i].start_channel ) / 512.0f );

        for (int univ_idx = 0; univ_idx < total_universes; univ_idx++)
        {
            unsigned int universe = devices[i].start_universe + univ_idx;
            bool universe_exists = false;

            for (int pkt_idx = 0; pkt_idx < packets.size(); pkt_idx++)
            {
                if(packets[pkt_idx].frame.universe == universe)
                {
                    universe_exists = true;
                }
            }

            if(!universe_exists)
            {
                e131_packet_t   packet;
                e131_addr_t     dest_addr;

                e131_pkt_init(&packet, universe, 512);
                e131_multicast_dest(&dest_addr, universe, E131_DEFAULT_PORT);

                packets.push_back(packet);
                universes.push_back(universe);
                dest_addrs.push_back(dest_addr);
            }
        }
	}
}

int RGBController_E131::GetMode()
{
    return 0;
}

void RGBController_E131::SetMode(int mode)
{

}

void RGBController_E131::SetCustomMode()
{

}

void RGBController_E131::SetAllLEDs(RGBColor color)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i] = color;
    }

	UpdateLEDs();
}

void RGBController_E131::SetAllZoneLEDs(int zone, RGBColor color)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i] = color;
    }

	UpdateLEDs();
}

void RGBController_E131::SetLED(int led, RGBColor color)
{
    colors[led] = color;

	UpdateLEDs();
}

void RGBController_E131::UpdateLEDs()
{
    for(int device_idx = 0; device_idx < devices.size(); device_idx++)
    {
        unsigned int total_universes = ceil( ( ( devices[device_idx].num_leds * 3 ) + devices[device_idx].start_channel ) / 512.0f );
        unsigned int channel_idx = devices[device_idx].start_channel;
        unsigned int led_idx = 0;
        unsigned int rgb_idx = 0;
        bool         done = false;

        for (int univ_idx = 0; univ_idx < total_universes; univ_idx++)
        {
            unsigned int universe = devices[device_idx].start_universe + univ_idx;

            for(int packet_idx = 0; packet_idx < packets.size(); packet_idx++)
            {
                if(!done && (universes[packet_idx] == universe))
                {
                    while(!done && (channel_idx <= 512))
                    {
                        switch(rgb_idx)
                        {
                            case 0:
                                packets[packet_idx].dmp.prop_val[channel_idx] = RGBGetRValue( colors[led_idx] );
                                rgb_idx = 1;
                                break;
                            case 1:
                                packets[packet_idx].dmp.prop_val[channel_idx] = RGBGetGValue( colors[led_idx] );
                                rgb_idx = 2;
                                break;
                            case 2:
                                packets[packet_idx].dmp.prop_val[channel_idx] = RGBGetBValue( colors[led_idx] );
                                rgb_idx = 0;
                                led_idx++;
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

    for(int packet_idx = 0; packet_idx < packets.size(); packet_idx++)
    {
	    e131_send(sockfd, &packets[packet_idx], &dest_addrs[packet_idx]);
        packets[packet_idx].frame.seq_number++;
    }
}