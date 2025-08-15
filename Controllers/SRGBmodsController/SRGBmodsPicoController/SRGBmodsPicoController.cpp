/*---------------------------------------------------------*\
| SRGBmodsPicoController.cpp                                |
|                                                           |
|   Driver for SRGBmods Raspberry Pi Pico LED Controller    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "SRGBmodsPicoController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

SRGBmodsPicoController::SRGBmodsPicoController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    /*-----------------------------------------------------*\
    | The SRGBmods Pico controller requires a packet within |
    | 10 seconds of sending the lighting change in order    |
    | to not revert back into hardware mode.  Start a thread|
    | to continuously send a keepalive packet every 5s      |
    \*-----------------------------------------------------*/
    keepalive_thread_run = 1;
    keepalive_thread     = new std::thread(&SRGBmodsPicoController::KeepaliveThread, this);
}

SRGBmodsPicoController::~SRGBmodsPicoController()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    hid_close(dev);
}

void SRGBmodsPicoController::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if((std::chrono::steady_clock::now() - last_commit_time) > std::chrono::seconds(1))
        {
            SendPacket(1, 0, 0, false, NULL, 0);
        }
        std::this_thread::sleep_for(5s);
    }
}

std::string SRGBmodsPicoController::GetLocationString()
{
    return("HID: " + location);
}

std::string SRGBmodsPicoController::GetNameString()
{
    return(name);
}

std::string SRGBmodsPicoController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void SRGBmodsPicoController::SetChannelLEDs(unsigned char channel, RGBColor* colors, unsigned int num_colors)
{
    unsigned int num_packets    = (num_colors / 20) + ((num_colors % 20) > 0);
    unsigned int color_idx      = 0;

    for(unsigned int packet_idx = 0; packet_idx < num_packets; packet_idx++)
    {
        unsigned int colors_in_packet = 20;

        if(num_colors - color_idx < colors_in_packet)
        {
            colors_in_packet = num_colors - color_idx;
        }

        SendPacket(channel, packet_idx + 1, num_packets, false, &colors[color_idx], colors_in_packet);

        color_idx += colors_in_packet;
    }
}

void SRGBmodsPicoController::SendPacket
    (
    unsigned char   channel,
    unsigned char   this_packet_id,
    unsigned char   last_packet_id,
    bool            reset,
    RGBColor*       colors,
    unsigned int    num_colors
    )
{
    unsigned char   usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Update last commit time                               |
    \*-----------------------------------------------------*/
    last_commit_time = std::chrono::steady_clock::now();

    /*-----------------------------------------------------*\
    | Set up Firmware Version Request packet                |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;             /* hidapi Report ID*/
    usb_buf[0x01]   = this_packet_id;   /* This Packet ID  */
    usb_buf[0x02]   = reset;            /* Reset Flag      */
    usb_buf[0x03]   = last_packet_id;   /* Last Packet ID  */
    usb_buf[0x04]   = channel + 1;      /* Channel (1 or 2)*/

    for(unsigned int color_idx = 0; color_idx < num_colors; color_idx++)
    {
        usb_buf[0x05 + (color_idx * 3)] = RGBGetRValue(colors[color_idx]);
        usb_buf[0x06 + (color_idx * 3)] = RGBGetGValue(colors[color_idx]);
        usb_buf[0x07 + (color_idx * 3)] = RGBGetBValue(colors[color_idx]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}
