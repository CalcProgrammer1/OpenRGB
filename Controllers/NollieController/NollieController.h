/*---------------------------------------------------------*\
| NollieController.h                                        |
|                                                           |
|   Driver for Nollie                                       |
|                                                           |
|   Name (cnn1236661)                           25 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include <vector>
#include <hidapi.h>
#include "NollieDevices.h"
#include "RGBController.h"

#define NOLLIE32_MOS_TRIGGER_CH    26
#define NOLLIE32_MOS_TRIGGER_LED   20
#define NOLLIE32_FLAG1_CHANNEL     15
#define NOLLIE32_FLAG2_CHANNEL     31

class NollieController
{
public:
    NollieController(hid_device* dev_handle, const char* path, unsigned short vid, unsigned short pid, std::string dev_name);

    std::string     GetLocationString();
    std::string     GetNameString();
    std::string     GetSerialString();

    unsigned short  GetUSBVID();
    unsigned short  GetUSBPID();
    unsigned short  GetNumChannels();
    unsigned short  GetLEDsPerChannel();

    const int*      GetChannelIndex();
    std::string     GetChannelName(unsigned short channel);

    void            SetMos(bool mos);
    void            InitChLEDs(unsigned int *led_num_list,int ch_num);
    void            SendUpdate();
    void            SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors);

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned int    device_index;

    void            SendPacket(unsigned char   channel,RGBColor * colors,unsigned int num_colors);
    void            SendPacketFS(unsigned char   channel,unsigned char packet_id,RGBColor * colors,unsigned int num_colors);
};
