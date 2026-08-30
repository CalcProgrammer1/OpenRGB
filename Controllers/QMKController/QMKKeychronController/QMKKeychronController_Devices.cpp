/*---------------------------------------------------------*\
| QMKKechronController_Devices.cpp                          |
|                                                           |
|   Keychron device matrix correction data                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "QMKKeychronController_Devices.h"

#define KEYCHRON_PATCH_ENTRIES_COUNT(entries) (sizeof(entries) / sizeof(keychron_patch_entry))

/*---------------------------------------------------------*\
| Keychron Patch Types                                      |
|   The Keychron protocol does not provide RGB matrix X/Y   |
|   coordinates, only the position in the wiring matrix.    |
|   For most Keychron keyboards, the wiring matrix mostly   |
|   matches the physical position, but there are often a    |
|   few misplaced LEDs.  Implement a matrix patching system |
|   to apply keyboard-specific patches for these misplaced  |
|   LEDs.                                                   |
\*---------------------------------------------------------*/

/*---------------------------------------------------------*\
| Geometry Group 1:                                         |
| 0x0522 C2 Pro 8K                                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_001_patch_entries[] =
{
    {  35,  1, 19 },
    {  36,  1, 20 },
    {  56,  2, 19 },
    {  57,  2, 20 },
    {  70,  3, 12 },
    {  73,  3, 19 },
    {  85,  4, 12 },
    {  89,  4, 19 },
    {  90,  4, 20 },
    { 103,  5, 19 },
};

static const keychron_patch keychron_c2_pro_8k_patch =
{
    KEYCHRON_C2_PRO_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_001_patch_entries),
    keychron_group_001_patch_entries
};

/*---------------------------------------------------------*\
| Geometry Group 2:                                         |
| 0x0532 C3 Pro 8K JIS                                      |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_002_patch_entries[] =
{
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  29,  1, 13 },
    {  30,  1, 14 },
    {  31,  1, 15 },
    {  32,  1, 16 },
    {  33,  1, 17 },
    {  48,  2, 15 },
    {  49,  2, 16 },
    {  50,  2, 17 },
    {  63,  3, 12 },
    {  77,  4, 16 },
    {  88,  5, 15 },
    {  89,  5, 16 },
    {  90,  5, 17 },
};

static const keychron_patch keychron_c3_pro_8k_jis_patch =
{
    KEYCHRON_C3_PRO_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_002_patch_entries),
    keychron_group_002_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 3:                                         |
| 0x0A11 K1 Max ISO                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_003_patch_entries[] =
{
    {  62,  3, 12 },
    {  75,  4, 12 },
};

static const keychron_patch keychron_k1_max_iso_patch =
{
    KEYCHRON_K1_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_003_patch_entries),
    keychron_group_003_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 4:                                         |
| 0x0A12 K1 Max JIS                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_004_patch_entries[] =
{
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  33,  1, 17 },
    {  48,  2, 15 },
    {  49,  2, 16 },
    {  50,  2, 17 },
    {  63,  3, 12 },
    {  77,  4, 16 },
    {  88,  5, 15 },
    {  89,  5, 16 },
    {  90,  5, 17 },
};

static const keychron_patch keychron_k1_max_jis_patch =
{
    KEYCHRON_K1_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_004_patch_entries),
    keychron_group_004_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 5:                                         |
| 0x0A10 K1 Max                                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_005_patch_entries[] =
{
    {  62,  3, 12 },
    {  74,  4, 12 },
};

static const keychron_patch keychron_k1_max_patch =
{
    KEYCHRON_K1_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_005_patch_entries),
    keychron_group_005_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 6:                                         |
| 0x0D12 K1 Version 6 JIS                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_006_patch_entries[] =
{
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  30,  1, 14 },
    {  31,  1, 15 },
    {  32,  1, 16 },
    {  33,  1, 17 },
    {  48,  2, 15 },
    {  49,  2, 16 },
    {  50,  2, 17 },
    {  63,  3, 12 },
    {  77,  4, 16 },
    {  88,  5, 15 },
    {  89,  5, 16 },
    {  90,  5, 17 },
};

static const keychron_patch keychron_k1_v6_jis_patch =
{
    KEYCHRON_K1_V6_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_006_patch_entries),
    keychron_group_006_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 9:                                         |
| 0x0AA1 K10 Max ISO                                        |
| 0x16A1 K10 Ultra 8K ISO                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_009_patch_entries[] =
{
    {  19,  0, 20 },
    {  40,  1, 20 },
    {  61,  2, 20 },
    {  90,  4, 12 },
    {  95,  4, 20 },
    { 107,  5, 17 },
};

static const keychron_patch keychron_k10_max_iso_patch =
{
    KEYCHRON_K10_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_009_patch_entries),
    keychron_group_009_patch_entries
};

static const keychron_patch keychron_k10_ultra_8k_iso_16a1_patch =
{
    KEYCHRON_K10_ULTRA_8K_ISO_16A1_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_009_patch_entries),
    keychron_group_009_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 10:                                        |
| 0x0AA2 K10 Max JIS                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_010_patch_entries[] =
{
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  16,  0, 18 },
    {  17,  0, 19 },
    {  18,  0, 20 },
    {  19,  0, 21 },
    {  33,  1, 13 },
    {  34,  1, 14 },
    {  35,  1, 15 },
    {  36,  1, 16 },
    {  37,  1, 17 },
    {  38,  1, 18 },
    {  39,  1, 19 },
    {  40,  1, 20 },
    {  41,  1, 21 },
    {  56,  2, 15 },
    {  57,  2, 16 },
    {  58,  2, 17 },
    {  59,  2, 18 },
    {  60,  2, 19 },
    {  61,  2, 20 },
    {  62,  2, 21 },
    {  76,  3, 18 },
    {  77,  3, 19 },
    {  78,  3, 20 },
    {  92,  4, 16 },
    {  93,  4, 18 },
    {  94,  4, 19 },
    {  95,  4, 20 },
    {  96,  4, 21 },
    { 107,  5, 15 },
    { 108,  5, 16 },
    { 109,  5, 17 },
    { 111,  5, 20 },
};

static const keychron_patch keychron_k10_max_jis_patch =
{
    KEYCHRON_K10_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_010_patch_entries),
    keychron_group_010_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 11:                                        |
| 0x0AA0 K10 Max                                            |
| 0x16A0 K10 Ultra 8K                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_011_patch_entries[] =
{
    {  19,  0, 20 },
    {  40,  1, 20 },
    {  61,  2, 20 },
    {  89,  4, 12 },
    {  94,  4, 20 },
    { 106,  5, 17 },
};

static const keychron_patch keychron_k10_max_patch =
{
    KEYCHRON_K10_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_011_patch_entries),
    keychron_group_011_patch_entries
};

static const keychron_patch keychron_k10_ultra_8k_patch =
{
    KEYCHRON_K10_ULTRA_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_011_patch_entries),
    keychron_group_011_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 12:                                        |
| 0x16A3 K10 Ultra 8K ISO                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_012_patch_entries[] =
{
    {  36,  1, 20 },
    {  57,  2, 20 },
    {  86,  4, 12 },
    {  91,  4, 20 },
    { 103,  5, 17 },
};

static const keychron_patch keychron_k10_ultra_8k_iso_16a3_patch =
{
    KEYCHRON_K10_ULTRA_8K_ISO_16A3_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_012_patch_entries),
    keychron_group_012_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 13:                                        |
| 0x0DA1 K10 Version 2 ISO                                  |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_013_patch_entries[] =
{
    {  70,  3, 12 },
    {  86,  4, 12 },
    { 104,  5, 19 },
};

static const keychron_patch keychron_k10_v2_iso_patch =
{
    KEYCHRON_K10_V2_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_013_patch_entries),
    keychron_group_013_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 14:                                        |
| 0x0DA2 K10 Version 2 JIS                                  |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_014_patch_entries[] =
{
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  30,  1, 14 },
    {  31,  1, 15 },
    {  32,  1, 16 },
    {  33,  1, 17 },
    {  34,  1, 18 },
    {  35,  1, 19 },
    {  36,  1, 20 },
    {  37,  1, 21 },
    {  52,  2, 15 },
    {  53,  2, 16 },
    {  54,  2, 17 },
    {  55,  2, 18 },
    {  56,  2, 19 },
    {  57,  2, 20 },
    {  58,  2, 21 },
    {  71,  3, 12 },
    {  72,  3, 18 },
    {  73,  3, 19 },
    {  74,  3, 20 },
    {  88,  4, 16 },
    {  89,  4, 18 },
    {  90,  4, 19 },
    {  91,  4, 20 },
    {  92,  4, 21 },
    { 103,  5, 15 },
    { 104,  5, 16 },
    { 105,  5, 17 },
    { 106,  5, 18 },
    { 107,  5, 20 },
};

static const keychron_patch keychron_k10_v2_jis_patch =
{
    KEYCHRON_K10_V2_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_014_patch_entries),
    keychron_group_014_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 15:                                        |
| 0x0DA0 K10X                                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_015_patch_entries[] =
{
    {  70,  3, 12 },
    {  85,  4, 12 },
    { 103,  5, 19 },
};

static const keychron_patch keychron_k10x_patch =
{
    KEYCHRON_K10X_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_015_patch_entries),
    keychron_group_015_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 16:                                        |
| 0x0AB4 K11 Max ISO Knob                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_016_patch_entries[] =
{
    {  17,  2, 12 },
    {  22,  3, 12 },
    {  23,  3, 13 },
    {  24,  3, 14 },
    {  46,  4,  4 },
    {  67,  4,  8 },
    {  69,  4, 10 },
};

static const keychron_patch keychron_k11_max_iso_knob_patch =
{
    KEYCHRON_K11_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_016_patch_entries),
    keychron_group_016_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 17:                                        |
| 0x0AB5 K11 Max JIS Knob                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_017_patch_entries[] =
{
    {   7,  0, 16 },
    {  13,  1, 16 },
    {  16,  2, 11 },
    {  17,  2, 12 },
    {  19,  2, 16 },
    {  22,  3, 13 },
    {  23,  3, 14 },
    {  24,  3, 15 },
    {  25,  3, 16 },
    {  30,  4, 15 },
    {  31,  4, 16 },
    {  32,  4, 17 },
    {  35,  1,  5 },
    {  36,  1,  6 },
    {  40,  2,  5 },
    {  44,  3,  6 },
    {  48,  3,  7 },
    {  51,  4,  5 },
    {  52,  1,  7 },
    {  56,  2,  6 },
    {  57,  2,  7 },
    {  61,  3,  8 },
    {  62,  2,  8 },
    {  63,  2,  9 },
    {  64,  2, 10 },
    {  65,  4,  7 },
    {  66,  3,  9 },
    {  67,  3, 10 },
    {  68,  3, 11 },
    {  69,  3, 12 },
    {  70,  4,  8 },
    {  72,  4, 10 },
};

static const keychron_patch keychron_k11_max_jis_knob_patch =
{
    KEYCHRON_K11_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_017_patch_entries),
    keychron_group_017_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 18:                                        |
| 0x0AB3 K11 Max Knob                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_018_patch_entries[] =
{
    {  42,  2, 12 },
    {  55,  3, 12 },
    {  56,  3, 13 },
    {  60,  4,  4 },
    {  63,  4,  8 },
    {  65,  4, 10 },
    {  66,  4, 14 },
    {  67,  4, 15 },
    {  68,  4, 16 },
};

static const keychron_patch keychron_k11_max_knob_patch =
{
    KEYCHRON_K11_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_018_patch_entries),
    keychron_group_018_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 19:                                        |
| 0x0AD1 K13 Max ISO                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_019_patch_entries[] =
{
    {  62,  3, 12 },
    {  78,  4, 12 },
};

static const keychron_patch keychron_k13_max_iso_patch =
{
    KEYCHRON_K13_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_019_patch_entries),
    keychron_group_019_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 20:                                        |
| 0x0AD2 K13 Max JIS                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_020_patch_entries[] =
{
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  33,  1, 17 },
    {  48,  2, 15 },
    {  49,  2, 16 },
    {  50,  2, 17 },
    {  63,  3, 12 },
    {  64,  3, 15 },
    {  65,  3, 16 },
    {  66,  3, 17 },
    {  80,  4, 16 },
    {  91,  5, 15 },
    {  92,  5, 16 },
    {  93,  5, 17 },
};

static const keychron_patch keychron_k13_max_jis_patch =
{
    KEYCHRON_K13_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_020_patch_entries),
    keychron_group_020_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 21:                                        |
| 0x0AD0 K13 Max                                            |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_021_patch_entries[] =
{
    {  62,  3, 12 },
    {  77,  4, 12 },
};

static const keychron_patch keychron_k13_max_patch =
{
    KEYCHRON_K13_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_021_patch_entries),
    keychron_group_021_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 22:                                        |
| 0x0AF1 K15 Max ISO Knob                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_022_patch_entries[] =
{
    {  15,  1, 16 },
    {  22,  2, 16 },
    {  29,  3, 16 },
    {  40,  5, 14 },
    {  41,  5, 15 },
    {  42,  5, 16 },
    {  63,  5,  4 },
    {  89,  5, 11 },
    {  90,  5, 12 },
};

static const keychron_patch keychron_k15_max_iso_knob_patch =
{
    KEYCHRON_K15_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_022_patch_entries),
    keychron_group_022_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 23:                                        |
| 0x0AF2 K15 Max JIS Knob                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_023_patch_entries[] =
{
    {   7,  0, 16 },
    {  16,  1, 17 },
    {  23,  2, 17 },
    {  30,  3, 17 },
    {  33,  4,  2 },
    {  37,  4, 16 },
    {  42,  5, 15 },
    {  43,  5, 16 },
    {  44,  5, 17 },
    {  61,  4,  3 },
    {  62,  4,  4 },
    {  63,  4,  5 },
    {  91,  5, 11 },
    {  92,  5, 12 },
};

static const keychron_patch keychron_k15_max_jis_knob_patch =
{
    KEYCHRON_K15_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_023_patch_entries),
    keychron_group_023_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 24:                                        |
| 0x0AF0 K15 Max Knob                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_024_patch_entries[] =
{
    {  15,  1, 16 },
    {  22,  2, 16 },
    {  29,  3, 16 },
    {  32,  4,  2 },
    {  39,  5, 14 },
    {  40,  5, 15 },
    {  41,  5, 16 },
    {  57,  3,  6 },
    {  58,  4,  3 },
    {  59,  4,  4 },
    {  60,  4,  5 },
    {  61,  4,  6 },
    {  63,  5,  4 },
    {  64,  5,  6 },
    {  87,  5,  8 },
    {  89,  5,  7 },
};

static const keychron_patch keychron_k15_max_knob_patch =
{
    KEYCHRON_K15_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_024_patch_entries),
    keychron_group_024_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 25:                                        |
| 0x0A01 K17 Max ISO Knob                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_025_patch_entries[] =
{
    {  33,  1, 13 },
    {  52,  2, 13 },
    {  70,  3, 12 },
};

static const keychron_patch keychron_k17_max_iso_knob_patch =
{
    KEYCHRON_K17_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_025_patch_entries),
    keychron_group_025_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 26:                                        |
| 0x0A02 K17 Max JIS Knob                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_026_patch_entries[] =
{
    {  53,  2, 13 },
    {  71,  3, 12 },
    {  88,  4, 13 },
    { 103,  5, 14 },
    { 104,  5, 15 },
    { 105,  5, 16 },
    { 106,  5, 17 },
};

static const keychron_patch keychron_k17_max_jis_knob_patch =
{
    KEYCHRON_K17_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_026_patch_entries),
    keychron_group_026_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 27:                                        |
| 0x0A00 K17 Max Knob                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_027_patch_entries[] =
{
    {  33,  1, 13 },
    {  52,  2, 13 },
    {  70,  3, 12 },
};

static const keychron_patch keychron_k17_max_knob_patch =
{
    KEYCHRON_K17_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_027_patch_entries),
    keychron_group_027_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 28:                                        |
| 0x0E20 K2 HE ANSI                                         |
| 0x0E30 K3 HE                                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_028_patch_entries[] =
{
    {  30,  1, 15 },
    {  45,  2, 15 },
    {  59,  3, 15 },
    {  72,  4, 14 },
    {  73,  4, 15 },
    {  81,  5, 13 },
    {  82,  5, 14 },
    {  83,  5, 15 },
};

static const keychron_patch keychron_k2_he_ansi_patch =
{
    KEYCHRON_K2_HE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_028_patch_entries),
    keychron_group_028_patch_entries
};

static const keychron_patch keychron_k3_he_patch =
{
    KEYCHRON_K3_HE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_028_patch_entries),
    keychron_group_028_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 29:                                        |
| 0x0E21 K2 HE ISO                                          |
| 0x0E31 K3 HE ISO                                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_029_patch_entries[] =
{
    {  30,  1, 15 },
    {  45,  2, 15 },
    {  59,  3, 15 },
    {  73,  4, 14 },
    {  74,  4, 15 },
    {  79,  5, 10 },
    {  80,  5, 11 },
    {  81,  5, 12 },
    {  82,  5, 13 },
    {  83,  5, 14 },
    {  84,  5, 15 },
};

static const keychron_patch keychron_k2_he_iso_patch =
{
    KEYCHRON_K2_HE_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_029_patch_entries),
    keychron_group_029_patch_entries
};

static const keychron_patch keychron_k3_he_iso_patch =
{
    KEYCHRON_K3_HE_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_029_patch_entries),
    keychron_group_029_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 30:                                        |
| 0x0E22 K2 HE JIS                                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_030_patch_entries[] =
{
    {  46,  2, 15 },
    {  60,  3, 15 },
    {  74,  4, 15 },
    {  80,  5,  9 },
    {  81,  5, 10 },
    {  82,  5, 11 },
    {  83,  5, 12 },
    {  84,  5, 13 },
    {  85,  5, 14 },
    {  86,  5, 15 },
};

static const keychron_patch keychron_k2_he_jis_patch =
{
    KEYCHRON_K2_HE_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_030_patch_entries),
    keychron_group_030_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 31:                                        |
| 0x0A21 K2 Max ISO                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_031_patch_entries[] =
{
    {  58,  3, 12 },
    {  73,  4, 14 },
    {  79,  5, 10 },
    {  80,  5, 11 },
    {  81,  5, 12 },
    {  82,  5, 13 },
    {  83,  5, 14 },
};

static const keychron_patch keychron_k2_max_iso_patch =
{
    KEYCHRON_K2_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_031_patch_entries),
    keychron_group_031_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 32:                                        |
| 0x0A22 K2 Max JIS                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_032_patch_entries[] =
{
    {  59,  3, 12 },
};

static const keychron_patch keychron_k2_max_jis_patch =
{
    KEYCHRON_K2_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_032_patch_entries),
    keychron_group_032_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 33:                                        |
| 0x0A20 K2 Max                                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_033_patch_entries[] =
{
    {  58,  3, 12 },
    {  72,  4, 14 },
    {  78,  5, 10 },
    {  79,  5, 11 },
    {  80,  5, 12 },
    {  81,  5, 13 },
    {  82,  5, 14 },
};

static const keychron_patch keychron_k2_max_patch =
{
    KEYCHRON_K2_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_033_patch_entries),
    keychron_group_033_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 34:                                        |
| 0x0A31 K3 Max ISO                                         |
| 0x0D21 K2 Version 3 ISO                                   |
| 0x0D31 K3 Version 3 ISO                                   |
| 0x1631 K3 Ultra 8K ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_034_patch_entries[] =
{
    {  58,  3, 12 },
    {  72,  4, 12 },
};

static const keychron_patch keychron_k3_max_iso_patch =
{
    KEYCHRON_K3_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_034_patch_entries),
    keychron_group_034_patch_entries
};

static const keychron_patch keychron_k2_v3_iso_patch =
{
    KEYCHRON_K2_V3_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_034_patch_entries),
    keychron_group_034_patch_entries
};

static const keychron_patch keychron_k3_v3_iso_patch =
{
    KEYCHRON_K3_V3_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_034_patch_entries),
    keychron_group_034_patch_entries
};

static const keychron_patch keychron_k3_ultra_8k_iso_patch =
{
    KEYCHRON_K3_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_034_patch_entries),
    keychron_group_034_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 35:                                        |
| 0x0A32 K3 Max JIS                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_035_patch_entries[] =
{
    {  59,  3, 12 },
    {  72,  4, 12 },
    {  73,  4, 13 },
};

static const keychron_patch keychron_k3_max_jis_patch =
{
    KEYCHRON_K3_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_035_patch_entries),
    keychron_group_035_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 36:                                        |
| 0x0A30 K3 Max                                             |
| 0x0D20 K2X                                                |
| 0x0D30 K3X                                                |
| 0x0D3B K3 Max SE                                          |
| 0x1630 K3 Ultra 8K ANSI                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_036_patch_entries[] =
{
    {  58,  3, 12 },
    {  71,  4, 12 },
};

static const keychron_patch keychron_k3_max_patch =
{
    KEYCHRON_K3_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_036_patch_entries),
    keychron_group_036_patch_entries
};

static const keychron_patch keychron_k2x_patch =
{
    KEYCHRON_K2X_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_036_patch_entries),
    keychron_group_036_patch_entries
};

static const keychron_patch keychron_k3x_patch =
{
    KEYCHRON_K3X_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_036_patch_entries),
    keychron_group_036_patch_entries
};

static const keychron_patch keychron_k3_max_se_patch =
{
    KEYCHRON_K3_MAX_SE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_036_patch_entries),
    keychron_group_036_patch_entries
};

static const keychron_patch keychron_k3_ultra_8k_ansi_patch =
{
    KEYCHRON_K3_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_036_patch_entries),
    keychron_group_036_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 37:                                        |
| 0x0E40 K4 HE                                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_037_patch_entries[] =
{
    {  32,  1, 13 },
    {  67,  3, 12 },
    {  82,  4, 12 },
};

static const keychron_patch keychron_k4_he_patch =
{
    KEYCHRON_K4_HE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_037_patch_entries),
    keychron_group_037_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 38:                                        |
| 0x0E41 K4 HE ISO                                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_038_patch_entries[] =
{
    {  32,  1, 13 },
    {  67,  3, 12 },
    {  83,  4, 12 },
};

static const keychron_patch keychron_k4_he_iso_patch =
{
    KEYCHRON_K4_HE_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_038_patch_entries),
    keychron_group_038_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 39:                                        |
| 0x0E42 K4 HE JIS                                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_039_patch_entries[] =
{
    {  68,  3, 12 },
    {  83,  4, 12 },
    {  84,  4, 13 },
};

static const keychron_patch keychron_k4_he_jis_patch =
{
    KEYCHRON_K4_HE_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_039_patch_entries),
    keychron_group_039_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 40:                                        |
| 0x0A40 K4 Max Ansi                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_040_patch_entries[] =
{
    {  18,  0, 18 },
    {  33,  1, 15 },
    {  34,  1, 16 },
    {  35,  1, 17 },
    {  36,  1, 18 },
    {  51,  2, 15 },
    {  52,  2, 16 },
    {  53,  2, 17 },
    {  54,  2, 18 },
    {  67,  3, 12 },
    {  68,  3, 15 },
    {  69,  3, 16 },
    {  70,  3, 17 },
    {  83,  4, 14 },
    {  84,  4, 15 },
    {  85,  4, 16 },
    {  86,  4, 17 },
    {  87,  4, 18 },
};

static const keychron_patch keychron_k4_max_ansi_patch =
{
    KEYCHRON_K4_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_040_patch_entries),
    keychron_group_040_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 41:                                        |
| 0x0D42 K4X JIS                                            |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_041_patch_entries[] =
{
    {  68,  3, 12 },
    {  94,  5,  9 },
    {  95,  5, 10 },
    {  96,  5, 11 },
    {  97,  5, 12 },
    {  98,  5, 13 },
    {  99,  5, 14 },
};

static const keychron_patch keychron_k4x_jis_patch =
{
    KEYCHRON_K4X_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_041_patch_entries),
    keychron_group_041_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 42:                                        |
| 0x0A51 K5 Max ISO                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_042_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  74,  3, 12 },
    {  90,  4, 12 },
};

static const keychron_patch keychron_k5_max_iso_patch =
{
    KEYCHRON_K5_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_042_patch_entries),
    keychron_group_042_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 43:                                        |
| 0x0A52 K5 Max JIS                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_043_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  7 },
    {   6,  0,  8 },
    {   7,  0,  9 },
    {   8,  0, 10 },
    {   9,  0, 11 },
    {  10,  0, 12 },
    {  11,  0, 13 },
    {  12,  0, 14 },
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  16,  0, 18 },
    {  17,  0, 19 },
    {  18,  0, 20 },
    {  19,  0, 21 },
    {  34,  1, 14 },
    {  35,  1, 15 },
    {  36,  1, 16 },
    {  37,  1, 17 },
    {  38,  1, 18 },
    {  39,  1, 19 },
    {  40,  1, 20 },
    {  41,  1, 21 },
    {  56,  2, 15 },
    {  57,  2, 16 },
    {  58,  2, 17 },
    {  59,  2, 18 },
    {  60,  2, 19 },
    {  61,  2, 20 },
    {  62,  2, 21 },
    {  75,  3, 12 },
    {  76,  3, 18 },
    {  77,  3, 19 },
    {  78,  3, 20 },
    {  92,  4, 16 },
    {  93,  4, 18 },
    {  94,  4, 19 },
    {  95,  4, 20 },
    {  96,  4, 21 },
    { 102,  5, 10 },
    { 103,  5, 11 },
    { 104,  5, 12 },
    { 105,  5, 13 },
    { 106,  5, 14 },
    { 107,  5, 15 },
    { 108,  5, 16 },
    { 109,  5, 17 },
    { 110,  5, 18 },
    { 111,  5, 20 },
};

static const keychron_patch keychron_k5_max_jis_0a52_patch =
{
    KEYCHRON_K5_MAX_JIS_0A52_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_043_patch_entries),
    keychron_group_043_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 44:                                        |
| 0x0A50 K5 Max                                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_044_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  74,  3, 12 },
    {  89,  4, 12 },
};

static const keychron_patch keychron_k5_max_patch =
{
    KEYCHRON_K5_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_044_patch_entries),
    keychron_group_044_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 45:                                        |
| 0x0D51 K5X ISO                                            |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_045_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  18,  0, 19 },
    {  19,  0, 20 },
    {  39,  1, 19 },
    {  40,  1, 20 },
    {  60,  2, 19 },
    {  61,  2, 20 },
    {  74,  3, 12 },
    {  77,  3, 19 },
    {  90,  4, 12 },
    {  94,  4, 19 },
    {  95,  4, 20 },
    { 108,  5, 19 },
};

static const keychron_patch keychron_k5x_iso_patch =
{
    KEYCHRON_K5X_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_045_patch_entries),
    keychron_group_045_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 46:                                        |
| 0x0E60 K6 HE                                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_046_patch_entries[] =
{
    {  42,  2, 12 },
};

static const keychron_patch keychron_k6_he_patch =
{
    KEYCHRON_K6_HE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_046_patch_entries),
    keychron_group_046_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 47:                                        |
| 0x0A71 K7 Max ISO                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_047_patch_entries[] =
{
    {  42,  2, 12 },
    {  56,  3, 12 },
};

static const keychron_patch keychron_k7_max_iso_patch =
{
    KEYCHRON_K7_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_047_patch_entries),
    keychron_group_047_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 48:                                        |
| 0x0A72 K7 Max JIS                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_048_patch_entries[] =
{
    {  43,  2, 12 },
};

static const keychron_patch keychron_k7_max_jis_patch =
{
    KEYCHRON_K7_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_048_patch_entries),
    keychron_group_048_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 49:                                        |
| 0x0A76 K7 Max JIS V2                                      |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_049_patch_entries[] =
{
    {  43,  2, 12 },
};

static const keychron_patch keychron_k7_max_jis_v2_patch =
{
    KEYCHRON_K7_MAX_JIS_V2_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_049_patch_entries),
    keychron_group_049_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 50:                                        |
| 0x0A70 K7 Max                                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_050_patch_entries[] =
{
    {  42,  2, 12 },
    {  55,  3, 12 },
};

static const keychron_patch keychron_k7_max_patch =
{
    KEYCHRON_K7_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_050_patch_entries),
    keychron_group_050_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 51:                                        |
| 0x0E80 K8 HE                                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_051_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  15,  0, 16 },
    {  32,  1, 16 },
    {  49,  2, 16 },
    {  62,  3, 12 },
    {  74,  4, 12 },
    {  80,  5, 10 },
    {  81,  5, 11 },
    {  82,  5, 12 },
    {  83,  5, 13 },
    {  84,  5, 14 },
    {  85,  5, 15 },
    {  86,  5, 16 },
};

static const keychron_patch keychron_k8_he_patch =
{
    KEYCHRON_K8_HE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_051_patch_entries),
    keychron_group_051_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 52:                                        |
| 0x0E81 K8 HE ISO                                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_052_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  15,  0, 16 },
    {  32,  1, 16 },
    {  49,  2, 16 },
    {  62,  3, 12 },
    {  75,  4, 12 },
    {  81,  5, 10 },
    {  82,  5, 11 },
    {  83,  5, 12 },
    {  84,  5, 13 },
    {  85,  5, 14 },
    {  86,  5, 15 },
    {  87,  5, 16 },
};

static const keychron_patch keychron_k8_he_iso_patch =
{
    KEYCHRON_K8_HE_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_052_patch_entries),
    keychron_group_052_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 53:                                        |
| 0x0E82 K8 HE JIS RBG                                      |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_053_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  7 },
    {   6,  0,  8 },
    {   7,  0,  9 },
    {   8,  0, 10 },
    {   9,  0, 11 },
    {  10,  0, 12 },
    {  11,  0, 13 },
    {  12,  0, 14 },
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  29,  1, 13 },
    {  30,  1, 14 },
    {  31,  1, 15 },
    {  32,  1, 16 },
    {  33,  1, 17 },
    {  48,  2, 15 },
    {  49,  2, 16 },
    {  50,  2, 17 },
    {  63,  3, 12 },
    {  77,  4, 16 },
    {  83,  5,  9 },
    {  84,  5, 10 },
    {  85,  5, 11 },
    {  86,  5, 12 },
    {  87,  5, 13 },
    {  88,  5, 15 },
    {  89,  5, 16 },
    {  90,  5, 17 },
};

static const keychron_patch keychron_k8_he_jis_rbg_patch =
{
    KEYCHRON_K8_HE_JIS_RBG_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_053_patch_entries),
    keychron_group_053_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 54:                                        |
| 0x0A82 K8 Max JIS                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_054_patch_entries[] =
{
    {   9,  0, 11 },
    {  10,  0, 12 },
    {  11,  0, 13 },
    {  12,  0, 14 },
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  29,  1, 13 },
    {  30,  1, 14 },
    {  31,  1, 15 },
    {  32,  1, 16 },
    {  33,  1, 17 },
    {  48,  2, 15 },
    {  49,  2, 16 },
    {  50,  2, 17 },
    {  63,  3, 12 },
    {  77,  4, 16 },
    {  83,  5, 10 },
    {  84,  5, 11 },
    {  85,  5, 12 },
    {  86,  5, 13 },
    {  87,  5, 14 },
    {  88,  5, 15 },
    {  89,  5, 16 },
    {  90,  5, 17 },
};

static const keychron_patch keychron_k8_max_jis_patch =
{
    KEYCHRON_K8_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_054_patch_entries),
    keychron_group_054_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 55:                                        |
| 0x0A90 K9 Max                                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_055_patch_entries[] =
{
    {  40,  2, 12 },
    {  52,  3, 12 },
};

static const keychron_patch keychron_k9_max_patch =
{
    KEYCHRON_K9_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_055_patch_entries),
    keychron_group_055_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 56:                                        |
| 0x1500 Keychron C0 HE 8K                                  |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_056_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {  34,  6,  5 },
};

static const keychron_patch keychron_c0_he_8k_patch =
{
    KEYCHRON_C0_HE_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_056_patch_entries),
    keychron_group_056_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 57:                                        |
| 0x051D C1 Pro 8K ISO                                      |
| 0x0531 C3 Pro Iso 8K                                      |
| 0x0A81 K8 Max ISO                                         |
| 0x0D11 K1X ISO                                            |
| 0x0D81 K8 Version 2 ISO                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_057_patch_entries[] =
{
    {  62,  3, 12 },
    {  75,  4, 12 },
};

static const keychron_patch keychron_c1_pro_8k_iso_patch =
{
    KEYCHRON_C1_PRO_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_057_patch_entries),
    keychron_group_057_patch_entries
};

static const keychron_patch keychron_c3_pro_iso_8k_patch =
{
    KEYCHRON_C3_PRO_ISO_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_057_patch_entries),
    keychron_group_057_patch_entries
};

static const keychron_patch keychron_k8_max_iso_patch =
{
    KEYCHRON_K8_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_057_patch_entries),
    keychron_group_057_patch_entries
};

static const keychron_patch keychron_k1x_iso_patch =
{
    KEYCHRON_K1X_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_057_patch_entries),
    keychron_group_057_patch_entries
};

static const keychron_patch keychron_k8_v2_iso_patch =
{
    KEYCHRON_K8_V2_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_057_patch_entries),
    keychron_group_057_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 58:                                        |
| 0x051E C1 Pro 8K JIS                                      |
| 0x0D82 K8 Version 2 JIS                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_058_patch_entries[] =
{
    {   5,  0,  7 },
    {   6,  0,  8 },
    {   7,  0,  9 },
    {   8,  0, 10 },
    {   9,  0, 11 },
    {  10,  0, 12 },
    {  11,  0, 13 },
    {  12,  0, 14 },
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  30,  1, 14 },
    {  31,  1, 15 },
    {  32,  1, 16 },
    {  33,  1, 17 },
    {  48,  2, 15 },
    {  49,  2, 16 },
    {  50,  2, 17 },
    {  63,  3, 12 },
    {  77,  4, 16 },
    {  88,  5, 15 },
    {  89,  5, 16 },
    {  90,  5, 17 },
};

static const keychron_patch keychron_c1_pro_8k_jis_patch =
{
    KEYCHRON_C1_PRO_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_058_patch_entries),
    keychron_group_058_patch_entries
};

static const keychron_patch keychron_k8_v2_jis_patch =
{
    KEYCHRON_K8_V2_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_058_patch_entries),
    keychron_group_058_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 59:                                        |
| 0x052D C2 Pro ISO 8K                                      |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_059_patch_entries[] =
{
    {  35,  1, 19 },
    {  36,  1, 20 },
    {  56,  2, 19 },
    {  57,  2, 20 },
    {  70,  3, 12 },
    {  73,  3, 19 },
    {  86,  4, 12 },
    {  90,  4, 19 },
    {  91,  4, 20 },
    { 104,  5, 19 },
};

static const keychron_patch keychron_c2_pro_iso_8k_patch =
{
    KEYCHRON_C2_PRO_ISO_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_059_patch_entries),
    keychron_group_059_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 60:                                        |
| 0x0405 Keychron J1                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_060_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  28,  1, 13 },
    {  43,  2, 13 },
    {  76,  5,  9 },
    {  77,  5, 10 },
    {  78,  5, 11 },
};

static const keychron_patch keychron_j1_patch =
{
    KEYCHRON_J1_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_060_patch_entries),
    keychron_group_060_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 61:                                        |
| 0x0406 J2                                                 |
| 0x0456 J2 ANSI Knob                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_061_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  28,  1, 13 },
    {  43,  2, 13 },
    {  77,  5,  9 },
    {  78,  5, 10 },
    {  79,  5, 11 },
};

static const keychron_patch keychron_j2_patch =
{
    KEYCHRON_J2_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_061_patch_entries),
    keychron_group_061_patch_entries
};

static const keychron_patch keychron_j2_ansi_knob_patch =
{
    KEYCHRON_J2_ANSI_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_061_patch_entries),
    keychron_group_061_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 62:                                        |
| 0x0416 J2 HE 8K ANSI                                      |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_062_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {   6,  0,  7 },
    {  22,  1,  7 },
    {  23,  1,  8 },
    {  24,  1,  9 },
    {  25,  1, 10 },
    {  26,  1, 11 },
    {  27,  1, 12 },
    {  28,  1, 13 },
    {  37,  2,  7 },
    {  38,  2,  8 },
    {  39,  2,  9 },
    {  40,  2, 10 },
    {  41,  2, 11 },
    {  42,  2, 12 },
    {  43,  2, 13 },
    {  52,  3,  7 },
    {  53,  3,  8 },
    {  54,  3,  9 },
    {  55,  3, 10 },
    {  56,  3, 11 },
    {  57,  3, 12 },
    {  65,  4,  7 },
    {  66,  4,  8 },
    {  67,  4,  9 },
    {  68,  4, 10 },
    {  69,  4, 11 },
    {  70,  4, 12 },
};

static const keychron_patch keychron_j2_he_8k_ansi_patch =
{
    KEYCHRON_J2_HE_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_062_patch_entries),
    keychron_group_062_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 63:                                        |
| 0x042D J3 SE Knob                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_063_patch_entries[] =
{
    {  16,  0, 18 },
    {  35,  1, 18 },
    {  54,  2, 18 },
    {  67,  3, 12 },
    {  87,  4, 18 },
    {  99,  5, 17 },
};

static const keychron_patch keychron_j3_se_knob_patch =
{
    KEYCHRON_J3_SE_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_063_patch_entries),
    keychron_group_063_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 64:                                        |
| 0x0408 J4                                                 |
| 0x0457 J4 ANSI                                            |
| 0x0521 C1 Pro 8K                                          |
| 0x0530 C3 Pro 8K                                          |
| 0x0A80 K8 Max                                             |
| 0x0D10 K1X US                                             |
| 0x0D80 K8 Version 2                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_064_patch_entries[] =
{
    {  62,  3, 12 },
    {  74,  4, 12 },
};

static const keychron_patch keychron_j4_patch =
{
    KEYCHRON_J4_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_064_patch_entries),
    keychron_group_064_patch_entries
};

static const keychron_patch keychron_j4_ansi_patch =
{
    KEYCHRON_J4_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_064_patch_entries),
    keychron_group_064_patch_entries
};

static const keychron_patch keychron_c1_pro_8k_patch =
{
    KEYCHRON_C1_PRO_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_064_patch_entries),
    keychron_group_064_patch_entries
};

static const keychron_patch keychron_c3_pro_8k_patch =
{
    KEYCHRON_C3_PRO_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_064_patch_entries),
    keychron_group_064_patch_entries
};

static const keychron_patch keychron_k8_max_patch =
{
    KEYCHRON_K8_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_064_patch_entries),
    keychron_group_064_patch_entries
};

static const keychron_patch keychron_k1x_us_patch =
{
    KEYCHRON_K1X_US_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_064_patch_entries),
    keychron_group_064_patch_entries
};

static const keychron_patch keychron_k8_v2_patch =
{
    KEYCHRON_K8_V2_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_064_patch_entries),
    keychron_group_064_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 65:                                        |
| 0x0409 J5                                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_065_patch_entries[] =
{
    {  17,  0, 18 },
    {  18,  0, 19 },
    {  19,  0, 20 },
    {  38,  1, 18 },
    {  39,  1, 19 },
    {  40,  1, 20 },
    {  59,  2, 18 },
    {  60,  2, 19 },
    {  61,  2, 20 },
    {  76,  3, 18 },
    {  77,  3, 19 },
    {  92,  4, 18 },
    {  93,  4, 19 },
    {  94,  4, 20 },
    { 107,  5, 19 },
};

static const keychron_patch keychron_j5_patch =
{
    KEYCHRON_J5_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_065_patch_entries),
    keychron_group_065_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 66:                                        |
| 0x0453 J5 ANSI Knob                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_066_patch_entries[] =
{
    {  74,  3, 12 },
    {  89,  4, 12 },
};

static const keychron_patch keychron_j5_ansi_knob_patch =
{
    KEYCHRON_J5_ANSI_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_066_patch_entries),
    keychron_group_066_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 67:                                        |
| 0x0A06 K0 Max                                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_067_patch_entries[] =
{
    {  23,  4,  4 },
};

static const keychron_patch keychron_k0_max_patch =
{
    KEYCHRON_K0_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_067_patch_entries),
    keychron_group_067_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 68:                                        |
| 0x1620 K2 Ultra 8K ANSI                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_068_patch_entries[] =
{
    {  58,  3, 12 },
};

static const keychron_patch keychron_k2_ultra_8k_ansi_patch =
{
    KEYCHRON_K2_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_068_patch_entries),
    keychron_group_068_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 69:                                        |
| 0x1621 K2 Ultra 8K ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_069_patch_entries[] =
{
    {  58,  3, 12 },
};

static const keychron_patch keychron_k2_ultra_8k_iso_patch =
{
    KEYCHRON_K2_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_069_patch_entries),
    keychron_group_069_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 70:                                        |
| 0x0D22 K2 Version 3 JIS                                   |
| 0x0D32 K3 Version 3 JIS                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_070_patch_entries[] =
{
    {  59,  3, 12 },
    {  72,  4, 12 },
    {  73,  4, 13 },
};

static const keychron_patch keychron_k2_v3_jis_patch =
{
    KEYCHRON_K2_V3_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_070_patch_entries),
    keychron_group_070_patch_entries
};

static const keychron_patch keychron_k3_v3_jis_patch =
{
    KEYCHRON_K3_V3_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_070_patch_entries),
    keychron_group_070_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 71:                                        |
| 0x0A41 K4 Max ISO                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_071_patch_entries[] =
{
    {  18,  0, 18 },
    {  33,  1, 15 },
    {  34,  1, 16 },
    {  35,  1, 17 },
    {  36,  1, 18 },
    {  51,  2, 15 },
    {  52,  2, 16 },
    {  53,  2, 17 },
    {  54,  2, 18 },
    {  67,  3, 12 },
    {  68,  3, 15 },
    {  69,  3, 16 },
    {  70,  3, 17 },
    {  84,  4, 14 },
    {  85,  4, 15 },
    {  86,  4, 16 },
    {  87,  4, 17 },
    {  88,  4, 18 },
};

static const keychron_patch keychron_k4_max_iso_patch =
{
    KEYCHRON_K4_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_071_patch_entries),
    keychron_group_071_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 72:                                        |
| 0x0A42 K4 Max JIS                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_072_patch_entries[] =
{
    {  18,  0, 18 },
    {  32,  1, 13 },
    {  33,  1, 14 },
    {  34,  1, 15 },
    {  35,  1, 16 },
    {  36,  1, 17 },
    {  37,  1, 18 },
    {  52,  2, 15 },
    {  53,  2, 16 },
    {  54,  2, 17 },
    {  55,  2, 18 },
    {  68,  3, 12 },
    {  69,  3, 15 },
    {  70,  3, 16 },
    {  71,  3, 17 },
    {  84,  4, 13 },
    {  85,  4, 14 },
    {  86,  4, 15 },
    {  87,  4, 16 },
    {  88,  4, 17 },
    {  89,  4, 18 },
};

static const keychron_patch keychron_k4_max_jis_patch =
{
    KEYCHRON_K4_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_072_patch_entries),
    keychron_group_072_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 73:                                        |
| 0x1640 K4 Ultra 8K                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_073_patch_entries[] =
{
    {  32,  1, 13 },
    {  54,  2, 18 },
    {  87,  4, 18 },
    {  92,  5, 10 },
    {  93,  5, 11 },
    {  94,  5, 12 },
    {  95,  5, 13 },
    {  96,  5, 14 },
    {  97,  5, 15 },
};

static const keychron_patch keychron_k4_ultra_8k_patch =
{
    KEYCHRON_K4_ULTRA_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_073_patch_entries),
    keychron_group_073_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 74:                                        |
| 0x0D41 K4 Version 3 ISO                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_074_patch_entries[] =
{
    {  67,  3, 12 },
    {  72,  4,  2 },
    {  73,  4,  3 },
    {  74,  4,  4 },
    {  75,  4,  5 },
    {  76,  4,  6 },
    {  77,  4,  7 },
    {  78,  4,  8 },
    {  79,  4,  9 },
    {  80,  4, 10 },
    {  81,  4, 11 },
    {  82,  4, 12 },
    {  83,  4, 13 },
    {  84,  4, 15 },
    {  85,  4, 16 },
    {  86,  4, 17 },
    {  87,  4, 18 },
    {  88,  4, 19 },
    {  93,  5, 10 },
    {  94,  5, 11 },
    {  95,  5, 12 },
    {  96,  5, 13 },
    {  97,  5, 14 },
};

static const keychron_patch keychron_k4_v3_iso_patch =
{
    KEYCHRON_K4_V3_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_074_patch_entries),
    keychron_group_074_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 75:                                        |
| 0x0D40 K4 Version 3                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_075_patch_entries[] =
{
    {  67,  3, 12 },
    {  83,  4, 14 },
    {  92,  5, 10 },
    {  93,  5, 11 },
    {  94,  5, 12 },
    {  95,  5, 13 },
    {  96,  5, 14 },
};

static const keychron_patch keychron_k4_v3_patch =
{
    KEYCHRON_K4_V3_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_075_patch_entries),
    keychron_group_075_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 76:                                        |
| 0x0A58 K5 Max JIS                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_076_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  7 },
    {   6,  0,  8 },
    {   7,  0,  9 },
    {   8,  0, 10 },
    {   9,  0, 11 },
    {  10,  0, 12 },
    {  11,  0, 13 },
    {  12,  0, 14 },
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  16,  0, 18 },
    {  17,  0, 19 },
    {  18,  0, 20 },
    {  19,  0, 21 },
    {  34,  1, 14 },
    {  35,  1, 15 },
    {  36,  1, 16 },
    {  37,  1, 17 },
    {  38,  1, 18 },
    {  39,  1, 19 },
    {  40,  1, 20 },
    {  41,  1, 21 },
    {  56,  2, 15 },
    {  57,  2, 16 },
    {  58,  2, 17 },
    {  59,  2, 18 },
    {  60,  2, 19 },
    {  61,  2, 20 },
    {  62,  2, 21 },
    {  75,  3, 12 },
    {  76,  3, 18 },
    {  77,  3, 19 },
    {  78,  3, 20 },
    {  92,  4, 16 },
    {  93,  4, 18 },
    {  94,  4, 19 },
    {  95,  4, 20 },
    {  96,  4, 21 },
    { 102,  5, 10 },
    { 103,  5, 11 },
    { 104,  5, 12 },
    { 105,  5, 13 },
    { 106,  5, 14 },
    { 107,  5, 15 },
    { 108,  5, 16 },
    { 109,  5, 17 },
    { 110,  5, 18 },
    { 111,  5, 20 },
};

static const keychron_patch keychron_k5_max_jis_0a58_patch =
{
    KEYCHRON_K5_MAX_JIS_0A58_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_076_patch_entries),
    keychron_group_076_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 77:                                        |
| 0x1650 K5 Ultra 8K Knob ANSI                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_077_patch_entries[] =
{
    {  20,  0, 20 },
    {  41,  1, 20 },
    {  62,  2, 20 },
    {  75,  3, 12 },
    {  90,  4, 12 },
    {  95,  4, 20 },
};

static const keychron_patch keychron_k5_ultra_8k_knob_ansi_patch =
{
    KEYCHRON_K5_ULTRA_8K_KNOB_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_077_patch_entries),
    keychron_group_077_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 78:                                        |
| 0x1651 K5 Ultra 8K Knob ISO                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_078_patch_entries[] =
{
    {  20,  0, 20 },
    {  41,  1, 20 },
    {  62,  2, 20 },
    {  75,  3, 12 },
    {  91,  4, 12 },
    {  96,  4, 20 },
};

static const keychron_patch keychron_k5_ultra_8k_knob_iso_patch =
{
    KEYCHRON_K5_ULTRA_8K_KNOB_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_078_patch_entries),
    keychron_group_078_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 79:                                        |
| 0x0D50 K5 Version 2                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_079_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  18,  0, 19 },
    {  19,  0, 20 },
    {  39,  1, 19 },
    {  40,  1, 20 },
    {  60,  2, 19 },
    {  61,  2, 20 },
    {  74,  3, 12 },
    {  77,  3, 19 },
    {  89,  4, 12 },
    {  93,  4, 19 },
    {  94,  4, 20 },
    { 107,  5, 19 },
};

static const keychron_patch keychron_k5_v2_patch =
{
    KEYCHRON_K5_V2_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_079_patch_entries),
    keychron_group_079_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 80:                                        |
| 0x0D52 K5 Version 2 JIS                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_080_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  7 },
    {   6,  0,  8 },
    {   7,  0,  9 },
    {   8,  0, 10 },
    {   9,  0, 11 },
    {  10,  0, 12 },
    {  11,  0, 13 },
    {  12,  0, 14 },
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  16,  0, 18 },
    {  17,  0, 19 },
    {  18,  0, 20 },
    {  19,  0, 21 },
    {  34,  1, 14 },
    {  35,  1, 15 },
    {  36,  1, 16 },
    {  37,  1, 17 },
    {  38,  1, 18 },
    {  39,  1, 19 },
    {  40,  1, 20 },
    {  41,  1, 21 },
    {  56,  2, 15 },
    {  57,  2, 16 },
    {  58,  2, 17 },
    {  59,  2, 18 },
    {  60,  2, 19 },
    {  61,  2, 20 },
    {  62,  2, 21 },
    {  75,  3, 12 },
    {  76,  3, 18 },
    {  77,  3, 19 },
    {  78,  3, 20 },
    {  92,  4, 16 },
    {  93,  4, 18 },
    {  94,  4, 19 },
    {  95,  4, 20 },
    {  96,  4, 21 },
    { 102,  5, 10 },
    { 103,  5, 11 },
    { 104,  5, 12 },
    { 105,  5, 13 },
    { 106,  5, 14 },
    { 107,  5, 15 },
    { 108,  5, 16 },
    { 109,  5, 17 },
    { 110,  5, 18 },
    { 111,  5, 20 },
};

static const keychron_patch keychron_k5_v2_jis_patch =
{
    KEYCHRON_K5_V2_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_080_patch_entries),
    keychron_group_080_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 81:                                        |
| 0x0E61 K6 HE ISO                                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_081_patch_entries[] =
{
    {  14,  0, 15 },
    {  29,  1, 15 },
    {  42,  2, 12 },
    {  43,  2, 15 },
    {  57,  3, 14 },
    {  58,  3, 15 },
    {  63,  4, 10 },
    {  64,  4, 11 },
    {  65,  4, 12 },
    {  66,  4, 13 },
    {  67,  4, 14 },
    {  68,  4, 15 },
};

static const keychron_patch keychron_k6_he_iso_patch =
{
    KEYCHRON_K6_HE_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_081_patch_entries),
    keychron_group_081_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 82:                                        |
| 0x1680 K8 Ultra 8K ANSI                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_082_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  62,  3, 12 },
    {  74,  4, 12 },
};

static const keychron_patch keychron_k8_ultra_8k_ansi_patch =
{
    KEYCHRON_K8_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_082_patch_entries),
    keychron_group_082_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 83:                                        |
| 0x1681 K8 Ultra 8K ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_083_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  75,  4, 12 },
};

static const keychron_patch keychron_k8_ultra_8k_iso_patch =
{
    KEYCHRON_K8_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_083_patch_entries),
    keychron_group_083_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 84:                                        |
| 0x0B14 Q1 HE                                              |
| 0x0B15 Q1 HE ANSI V2 SILENT                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_084_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  29,  1, 15 },
    {  44,  2, 15 },
    {  58,  3, 15 },
    {  70,  4, 12 },
    {  76,  5, 10 },
    {  77,  5, 11 },
    {  78,  5, 12 },
    {  79,  5, 13 },
    {  80,  5, 14 },
    {  81,  5, 15 },
};

static const keychron_patch keychron_q1_he_patch =
{
    KEYCHRON_Q1_HE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_084_patch_entries),
    keychron_group_084_patch_entries
};

static const keychron_patch keychron_q1_he_ansi_v2_silent_patch =
{
    KEYCHRON_Q1_HE_ANSI_V2_SILENT_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_084_patch_entries),
    keychron_group_084_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 85:                                        |
| 0x1010 Q1 HE 8K ANSI                                      |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_085_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  29,  1, 15 },
    {  31,  2,  2 },
    {  32,  2,  3 },
    {  33,  2,  4 },
    {  34,  2,  5 },
    {  35,  2,  6 },
    {  36,  2,  7 },
    {  37,  2,  8 },
    {  38,  2,  9 },
    {  39,  2, 10 },
    {  40,  2, 11 },
    {  41,  2, 12 },
    {  42,  2, 13 },
    {  43,  2, 14 },
    {  44,  2, 15 },
    {  46,  3,  2 },
    {  47,  3,  3 },
    {  48,  3,  4 },
    {  49,  3,  5 },
    {  50,  3,  6 },
    {  51,  3,  7 },
    {  52,  3,  8 },
    {  53,  3,  9 },
    {  54,  3, 10 },
    {  55,  3, 11 },
    {  56,  3, 12 },
    {  57,  3, 13 },
    {  58,  3, 15 },
    {  70,  4, 12 },
    {  76,  5, 10 },
    {  77,  5, 11 },
    {  78,  5, 12 },
    {  79,  5, 13 },
    {  80,  5, 14 },
    {  81,  5, 15 },
};

static const keychron_patch keychron_q1_he_8k_ansi_patch =
{
    KEYCHRON_Q1_HE_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_085_patch_entries),
    keychron_group_085_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 86:                                        |
| 0x1011 Q1 HE 8K ISO Knob                                  |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_086_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  29,  1, 15 },
    {  44,  2, 15 },
    {  58,  3, 15 },
    {  71,  4, 12 },
    {  77,  5, 10 },
    {  78,  5, 11 },
    {  79,  5, 12 },
    {  80,  5, 13 },
    {  81,  5, 14 },
    {  82,  5, 15 },
};

static const keychron_patch keychron_q1_he_8k_iso_knob_patch =
{
    KEYCHRON_Q1_HE_8K_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_086_patch_entries),
    keychron_group_086_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 87:                                        |
| 0x12B0 Q11 Ultra 8K Knob                                  |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_087_patch_entries[] =
{
    {  40,  2,  7 },
    {  41,  2,  8 },
    {  42,  2,  9 },
    {  43,  2, 10 },
    {  44,  2, 11 },
    {  45,  2, 12 },
    {  46,  2, 13 },
    {  47,  2, 14 },
    {  51,  3,  3 },
    {  52,  3,  4 },
    {  53,  3,  5 },
    {  54,  3,  6 },
    {  55,  3,  7 },
};

static const keychron_patch keychron_q11_ultra_8k_knob_patch =
{
    KEYCHRON_Q11_ULTRA_8K_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_087_patch_entries),
    keychron_group_087_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 88:                                        |
| 0x12B2 Q11 Ultra 8K Knob ISO                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_088_patch_entries[] =
{
    {  40,  2,  7 },
    {  41,  2,  8 },
    {  42,  2,  9 },
    {  43,  2, 10 },
    {  44,  2, 11 },
    {  45,  2, 12 },
    {  46,  2, 13 },
    {  47,  2, 14 },
    {  51,  3,  3 },
    {  52,  3,  4 },
    {  53,  3,  5 },
    {  54,  3,  6 },
    {  55,  3,  7 },
    {  83,  5,  4 },
};

static const keychron_patch keychron_q11_ultra_8k_knob_iso_patch =
{
    KEYCHRON_Q11_ULTRA_8K_KNOB_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_088_patch_entries),
    keychron_group_088_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 89:                                        |
| 0x0BC1 Q12 HE ISO Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_089_patch_entries[] =
{
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  15,  0, 16 },
    {  16,  0, 17 },
    {  17,  0, 18 },
    {  18,  0, 19 },
    {  37,  1, 19 },
    {  56,  2, 19 },
    {  73,  3, 19 },
    {  89,  4, 15 },
    {  90,  4, 16 },
    {  92,  5,  0 },
    {  97,  5,  9 },
    {  98,  5, 14 },
    {  99,  5, 15 },
    { 100,  5, 16 },
    { 101,  5, 17 },
    { 102,  5, 18 },
    { 103,  5, 19 },
};

static const keychron_patch keychron_q12_he_iso_knob_patch =
{
    KEYCHRON_Q12_HE_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_089_patch_entries),
    keychron_group_089_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 90:                                        |
| 0x0BC0 Q12 HE Knob                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_090_patch_entries[] =
{
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  15,  0, 16 },
    {  16,  0, 17 },
    {  17,  0, 18 },
    {  18,  0, 19 },
    {  37,  1, 19 },
    {  56,  2, 19 },
    {  61,  3,  6 },
    {  62,  3,  7 },
    {  63,  3,  8 },
    {  64,  3,  9 },
    {  65,  3, 10 },
    {  66,  3, 11 },
    {  67,  3, 12 },
    {  68,  3, 13 },
    {  69,  3, 14 },
    {  70,  3, 15 },
    {  71,  3, 16 },
    {  72,  3, 17 },
    {  73,  3, 19 },
    {  88,  4, 15 },
    {  89,  4, 16 },
    {  91,  5,  0 },
    {  97,  5, 14 },
    {  98,  5, 15 },
    {  99,  5, 16 },
    { 100,  5, 17 },
    { 101,  5, 18 },
    { 102,  5, 19 },
};

static const keychron_patch keychron_q12_he_knob_patch =
{
    KEYCHRON_Q12_HE_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_090_patch_entries),
    keychron_group_090_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 91:                                        |
| 0x08D2 Q13 Max JIS Knob                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_091_patch_entries[] =
{
    {  43,  4,  2 },
    {  82,  5,  5 },
    { 107,  5,  8 },
    { 108,  5, 10 },
};

static const keychron_patch keychron_q13_max_jis_knob_patch =
{
    KEYCHRON_Q13_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_091_patch_entries),
    keychron_group_091_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 92:                                        |
| 0x12D0 Q13 Ultra 8K Knob                                  |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_092_patch_entries[] =
{
    {   4,  0, 13 },
    {   5,  0, 14 },
    {  43,  4,  2 },
    {  75,  4,  3 },
    {  76,  4,  4 },
    {  77,  4,  5 },
    {  78,  4,  6 },
    {  79,  5,  3 },
    {  80,  5,  4 },
    {  81,  5,  6 },
    {  82,  0,  9 },
    {  83,  0, 10 },
    {  84,  0, 11 },
    {  85,  0, 12 },
    { 104,  5,  8 },
};

static const keychron_patch keychron_q13_ultra_8k_knob_patch =
{
    KEYCHRON_Q13_ULTRA_8K_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_092_patch_entries),
    keychron_group_092_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 95:                                        |
| 0x1020 Q2 HE 8K                                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_095_patch_entries[] =
{
    {  55,  3, 12 },
    {  56,  3, 13 },
};

static const keychron_patch keychron_q2_he_8k_patch =
{
    KEYCHRON_Q2_HE_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_095_patch_entries),
    keychron_group_095_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 96:                                        |
| 0x1030 Q3 HE 8K                                           |
| 0x1033 Q3 HE 8K ANSI                                      |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_096_patch_entries[] =
{
    {  75,  4, 12 },
};

static const keychron_patch keychron_q3_he_8k_patch =
{
    KEYCHRON_Q3_HE_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_096_patch_entries),
    keychron_group_096_patch_entries
};

static const keychron_patch keychron_q3_he_8k_ansi_patch =
{
    KEYCHRON_Q3_HE_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_096_patch_entries),
    keychron_group_096_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 97:                                        |
| 0x1032 Q3 HE 8K JIS                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_097_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
};

static const keychron_patch keychron_q3_he_8k_jis_patch =
{
    KEYCHRON_Q3_HE_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_097_patch_entries),
    keychron_group_097_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 98:                                        |
| 0x0B32 Q3 HE JIS Knob                                     |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_098_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  15,  0, 16 },
    {  16,  0, 17 },
    {  31,  1, 14 },
    {  32,  1, 15 },
    {  33,  1, 16 },
    {  34,  1, 17 },
    {  49,  2, 15 },
    {  50,  2, 16 },
    {  51,  2, 17 },
    {  64,  3, 12 },
    {  78,  4, 16 },
    {  84,  5,  9 },
    {  85,  5, 10 },
    {  86,  5, 11 },
    {  87,  5, 12 },
    {  88,  5, 13 },
    {  89,  5, 15 },
    {  90,  5, 16 },
    {  91,  5, 17 },
};

static const keychron_patch keychron_q3_he_jis_knob_patch =
{
    KEYCHRON_Q3_HE_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_098_patch_entries),
    keychron_group_098_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 99:                                        |
| 0x1231 Q3 Ultra 8K Knob ISO                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_099_patch_entries[] =
{
    {  63,  3, 12 },
    {  76,  4, 12 },
};

static const keychron_patch keychron_q3_ultra_8k_knob_iso_patch =
{
    KEYCHRON_Q3_ULTRA_8K_KNOB_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_099_patch_entries),
    keychron_group_099_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 100:                                       |
| 0x1050 Q5 HE 8K                                           |
| 0x1053 Q5 HE 8K                                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_100_patch_entries[] =
{
    {  84,  4, 12 },
    {  85,  4, 13 },
    { 100,  5, 15 },
};

static const keychron_patch keychron_q5_he_8k_1050_patch =
{
    KEYCHRON_Q5_HE_8K_1050_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_100_patch_entries),
    keychron_group_100_patch_entries
};

static const keychron_patch keychron_q5_he_8k_1053_patch =
{
    KEYCHRON_Q5_HE_8K_1053_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_100_patch_entries),
    keychron_group_100_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 101:                                       |
| 0x1250 Q5 Ultra 8K Knob                                   |
| 0x1253 Q5 Ultra 8K Knob ANSI                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_101_patch_entries[] =
{
    {  55,  2, 18 },
};

static const keychron_patch keychron_q5_ultra_8k_knob_patch =
{
    KEYCHRON_Q5_ULTRA_8K_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_101_patch_entries),
    keychron_group_101_patch_entries
};

static const keychron_patch keychron_q5_ultra_8k_knob_ansi_patch =
{
    KEYCHRON_Q5_ULTRA_8K_KNOB_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_101_patch_entries),
    keychron_group_101_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 102:                                       |
| 0x1060 Q6 HE 8K                                           |
| 0x1063 Q6 HE 8K                                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_102_patch_entries[] =
{
    {  90,  4, 12 },
    { 108,  5, 19 },
};

static const keychron_patch keychron_q6_he_8k_1060_patch =
{
    KEYCHRON_Q6_HE_8K_1060_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_102_patch_entries),
    keychron_group_102_patch_entries
};

static const keychron_patch keychron_q6_he_8k_1063_patch =
{
    KEYCHRON_Q6_HE_8K_1063_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_102_patch_entries),
    keychron_group_102_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 103:                                       |
| 0x1061 Q6 HE 8K ISO                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_103_patch_entries[] =
{
    {  91,  4, 12 },
    { 109,  5, 19 },
};

static const keychron_patch keychron_q6_he_8k_iso_patch =
{
    KEYCHRON_Q6_HE_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_103_patch_entries),
    keychron_group_103_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 104:                                       |
| 0x1062 Q6 HE 8K JIS                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_104_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  15,  0, 16 },
    {  16,  0, 17 },
    {  17,  0, 18 },
    {  18,  0, 19 },
    {  19,  0, 20 },
    {  20,  0, 21 },
    { 112,  5, 20 },
};

static const keychron_patch keychron_q6_he_8k_jis_patch =
{
    KEYCHRON_Q6_HE_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_104_patch_entries),
    keychron_group_104_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 105:                                       |
| 0x0B60 Q6 HE ANSI Knob                                    |
| 0x0B63 Q6 HE ANSI Knob                                    |
| 0x0B64 Keychron Q6 HE Version 2 ANSI Knob Silent          |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_105_patch_entries[] =
{
    {  90,  4, 12 },
    { 100,  5, 10 },
    { 101,  5, 11 },
    { 102,  5, 12 },
    { 103,  5, 13 },
    { 108,  5, 19 },
};

static const keychron_patch keychron_q6_he_ansi_knob_0b60_patch =
{
    KEYCHRON_Q6_HE_ANSI_KNOB_0B60_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_105_patch_entries),
    keychron_group_105_patch_entries
};

static const keychron_patch keychron_q6_he_ansi_knob_0b63_patch =
{
    KEYCHRON_Q6_HE_ANSI_KNOB_0B63_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_105_patch_entries),
    keychron_group_105_patch_entries
};

static const keychron_patch keychron_q6_he_v2_ansi_knob_silent_patch =
{
    KEYCHRON_Q6_HE_V2_ANSI_KNOB_SILENT_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_105_patch_entries),
    keychron_group_105_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 106:                                       |
| 0x0B61 Q6 HE ISO Knob                                     |
| 0x0B65 Q6 HE Version 2 ISO                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_106_patch_entries[] =
{
    {  91,  4, 12 },
    { 101,  5, 10 },
    { 102,  5, 11 },
    { 103,  5, 12 },
    { 104,  5, 13 },
    { 109,  5, 19 },
};

static const keychron_patch keychron_q6_he_iso_knob_patch =
{
    KEYCHRON_Q6_HE_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_106_patch_entries),
    keychron_group_106_patch_entries
};

static const keychron_patch keychron_q6_he_version2_iso_patch =
{
    KEYCHRON_Q6_HE_VERSION2_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_106_patch_entries),
    keychron_group_106_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 107:                                       |
| 0x0B62 Q6 HE JIS Knob                                     |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_107_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  15,  0, 16 },
    {  16,  0, 17 },
    {  17,  0, 18 },
    {  18,  0, 19 },
    {  19,  0, 20 },
    {  20,  0, 21 },
    {  35,  1, 14 },
    {  36,  1, 15 },
    {  37,  1, 16 },
    {  38,  1, 17 },
    {  39,  1, 18 },
    {  40,  1, 19 },
    {  41,  1, 20 },
    {  42,  1, 21 },
    {  57,  2, 15 },
    {  58,  2, 16 },
    {  59,  2, 17 },
    {  60,  2, 18 },
    {  61,  2, 19 },
    {  62,  2, 20 },
    {  63,  2, 21 },
    {  77,  3, 18 },
    {  78,  3, 19 },
    {  79,  3, 20 },
    {  93,  4, 16 },
    {  94,  4, 18 },
    {  95,  4, 19 },
    {  96,  4, 20 },
    {  97,  4, 21 },
    { 103,  5,  9 },
    { 104,  5, 10 },
    { 105,  5, 11 },
    { 106,  5, 12 },
    { 107,  5, 13 },
    { 108,  5, 15 },
    { 109,  5, 16 },
    { 110,  5, 17 },
    { 111,  5, 18 },
    { 112,  5, 20 },
};

static const keychron_patch keychron_q6_he_jis_knob_patch =
{
    KEYCHRON_Q6_HE_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_107_patch_entries),
    keychron_group_107_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 108:                                       |
| 0x1260 Q6 Ultra 8K ANSI                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_108_patch_entries[] =
{
    { 19,  0, 20 },
    { 40,  1, 20 },
    { 77,  3, 20 },
    { 107, 5, 20 },
};

static const keychron_patch keychron_q6_ultra_8k_ansi_patch =
{
    KEYCHRON_Q6_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_108_patch_entries),
    keychron_group_108_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 109:                                       |
| 0x1261 Q6 Ultra 8K Knob ISO                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_109_patch_entries[] =
{
    {  20,  0, 20 },
    {  41,  1, 20 },
    {  62,  2, 20 },
    {  96,  4, 20 },
    { 109,  5, 19 },
};

static const keychron_patch keychron_q6_ultra_8k_knob_iso_patch =
{
    KEYCHRON_Q6_ULTRA_8K_KNOB_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_109_patch_entries),
    keychron_group_109_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 110:                                       |
| 0x0F10 V1 8K ANSI Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_110_patch_entries[] =
{
    {  43,  2, 13 },
    {  57,  3, 12 },
    {  70,  4, 12 },
};

static const keychron_patch keychron_v1_8k_ansi_knob_patch =
{
    KEYCHRON_V1_8K_ANSI_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_110_patch_entries),
    keychron_group_110_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 111:                                       |
| 0x0C11 V1 Ultra 8K ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_111_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  57,  3, 12 },
};

static const keychron_patch keychron_v1_ultra_8k_iso_patch =
{
    KEYCHRON_V1_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_111_patch_entries),
    keychron_group_111_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 112:                                       |
| 0x0C12 V1 Ultra 8K JIS                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_112_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
};

static const keychron_patch keychron_v1_ultra_8k_jis_patch =
{
    KEYCHRON_V1_ULTRA_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_112_patch_entries),
    keychron_group_112_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 115:                                       |
| 0x0C32 V3 Ultra 8K Knob JIS                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_115_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  15,  0, 16 },
    {  16,  0, 17 },
    {  31,  1, 14 },
    {  32,  1, 15 },
    {  33,  1, 16 },
    {  34,  1, 17 },
    {  49,  2, 15 },
    {  50,  2, 16 },
    {  51,  2, 17 },
    {  78,  4, 16 },
    {  89,  5, 15 },
    {  90,  5, 16 },
    {  91,  5, 17 },
};

static const keychron_patch keychron_v3_ultra_8k_knob_jis_patch =
{
    KEYCHRON_V3_ULTRA_8K_KNOB_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_115_patch_entries),
    keychron_group_115_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 116:                                       |
| 0x0F50 V5 8K ANSI Knob                                    |
| 0x0F53 V5 8K ANSI Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_116_patch_entries[] =
{
    {   5,  0,  7 },
    {   6,  0,  8 },
    {   7,  0,  9 },
    {   8,  0, 10 },
    {   9,  0, 11 },
    {  10,  0, 12 },
    {  11,  0, 13 },
    {  12,  0, 14 },
};

static const keychron_patch keychron_v5_8k_ansi_knob_0f50_patch =
{
    KEYCHRON_V5_8K_ANSI_KNOB_0F50_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_116_patch_entries),
    keychron_group_116_patch_entries
};

static const keychron_patch keychron_v5_8k_ansi_knob_0f53_patch =
{
    KEYCHRON_V5_8K_ANSI_KNOB_0F53_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_116_patch_entries),
    keychron_group_116_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 117:                                       |
| 0x0F60 V6 8K ANSI Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_117_patch_entries[] =
{
    {  78,  2, 20 },
    { 107,  5, 18 },
    { 108,  4, 20 },
};

static const keychron_patch keychron_v6_8k_ansi_knob_patch =
{
    KEYCHRON_V6_8K_ANSI_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_117_patch_entries),
    keychron_group_117_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 118:                                       |
| 0x0C60 V6 Ultra ANSI                                      |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_118_patch_entries[] =
{
    {  20,  0, 20 },
    {  41,  1, 20 },
    {  62,  2, 20 },
    {  95,  4, 20 },
};

static const keychron_patch keychron_v6_ultra_ansi_patch =
{
    KEYCHRON_V6_ULTRA_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_118_patch_entries),
    keychron_group_118_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 120:                                       |
| 0x1012 Q1 HE 8K JIS                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_120_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  45,  2, 15 },
    {  59,  3, 15 },
    {  79,  5,  9 },
    {  80,  5, 10 },
    {  81,  5, 11 },
    {  82,  5, 12 },
    {  83,  5, 13 },
    {  84,  5, 14 },
    {  85,  5, 15 },
};

static const keychron_patch keychron_q1_he_8k_jis_patch =
{
    KEYCHRON_Q1_HE_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_120_patch_entries),
    keychron_group_120_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 121:                                       |
| 0x0B11 Q1 HE ISO                                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_121_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  29,  1, 15 },
    {  44,  2, 15 },
    {  58,  3, 15 },
    {  70,  4, 11 },
    {  71,  4, 12 },
    {  77,  5, 10 },
    {  78,  5, 11 },
    {  79,  5, 12 },
    {  80,  5, 13 },
    {  81,  5, 14 },
    {  82,  5, 15 },
};

static const keychron_patch keychron_q1_he_iso_patch =
{
    KEYCHRON_Q1_HE_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_121_patch_entries),
    keychron_group_121_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 122:                                       |
| 0x0B12 Q1 HE JIS                                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_122_patch_entries[] =
{
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  29,  1, 14 },
    {  30,  1, 15 },
    {  45,  2, 15 },
    {  59,  3, 15 },
    {  79,  5,  9 },
    {  80,  5, 10 },
    {  81,  5, 11 },
    {  82,  5, 12 },
    {  83,  5, 13 },
    {  84,  5, 14 },
    {  85,  5, 15 },
};

static const keychron_patch keychron_q1_he_jis_patch =
{
    KEYCHRON_Q1_HE_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_122_patch_entries),
    keychron_group_122_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 123:                                       |
| 0x0B10 Q1 HE Knob                                         |
| 0x0B13 Keychron Q1 HE DAYZ                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_123_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  29,  1, 15 },
    {  44,  2, 15 },
    {  58,  3, 15 },
    {  69,  4, 11 },
    {  70,  4, 12 },
    {  79,  5, 13 },
    {  80,  5, 14 },
    {  81,  5, 15 },
};

static const keychron_patch keychron_q1_he_knob_patch =
{
    KEYCHRON_Q1_HE_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_123_patch_entries),
    keychron_group_123_patch_entries
};

static const keychron_patch keychron_q1_he_dayz_patch =
{
    KEYCHRON_Q1_HE_DAYZ_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_123_patch_entries),
    keychron_group_123_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 124:                                       |
| 0x0811 Q1 Max ISO Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_124_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  29,  1, 15 },
    {  44,  2, 15 },
    {  58,  3, 15 },
    {  71,  4, 12 },
    {  77,  5, 10 },
    {  78,  5, 11 },
    {  79,  5, 12 },
    {  80,  5, 13 },
    {  81,  5, 14 },
    {  82,  5, 15 },
};

static const keychron_patch keychron_q1_max_iso_knob_patch =
{
    KEYCHRON_Q1_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_124_patch_entries),
    keychron_group_124_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 125:                                       |
| 0x0812 Q1 Max JIS Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_125_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  30,  1, 15 },
    {  45,  2, 15 },
    {  59,  3, 15 },
    {  79,  5,  9 },
    {  80,  5, 10 },
    {  81,  5, 11 },
    {  82,  5, 12 },
    {  83,  5, 13 },
    {  84,  5, 14 },
    {  85,  5, 15 },
};

static const keychron_patch keychron_q1_max_jis_knob_patch =
{
    KEYCHRON_Q1_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_125_patch_entries),
    keychron_group_125_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 126:                                       |
| 0x0810 Q1 Max Knob                                        |
| 0x1210 Q1 Ultra 8K ANSI                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_126_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  29,  1, 15 },
    {  44,  2, 15 },
    {  58,  3, 15 },
    {  70,  4, 12 },
    {  76,  5, 10 },
    {  77,  5, 11 },
    {  78,  5, 12 },
    {  79,  5, 13 },
    {  80,  5, 14 },
    {  81,  5, 15 },
};

static const keychron_patch keychron_q1_max_knob_patch =
{
    KEYCHRON_Q1_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_126_patch_entries),
    keychron_group_126_patch_entries
};

static const keychron_patch keychron_q1_ultra_8k_ansi_patch =
{
    KEYCHRON_Q1_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_126_patch_entries),
    keychron_group_126_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 127:                                       |
| 0x08A1 Q10 Max ISO knob                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_127_patch_entries[] =
{
    {   4,  0, 13 },
    {   5,  0, 14 },
    {   6,  0, 15 },
    {   7,  0, 16 },
    {  15,  1, 16 },
    {  22,  2, 16 },
    {  26,  3, 11 },
    {  27,  3, 12 },
    {  28,  3, 13 },
    {  29,  3, 16 },
    {  40,  5, 14 },
    {  41,  5, 15 },
    {  42,  5, 16 },
    {  63,  5,  4 },
    {  64,  5,  5 },
    {  66,  0,  9 },
    {  67,  0, 10 },
    {  68,  0, 11 },
    {  69,  0, 12 },
    {  79,  3,  7 },
    {  80,  3,  8 },
    {  81,  3,  9 },
    {  82,  3, 10 },
    {  89,  5, 10 },
    {  90,  5, 11 },
};

static const keychron_patch keychron_q10_max_iso_knob_patch =
{
    KEYCHRON_Q10_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_127_patch_entries),
    keychron_group_127_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 128:                                       |
| 0x08A2 Q10 MAX JIS                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_128_patch_entries[] =
{
    {   4,  0, 13 },
    {   5,  0, 14 },
    {   6,  0, 15 },
    {   7,  0, 17 },
    {  16,  1, 17 },
    {  20,  2, 13 },
    {  21,  2, 14 },
    {  22,  2, 15 },
    {  23,  2, 17 },
    {  30,  3, 17 },
    {  33,  4,  2 },
    {  37,  4, 16 },
    {  42,  5, 15 },
    {  43,  5, 16 },
    {  44,  5, 17 },
    {  61,  4,  3 },
    {  62,  4,  4 },
    {  63,  4,  5 },
    {  64,  4,  6 },
    {  67,  5,  6 },
    {  68,  0,  9 },
    {  69,  0, 10 },
    {  70,  0, 11 },
    {  71,  0, 12 },
    {  76,  2,  8 },
    {  77,  2,  9 },
    {  78,  2, 10 },
    {  79,  2, 11 },
    {  80,  2, 12 },
};

static const keychron_patch keychron_q10_max_jis_patch =
{
    KEYCHRON_Q10_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_128_patch_entries),
    keychron_group_128_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 129:                                       |
| 0x08A0 Q10 Max Knob                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_129_patch_entries[] =
{
    {   4,  0, 13 },
    {   5,  0, 14 },
    {   6,  0, 15 },
    {   7,  0, 16 },
    {  15,  1, 16 },
    {  19,  2, 13 },
    {  20,  2, 14 },
    {  21,  2, 15 },
    {  22,  2, 16 },
    {  29,  3, 16 },
    {  39,  5, 14 },
    {  40,  5, 15 },
    {  41,  5, 16 },
    {  62,  5,  4 },
    {  63,  5,  5 },
    {  65,  0,  9 },
    {  66,  0, 10 },
    {  67,  0, 11 },
    {  68,  0, 12 },
    {  73,  2,  8 },
    {  74,  2,  9 },
    {  75,  2, 10 },
    {  76,  2, 11 },
    {  77,  2, 12 },
    {  87,  5,  9 },
    {  88,  5, 11 },
    {  89,  5, 12 },
};

static const keychron_patch keychron_q10_max_knob_patch =
{
    KEYCHRON_Q10_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_129_patch_entries),
    keychron_group_129_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 130:                                       |
| 0x08C4 Q12 MAX ISO Knob                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_130_patch_entries[] =
{
    {  73,  3, 18 },
    {  92,  5,  0 },
};

static const keychron_patch keychron_q12_max_iso_knob_patch =
{
    KEYCHRON_Q12_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_130_patch_entries),
    keychron_group_130_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 131:                                       |
| 0x08C5 Q12 MAX JIS Knob                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_131_patch_entries[] =
{
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  15,  0, 16 },
    {  16,  0, 17 },
    {  17,  0, 18 },
    {  18,  0, 19 },
    {  38,  1, 19 },
    {  57,  2, 19 },
    {  74,  3, 19 },
    {  79,  4,  5 },
    {  93,  5,  0 },
    { 100,  5, 13 },
    { 101,  5, 14 },
    { 102,  5, 15 },
    { 103,  5, 16 },
    { 104,  5, 17 },
    { 105,  5, 18 },
    { 106,  5, 19 },
};

static const keychron_patch keychron_q12_max_jis_knob_patch =
{
    KEYCHRON_Q12_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_131_patch_entries),
    keychron_group_131_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 132:                                       |
| 0x08C3 Q12 MAX Knob                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_132_patch_entries[] =
{
    {  73,  3, 18 },
    {  91,  5,  0 },
};

static const keychron_patch keychron_q12_max_knob_patch =
{
    KEYCHRON_Q12_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_132_patch_entries),
    keychron_group_132_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 133:                                       |
| 0x08D0 Q13 Max Knob                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_133_patch_entries[] =
{
    {   4,  0, 13 },
    {   5,  0, 14 },
    {  43,  4,  2 },
    {  75,  4,  3 },
    {  76,  4,  4 },
    {  77,  4,  5 },
    {  78,  4,  6 },
    {  79,  5,  3 },
    {  80,  5,  4 },
    {  81,  5,  6 },
    {  82,  0,  9 },
    {  83,  0, 10 },
    {  84,  0, 11 },
    {  85,  0, 12 },
    { 104,  5,  8 },
};

static const keychron_patch keychron_q13_max_knob_patch =
{
    KEYCHRON_Q13_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_133_patch_entries),
    keychron_group_133_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 134:                                       |
| 0x08E0 Q14 Max Knob                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_134_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0, 18 },
    {   8,  0, 19 },
    {   9,  0, 20 },
    {  10,  0, 21 },
    {  15,  1,  5 },
    {  16,  1,  6 },
    {  17,  1,  7 },
    {  18,  1, 17 },
    {  19,  1, 18 },
    {  20,  1, 19 },
    {  21,  1, 21 },
    {  26,  2,  5 },
    {  27,  2,  6 },
    {  28,  2, 18 },
    {  29,  2, 19 },
    {  30,  2, 20 },
    {  31,  2, 21 },
    {  36,  3,  6 },
    {  37,  3, 17 },
    {  38,  3, 18 },
    {  39,  3, 19 },
    {  40,  3, 21 },
    {  47,  4, 17 },
    {  48,  4, 18 },
    {  49,  4, 20 },
    {  54,  5, 19 },
    {  55,  5, 20 },
    {  56,  5, 21 },
    {  57,  0,  8 },
    {  58,  0,  9 },
    {  59,  0, 10 },
    {  60,  0, 11 },
    {  61,  1,  8 },
    {  62,  1,  9 },
    {  63,  1, 10 },
    {  64,  1, 11 },
    {  65,  2,  9 },
    {  66,  2,  8 },
    {  67,  2, 10 },
    {  68,  2, 11 },
    {  69,  3,  8 },
    {  70,  3,  9 },
    {  71,  3, 10 },
    {  72,  3, 11 },
    {  73,  4,  8 },
    {  74,  4,  9 },
    {  75,  4, 10 },
    {  76,  4, 11 },
    {  77,  5,  8 },
    {  79,  5, 11 },
    {  80,  0, 14 },
    {  81,  0, 15 },
    {  82,  0, 16 },
    {  83,  0, 17 },
    {  84,  1, 13 },
    {  85,  1, 14 },
    {  86,  1, 15 },
    {  87,  1, 16 },
    {  88,  2, 13 },
    {  89,  2, 14 },
    {  90,  2, 15 },
    {  91,  2, 16 },
    {  92,  2, 17 },
    {  93,  3, 13 },
    {  94,  3, 14 },
    {  95,  3, 15 },
    {  96,  3, 16 },
    {  97,  4, 12 },
    {  98,  4, 13 },
    {  99,  4, 14 },
    { 100,  4, 15 },
    { 101,  4, 16 },
    { 103,  5, 15 },
    { 104,  5, 16 },
};

static const keychron_patch keychron_q14_max_knob_patch =
{
    KEYCHRON_Q14_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_134_patch_entries),
    keychron_group_134_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 135:                                       |
| 0x08F0 Q15 Max Knob                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_135_patch_entries[] =
{
    {  60,  4,  6 },
};

static const keychron_patch keychron_q15_max_knob_patch =
{
    KEYCHRON_Q15_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_135_patch_entries),
    keychron_group_135_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 136:                                       |
| 0x0B20 Q2 HE Knob                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_136_patch_entries[] =
{
    {  42,  2, 12 },
};

static const keychron_patch keychron_q2_he_knob_patch =
{
    KEYCHRON_Q2_HE_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_136_patch_entries),
    keychron_group_136_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 137:                                       |
| 0x0821 Q2 Max ISO Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_137_patch_entries[] =
{
    {  43,  2, 14 },
    {  56,  3, 12 },
    {  57,  3, 13 },
};

static const keychron_patch keychron_q2_max_iso_knob_patch =
{
    KEYCHRON_Q2_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_137_patch_entries),
    keychron_group_137_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 138:                                       |
| 0x0822 Q2 Max JIS Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_138_patch_entries[] =
{
    {   0,  0, 15 },
    {  30,  1, 15 },
    {  44,  2, 15 },
    {  64,  4,  9 },
    {  65,  4, 10 },
    {  66,  4, 11 },
    {  67,  4, 12 },
    {  68,  4, 13 },
    {  69,  4, 14 },
    {  70,  4, 15 },
};

static const keychron_patch keychron_q2_max_jis_knob_patch =
{
    KEYCHRON_Q2_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_138_patch_entries),
    keychron_group_138_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 139:                                       |
| 0x0820 Q2 Max Knob                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_139_patch_entries[] =
{
    {  14,  0, 15 },
    {  29,  1, 15 },
    {  43,  2, 15 },
    {  55,  3, 12 },
    {  61,  4, 10 },
    {  62,  4, 11 },
    {  63,  4, 12 },
    {  64,  4, 13 },
    {  65,  4, 14 },
    {  66,  4, 15 },
};

static const keychron_patch keychron_q2_max_knob_patch =
{
    KEYCHRON_Q2_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_139_patch_entries),
    keychron_group_139_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 140:                                       |
| 0x0B30 Q3 HE                                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_140_patch_entries[] =
{
    {  13,  0, 13 },
    {  14,  0, 14 },
    {  15,  0, 15 },
    {  16,  0, 16 },
    {  33,  1, 16 },
    {  50,  2, 16 },
    {  63,  3, 12 },
    {  75,  4, 12 },
    {  85,  5, 14 },
    {  86,  5, 15 },
    {  87,  5, 16 },
};

static const keychron_patch keychron_q3_he_patch =
{
    KEYCHRON_Q3_HE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_140_patch_entries),
    keychron_group_140_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 141:                                       |
| 0x0B31 Q3 HE ISO Knob                                     |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_141_patch_entries[] =
{
    {  13,  0, 13 },
    {  14,  0, 14 },
    {  15,  0, 15 },
    {  16,  0, 16 },
    {  33,  1, 16 },
    {  50,  2, 16 },
    {  63,  3, 12 },
    {  76,  4, 12 },
    {  86,  5, 14 },
    {  87,  5, 15 },
    {  88,  5, 16 },
};

static const keychron_patch keychron_q3_he_iso_knob_patch =
{
    KEYCHRON_Q3_HE_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_141_patch_entries),
    keychron_group_141_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 142:                                       |
| 0x0831 Q3 Max ISO Knob                                    |
| 0x1031 Q3 HE 8K ISO                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_142_patch_entries[] =
{
    {  76,  4, 12 },
};

static const keychron_patch keychron_q3_max_iso_knob_patch =
{
    KEYCHRON_Q3_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_142_patch_entries),
    keychron_group_142_patch_entries
};

static const keychron_patch keychron_q3_he_8k_iso_patch =
{
    KEYCHRON_Q3_HE_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_142_patch_entries),
    keychron_group_142_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 143:                                       |
| 0x0832 Q3 Max JIS Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_143_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  15,  0, 16 },
    {  16,  0, 17 },
    {  31,  1, 14 },
    {  32,  1, 15 },
    {  33,  1, 16 },
    {  34,  1, 17 },
    {  49,  2, 15 },
    {  50,  2, 16 },
    {  51,  2, 17 },
    {  64,  3, 12 },
};

static const keychron_patch keychron_q3_max_jis_knob_patch =
{
    KEYCHRON_Q3_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_143_patch_entries),
    keychron_group_143_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 144:                                       |
| 0x0830 Q3 Max Knob                                        |
| 0x1230 Q3 Ultra                                           |
| 0x1610 K1 Ultra 8K Knob                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_144_patch_entries[] =
{
    {  63,  3, 12 },
    {  75,  4, 12 },
};

static const keychron_patch keychron_q3_max_knob_patch =
{
    KEYCHRON_Q3_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_144_patch_entries),
    keychron_group_144_patch_entries
};

static const keychron_patch keychron_q3_ultra_patch =
{
    KEYCHRON_Q3_ULTRA_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_144_patch_entries),
    keychron_group_144_patch_entries
};

static const keychron_patch keychron_k1_ultra_8k_knob_patch =
{
    KEYCHRON_K1_ULTRA_8K_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_144_patch_entries),
    keychron_group_144_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 145:                                       |
| 0x0B40 Q4 HE                                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_145_patch_entries[] =
{
    {  40,  2, 12 },
    {  57,  4, 10 },
    {  58,  4, 11 },
    {  59,  4, 12 },
};

static const keychron_patch keychron_q4_he_patch =
{
    KEYCHRON_Q4_HE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_145_patch_entries),
    keychron_group_145_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 146:                                       |
| 0x0B51 Q5 HE ISO Knob                                     |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_146_patch_entries[] =
{
    {   5,  0,  7 },
    {   6,  0,  8 },
    {   7,  0,  9 },
    {   8,  0, 10 },
    {   9,  0, 11 },
    {  10,  0, 12 },
    {  11,  0, 13 },
    {  12,  0, 14 },
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  16,  0, 18 },
    {  17,  0, 19 },
    {  32,  1, 15 },
    {  33,  1, 16 },
    {  34,  1, 17 },
    {  35,  1, 18 },
    {  36,  1, 19 },
    {  51,  2, 15 },
    {  52,  2, 16 },
    {  53,  2, 17 },
    {  54,  2, 18 },
    {  55,  2, 19 },
    {  69,  3, 15 },
    {  70,  3, 16 },
    {  71,  3, 17 },
    {  72,  3, 18 },
    {  84,  4, 11 },
    {  85,  4, 12 },
    {  87,  4, 16 },
    {  88,  4, 17 },
    {  89,  4, 18 },
    {  90,  4, 19 },
    {  95,  5, 10 },
    {  96,  5, 11 },
    {  97,  5, 12 },
    {  98,  5, 13 },
    {  99,  5, 14 },
    { 100,  5, 15 },
    { 102,  5, 18 },
};

static const keychron_patch keychron_q5_he_iso_knob_patch =
{
    KEYCHRON_Q5_HE_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_146_patch_entries),
    keychron_group_146_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 147:                                       |
| 0x0B52 Q5 HE JIS Knob                                     |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_147_patch_entries[] =
{
    {   5,  0,  7 },
    {   6,  0,  8 },
    {   7,  0,  9 },
    {   8,  0, 10 },
    {   9,  0, 11 },
    {  10,  0, 12 },
    {  11,  0, 13 },
    {  12,  0, 14 },
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  16,  0, 18 },
    {  17,  0, 19 },
    {  32,  1, 14 },
    {  33,  1, 15 },
    {  34,  1, 16 },
    {  35,  1, 17 },
    {  36,  1, 18 },
    {  37,  1, 19 },
    {  52,  2, 15 },
    {  53,  2, 16 },
    {  54,  2, 17 },
    {  55,  2, 18 },
    {  56,  2, 19 },
    {  70,  3, 15 },
    {  71,  3, 16 },
    {  72,  3, 17 },
    {  73,  3, 18 },
    {  87,  4, 15 },
    {  88,  4, 16 },
    {  89,  4, 17 },
    {  90,  4, 18 },
    {  91,  4, 19 },
    {  97,  5,  9 },
    {  98,  5, 10 },
    {  99,  5, 11 },
    { 100,  5, 12 },
    { 101,  5, 14 },
    { 102,  5, 15 },
    { 103,  5, 16 },
    { 104,  5, 17 },
    { 105,  5, 18 },
};

static const keychron_patch keychron_q5_he_jis_knob_patch =
{
    KEYCHRON_Q5_HE_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_147_patch_entries),
    keychron_group_147_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 148:                                       |
| 0x0B50 Q5 HE Knob                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_148_patch_entries[] =
{
    {  69,  3, 14 },
    {  83,  4, 11 },
    {  84,  4, 12 },
    {  85,  4, 13 },
    { 100,  5, 15 },
};

static const keychron_patch keychron_q5_he_knob_patch =
{
    KEYCHRON_Q5_HE_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_148_patch_entries),
    keychron_group_148_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 149:                                       |
| 0x0851 Q5 Max ISO Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_149_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  7 },
    {   6,  0,  8 },
    {   7,  0,  9 },
    {   8,  0, 10 },
    {   9,  0, 11 },
    {  10,  0, 12 },
    {  11,  0, 13 },
    {  12,  0, 14 },
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  16,  0, 18 },
    {  17,  0, 19 },
    {  32,  1, 15 },
    {  33,  1, 16 },
    {  34,  1, 17 },
    {  35,  1, 18 },
    {  36,  1, 19 },
    {  51,  2, 15 },
    {  52,  2, 16 },
    {  53,  2, 17 },
    {  54,  2, 18 },
    {  55,  2, 19 },
    {  69,  3, 15 },
    {  70,  3, 16 },
    {  71,  3, 17 },
    {  72,  3, 18 },
    {  86,  4, 16 },
    {  87,  4, 17 },
    {  88,  4, 18 },
    {  89,  4, 19 },
    {  90,  4, 14 },
    {  95,  5, 10 },
    {  96,  5, 11 },
    {  97,  5, 12 },
    {  99,  5, 18 },
    { 100,  5, 13 },
    { 101,  5, 14 },
    { 102,  5, 15 },
};

static const keychron_patch keychron_q5_max_iso_knob_patch =
{
    KEYCHRON_Q5_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_149_patch_entries),
    keychron_group_149_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 150:                                       |
| 0x0852 Q5 Max JIS Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_150_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  7 },
    {   6,  0,  8 },
    {   7,  0,  9 },
    {   8,  0, 10 },
    {   9,  0, 11 },
    {  10,  0, 12 },
    {  11,  0, 13 },
    {  12,  0, 14 },
    {  13,  0, 15 },
    {  14,  0, 16 },
    {  15,  0, 17 },
    {  16,  0, 18 },
    {  17,  0, 19 },
    {  33,  1, 15 },
    {  34,  1, 16 },
    {  35,  1, 17 },
    {  36,  1, 18 },
    {  37,  1, 19 },
    {  52,  2, 15 },
    {  53,  2, 16 },
    {  54,  2, 17 },
    {  55,  2, 18 },
    {  56,  2, 19 },
    {  70,  3, 15 },
    {  71,  3, 16 },
    {  72,  3, 17 },
    {  73,  3, 18 },
    {  87,  4, 16 },
    {  88,  4, 17 },
    {  89,  4, 18 },
    {  90,  4, 19 },
    {  91,  4, 15 },
    {  97,  5,  9 },
    {  98,  5, 10 },
    {  99,  5, 11 },
    { 100,  5, 12 },
    { 101,  5, 17 },
    { 102,  5, 18 },
    { 103,  5, 14 },
    { 104,  5, 15 },
    { 105,  5, 16 },
};

static const keychron_patch keychron_q5_max_jis_knob_patch =
{
    KEYCHRON_Q5_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_150_patch_entries),
    keychron_group_150_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 151:                                       |
| 0x0850 Q5 Max Knob                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_151_patch_entries[] =
{
    {   1,  0,  2 },
    {   2,  0,  3 },
    {   3,  0,  4 },
    {   4,  0,  5 },
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  69,  3, 14 },
    { 100,  5, 15 },
};

static const keychron_patch keychron_q5_max_knob_patch =
{
    KEYCHRON_Q5_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_151_patch_entries),
    keychron_group_151_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 152:                                       |
| 0x0861 Q6 Max ISO Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_152_patch_entries[] =
{
    {  20,  0, 20 },
    {  41,  1, 20 },
    {  62,  2, 20 },
    {  91,  4, 12 },
    {  96,  4, 20 },
    { 108,  5, 17 },
};

static const keychron_patch keychron_q6_max_iso_knob_patch =
{
    KEYCHRON_Q6_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_152_patch_entries),
    keychron_group_152_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 153:                                       |
| 0x0862 Q6 Max JIS Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_153_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  15,  0, 16 },
    {  16,  0, 17 },
    {  17,  0, 18 },
    {  18,  0, 19 },
    {  19,  0, 20 },
    {  20,  0, 21 },
    {  35,  1, 14 },
    {  36,  1, 15 },
    {  37,  1, 16 },
    {  38,  1, 17 },
    {  39,  1, 18 },
    {  40,  1, 19 },
    {  41,  1, 20 },
    {  42,  1, 21 },
    {  57,  2, 15 },
    {  58,  2, 16 },
    {  59,  2, 17 },
    {  60,  2, 18 },
    {  61,  2, 19 },
    {  62,  2, 20 },
    {  63,  2, 21 },
    {  77,  3, 18 },
    {  78,  3, 19 },
    {  79,  3, 20 },
    {  91,  4, 12 },
    {  92,  4, 13 },
    {  93,  4, 16 },
    {  94,  4, 18 },
    {  95,  4, 19 },
    {  96,  4, 20 },
    {  97,  4, 21 },
    { 107,  5, 14 },
    { 108,  5, 15 },
    { 109,  5, 16 },
    { 110,  5, 17 },
    { 112,  5, 20 },
};

static const keychron_patch keychron_q6_max_jis_knob_patch =
{
    KEYCHRON_Q6_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_153_patch_entries),
    keychron_group_153_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 154:                                       |
| 0x0860 Q6 Max Knob                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_154_patch_entries[] =
{
    {  20,  0, 20 },
    {  41,  1, 20 },
    {  62,  2, 20 },
    {  90,  4, 12 },
    {  95,  4, 20 },
    { 107,  5, 17 },
};

static const keychron_patch keychron_q6_max_knob_patch =
{
    KEYCHRON_Q6_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_154_patch_entries),
    keychron_group_154_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 155:                                       |
| 0x08C0 Q60 Max                                            |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_155_patch_entries[] =
{
    {  54,  3, 14 },
};

static const keychron_patch keychron_q60_max_patch =
{
    KEYCHRON_Q60_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_155_patch_entries),
    keychron_group_155_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 156:                                       |
| 0x08B0 Q65 Max Knob                                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_156_patch_entries[] =
{
    {  46,  2, 15 },
};

static const keychron_patch keychron_q65_max_knob_patch =
{
    KEYCHRON_Q65_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_156_patch_entries),
    keychron_group_156_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 157:                                       |
| 0x0880 Q8 Max Knob                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_157_patch_entries[] =
{
    {   0,  0,  3 },
    {   1,  0, 12 },
    {   2,  0, 16 },
    {   3,  0,  1 },
    {   4,  0,  2 },
    {   5,  0, 13 },
    {   6,  0, 14 },
    {   8,  1,  2 },
    {   9,  1, 13 },
    {  10,  1, 14 },
    {  11,  1, 15 },
    {  12,  1, 16 },
    {  14,  2,  2 },
    {  15,  2, 12 },
    {  16,  2, 13 },
    {  17,  2, 14 },
    {  18,  2, 16 },
    {  21,  3, 13 },
    {  22,  3, 14 },
    {  23,  3, 15 },
    {  26,  4, 14 },
    {  27,  4, 15 },
    {  28,  4, 16 },
    {  29,  0,  4 },
    {  30,  0,  5 },
    {  31,  0,  6 },
    {  32,  0,  7 },
    {  33,  1,  3 },
    {  34,  1,  4 },
    {  35,  1,  5 },
    {  36,  1,  6 },
    {  37,  2,  3 },
    {  38,  2,  4 },
    {  39,  2,  5 },
    {  40,  2,  6 },
    {  45,  4,  3 },
    {  46,  4,  4 },
    {  47,  4,  7 },
    {  48,  0,  8 },
    {  49,  0,  9 },
    {  50,  0, 10 },
    {  51,  0, 11 },
    {  52,  1,  8 },
    {  53,  1,  9 },
    {  54,  1, 10 },
    {  55,  1, 11 },
    {  56,  1, 12 },
    {  57,  2,  8 },
    {  58,  2,  9 },
    {  59,  2, 10 },
    {  60,  2, 11 },
    {  61,  3,  8 },
    {  62,  3,  9 },
    {  63,  3, 10 },
    {  64,  3, 11 },
    {  65,  3, 12 },
    {  66,  4,  8 },
    {  67,  4,  9 },
    {  68,  4, 12 },
};

static const keychron_patch keychron_q8_max_knob_patch =
{
    KEYCHRON_Q8_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_157_patch_entries),
    keychron_group_157_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 158:                                       |
| 0x0C00 V0 Ultra ANSI                                      |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_158_patch_entries[] =
{
    {  25,  5,  1 },
};

static const keychron_patch keychron_v0_ultra_ansi_patch =
{
    KEYCHRON_V0_ULTRA_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_158_patch_entries),
    keychron_group_158_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 159:                                       |
| 0x0914 V1 Max ISO Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_159_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  43,  2, 13 },
};

static const keychron_patch keychron_v1_max_iso_knob_patch =
{
    KEYCHRON_V1_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_159_patch_entries),
    keychron_group_159_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 160:                                       |
| 0x0915 V1 Max JIS Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_160_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  44,  2, 13 },
};

static const keychron_patch keychron_v1_max_jis_knob_patch =
{
    KEYCHRON_V1_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_160_patch_entries),
    keychron_group_160_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 161:                                       |
| 0x0913 V1 Max Knob                                        |
| 0x0C10 V1 Ultra 8K                                        |
| 0x0C13 V1 Ultra SE Knob ANSI                              |
| 0x0C14 V1 Ultra 8K                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_161_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  57,  3, 12 },
};

static const keychron_patch keychron_v1_max_knob_patch =
{
    KEYCHRON_V1_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_161_patch_entries),
    keychron_group_161_patch_entries
};

static const keychron_patch keychron_v1_ultra_8k_0c10_patch =
{
    KEYCHRON_V1_ULTRA_8K_0C10_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_161_patch_entries),
    keychron_group_161_patch_entries
};

static const keychron_patch keychron_v1_ultra_se_knob_ansi_patch =
{
    KEYCHRON_V1_ULTRA_SE_KNOB_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_161_patch_entries),
    keychron_group_161_patch_entries
};

static const keychron_patch keychron_v1_ultra_8k_0c14_patch =
{
    KEYCHRON_V1_ULTRA_8K_0C14_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_161_patch_entries),
    keychron_group_161_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 163:                                       |
| 0x09A0 V10 Max US Knob                                    |
| 0x0CA0 V10 Ultra 8K Knob                                  |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_163_patch_entries[] =
{
    {  63,  5,  5 },
};

static const keychron_patch keychron_v10_max_us_knob_patch =
{
    KEYCHRON_V10_MAX_US_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_163_patch_entries),
    keychron_group_163_patch_entries
};

static const keychron_patch keychron_v10_ultra_8k_knob_patch =
{
    KEYCHRON_V10_ULTRA_8K_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_163_patch_entries),
    keychron_group_163_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 166:                                       |
| 0x0F30 V3 8K Knob                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_166_patch_entries[] =
{
    {  63,  3, 12 },
    {  75,  4, 12 },
};

static const keychron_patch keychron_v3_8k_knob_patch =
{
    KEYCHRON_V3_8K_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_166_patch_entries),
    keychron_group_166_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 168:                                       |
| 0x0935 V3 Max JIS Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_168_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  15,  0, 16 },
    {  16,  0, 17 },
    {  31,  1, 14 },
    {  32,  1, 15 },
    {  33,  1, 16 },
    {  34,  1, 17 },
    {  49,  2, 15 },
    {  50,  2, 16 },
    {  51,  2, 17 },
    {  78,  4, 16 },
    {  89,  5, 15 },
    {  90,  5, 16 },
    {  91,  5, 17 },
};

static const keychron_patch keychron_v3_max_jis_knob_patch =
{
    KEYCHRON_V3_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_168_patch_entries),
    keychron_group_168_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 170:                                       |
| 0x0940 V4 Max                                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_170_patch_entries[] =
{
    {  40,  2, 12 },
    {  52,  3, 12 },
};

static const keychron_patch keychron_v4_max_patch =
{
    KEYCHRON_V4_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_170_patch_entries),
    keychron_group_170_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 171:                                       |
| 0x0941 V4 MAX ISO                                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_171_patch_entries[] =
{
    {  40,  2, 12 },
    {  53,  3, 12 },
};

static const keychron_patch keychron_v4_max_iso_patch =
{
    KEYCHRON_V4_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_171_patch_entries),
    keychron_group_171_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 172:                                       |
| 0x0951 V5 Max ISO Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_172_patch_entries[] =
{
    {  52,  2, 18 },
};

static const keychron_patch keychron_v5_max_iso_knob_patch =
{
    KEYCHRON_V5_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_172_patch_entries),
    keychron_group_172_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 173:                                       |
| 0x0952 V5 Max JIS Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_173_patch_entries[] =
{
    {  53,  2, 18 },
};

static const keychron_patch keychron_v5_max_jis_knob_patch =
{
    KEYCHRON_V5_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_173_patch_entries),
    keychron_group_173_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 174:                                       |
| 0x0950 V5 Max Knob                                        |
| 0x0C50 V5 Ultra ANSI                                      |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_174_patch_entries[] =
{
    {  52,  2, 18 },
};

static const keychron_patch keychron_v5_max_knob_patch =
{
    KEYCHRON_V5_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_174_patch_entries),
    keychron_group_174_patch_entries
};

static const keychron_patch keychron_v5_ultra_ansi_patch =
{
    KEYCHRON_V5_ULTRA_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_174_patch_entries),
    keychron_group_174_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 175:                                       |
| 0x0961 V6 Max ISO Knob                                    |
| 0x0F61 V6 8K ISO Knob                                     |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_175_patch_entries[] =
{
    {  62,  2, 20 },
    {  96,  4, 20 },
};

static const keychron_patch keychron_v6_max_iso_knob_patch =
{
    KEYCHRON_V6_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_175_patch_entries),
    keychron_group_175_patch_entries
};

static const keychron_patch keychron_v6_8k_iso_knob_patch =
{
    KEYCHRON_V6_8K_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_175_patch_entries),
    keychron_group_175_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 176:                                       |
 0x0962 V6 Max JIS Knob                                     |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_176_patch_entries[] =
{
    {   5,  0,  6 },
    {   6,  0,  7 },
    {   7,  0,  8 },
    {   8,  0,  9 },
    {   9,  0, 10 },
    {  10,  0, 11 },
    {  11,  0, 12 },
    {  12,  0, 13 },
    {  13,  0, 14 },
    {  14,  0, 15 },
    {  15,  0, 16 },
    {  16,  0, 17 },
    {  17,  0, 18 },
    {  18,  0, 19 },
    {  19,  0, 20 },
    {  20,  0, 21 },
    {  35,  1, 14 },
    {  36,  1, 15 },
    {  37,  1, 16 },
    {  38,  1, 17 },
    {  39,  1, 18 },
    {  40,  1, 19 },
    {  41,  1, 20 },
    {  42,  1, 21 },
    {  57,  2, 15 },
    {  58,  2, 16 },
    {  59,  2, 17 },
    {  60,  2, 18 },
    {  61,  2, 19 },
    {  62,  2, 20 },
    {  76,  3, 18 },
    {  77,  3, 19 },
    {  78,  3, 20 },
    {  79,  2, 21 },
    {  93,  4, 16 },
    {  94,  4, 18 },
    {  95,  4, 19 },
    {  96,  4, 20 },
    { 107,  5, 15 },
    { 108,  5, 16 },
    { 109,  5, 17 },
    { 110,  5, 18 },
    { 111,  5, 20 },
    { 112,  4, 21 },
};

static const keychron_patch keychron_v6_max_jis_knob_patch =
{
    KEYCHRON_V6_MAX_JIS_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_176_patch_entries),
    keychron_group_176_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 177:                                       |
| 0x0960 V6 Max Knob                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_177_patch_entries[] =
{
    {  62,  2, 20 },
    {  95,  4, 20 },
};

static const keychron_patch keychron_v6_max_knob_patch =
{
    KEYCHRON_V6_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_177_patch_entries),
    keychron_group_177_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 178:                                       |
| 0x0981 V8 Max ISO Knob                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_178_patch_entries[] =
{
    {   6,  0, 13 },
    {  17,  2, 13 },
    {  68,  4,  8 },
    {  69,  4, 10 },
};

static const keychron_patch keychron_v8_max_iso_knob_patch =
{
    KEYCHRON_V8_MAX_ISO_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_178_patch_entries),
    keychron_group_178_patch_entries
};


/*---------------------------------------------------------*\
| Geometry Group 179:                                       |
| 0x0980 V8 Max Knob                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry keychron_group_179_patch_entries[] =
{
    {   6,  0, 13 },
    {  17,  2, 13 },
    {  46,  4,  4 },
};

static const keychron_patch keychron_v8_max_knob_patch =
{
    KEYCHRON_V8_MAX_KNOB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(keychron_group_179_patch_entries),
    keychron_group_179_patch_entries
};

/*---------------------------------------------------------*\
| List of all Keychron matrix patches                       |
\*---------------------------------------------------------*/
const keychron_patch* const keychron_patches[] =
{
    &keychron_c2_pro_8k_patch,
    &keychron_c3_pro_8k_jis_patch,
    &keychron_k1_max_iso_patch,
    &keychron_k1_max_jis_patch,
    &keychron_k1_max_patch,
    &keychron_k1_v6_jis_patch,
    &keychron_k10_max_iso_patch,
    &keychron_k10_ultra_8k_iso_16a1_patch,
    &keychron_k10_max_jis_patch,
    &keychron_k10_max_patch,
    &keychron_k10_ultra_8k_patch,
    &keychron_k10_ultra_8k_iso_16a3_patch,
    &keychron_k10_v2_iso_patch,
    &keychron_k10_v2_jis_patch,
    &keychron_k10x_patch,
    &keychron_k11_max_iso_knob_patch,
    &keychron_k11_max_jis_knob_patch,
    &keychron_k11_max_knob_patch,
    &keychron_k13_max_iso_patch,
    &keychron_k13_max_jis_patch,
    &keychron_k13_max_patch,
    &keychron_k15_max_iso_knob_patch,
    &keychron_k15_max_jis_knob_patch,
    &keychron_k15_max_knob_patch,
    &keychron_k17_max_iso_knob_patch,
    &keychron_k17_max_jis_knob_patch,
    &keychron_k17_max_knob_patch,
    &keychron_k2_he_ansi_patch,
    &keychron_k3_he_patch,
    &keychron_k2_he_iso_patch,
    &keychron_k3_he_iso_patch,
    &keychron_k2_he_jis_patch,
    &keychron_k2_max_iso_patch,
    &keychron_k2_max_jis_patch,
    &keychron_k2_max_patch,
    &keychron_k3_max_iso_patch,
    &keychron_k2_v3_iso_patch,
    &keychron_k3_v3_iso_patch,
    &keychron_k3_ultra_8k_iso_patch,
    &keychron_k3_max_jis_patch,
    &keychron_k3_max_patch,
    &keychron_k2x_patch,
    &keychron_k3x_patch,
    &keychron_k3_max_se_patch,
    &keychron_k3_ultra_8k_ansi_patch,
    &keychron_k4_he_patch,
    &keychron_k4_he_iso_patch,
    &keychron_k4_he_jis_patch,
    &keychron_k4_max_ansi_patch,
    &keychron_k4x_jis_patch,
    &keychron_k5_max_iso_patch,
    &keychron_k5_max_jis_0a52_patch,
    &keychron_k5_max_patch,
    &keychron_k5x_iso_patch,
    &keychron_k6_he_patch,
    &keychron_k7_max_iso_patch,
    &keychron_k7_max_jis_patch,
    &keychron_k7_max_jis_v2_patch,
    &keychron_k7_max_patch,
    &keychron_k8_he_patch,
    &keychron_k8_he_iso_patch,
    &keychron_k8_he_jis_rbg_patch,
    &keychron_k8_max_jis_patch,
    &keychron_k9_max_patch,
    &keychron_c0_he_8k_patch,
    &keychron_c1_pro_8k_iso_patch,
    &keychron_c3_pro_iso_8k_patch,
    &keychron_k8_max_iso_patch,
    &keychron_k1x_iso_patch,
    &keychron_k8_v2_iso_patch,
    &keychron_c1_pro_8k_jis_patch,
    &keychron_k8_v2_jis_patch,
    &keychron_c2_pro_iso_8k_patch,
    &keychron_j1_patch,
    &keychron_j2_patch,
    &keychron_j2_ansi_knob_patch,
    &keychron_j2_he_8k_ansi_patch,
    &keychron_j3_se_knob_patch,
    &keychron_j4_patch,
    &keychron_j4_ansi_patch,
    &keychron_c1_pro_8k_patch,
    &keychron_c3_pro_8k_patch,
    &keychron_k8_max_patch,
    &keychron_k1x_us_patch,
    &keychron_k8_v2_patch,
    &keychron_j5_patch,
    &keychron_j5_ansi_knob_patch,
    &keychron_k0_max_patch,
    &keychron_k2_ultra_8k_ansi_patch,
    &keychron_k2_ultra_8k_iso_patch,
    &keychron_k2_v3_jis_patch,
    &keychron_k3_v3_jis_patch,
    &keychron_k4_max_iso_patch,
    &keychron_k4_max_jis_patch,
    &keychron_k4_ultra_8k_patch,
    &keychron_k4_v3_iso_patch,
    &keychron_k4_v3_patch,
    &keychron_k5_max_jis_0a58_patch,
    &keychron_k5_ultra_8k_knob_ansi_patch,
    &keychron_k5_ultra_8k_knob_iso_patch,
    &keychron_k5_v2_patch,
    &keychron_k5_v2_jis_patch,
    &keychron_k6_he_iso_patch,
    &keychron_k8_ultra_8k_ansi_patch,
    &keychron_k8_ultra_8k_iso_patch,
    &keychron_q1_he_patch,
    &keychron_q1_he_ansi_v2_silent_patch,
    &keychron_q1_he_8k_ansi_patch,
    &keychron_q1_he_8k_iso_knob_patch,
    &keychron_q11_ultra_8k_knob_patch,
    &keychron_q11_ultra_8k_knob_iso_patch,
    &keychron_q12_he_iso_knob_patch,
    &keychron_q12_he_knob_patch,
    &keychron_q13_max_jis_knob_patch,
    &keychron_q13_ultra_8k_knob_patch,
    &keychron_q2_he_8k_patch,
    &keychron_q3_he_8k_patch,
    &keychron_q3_he_8k_ansi_patch,
    &keychron_q3_he_8k_jis_patch,
    &keychron_q3_he_jis_knob_patch,
    &keychron_q3_ultra_8k_knob_iso_patch,
    &keychron_q5_he_8k_1050_patch,
    &keychron_q5_he_8k_1053_patch,
    &keychron_q5_ultra_8k_knob_patch,
    &keychron_q5_ultra_8k_knob_ansi_patch,
    &keychron_q6_he_8k_1060_patch,
    &keychron_q6_he_8k_1063_patch,
    &keychron_q6_he_8k_iso_patch,
    &keychron_q6_he_8k_jis_patch,
    &keychron_q6_he_ansi_knob_0b60_patch,
    &keychron_q6_he_ansi_knob_0b63_patch,
    &keychron_q6_he_v2_ansi_knob_silent_patch,
    &keychron_q6_he_iso_knob_patch,
    &keychron_q6_he_version2_iso_patch,
    &keychron_q6_he_jis_knob_patch,
    &keychron_q6_ultra_8k_ansi_patch,
    &keychron_q6_ultra_8k_knob_iso_patch,
    &keychron_v1_8k_ansi_knob_patch,
    &keychron_v1_ultra_8k_iso_patch,
    &keychron_v1_ultra_8k_jis_patch,
    &keychron_v3_ultra_8k_knob_jis_patch,
    &keychron_v5_8k_ansi_knob_0f50_patch,
    &keychron_v5_8k_ansi_knob_0f53_patch,
    &keychron_v6_8k_ansi_knob_patch,
    &keychron_v6_ultra_ansi_patch,
    &keychron_q1_he_8k_jis_patch,
    &keychron_q1_he_iso_patch,
    &keychron_q1_he_jis_patch,
    &keychron_q1_he_knob_patch,
    &keychron_q1_he_dayz_patch,
    &keychron_q1_max_iso_knob_patch,
    &keychron_q1_max_jis_knob_patch,
    &keychron_q1_max_knob_patch,
    &keychron_q1_ultra_8k_ansi_patch,
    &keychron_q10_max_iso_knob_patch,
    &keychron_q10_max_jis_patch,
    &keychron_q10_max_knob_patch,
    &keychron_q12_max_iso_knob_patch,
    &keychron_q12_max_jis_knob_patch,
    &keychron_q12_max_knob_patch,
    &keychron_q13_max_knob_patch,
    &keychron_q14_max_knob_patch,
    &keychron_q15_max_knob_patch,
    &keychron_q2_he_knob_patch,
    &keychron_q2_max_iso_knob_patch,
    &keychron_q2_max_jis_knob_patch,
    &keychron_q2_max_knob_patch,
    &keychron_q3_he_patch,
    &keychron_q3_he_iso_knob_patch,
    &keychron_q3_max_iso_knob_patch,
    &keychron_q3_he_8k_iso_patch,
    &keychron_q3_max_jis_knob_patch,
    &keychron_q3_max_knob_patch,
    &keychron_q3_ultra_patch,
    &keychron_k1_ultra_8k_knob_patch,
    &keychron_q4_he_patch,
    &keychron_q5_he_iso_knob_patch,
    &keychron_q5_he_jis_knob_patch,
    &keychron_q5_he_knob_patch,
    &keychron_q5_max_iso_knob_patch,
    &keychron_q5_max_jis_knob_patch,
    &keychron_q5_max_knob_patch,
    &keychron_q6_max_iso_knob_patch,
    &keychron_q6_max_jis_knob_patch,
    &keychron_q6_max_knob_patch,
    &keychron_q60_max_patch,
    &keychron_q65_max_knob_patch,
    &keychron_q8_max_knob_patch,
    &keychron_v0_ultra_ansi_patch,
    &keychron_v1_max_iso_knob_patch,
    &keychron_v1_max_jis_knob_patch,
    &keychron_v1_max_knob_patch,
    &keychron_v1_ultra_8k_0c10_patch,
    &keychron_v1_ultra_se_knob_ansi_patch,
    &keychron_v1_ultra_8k_0c14_patch,
    &keychron_v10_max_us_knob_patch,
    &keychron_v10_ultra_8k_knob_patch,
    &keychron_v3_8k_knob_patch,
    &keychron_v3_max_jis_knob_patch,
    &keychron_v4_max_patch,
    &keychron_v4_max_iso_patch,
    &keychron_v5_max_iso_knob_patch,
    &keychron_v5_max_jis_knob_patch,
    &keychron_v5_max_knob_patch,
    &keychron_v5_ultra_ansi_patch,
    &keychron_v6_max_iso_knob_patch,
    &keychron_v6_8k_iso_knob_patch,
    &keychron_v6_max_jis_knob_patch,
    &keychron_v6_max_knob_patch,
    &keychron_v8_max_iso_knob_patch,
    &keychron_v8_max_knob_patch,
};

const unsigned int KEYCHRON_PATCH_COUNT = sizeof(keychron_patches) / sizeof(keychron_patch*);
