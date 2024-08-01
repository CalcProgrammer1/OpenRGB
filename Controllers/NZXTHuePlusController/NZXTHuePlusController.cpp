/*---------------------------------------------------------*\
| NZXTHuePlusController.cpp                                 |
|                                                           |
|   Driver for NZXT Hue Plus                                |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      27 Aug 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include "NZXTHuePlusController.h"

using namespace std::chrono_literals;

HuePlusController::HuePlusController()
{

}

HuePlusController::~HuePlusController()
{
    delete serialport;
}

void HuePlusController::Initialize(char* port)
{
    port_name = port;

    serialport = new serial_port(port_name.c_str(), HUE_PLUS_BAUD);

    channel_leds[HUE_PLUS_CHANNEL_1_IDX] = GetLEDsOnChannel(HUE_PLUS_CHANNEL_1);
    channel_leds[HUE_PLUS_CHANNEL_2_IDX] = GetLEDsOnChannel(HUE_PLUS_CHANNEL_2);
}

std::string HuePlusController::GetLocation()
{
    return("COM: " + port_name);
}

unsigned int HuePlusController::GetLEDsOnChannel(unsigned int channel)
{
    unsigned char serial_buf[] =
    {
        0x8D, 0x00, 0x00, 0x00, 0x00
    };

    unsigned int ret_val = 0;

    /*-----------------------------------------------------*\
    | Set channel in serial packet                          |
    \*-----------------------------------------------------*/
    serial_buf[0x01]   = channel;

    serialport->serial_flush_rx();
    serialport->serial_write((char *)serial_buf, 2);

    std::this_thread::sleep_for(50ms);

    int bytes_read = serialport->serial_read((char *)serial_buf, 5);

    if(bytes_read == 5)
    {
        if(serial_buf[3] == 0x01)
        {
            ret_val = serial_buf[4] * 8;
        }
        else
        {
            ret_val += serial_buf[4] * 10;
        }
    }

    return(ret_val);
}

void HuePlusController::SetChannelEffect
    (
    unsigned char   channel,
    unsigned char   mode,
    unsigned char   speed,
    bool            direction,
    RGBColor *      colors,
    unsigned int    num_colors
    )
{
    unsigned char color_data[120];

    /*-----------------------------------------------------*\
    | If mode requires no colors, send packet               |
    \*-----------------------------------------------------*/
    if(num_colors == 0)
    {
        /*-----------------------------------------------------*\
        | Send mode without color data                          |
        \*-----------------------------------------------------*/
        SendPacket(channel, mode, direction, 0, speed, 0, NULL);
    }
    /*-----------------------------------------------------*\
    | If mode requires indexed colors, send color index     |
    | packets for each mode color                           |
    \*-----------------------------------------------------*/
    else if(num_colors <= 8)
    {
        for(std::size_t color_idx = 0; color_idx < num_colors; color_idx++)
        {
            /*-----------------------------------------------------*\
            | Fill in color data (40 entries per color)             |
            \*-----------------------------------------------------*/
            for (std::size_t idx = 0; idx < 40; idx++)
            {
                int pixel_idx = (int)idx * 3;
                RGBColor color = colors[color_idx];
                color_data[pixel_idx + 0x00] = RGBGetGValue(color);
                color_data[pixel_idx + 0x01] = RGBGetRValue(color);
                color_data[pixel_idx + 0x02] = RGBGetBValue(color);
            }

            /*-----------------------------------------------------*\
            | Send mode and color data                              |
            \*-----------------------------------------------------*/
            SendPacket(channel, mode, direction, (unsigned char)color_idx, speed, 40, &color_data[0]);
        }
    }
    /*-----------------------------------------------------*\
    | If mode requires per-LED colors, fill colors array    |
    \*-----------------------------------------------------*/
    else
    {
        /*-----------------------------------------------------*\
        | Fill in color data (up to 40 colors)                  |
        \*-----------------------------------------------------*/
        for (std::size_t idx = 0; idx < num_colors; idx++)
        {
            int pixel_idx = (int)idx * 3;
            RGBColor color = colors[idx];
            color_data[pixel_idx + 0x00] = RGBGetGValue(color);
            color_data[pixel_idx + 0x01] = RGBGetRValue(color);
            color_data[pixel_idx + 0x02] = RGBGetBValue(color);
        }

        /*-----------------------------------------------------*\
        | Send mode and color data                              |
        \*-----------------------------------------------------*/
        SendPacket(channel, mode, direction, 0, speed, num_colors, &color_data[0]);
    }
}

void HuePlusController::SetChannelLEDs
    (
    unsigned char   channel,
    RGBColor *      colors,
    unsigned int    num_colors
    )
{
    unsigned char color_data[120];

    /*-----------------------------------------------------*\
    | Fill in color data (up to 40 colors)                  |
    \*-----------------------------------------------------*/
    for (std::size_t idx = 0; idx < num_colors; idx++)
    {
        int pixel_idx = (int)idx * 3;
        RGBColor color = colors[idx];
        color_data[pixel_idx + 0x00] = RGBGetGValue(color);
        color_data[pixel_idx + 0x01] = RGBGetRValue(color);
        color_data[pixel_idx + 0x02] = RGBGetBValue(color);
    }

    /*-----------------------------------------------------*\
    | Send color data                                       |
    \*-----------------------------------------------------*/
    SendPacket(channel, HUE_PLUS_MODE_DIRECT, false, 0, 0, num_colors, &color_data[0]);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void HuePlusController::SendPacket
    (
    unsigned char   channel,
    unsigned char   mode,
    bool            direction,
    unsigned char   color_idx,
    unsigned char   speed,
    unsigned char   color_count,
    unsigned char*  color_data
    )
{
    unsigned char serial_buf[HUE_PLUS_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(serial_buf, 0x00, sizeof(serial_buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    serial_buf[0x00]    = 0x4B;

    /*-----------------------------------------------------*\
    | Set channel in serial packet                          |
    \*-----------------------------------------------------*/
    serial_buf[0x01]   = channel + 1;

    /*-----------------------------------------------------*\
    | Set mode in serial packet                             |
    \*-----------------------------------------------------*/
    serial_buf[0x02]   = mode;

    /*-----------------------------------------------------*\
    | Set options bitfield in serial packet                 |
    \*-----------------------------------------------------*/
    serial_buf[0x03]   = 0;
    serial_buf[0x03]   |= direction ? ( 1 << 4 ) : 0;

    /*-----------------------------------------------------*\
    | Set color index and speed in serial packet            |
    \*-----------------------------------------------------*/
    serial_buf[0x04]   = ( color_idx << 5 ) | speed;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&serial_buf[0x05], color_data, color_count * 3);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    serialport->serial_write((char *)serial_buf, HUE_PLUS_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Delay to allow Hue+ device to ready for next packet   |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(5ms);
}
