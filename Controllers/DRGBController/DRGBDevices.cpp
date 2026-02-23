/*---------------------------------------------------------*\
| DRGBDevices.cpp                                           |
|                                                           |
|   Device list for DRGB devices                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                16 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DRGBDevices.h"

/*---------------------------------------------------------*\
| DRGB Devices                                              |
\*---------------------------------------------------------*/
static const drgb_device drgb_led_v4 =
{
    DRGB_LED_V4_PID,
    8,
    512,
    4
};

static const drgb_device drgb_ultra_v4f =
{
    DRGB_ULTRA_V4F_PID,
    16,
    256,
    4
};

static const drgb_device drgb_core_v4f =
{
    DRGB_CORE_V4F_PID,
    32,
    256,
    4
};

static const drgb_device drgb_sig_v4f =
{
    DRGB_SIG_V4F_PID,
    36,
    256,
    4
};

static const drgb_device drgb_ag_04_v4f =
{
    DRGB_AG_04_V4F_PID,
    4,
    256,
    4
};

static const drgb_device drgb_ag_16_v4f =
{
    DRGB_AG_16_V4F_PID,
    16,
    256,
    4
};

static const drgb_device drgb_ag_08 =
{
    DRGB_AG_08_PID,
    8,
    256,
    4
};

static const drgb_device drgb_ag_08_f08 =
{
    DRGB_AG_08_F08_PID,
    8,
    256,
    4
};

static const drgb_device drgb_ag_16_f12 =
{
    DRGB_AG_16_F12_PID,
    16,
    256,
    4
};

static const drgb_device drgb_l8_v5 =
{
    DRGB_L8_V5_PID,
    8,
    512,
    4
};

static const drgb_device drgb_u16_v5 =
{
    DRGB_U16_V5_PID,
    16,
    256,
    4
};

static const drgb_device drgb_u16_v5f =
{
    DRGB_U16_V5F_PID,
    16,
    256,
    4
};

static const drgb_device drgb_c16_v5 =
{
    DRGB_C16_V5_PID,
    32,
    256,
    4
};

static const drgb_device drgb_c16_v5f =
{
    DRGB_C16_V5F_PID,
    32,
    256,
    4
};

static const drgb_device drgb_s16_v5f =
{
    DRGB_S16_V5F_PID,
    32,
    256,
    4
};

static const drgb_device drgb_led_v3 =
{
    DRGB_LED_V3_PID,
    8,
    256,
    3
};

static const drgb_device drgb_ultra_v3 =
{
    DRGB_ULTRA_V3_PID,
    16,
    256,
    3
};

static const drgb_device drgb_core_v3 =
{
    DRGB_CORE_V3_PID,
    30,
    256,
    3
};

static const drgb_device drgb_e8_f =
{
    DRGB_E8_F_PID,
    8,
    132,
    1
};

static const drgb_device drgb_e8 =
{
    DRGB_E8_PID,
    8,
    132,
    1
};

static const drgb_device drgb_e16 =
{
    DRGB_E16_PID,
    16,
    132,
    1
};

static const drgb_device dm_10 =
{
    DM_10_PID,
    10,
    132,
    1
};

static const drgb_device jpu_12 =
{
    JPU_12_PID,
    12,
    60,
    1
};

static const drgb_device drgb_led =
{
    DRGB_LED_PID,
    8,
    256,
    2
};

static const drgb_device drgb_ultra =
{
    DRGB_ULTRA_PID,
    16,
    256,
    2
};

static const drgb_device drgb_sig_ab =
{
    DRGB_SIG_AB_PID,
    16,
    256,
    2
};

static const drgb_device drgb_sig_cd =
{
    DRGB_SIG_CD_PID,
    6,
    256,
    2
};

static const drgb_device drgb_strimer =
{
    DRGB_STRIMER_PID,
    6,
    256,
    2
};

static const drgb_device yico_8 =
{
    YICO_8_PID,
    8,
    256,
    3
};

static const drgb_device yico_08 =
{
    YICO_08_PID,
    8,
    256,
    3
};

static const drgb_device yico_08_1 =
{
    YICO_08_1_PID,
    8,
    132,
    3
};

static const drgb_device yico_14 =
{
    YICO_14_PID,
    14,
    132,
    1
};

static const drgb_device yico_16 =
{
    YICO_16_PID,
    16,
    256,
    4
};

/*---------------------------------------------------------*\
| DRGB Device List                                          |
\*---------------------------------------------------------*/
static const drgb_device* device_list[] =
{
    &drgb_led_v4,
    &drgb_ultra_v4f,
    &drgb_core_v4f,
    &drgb_sig_v4f,
    &drgb_ag_04_v4f,
    &drgb_ag_16_v4f,
    &drgb_ag_08,
    &drgb_ag_08_f08,
    &drgb_ag_16_f12,
    &drgb_l8_v5,
    &drgb_u16_v5,
    &drgb_u16_v5f,
    &drgb_c16_v5,
    &drgb_c16_v5f,
    &drgb_s16_v5f,
    &drgb_led_v3,
    &drgb_ultra_v3,
    &drgb_core_v3,
    &drgb_e8_f,
    &drgb_e8,
    &drgb_e16,
    &dm_10,
    &jpu_12,
    &drgb_led,
    &drgb_ultra,
    &drgb_sig_ab,
    &drgb_sig_cd,
    &drgb_strimer,
    &yico_8,
    &yico_08,
    &yico_08_1,
    &yico_14,
    &yico_16
};

const unsigned int  DRGB_NUM_DEVICES = (sizeof(device_list) / sizeof(device_list[ 0 ]));
const drgb_device** drgb_device_list = device_list;
