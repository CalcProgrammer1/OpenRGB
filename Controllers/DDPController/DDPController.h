/*---------------------------------------------------------*\
| DDPController.h                                           |
|                                                           |
|   Driver for DDP protocol devices                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include "net_port.h"

#define DDP_DEFAULT_PORT        4048
#define DDP_HEADER_SIZE         10
#define DDP_HEADER_SIZE_TC      14
#define DDP_VERSION             1
#define DDP_MAX_PACKET_SIZE     1450
#define DDP_MAX_DATA_SIZE       1440

#define DDP_FLAG_VER_MASK       0xC0
#define DDP_FLAG_VER_1          0x40
#define DDP_FLAG_TIMECODE       0x10
#define DDP_FLAG_STORAGE        0x08
#define DDP_FLAG_REPLY          0x04
#define DDP_FLAG_QUERY          0x02
#define DDP_FLAG_PUSH           0x01

#define DDP_TYPE_RGB8           0x0B
#define DDP_TYPE_RGB_SIMPLE     1

#pragma pack(push, 1)
struct ddp_header
{
    unsigned char   flags;
    unsigned char   sequence;
    unsigned char   data_type;
    unsigned char   dest_id;
    unsigned int    data_offset;
    unsigned short  data_length;
};
#pragma pack(pop)

struct DDPDevice
{
    std::string         name;
    std::string         ip;
    unsigned short      port;
    unsigned int        num_leds;
};

struct DDPEndpoint
{
    char            ip[16];
    unsigned short  port;
};

class DDPController
{
public:
    DDPController(const std::vector<DDPDevice>& devices);
    ~DDPController();

    void                UpdateLEDs(const std::vector<unsigned int>& colors);
    void                SetKeepaliveTime(unsigned int time_ms);
    
private:
    std::vector<DDPDevice>  devices;
    std::vector<net_port*>  udp_ports;
    DDPEndpoint*            unique_endpoints;
    unsigned int            num_endpoints;
    unsigned char           sequence_number;
    

    std::atomic<bool>       keepalive_thread_run;
    std::thread             keepalive_thread;
    std::mutex              last_update_mutex;
    std::chrono::steady_clock::time_point last_update_time;
    std::vector<unsigned int> last_colors;
    unsigned int            keepalive_time_ms;
    
    bool                    InitializeNetPorts();
    void                    CloseNetPorts();
    int                     GetPortIndex(const DDPDevice& device);
    bool                    SendDDPPacket(const DDPDevice& device, 
                                         const unsigned char* data, 
                                         unsigned short length, 
                                         unsigned int offset = 0);
    void                    KeepaliveThreadFunction();
};
