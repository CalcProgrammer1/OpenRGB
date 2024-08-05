/*---------------------------------------------------------*\
| ThermaltakeRiingQuadController.h                          |
|                                                           |
|   Driver for Thermaltake Riing Quad                       |
|                                                           |
|   Chris M (Dr_No)                             15 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

#define THERMALTAKE_QUAD_PACKET_SIZE        193
#define THERMALTAKE_QUAD_INTERRUPT_TIMEOUT  250
#define THERMALTAKE_QUAD_KEEPALIVE          3
#define HID_MAX_STR                         255

enum
{
    THERMALTAKE_QUAD_COMMAND_BYTE   = 1,
    THERMALTAKE_QUAD_FUNCTION_BYTE  = 2,
    THERMALTAKE_QUAD_ZONE_BYTE      = 3,
    THERMALTAKE_QUAD_MODE_BYTE      = 4,
    THERMALTAKE_QUAD_DATA_BYTE      = 5,
};

enum
{
    THERMALTAKE_QUAD_MODE_DIRECT    = 0x24
};

enum
{
    THERMALTAKE_QUAD_SPEED_EXTREME  = 0x00,
    THERMALTAKE_QUAD_SPEED_FAST     = 0x01,
    THERMALTAKE_QUAD_SPEED_NORMAL   = 0x02,
    THERMALTAKE_QUAD_SPEED_SLOW     = 0x03,
};

#define THERMALTAKE_QUAD_NUM_CHANNELS   5

class ThermaltakeRiingQuadController
{
public:
    ThermaltakeRiingQuadController(hid_device* dev_handle, const char* path);
    ~ThermaltakeRiingQuadController();

    std::string             GetDeviceName();
    std::string             GetDeviceLocation();
    std::string             GetSerial();

    void                    SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors);
    void                    SetMode(unsigned char mode, unsigned char speed);

private:
    hid_device*             dev;

    unsigned char           current_mode;
    unsigned char           current_speed;
    std::string             device_name;
    std::string             location;

    uint8_t                 tt_quad_buffer[THERMALTAKE_QUAD_NUM_CHANNELS][THERMALTAKE_QUAD_PACKET_SIZE];
    std::thread*            keepalive_thread;
    std::atomic<bool>       keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock> last_commit_time;

    void                    SendBuffer();
    void                    KeepaliveThread();

    void                    SendInit();

    void                    SendFan();
    void                    SendSave();
};
