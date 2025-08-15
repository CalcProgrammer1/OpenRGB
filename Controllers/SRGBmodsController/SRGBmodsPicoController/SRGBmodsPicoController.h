/*---------------------------------------------------------*\
| SRGBmodsPicoController.h                                  |
|                                                           |
|   Driver for SRGBmods Raspberry Pi Pico LED Controller    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

class SRGBmodsPicoController
{
public:
    SRGBmodsPicoController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~SRGBmodsPicoController();

    std::string     GetLocationString();
    std::string     GetNameString();
    std::string     GetSerialString();

    void            SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors);

    void            KeepaliveThread();
private:
    hid_device*             dev;
    std::string             location;
    std::string             name;
    std::thread*            keepalive_thread;
    std::atomic<bool>       keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock> last_commit_time;

    void            SendPacket
                        (
                        unsigned char   channel,
                        unsigned char   this_packet_id,
                        unsigned char   last_packet_id,
                        bool            reset,
                        RGBColor*       colors,
                        unsigned int    num_colors
                        );
};
