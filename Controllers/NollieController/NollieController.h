/*---------------------------------------------------------*\
| NollieController.h                                        |
|                                                           |
|   Driver for Nollie                                       |
|                                                           |
|   Name (cnn1236661)                           25 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include <vector>
#include <hidapi/hidapi.h>
#include "RGBController.h"

class NollieController
{
public:
    NollieController(hid_device* dev_handle, const char* path,  unsigned short pid);
    std::string     GetLocationString();
    std::string     GetSerialString();
    unsigned short  GetUSBPID();
    void            SetMos(bool mos);
    void            SendUpdate();
    void            SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors);
private:
    hid_device*     dev;
    std::string     location;
    unsigned short  usb_pid;
    void            SendPacket(unsigned char   channel,RGBColor * colors,unsigned int num_colors);
    void            SendPacketFS(unsigned char   channel,unsigned char packet_id,RGBColor * colors,unsigned int num_colors);
};
