/*---------------------------------------------------------*\
| CorsairDRAMDevices.h                                      |
|                                                           |
|   Device list for Corsair DRAM RGB controllers            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                07 Apr 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>

/*---------------------------------------------------------*\
| Maximum number of PIDs for a given DRAM model             |
\*---------------------------------------------------------*/
#define CORSAIR_DRAM_MAX_PIDS                       6

/*---------------------------------------------------------*\
| Corsair DRAM vendor ID                                    |
\*---------------------------------------------------------*/
#define CORSAIR_DRAM_VID                            0x1B1C

/*---------------------------------------------------------*\
| Corsair DRAM product IDs                                  |
\*---------------------------------------------------------*/
#define CORSAIR_VENGEANCE_PRO_DDR4_PID_1            0x0100
#define CORSAIR_VENGEANCE_PRO_DDR4_PID_2            0x0101
#define CORSAIR_DOMINATOR_PLATINUM_DDR4_PID_1       0x0200
#define CORSAIR_DOMINATOR_PLATINUM_DDR4_PID_2       0x0201
#define CORSAIR_VENGEANCE_PRO_SL_DDR4_PID_1         0x0300
#define CORSAIR_VENGEANCE_PRO_SL_DDR4_PID_2         0x0301
#define CORSAIR_VENGEANCE_RS_DDR4_PID_1             0x0400
#define CORSAIR_VENGEANCE_RS_DDR4_PID_2             0x0401
#define CORSAIR_DOMINATOR_PLATINUM_DDR5_PID_1       0x0600
#define CORSAIR_DOMINATOR_PLATINUM_DDR5_PID_2       0x0601
#define CORSAIR_DOMINATOR_TITANIUM_DDR5_PID_1       0x0800
#define CORSAIR_DOMINATOR_TITANIUM_DDR5_PID_2       0x0801
#define CORSAIR_DOMINATOR_TITANIUM_DDR5_PID_3       0x0810
#define CORSAIR_DOMINATOR_TITANIUM_DDR5_PID_4       0x0811
#define CORSAIR_VENGEANCE_DDR5_PID_1                0x0700
#define CORSAIR_VENGEANCE_DDR5_PID_2                0x0701
#define CORSAIR_VENGEANCE_DDR5_PID_3                0x0900
#define CORSAIR_VENGEANCE_DDR5_PID_4                0x0901
#define CORSAIR_VENGEANCE_DDR5_PID_5                0x0910
#define CORSAIR_VENGEANCE_DDR5_PID_6                0x0911
#define CORSAIR_VENGEANCE_SHUGO_SERIES_DDR5_PID_1   0x0A00
#define CORSAIR_VENGEANCE_SHUGO_SERIES_DDR5_PID_2   0x0A01
#define CORSAIR_VENGEANCE_SHUGO_SERIES_DDR5_PID_3   0x0A10
#define CORSAIR_VENGEANCE_SHUGO_SERIES_DDR5_PID_4   0x0A11
#define CORSAIR_VENGEANCE_RS_DDR5_PID_1             0x0B00
#define CORSAIR_VENGEANCE_RS_DDR5_PID_2             0x0B01

typedef struct
{
    std::string                         name;
    unsigned short                      pids[CORSAIR_DRAM_MAX_PIDS];
    unsigned int                        led_count;
    bool                                reverse;
} corsair_dram_device;

/*-----------------------------------------------------*\
| These constant values are defined in RazerDevices.cpp |
\*-----------------------------------------------------*/
extern const unsigned int CORSAIR_DRAM_NUM_DEVICES;
extern const corsair_dram_device** corsair_dram_device_list;
