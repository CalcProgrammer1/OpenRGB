/*---------------------------------------------------------*\
| SPDCommon.h                                               |
|                                                           |
|   Common definitions for SPD                              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "i2c_smbus.h"

typedef enum
{
    JEDEC_KINGSTON    = 0x0117,
    JEDEC_CORSAIR     = 0x021D,
    JEDEC_ADATA       = 0x044A,
    JEDEC_GSKILL      = 0x044C,
    JEDEC_TEAMGROUP   = 0x046E,
    JEDEC_KINGSTON_2  = 0x300F,
    JEDEC_KINGSTON_3  = 0x3011,
    JEDEC_MUSHKIN     = 0x8313,
    JEDEC_GIGABYTE    = 0x8971,
    JEDEC_THERMALTAKE = 0x8A41,
    JEDEC_PATRIOT     = 0x8501
} JedecIdentifier;

typedef enum
{
    SPD_RESERVED      =  0,
    SPD_FPM_DRAM      =  1,
    SPD_EDO           =  2,
    SPD_NIBBLE        =  3,
    SPD_SDR_SDRAM     =  4,
    SPD_MUX_ROM       =  5,
    SPD_DDR_SGRAM     =  6,
    SPD_DDR_SDRAM     =  7,
    SPD_DDR2_SDRAM    =  8,
    SPD_FB_DIMM       =  9,
    SPD_FB_PROBE      = 10,
    SPD_DDR3_SDRAM    = 11,
    SPD_DDR4_SDRAM    = 12,
    SPD_RESERVED2     = 13,
    SPD_DDR4E_SDRAM   = 14,
    SPD_LPDDR3_SDRAM  = 15,
    SPD_LPDDR4_SDRAM  = 16,
    SPD_LPDDR4X_SDRAM = 17,
    SPD_DDR5_SDRAM    = 18,
    SPD_LPDDR5_SDRAM  = 19
} SPDMemoryType;

#define SPD_IO_DELAY   1ms

extern const char *spd_memory_type_name[];
