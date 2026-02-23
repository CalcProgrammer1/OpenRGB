/*---------------------------------------------------------*\
| NollieDevices.h                                           |
|                                                           |
|   Device list for Nollie devices                          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                20 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "NollieController.h"

/*---------------------------------------------------------*\
| Nollie vendor IDs                                         |
\*---------------------------------------------------------*/
#define NOLLIERGBOS_2_VID          0x16D5
#define NOLLIE32_VID               0x3061
#define NOLLIE16_VID               0x3061
#define NOLLIE8_VID                0x16D2
#define NOLLIE1_VID                0x16D2
#define NOLLIE28_12_VID            0x16D2

/*---------------------------------------------------------*\
| Nollie product IDs                                        |
\*---------------------------------------------------------*/
#define NOLLIE32_PID               0x4714
#define NOLLIE16_PID               0x4716
#define NOLLIE8_PID                0x1F01
#define NOLLIE1_PID                0x1F11
#define NOLLIE28_12_PID            0x1616
#define NOLLIE28_L1_PID            0x1617
#define NOLLIE28_L2_PID            0x1618

/*---------------------------------------------------------*\
| Common Nollie LED counts                                  |
\*---------------------------------------------------------*/
#define NOLLIE_HS_CH_LED_NUM       256
#define NOLLIE_FS_CH_LED_NUM       525

typedef struct
{
    unsigned short              vid;
    unsigned short              pid;
    unsigned short              channels;
    unsigned short              leds_per_channel;
    const int*                  channel_index;
} nollie_device;

/*---------------------------------------------------------*\
| These constant values are defined in NollieDevices.cpp    |
\*---------------------------------------------------------*/
extern const unsigned int       NOLLIE_NUM_DEVICES;
extern const nollie_device**    nollie_device_list;
