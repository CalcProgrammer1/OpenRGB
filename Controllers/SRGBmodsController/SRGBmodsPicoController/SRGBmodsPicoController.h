/*-----------------------------------------*\
|  SRGBmodsPicoController.h                 |
|                                           |
|  Definitions and types for SRGBmods       |
|  Raspberry Pi Pico LED Controller         |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/21/2022   |
\*-----------------------------------------*/

#include "RGBController.h"
#include <chrono>
#include <vector>
#include <hidapi/hidapi.h>

#pragma once

class SRGBmodsPicoController
{
public:
    SRGBmodsPicoController(hid_device* dev_handle, const char* path);
    ~SRGBmodsPicoController();

    std::string     GetLocationString();
    std::string     GetSerialString();

    void            SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors);

    void            KeepaliveThread();
private:
    hid_device*             dev;
    std::string             location;
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
