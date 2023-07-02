﻿/*-----------------------------------------*\
|  RGBController.h                          |
|                                           |
|  Driver for Nollie32 LED strip controller |
|                                           |
|  Name (cnn1236661)          25th Jun 2023 |
\*-----------------------------------------*/

#include "RGBController.h"
#include <chrono>
#include <vector>
#include <hidapi/hidapi.h>

#pragma once

class NollieController
{
public:
    NollieController(hid_device* dev_handle, const char* path,  unsigned short pid);
    std::string     GetLocationString();
    std::string     GetSerialString();
    void            SetMos(bool mos);
    void            SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors);
private:
    hid_device*     dev;
    std::string     location;
    void            SendPacket(unsigned char   channel,RGBColor * colors,unsigned int num_colors);
};
