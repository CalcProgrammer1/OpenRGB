/*---------------------------------------------------------*\
| DRGBController.h                                          |
|                                                           |
|   Driver for DRGBmods                                     |
|                                                           |
|   Zhi Yan                                     25 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include <vector>
#include <hidapi/hidapi.h>
#include "RGBController.h"

class DRGBController
{
public:
    DRGBController(hid_device* dev_handle, const char* path,  unsigned short pid);
    ~DRGBController();
    std::string     GetLocationString();
    std::string     GetSerialString();
    unsigned short  GetDevicePID();
    void            SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors);
    void            SendPacket(unsigned char* colors,unsigned int buf_packets ,unsigned int Array);
    void            SendPacketFS(unsigned char* colors,unsigned int buf_packets ,bool Array);
private:
    hid_device*             dev;
    unsigned short          device_pid;
    std::string             location;
};
