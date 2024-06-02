﻿/*---------------------------------------------------------*\
| NollieController.cpp                                      |
|                                                           |
|   Driver for Nollie                                       |
|                                                           |
|   Name (cnn1236661)                           25 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "NollieController.h"
#include "RGBController_Nollie.h"

using namespace std::chrono_literals;

NollieController::NollieController(hid_device* dev_handle, const char* path, unsigned short pid)
{
    dev            = dev_handle;
    location       = path;
    usb_pid        = pid;
}

std::string NollieController::GetLocationString()
{
    return("HID: " + location);
}

std::string NollieController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());
    return(return_string);
}

unsigned short NollieController::GetUSBPID()
{
    return(usb_pid);
}

void NollieController::SetMos(bool mos)
{
    unsigned char   usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x01]  = 0x80;
    usb_buf[0x02]  = mos;
    hid_write(dev, usb_buf, 65);
}

void NollieController::SetChannelLEDs(unsigned char channel, RGBColor* colors, unsigned int num_colors)
{
   if(usb_pid == NOLLIE32_PID || usb_pid == NOLLIE16_PID)
   {
       SendPacket(channel,&colors[0], num_colors);
   }
   else
   {
       unsigned int num_packets    = (num_colors / 21) + ((num_colors % 21) > 0);
       unsigned int color_idx      = 0;

       for(unsigned int packet_idx = 0; packet_idx < num_packets; packet_idx++)
       {
           unsigned int colors_in_packet = 21;
           if(num_colors - color_idx < colors_in_packet)
           {
               colors_in_packet = num_colors - color_idx;
           }
           SendPacketFS(channel,packet_idx,&colors[color_idx], colors_in_packet);
           color_idx += colors_in_packet;
       }
   }
}

void NollieController::SendUpdate()
{
    unsigned char   usb_buf[65] ;
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[1] = 0xff;
    hid_write(dev, usb_buf, 65);
}

void NollieController::SendPacket(unsigned char channel,RGBColor* colors,unsigned int num_colors)
{

    unsigned char   usb_buf[1025];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[1] = channel;
    usb_buf[2] = 0;
    usb_buf[3] = num_colors / 256;
    usb_buf[4] = num_colors % 256;
    if(!num_colors)
    {
        hid_write(dev, usb_buf, 1025);
        return;
    }
    for(unsigned int color_idx = 0; color_idx < num_colors; color_idx++)
    {
        usb_buf[0x05 + (color_idx * 3)] = RGBGetGValue(colors[color_idx]);
        usb_buf[0x06 + (color_idx * 3)] = RGBGetRValue(colors[color_idx]);
        usb_buf[0x07 + (color_idx * 3)] = RGBGetBValue(colors[color_idx]);
    }
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 1025);
    if(channel == NOLLIE32_FLAG1_CHANNEL || channel == NOLLIE32_FLAG2_CHANNEL)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }
}

void NollieController::SendPacketFS(unsigned char channel,unsigned char packet_id,RGBColor* colors,unsigned int num_colors)
{
    unsigned char   usb_buf[65];
    unsigned int    packet_interval;
    unsigned int    dev_pid = GetUSBPID();
    switch(dev_pid)
    {
        case NOLLIE28_12_PID:
            packet_interval = 2;
            break;
        case NOLLIE8_PID:
            packet_interval = 6;
            break;
        case NOLLIE1_PID:
            packet_interval = 30;
            break;
        default:
            packet_interval = 25;
            break;
    }
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = packet_id + channel * packet_interval;
    for(unsigned int color_idx = 0; color_idx < num_colors; color_idx++)
    {
        usb_buf[0x02 + (color_idx * 3)] = RGBGetRValue(colors[color_idx]);
        usb_buf[0x03 + (color_idx * 3)] = RGBGetGValue(colors[color_idx]);
        usb_buf[0x04 + (color_idx * 3)] = RGBGetBValue(colors[color_idx]);
        if(dev_pid == NOLLIE8_PID || dev_pid == NOLLIE1_PID )
        {
            usb_buf[0x02 + (color_idx * 3)] = RGBGetGValue(colors[color_idx]);
            usb_buf[0x03 + (color_idx * 3)] = RGBGetRValue(colors[color_idx]);
        }
    }
    hid_write(dev, usb_buf, 65);
}
