/*---------------------------------------------------------*\
| ThermaltakeRiingTrioController.h                          |
|                                                           |
|   Driver for Thermaltake Riing Trio                       |
|                                                           |
|   Sam B (4rcheria)                            24 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

#define THERMALTAKE_TRIO_PACKET_SIZE        65
#define THERMALTAKE_TRIO_INTERRUPT_TIMEOUT  250
#define THERMALTAKE_TRIO_KEEPALIVE          3
#define HID_MAX_STR                         255

enum
{
    THERMALTAKE_TRIO_COMMAND_BYTE   = 1,
    THERMALTAKE_TRIO_FUNCTION_BYTE  = 2,
    THERMALTAKE_TRIO_ZONE_BYTE      = 3,
    THERMALTAKE_TRIO_MODE_BYTE      = 4,
    THERMALTAKE_TRIO_CHUNK_ID     = 6,
    THERMALTAKE_TRIO_DATA_BYTE      = 8,
    THERMALTAKE_TRIO_CHUNK_LENGTH   = 19,
};

enum
{
    THERMALTAKE_TRIO_MODE_DIRECT    = 0x24
};

enum
{
    THERMALTAKE_TRIO_SPEED_EXTREME  = 0x00,
    THERMALTAKE_TRIO_SPEED_FAST     = 0x01,
    THERMALTAKE_TRIO_SPEED_NORMAL   = 0x02,
    THERMALTAKE_TRIO_SPEED_SLOW     = 0x03,
};

#define THERMALTAKE_TRIO_NUM_CHANNELS   5

class ThermaltakeRiingTrioController
{
public:
    ThermaltakeRiingTrioController(hid_device* dev_handle, const char* path);
    ~ThermaltakeRiingTrioController();

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

    uint8_t                 tt_trio_buffer[THERMALTAKE_TRIO_NUM_CHANNELS][THERMALTAKE_TRIO_PACKET_SIZE];

    void                    SendInit();

    void                    SendFan();
    void                    SendSave();
};
