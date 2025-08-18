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
#include <hidapi.h>
#include "RGBController.h"

#define NOLLIE_12_CH_LED_NUM       42
#define NOLLIE_8_CH_LED_NUM        126
#define NOLLIE_1_CH_LED_NUM        630
#define NOLLIE_HS_CH_LED_NUM       256
#define NOLLIE_FS_CH_LED_NUM       525

#define NOLLIERGBOS_2_VID          0x16D5

#define NOLLIE32_CHANNELS_NUM      32
#define NOLLIE32_PID               0x4714
#define NOLLIE32_VID               0x3061

#define NOLLIE16_CHANNELS_NUM      16
#define NOLLIE16_PID               0x4716
#define NOLLIE16_VID               0x3061

#define NOLLIE8_CHANNELS_NUM       8
#define NOLLIE8_PID                0x1F01
#define NOLLIE8_VID                0x16D2

#define NOLLIE1_CHANNELS_NUM       1
#define NOLLIE1_PID                0x1F11
#define NOLLIE1_VID                0x16D2

#define NOLLIE28_12_CHANNELS_NUM   12
#define NOLLIE28_12_VID            0x16D2
#define NOLLIE28_12_PID            0x1616
#define NOLLIE28_L1_PID            0x1617
#define NOLLIE28_L2_PID            0x1618

#define NOLLIE32_MOS_TRIGGER_CH    26
#define NOLLIE32_MOS_TRIGGER_LED   20
#define NOLLIE32_FLAG1_CHANNEL     15
#define NOLLIE32_FLAG2_CHANNEL     31

class NollieController
{
public:
    NollieController(hid_device* dev_handle, const char* path, unsigned short vid, unsigned short pid, std::string dev_name);

    std::string     GetLocationString();
    std::string     GetNameString();
    std::string     GetSerialString();
    unsigned short  GetUSBVID();
    unsigned short  GetUSBPID();

    void            SetMos(bool mos);
    void            InitChLEDs(int *led_num_list,int ch_num);
    void            SendUpdate();
    void            SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors);

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;
    unsigned short  usb_vid;
    unsigned short  usb_pid;

    void            SendPacket(unsigned char   channel,RGBColor * colors,unsigned int num_colors);
    void            SendPacketFS(unsigned char   channel,unsigned char packet_id,RGBColor * colors,unsigned int num_colors);
};
