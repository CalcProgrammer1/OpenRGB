/*---------------------------------------------------------*\
| HYTENexusController.h                                     |
|                                                           |
|   Driver for HYTE Nexus                                   |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>
#include "RGBController.h"
#include "serial_port.h"

#define HYTE_THICC_Q60_PID                          0x0400
#define HYTE_NEXUS_PORTAL_NP50_PID                  0x0901

typedef struct
{
    unsigned int        device_type;
    unsigned int        hardware_version;
    unsigned char       led_count;
} hyte_nexus_device;

typedef struct
{
    bool                is_nexus_channel;
    bool                has_4_led_logo;
    bool                has_6_led_logo;
    bool                has_lcd_leds;
    unsigned int        num_devices;
    hyte_nexus_device   devices[19];
} hyte_nexus_channel;

enum
{
    HYTE_NEXUS_DEVICE_TYPE_LS10         = 0x01,
    HYTE_NEXUS_DEVICE_TYPE_LS30         = 0x02,
    HYTE_NEXUS_DEVICE_TYPE_FP12         = 0x03,
    HYTE_NEXUS_DEVICE_TYPE_FP12_DUO     = 0x04,
    HYTE_NEXUS_DEVICE_TYPE_FP12_TRIO    = 0x05,
    HYTE_NEXUS_DEVICE_TYPE_LN4060       = 0x06,
    HYTE_NEXUS_DEVICE_TYPE_LN70         = 0x07,
};

class HYTENexusController
{
public:
    HYTENexusController(char* port, unsigned short pid, std::string dev_name);
    ~HYTENexusController();

    std::string GetFirmwareVersion();
    std::string GetLocation();
    std::string GetName();
    std::string GetDeviceName(unsigned int device_type);

    void LEDStreaming(unsigned char channel, unsigned short num_leds, RGBColor* colors);

    hyte_nexus_channel  channels[4];
    unsigned int        num_channels;
    unsigned short      device_pid;

private:
    std::string                                         firmware_version;
    std::string                                         name;
    std::string                                         port_name;
    std::mutex                                          port_mutex;
    serial_port *                                       serialport = nullptr;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
    std::atomic<bool>                                   keepalive_thread_run;
    std::thread                                         keepalive_thread;

    void KeepaliveThreadFunction();

    void ReadChannelInfo(unsigned char channel);
    void ReadDeviceInfo();
    void ReadFirmwareVersion();

    void SetStartupAnimation(bool enable);
};
