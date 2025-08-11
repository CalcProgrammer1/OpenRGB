/*---------------------------------------------------------*\
| HyperXPulsefireRaidController.h                           |
|                                                           |
|   Driver for HyperX Pulsefire Raid                        |
|                                                           |
|   Morgan Guimard (morg)                       06 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define HYPERX_PULSFIRE_RAID_PACKET_DATA_LENGTH         264
#define HYPERX_PULSFIRE_RAID_REPORT_ID                  0x07
#define HYPERX_PULSFIRE_RAID_LEDS_COUNT                 2
#define HYPERX_PULSFIRE_RAID_DIRECT_MODE_START_PACKET   0x0A
#define HYPERX_PULSFIRE_RAID_DIRECT_MODE_END_PACKET     0xA0

enum
{
    HYPERX_PULSFIRE_RAID_BRIGHTNESS_MIN = 0x00,
    HYPERX_PULSFIRE_RAID_BRIGHTNESS_MAX = 0x64
};

class HyperXPulsefireRaidController
{
public:
    HyperXPulsefireRaidController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~HyperXPulsefireRaidController();

    std::string GetNameString();
    std::string GetSerialString();
    std::string GetDeviceLocation();

    void        SendColors(std::vector<RGBColor> colors);
    void        SetBrightness(unsigned char brightness);

protected:
    hid_device* dev;

private:
    std::string location;
    std::string name;

    void        Send(unsigned char* packet);
};
