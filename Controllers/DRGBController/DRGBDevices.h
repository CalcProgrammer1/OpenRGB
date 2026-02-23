/*---------------------------------------------------------*\
| DRGBDevices.h                                             |
|                                                           |
|   Device list for DRGB devices                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                16 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "DRGBController.h"

/*---------------------------------------------------------*\
| DRGB vendor IDs                                           |
\*---------------------------------------------------------*/
#define DRGBV2_VID              0x2023
#define DRGBV3_VID              0x2023
#define DRGBV4_VID              0x2486
#define YICO_VID                0x1368

/*---------------------------------------------------------*\
| DRGB product IDs                                          |
\*---------------------------------------------------------*/
#define DRGB_LED_V4_PID         0x3608
#define DRGB_ULTRA_V4F_PID      0x3616
#define DRGB_CORE_V4F_PID       0x3628
#define DRGB_SIG_V4F_PID        0x3636
#define DRGB_AG_04_V4F_PID      0x3204
#define DRGB_AG_16_V4F_PID      0x3216
#define DRGB_AG_08_PID          0x3F08
#define DRGB_AG_08_F08_PID      0x3F16
#define DRGB_AG_16_F12_PID      0x3F28

#define DRGB_L8_V5_PID          0x3208
#define DRGB_U16_V5_PID         0x3215
#define DRGB_U16_V5F_PID        0x3217
#define DRGB_C16_V5_PID         0x3228
#define DRGB_C16_V5F_PID        0x3229
#define DRGB_S16_V5F_PID        0x3232

#define DRGB_LED_V3_PID         0x1209
#define DRGB_ULTRA_V3_PID       0x1221
#define DRGB_CORE_V3_PID        0x1226
#define DRGB_E8_F_PID           0x1408
#define DRGB_E8_PID             0x1407
#define DRGB_E16_PID            0x1416
#define DM_10_PID               0x1410
#define JPU_12_PID              0x1412

#define DRGB_LED_PID            0x1208
#define DRGB_ULTRA_PID          0x1220
#define DRGB_SIG_AB_PID         0x1210
#define DRGB_SIG_CD_PID         0x1211
#define DRGB_STRIMER_PID        0x1215

#define YICO_8_PID              0x6077
#define YICO_08_PID             0x6078
#define YICO_08_1_PID           0x6079
#define YICO_14_PID             0x1614
#define YICO_16_PID             0x1616

typedef struct
{
    unsigned short              pid;
    unsigned char               channels;
    unsigned short              leds_per_channel;
    unsigned short              version;
} drgb_device;

/*---------------------------------------------------------*\
| These constant values are defined in DRGBDevices.cpp      |
\*---------------------------------------------------------*/
extern const unsigned int       DRGB_NUM_DEVICES;
extern const drgb_device**      drgb_device_list;
