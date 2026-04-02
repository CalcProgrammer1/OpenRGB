/*---------------------------------------------------------*\
| NollieDevices.cpp                                         |
|                                                           |
|   Device list for Nollie devices                          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                20 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "NollieDevices.h"

/*---------------------------------------------------------*\
| Channel Index Maps                                        |
\*---------------------------------------------------------*/
static const int dflt[32]   = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
static const int ch32[32]   = {5, 4, 3, 2, 1, 0, 15, 14, 26, 27, 28, 29, 30, 31, 8, 9, 19, 18, 17, 16, 7, 6, 25, 24, 23, 22, 21, 20, 13, 12, 11, 10};
static const int ch16[32]   = {19, 18, 17, 16, 24, 25, 26, 27, 20, 21, 22, 23, 31, 30, 29, 28, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
static const int n16[16]    = {3, 2, 1, 0, 8, 9, 10, 11, 4, 5, 6, 7, 15, 14, 13, 12};

/*---------------------------------------------------------*\
| Nollie Devices                                            |
\*---------------------------------------------------------*/
static const nollie_device nollie32 =
{
    NOLLIE32_VID,
    NOLLIE32_PID,
    32,
    NOLLIE_HS_CH_LED_NUM,
    ch32
};

static const nollie_device nollie16 =
{
    NOLLIE16_VID,
    NOLLIE16_PID,
    16,
    NOLLIE_HS_CH_LED_NUM,
    ch16
};

static const nollie_device nollie8 =
{
    NOLLIE8_VID,
    NOLLIE8_PID,
    8,
    126,
    dflt
};

static const nollie_device nollie1 =
{
    NOLLIE1_VID,
    NOLLIE1_PID,
    1,
    630,
    dflt
};

static const nollie_device nollie28_12 =
{
    NOLLIE28_12_VID,
    NOLLIE28_12_PID,
    1,
    42,
    dflt
};

static const nollie_device nollie28_l1 =
{
    NOLLIE28_12_VID,
    NOLLIE28_L1_PID,
    8,
    NOLLIE_FS_CH_LED_NUM,
    dflt
};

static const nollie_device nollie28_l2 =
{
    NOLLIE28_12_VID,
    NOLLIE28_L2_PID,
    8,
    NOLLIE_FS_CH_LED_NUM,
    dflt
};

static const nollie_device nollie32_os2 =
{
    NOLLIERGBOS_2_VID,
    NOLLIE32_PID,
    32,
    NOLLIE_HS_CH_LED_NUM,
    ch32
};

static const nollie_device nollie16_os2 =
{
    NOLLIERGBOS_2_VID,
    NOLLIE16_PID,
    16,
    NOLLIE_HS_CH_LED_NUM,
    n16
};

static const nollie_device nollie8_os2 =
{
    NOLLIERGBOS_2_VID,
    NOLLIE8_PID,
    8,
    126,
    dflt
};

static const nollie_device nollie1_os2 =
{
    NOLLIERGBOS_2_VID,
    NOLLIE1_PID,
    1,
    630,
    dflt
};

static const nollie_device nollie32_os2_1 =
{
    NOLLIERGBOS_2_VID,
    NOLLIE32_OS2_1_PID,
    32,
    NOLLIE_HS_CH_LED_NUM,
    ch32
};

static const nollie_device nollie16_os2_1 =
{
    NOLLIERGBOS_2_VID,
    NOLLIE16_OS2_1_PID,
    16,
    NOLLIE_HS_CH_LED_NUM,
    n16
};

static const nollie_device nollie8_os2_1 =
{
    NOLLIERGBOS_2_VID,
    NOLLIE8_OS2_1_PID,
    8,
    126,
    dflt
};

static const nollie_device prism8_os2_1 =
{
    NOLLIERGBOS_2_VID,
    PRISM8_OS2_1_PID,
    8,
    126,
    dflt
};

static const nollie_device nollie1_os2_1 =
{
    NOLLIERGBOS_2_VID,
    NOLLIE1_OS2_1_PID,
    1,
    630,
    dflt
};

/*---------------------------------------------------------*\
| Nollie Device List                                        |
\*---------------------------------------------------------*/
static const nollie_device* device_list[] =
{
    &nollie32,
    &nollie16,
    &nollie8,
    &nollie1,
    &nollie28_12,
    &nollie28_l1,
    &nollie28_l2,
    &nollie32_os2,
    &nollie16_os2,
    &nollie8_os2,
    &nollie1_os2,
    &nollie32_os2_1,
    &nollie16_os2_1,
    &nollie8_os2_1,
    &prism8_os2_1,
    &nollie1_os2_1,
};

const unsigned int      NOLLIE_NUM_DEVICES = (sizeof(device_list) / sizeof(device_list[ 0 ]));
const nollie_device**   nollie_device_list = device_list;
