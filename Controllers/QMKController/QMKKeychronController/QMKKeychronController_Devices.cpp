/*---------------------------------------------------------*\
| QMKKeychronController_Devices.cpp                         |
|                                                           |
| Keychron device matrix correction data                    |
|                                                           |
| Generated from hand-corrected raw-to-display matrix       |
| mappings.                                                 |
|                                                           |
| This file is part of the OpenRGB project                  |
| SPDX-License-Identifier: GPL-2.0-or-later                 |
\*---------------------------------------------------------*/

#include "QMKKeychronController_Devices.h"

#define KEYCHRON_PATCH_ENTRIES_COUNT(entries) (sizeof(entries) / sizeof(keychron_patch_entry))

/*---------------------------------------------------------*\
| Keychron Patch Types                                      |
|                                                           |
| The Keychron protocol does not provide RGB matrix X/Y     |
| coordinates, only the position in the wiring matrix.      |
| For most Keychron keyboards, the wiring matrix mostly     |
| matches the physical position, but there are often a      |
| few misplaced LEDs. Implement a matrix patching system    |
| to apply keyboard-specific patches for these misplaced    |
| LEDs.                                                     |
\*---------------------------------------------------------*/

/*---------------------------------------------------------*\
| Geometry Group 1:                                         |
| 0x0106 Keychron Q1V2 ANSI                                 |
| 0x0107 Keychron Q1V2 ANSI                                 |
| 0x0108 Keychron Q1V2 ISO                                  |
| 0x0109 Keychron Q1V2 ISO                                  |
| 0x0310 Keychron V1 ANSI                                   |
| 0x0311 Keychron V1 ANSI                                   |
| 0x0312 Keychron V1 ISO                                    |
| 0x0313 Keychron V1 ISO                                    |
| 0x0610 Keychron Q1 Pro ANSI                               |
| 0x0611 Keychron Q1 Pro ISO                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry q1v2_ansi_0106_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  3, 13,  3, 12 },
    {  4, 13,  4, 12 },
};

static const keychron_patch q1v2_ansi_0106_patch =
{
    KEYCHRON_Q1V2_ANSI_0106_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_ansi_0106_entries),
    q1v2_ansi_0106_entries
};

static const keychron_patch q1v2_ansi_0107_patch =
{
    KEYCHRON_Q1V2_ANSI_0107_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_ansi_0106_entries),
    q1v2_ansi_0106_entries
};

static const keychron_patch q1v2_iso_0108_patch =
{
    KEYCHRON_Q1V2_ISO_0108_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_ansi_0106_entries),
    q1v2_ansi_0106_entries
};

static const keychron_patch q1v2_iso_0109_patch =
{
    KEYCHRON_Q1V2_ISO_0109_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_ansi_0106_entries),
    q1v2_ansi_0106_entries
};

static const keychron_patch v1_ansi_0310_patch =
{
    KEYCHRON_V1_ANSI_0310_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_ansi_0106_entries),
    q1v2_ansi_0106_entries
};

static const keychron_patch v1_ansi_0311_patch =
{
    KEYCHRON_V1_ANSI_0311_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_ansi_0106_entries),
    q1v2_ansi_0106_entries
};

static const keychron_patch v1_iso_0312_patch =
{
    KEYCHRON_V1_ISO_0312_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_ansi_0106_entries),
    q1v2_ansi_0106_entries
};

static const keychron_patch v1_iso_0313_patch =
{
    KEYCHRON_V1_ISO_0313_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_ansi_0106_entries),
    q1v2_ansi_0106_entries
};

static const keychron_patch q1_pro_ansi_patch =
{
    KEYCHRON_Q1_PRO_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_ansi_0106_entries),
    q1v2_ansi_0106_entries
};

static const keychron_patch q1_pro_iso_patch =
{
    KEYCHRON_Q1_PRO_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_ansi_0106_entries),
    q1v2_ansi_0106_entries
};

/*---------------------------------------------------------*\
| Geometry Group 2:                                         |
| 0x010A Keychron Q1V2 JIS                                  |
| 0x010B Keychron Q1V2 JIS                                  |
| 0x0913 Keychron V1 Max ANSI                               |
| 0x0C10 Keychron V1 Ultra 8K ANSI                          |
| 0x0C11 Keychron V1 Ultra 8K ISO                           |
| 0x0C13 Keychron V1 Ultra SE ANSI                          |
| 0x0C14 Keychron V1 Ultra 8K                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry q1v2_jis_010a_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  3, 13,  3, 12 },
};

static const keychron_patch q1v2_jis_010a_patch =
{
    KEYCHRON_Q1V2_JIS_010A_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_jis_010a_entries),
    q1v2_jis_010a_entries
};

static const keychron_patch q1v2_jis_010b_patch =
{
    KEYCHRON_Q1V2_JIS_010B_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_jis_010a_entries),
    q1v2_jis_010a_entries
};

static const keychron_patch v1_max_ansi_patch =
{
    KEYCHRON_V1_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_jis_010a_entries),
    q1v2_jis_010a_entries
};

static const keychron_patch v1_ultra_8k_ansi_patch =
{
    KEYCHRON_V1_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_jis_010a_entries),
    q1v2_jis_010a_entries
};

static const keychron_patch v1_ultra_8k_iso_patch =
{
    KEYCHRON_V1_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_jis_010a_entries),
    q1v2_jis_010a_entries
};

static const keychron_patch v1_ultra_se_ansi_patch =
{
    KEYCHRON_V1_ULTRA_SE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_jis_010a_entries),
    q1v2_jis_010a_entries
};

static const keychron_patch v1_ultra_8k_patch =
{
    KEYCHRON_V1_ULTRA_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1v2_jis_010a_entries),
    q1v2_jis_010a_entries
};

/*---------------------------------------------------------*\
| Geometry Group 3:                                         |
| 0x0110 Keychron Q2 ANSI                                   |
| 0x0112 Keychron Q2 ISO                                    |
| 0x0320 Keychron V2 ANSI                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry q2_ansi_0110_entries[] =
{
    {  0, 14,  0, 15 },
    {  1, 14,  1, 15 },
    {  2, 12,  4, 14 },
    {  2, 13,  2, 12 },
    {  2, 14,  2, 15 },
    {  3, 13,  3, 12 },
    {  4, 14,  4, 15 },
};

static const keychron_patch q2_ansi_0110_patch =
{
    KEYCHRON_Q2_ANSI_0110_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q2_ansi_0110_entries),
    q2_ansi_0110_entries
};

static const keychron_patch q2_iso_0112_patch =
{
    KEYCHRON_Q2_ISO_0112_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q2_ansi_0110_entries),
    q2_ansi_0110_entries
};

static const keychron_patch v2_ansi_0320_patch =
{
    KEYCHRON_V2_ANSI_0320_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q2_ansi_0110_entries),
    q2_ansi_0110_entries
};

/*---------------------------------------------------------*\
| Geometry Group 4:                                         |
| 0x0111 Keychron Q2 ANSI                                   |
| 0x0113 Keychron Q2 ISO                                    |
| 0x0321 Keychron V2 ANSI                                   |
| 0x0323 Keychron V2 ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry q2_ansi_0111_entries[] =
{
    {  1, 14,  1, 15 },
    {  2, 12,  4, 14 },
    {  2, 13,  2, 12 },
    {  2, 14,  2, 15 },
    {  3, 13,  3, 12 },
    {  4, 14,  4, 15 },
};

static const keychron_patch q2_ansi_0111_patch =
{
    KEYCHRON_Q2_ANSI_0111_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q2_ansi_0111_entries),
    q2_ansi_0111_entries
};

static const keychron_patch q2_iso_0113_patch =
{
    KEYCHRON_Q2_ISO_0113_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q2_ansi_0111_entries),
    q2_ansi_0111_entries
};

static const keychron_patch v2_ansi_0321_patch =
{
    KEYCHRON_V2_ANSI_0321_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q2_ansi_0111_entries),
    q2_ansi_0111_entries
};

static const keychron_patch v2_iso_patch =
{
    KEYCHRON_V2_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q2_ansi_0111_entries),
    q2_ansi_0111_entries
};

/*---------------------------------------------------------*\
| Geometry Group 5:                                         |
| 0x0120 Keychron Q3 ANSI                                   |
| 0x0122 Keychron Q3 ISO                                    |
| 0x0330 Keychron V3 ANSI                                   |
| 0x0332 Keychron V3 ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry q3_ansi_0120_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  3, 12,  2, 16 },
    {  3, 13,  3, 12 },
    {  3, 14,  0, 16 },
    {  3, 15,  1, 16 },
    {  4, 13,  4, 12 },
    {  4, 14,  5, 16 },
};

static const keychron_patch q3_ansi_0120_patch =
{
    KEYCHRON_Q3_ANSI_0120_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_ansi_0120_entries),
    q3_ansi_0120_entries
};

static const keychron_patch q3_iso_0122_patch =
{
    KEYCHRON_Q3_ISO_0122_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_ansi_0120_entries),
    q3_ansi_0120_entries
};

static const keychron_patch v3_ansi_0330_patch =
{
    KEYCHRON_V3_ANSI_0330_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_ansi_0120_entries),
    q3_ansi_0120_entries
};

static const keychron_patch v3_iso_0332_patch =
{
    KEYCHRON_V3_ISO_0332_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_ansi_0120_entries),
    q3_ansi_0120_entries
};

/*---------------------------------------------------------*\
| Geometry Group 6:                                         |
| 0x0121 Keychron Q3 ANSI                                   |
| 0x0331 Keychron V3 ANSI                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry q3_ansi_0121_entries[] =
{
    {  3, 12,  2, 16 },
    {  3, 13,  3, 12 },
    {  3, 14,  0, 16 },
    {  3, 15,  1, 16 },
    {  4, 13,  4, 12 },
    {  4, 14,  5, 16 },
};

static const keychron_patch q3_ansi_0121_patch =
{
    KEYCHRON_Q3_ANSI_0121_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_ansi_0121_entries),
    q3_ansi_0121_entries
};

static const keychron_patch v3_ansi_0331_patch =
{
    KEYCHRON_V3_ANSI_0331_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_ansi_0121_entries),
    q3_ansi_0121_entries
};

/*---------------------------------------------------------*\
| Geometry Group 7:                                         |
| 0x0123 Keychron Q3 ISO                                    |
| 0x0333 Keychron V3 ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry q3_iso_0123_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  3, 12,  2, 16 },
    {  3, 13,  3, 12 },
    {  3, 14,  0, 16 },
    {  3, 15,  1, 16 },
    {  4, 13,  4, 12 },
    {  4, 14,  5, 16 },
};

static const keychron_patch q3_iso_0123_patch =
{
    KEYCHRON_Q3_ISO_0123_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_iso_0123_entries),
    q3_iso_0123_entries
};

static const keychron_patch v3_iso_0333_patch =
{
    KEYCHRON_V3_ISO_0333_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_iso_0123_entries),
    q3_iso_0123_entries
};

/*---------------------------------------------------------*\
| Geometry Group 8:                                         |
| 0x0140 Keychron Q4 ANSI                                   |
| 0x0142 Keychron Q4 ISO                                    |
| 0x0170 Keychron Q7 ANSI                                   |
| 0x0172 Keychron Q7 ISO                                    |
| 0x0270 Keychron K7 Pro RGB                                |
| 0x0271 Keychron K7 Pro ISO RGB                            |
| 0x0340 Keychron V4 ANSI                                   |
| 0x0342 Keychron V4 ISO                                    |
| 0x0370 Keychron V7 ANSI                                   |
| 0x0372 Keychron V7 ISO                                    |
| 0x0620 Keychron Q2 Pro ANSI                               |
| 0x0621 Keychron Q2 Pro ISO                                |
| 0x0640 Keychron Q4 Pro ANSI RGB                           |
| 0x0940 Keychron V4 Max ANSI                               |
| 0x0A70 Keychron K7 Max ANSI RGB                           |
| 0x0A71 Keychron K7 Max ISO RGB                            |
| 0x0A90 Keychron K9 Max ANSI RGB                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry q4_ansi_entries[] =
{
    {  2, 13,  2, 12 },
    {  3, 13,  3, 12 },
};

static const keychron_patch q4_ansi_patch =
{
    KEYCHRON_Q4_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch q4_iso_patch =
{
    KEYCHRON_Q4_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch q7_ansi_patch =
{
    KEYCHRON_Q7_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch q7_iso_patch =
{
    KEYCHRON_Q7_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch k7_pro_rgb_patch =
{
    KEYCHRON_K7_PRO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch k7_pro_iso_rgb_patch =
{
    KEYCHRON_K7_PRO_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch v4_ansi_patch =
{
    KEYCHRON_V4_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch v4_iso_patch =
{
    KEYCHRON_V4_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch v7_ansi_patch =
{
    KEYCHRON_V7_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch v7_iso_patch =
{
    KEYCHRON_V7_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch q2_pro_ansi_patch =
{
    KEYCHRON_Q2_PRO_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch q2_pro_iso_patch =
{
    KEYCHRON_Q2_PRO_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch q4_pro_ansi_rgb_patch =
{
    KEYCHRON_Q4_PRO_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch v4_max_ansi_patch =
{
    KEYCHRON_V4_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch k7_max_ansi_rgb_patch =
{
    KEYCHRON_K7_MAX_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch k7_max_iso_rgb_patch =
{
    KEYCHRON_K7_MAX_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

static const keychron_patch k9_max_ansi_rgb_patch =
{
    KEYCHRON_K9_MAX_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_ansi_entries),
    q4_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 9:                                         |
| 0x0150 Keychron Q5 ANSI                                   |
| 0x0152 Keychron Q5 ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry q5_ansi_0150_entries[] =
{
    {  2, 14,  2, 18 },
    {  3, 12,  0, 18 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 18 },
    {  4, 13,  4, 12 },
    {  5,  5,  4, 18 },
    {  5, 10,  5,  9 },
    {  5, 11,  5, 10 },
    {  5, 12,  5, 11 },
};

static const keychron_patch q5_ansi_0150_patch =
{
    KEYCHRON_Q5_ANSI_0150_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_ansi_0150_entries),
    q5_ansi_0150_entries
};

static const keychron_patch q5_iso_0152_patch =
{
    KEYCHRON_Q5_ISO_0152_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_ansi_0150_entries),
    q5_ansi_0150_entries
};

/*---------------------------------------------------------*\
| Geometry Group 10:                                        |
| 0x0151 Keychron Q5 ANSI                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry q5_ansi_0151_entries[] =
{
    {  0,  7,  0,  6 },
    {  0,  8,  0,  7 },
    {  0,  9,  0,  8 },
    {  0, 10,  0,  9 },
    {  0, 11,  0, 10 },
    {  0, 12,  0, 11 },
    {  0, 13,  0, 12 },
    {  0, 14,  0, 13 },
    {  0, 15,  0, 14 },
    {  0, 16,  0, 15 },
    {  0, 17,  0, 16 },
    {  1, 15,  1, 14 },
    {  1, 16,  1, 15 },
    {  1, 17,  1, 16 },
    {  2, 14,  2, 17 },
    {  2, 15,  2, 14 },
    {  2, 16,  2, 15 },
    {  2, 17,  2, 16 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 17 },
    {  3, 15,  3, 14 },
    {  3, 16,  3, 15 },
    {  3, 17,  3, 16 },
    {  4, 13,  4, 12 },
    {  4, 14,  4, 13 },
    {  4, 15,  4, 14 },
    {  4, 16,  4, 15 },
    {  4, 17,  4, 16 },
    {  5,  5,  4, 17 },
    {  5, 10,  5,  9 },
    {  5, 11,  5, 10 },
    {  5, 12,  5, 11 },
    {  5, 13,  5, 12 },
    {  5, 14,  5, 13 },
    {  5, 15,  5, 14 },
    {  5, 16,  5, 15 },
    {  5, 17,  5, 16 },
};

static const keychron_patch q5_ansi_0151_patch =
{
    KEYCHRON_Q5_ANSI_0151_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_ansi_0151_entries),
    q5_ansi_0151_entries
};

/*---------------------------------------------------------*\
| Geometry Group 11:                                        |
| 0x0153 Keychron Q5 ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry q5_iso_0153_entries[] =
{
    {  2, 14,  2, 18 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 18 },
    {  4, 13,  4, 12 },
    {  5,  5,  4, 18 },
    {  5, 10,  5,  9 },
    {  5, 11,  5, 10 },
    {  5, 12,  5, 11 },
};

static const keychron_patch q5_iso_0153_patch =
{
    KEYCHRON_Q5_ISO_0153_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_iso_0153_entries),
    q5_iso_0153_entries
};

/*---------------------------------------------------------*\
| Geometry Group 12:                                        |
| 0x0160 Keychron Q6 ANSI                                   |
| 0x0162 Keychron Q6 ISO                                    |
| 0x0360 Keychron V6 ANSI                                   |
| 0x0362 Keychron V6 ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry q6_ansi_0160_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 17,  1, 19 },
    {  0, 18,  1, 20 },
    {  0, 19,  0, 17 },
    {  1, 19,  0, 18 },
    {  2, 19,  0, 19 },
    {  3, 13,  3, 12 },
    {  3, 14,  2, 19 },
    {  3, 15,  2, 20 },
    {  3, 16,  3, 19 },
    {  3, 19,  0, 20 },
    {  4, 13,  4, 12 },
    {  4, 14,  4, 19 },
    {  4, 16,  4, 20 },
    {  5, 18,  5, 19 },
};

static const keychron_patch q6_ansi_0160_patch =
{
    KEYCHRON_Q6_ANSI_0160_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_ansi_0160_entries),
    q6_ansi_0160_entries
};

static const keychron_patch q6_iso_patch =
{
    KEYCHRON_Q6_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_ansi_0160_entries),
    q6_ansi_0160_entries
};

static const keychron_patch v6_ansi_0360_patch =
{
    KEYCHRON_V6_ANSI_0360_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_ansi_0160_entries),
    q6_ansi_0160_entries
};

static const keychron_patch v6_iso_0362_patch =
{
    KEYCHRON_V6_ISO_0362_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_ansi_0160_entries),
    q6_ansi_0160_entries
};

/*---------------------------------------------------------*\
| Geometry Group 13:                                        |
| 0x0161 Keychron Q6 ANSI                                   |
| 0x0163 Keychron Q6 ANSI                                   |
| 0x0361 Keychron V6 ANSI                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry q6_ansi_0161_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 17,  1, 19 },
    {  0, 18,  1, 20 },
    {  0, 19,  0, 17 },
    {  1, 19,  0, 18 },
    {  2, 19,  0, 19 },
    {  3, 13,  3, 12 },
    {  3, 14,  2, 19 },
    {  3, 15,  2, 20 },
    {  3, 16,  3, 19 },
    {  3, 19,  0, 20 },
    {  4, 13,  4, 12 },
    {  4, 14,  4, 19 },
    {  4, 16,  4, 20 },
    {  5, 18,  5, 19 },
};

static const keychron_patch q6_ansi_0161_patch =
{
    KEYCHRON_Q6_ANSI_0161_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_ansi_0161_entries),
    q6_ansi_0161_entries
};

static const keychron_patch q6_ansi_0163_patch =
{
    KEYCHRON_Q6_ANSI_0163_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_ansi_0161_entries),
    q6_ansi_0161_entries
};

static const keychron_patch v6_ansi_0361_patch =
{
    KEYCHRON_V6_ANSI_0361_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_ansi_0161_entries),
    q6_ansi_0161_entries
};

/*---------------------------------------------------------*\
| Geometry Group 14:                                        |
| 0x0180 Keychron Q8 ANSI                                   |
| 0x0182 Keychron Q8 ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry q8_ansi_0180_entries[] =
{
    {  0, 14,  0, 15 },
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  4,  2,  4,  3 },
    {  4,  3,  4,  4 },
    {  4,  9,  4, 11 },
    {  4, 12,  4, 13 },
    {  4, 13,  4, 14 },
    {  4, 14,  4, 15 },
};

static const keychron_patch q8_ansi_0180_patch =
{
    KEYCHRON_Q8_ANSI_0180_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q8_ansi_0180_entries),
    q8_ansi_0180_entries
};

static const keychron_patch q8_iso_0182_patch =
{
    KEYCHRON_Q8_ISO_0182_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q8_ansi_0180_entries),
    q8_ansi_0180_entries
};

/*---------------------------------------------------------*\
| Geometry Group 15:                                        |
| 0x0181 Keychron Q8 ANSI                                   |
| 0x0183 Keychron Q8 ISO                                    |
| 0x0381 Keychron V8 ANSI                                   |
| 0x0680 Keychron Q8 Pro ANSI                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry q8_ansi_0181_entries[] =
{
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  4,  2,  4,  3 },
    {  4,  3,  4,  4 },
    {  4,  9,  4, 11 },
    {  4, 12,  4, 13 },
    {  4, 13,  4, 14 },
    {  4, 14,  4, 15 },
};

static const keychron_patch q8_ansi_0181_patch =
{
    KEYCHRON_Q8_ANSI_0181_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q8_ansi_0181_entries),
    q8_ansi_0181_entries
};

static const keychron_patch q8_iso_0183_patch =
{
    KEYCHRON_Q8_ISO_0183_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q8_ansi_0181_entries),
    q8_ansi_0181_entries
};

static const keychron_patch v8_ansi_patch =
{
    KEYCHRON_V8_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q8_ansi_0181_entries),
    q8_ansi_0181_entries
};

static const keychron_patch q8_pro_ansi_patch =
{
    KEYCHRON_Q8_PRO_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q8_ansi_0181_entries),
    q8_ansi_0181_entries
};

/*---------------------------------------------------------*\
| Geometry Group 16:                                        |
| 0x01A1 Keychron Q10 ANSI                                  |
| 0x03A1 Keychron V10 ANSI                                  |
\*---------------------------------------------------------*/
static const keychron_patch_entry q10_ansi_entries[] =
{
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  1, 15,  1, 16 },
    {  2, 15,  2, 16 },
    {  3, 15,  3, 16 },
    {  5,  3,  5,  4 },
    {  5,  4,  5,  5 },
    {  5,  9,  5, 10 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
};

static const keychron_patch q10_ansi_patch =
{
    KEYCHRON_Q10_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q10_ansi_entries),
    q10_ansi_entries
};

static const keychron_patch v10_ansi_patch =
{
    KEYCHRON_V10_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q10_ansi_entries),
    q10_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 17:                                        |
| 0x01A3 Keychron Q10 ISO                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry q10_iso_entries[] =
{
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  1, 15,  1, 16 },
    {  2, 15,  2, 16 },
    {  3, 15,  3, 16 },
    {  5,  7,  5,  5 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
};

static const keychron_patch q10_iso_patch =
{
    KEYCHRON_Q10_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q10_iso_entries),
    q10_iso_entries
};

/*---------------------------------------------------------*\
| Geometry Group 18:                                        |
| 0x01B1 Keychron Q65 ANSI                                  |
\*---------------------------------------------------------*/
static const keychron_patch_entry q65_ansi_entries[] =
{
    {  2, 14,  2, 13 },
    {  3,  1,  3,  2 },
};

static const keychron_patch q65_ansi_patch =
{
    KEYCHRON_Q65_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q65_ansi_entries),
    q65_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 19:                                        |
| 0x01C0 Keychron Q60 ANSI                                  |
\*---------------------------------------------------------*/
static const keychron_patch_entry q60_ansi_entries[] =
{
    {  2, 13,  2, 12 },
    {  4,  0,  4,  1 },
    {  4,  1,  4,  2 },
    {  4, 10,  4, 11 },
    {  4, 11,  4, 12 },
    {  4, 12,  0, 14 },
};

static const keychron_patch q60_ansi_patch =
{
    KEYCHRON_Q60_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q60_ansi_entries),
    q60_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 20:                                        |
| 0x01D1 Keychron Q12 ANSI                                  |
\*---------------------------------------------------------*/
static const keychron_patch_entry q12_ansi_entries[] =
{
    {  0,  0,  0,  4 },
    {  0,  1,  0,  5 },
    {  0,  2,  0,  6 },
    {  0,  3,  0,  7 },
    {  0,  4,  0,  8 },
    {  0,  5,  0, 10 },
    {  0,  6,  0, 11 },
    {  0,  7,  0, 12 },
    {  0,  8,  0, 13 },
    {  0,  9,  0, 14 },
    {  0, 10,  0, 15 },
    {  0, 11,  0, 16 },
    {  0, 12,  0, 17 },
    {  0, 13,  0, 18 },
    {  0, 14,  0, 19 },
    {  0, 16,  0,  1 },
    {  0, 17,  0,  2 },
    {  1,  0,  1,  4 },
    {  1,  1,  1,  5 },
    {  1,  2,  1,  6 },
    {  1,  3,  1,  7 },
    {  1,  4,  1,  8 },
    {  1,  5,  1,  9 },
    {  1,  6,  1, 10 },
    {  1,  7,  1, 11 },
    {  1,  8,  1, 12 },
    {  1,  9,  1, 13 },
    {  1, 10,  1, 14 },
    {  1, 11,  1, 15 },
    {  1, 12,  1, 16 },
    {  1, 13,  1, 17 },
    {  1, 14,  1, 19 },
    {  1, 15,  1,  0 },
    {  1, 16,  1,  1 },
    {  1, 17,  1,  2 },
    {  2,  0,  2,  4 },
    {  2,  1,  2,  5 },
    {  2,  2,  2,  6 },
    {  2,  3,  2,  7 },
    {  2,  4,  2,  8 },
    {  2,  5,  2,  9 },
    {  2,  6,  2, 10 },
    {  2,  7,  2, 11 },
    {  2,  8,  2, 12 },
    {  2,  9,  2, 13 },
    {  2, 10,  2, 14 },
    {  2, 11,  2, 15 },
    {  2, 12,  2, 16 },
    {  2, 13,  2, 17 },
    {  2, 14,  2, 19 },
    {  2, 15,  2,  0 },
    {  2, 16,  2,  1 },
    {  2, 17,  2,  2 },
    {  3,  0,  3,  4 },
    {  3,  1,  3,  6 },
    {  3,  2,  3,  7 },
    {  3,  3,  3,  8 },
    {  3,  4,  3,  9 },
    {  3,  5,  3, 10 },
    {  3,  6,  3, 11 },
    {  3,  7,  3, 12 },
    {  3,  8,  3, 13 },
    {  3,  9,  3, 14 },
    {  3, 10,  3, 15 },
    {  3, 11,  3, 16 },
    {  3, 12,  4,  3 },
    {  3, 13,  3, 17 },
    {  3, 14,  3, 19 },
    {  3, 15,  3,  0 },
    {  3, 16,  3,  1 },
    {  3, 17,  3,  2 },
    {  4,  0,  4,  4 },
    {  4,  2,  4,  6 },
    {  4,  3,  4,  7 },
    {  4,  4,  4,  8 },
    {  4,  5,  4,  9 },
    {  4,  6,  4, 10 },
    {  4,  7,  4, 11 },
    {  4,  8,  4, 12 },
    {  4,  9,  4, 13 },
    {  4, 10,  4, 14 },
    {  4, 11,  4, 15 },
    {  4, 13,  4, 16 },
    {  4, 14,  4, 18 },
    {  4, 15,  4,  0 },
    {  4, 16,  4,  1 },
    {  4, 17,  4,  2 },
    {  5,  0,  5,  4 },
    {  5,  1,  5,  5 },
    {  5,  2,  5,  6 },
    {  5,  5,  1,  3 },
    {  5,  6,  5,  9 },
    {  5,  7,  2,  3 },
    {  5,  9,  5, 13 },
    {  5, 10,  5, 14 },
    {  5, 11,  5, 15 },
    {  5, 12,  5, 17 },
    {  5, 13,  5, 18 },
    {  5, 14,  5, 19 },
    {  5, 15,  5,  0 },
    {  5, 16,  0,  3 },
    {  5, 17,  5,  2 },
};

static const keychron_patch q12_ansi_patch =
{
    KEYCHRON_Q12_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q12_ansi_entries),
    q12_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 21:                                        |
| 0x01D3 Keychron Q12 ISO                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry q12_iso_entries[] =
{
    {  0,  0,  0,  4 },
    {  0,  1,  0,  5 },
    {  0,  2,  0,  6 },
    {  0,  3,  0,  7 },
    {  0,  4,  0,  8 },
    {  0,  5,  0,  9 },
    {  0,  6,  0, 10 },
    {  0,  7,  0, 11 },
    {  0,  8,  0, 12 },
    {  0,  9,  0, 14 },
    {  0, 10,  0, 15 },
    {  0, 11,  0, 16 },
    {  0, 12,  0, 17 },
    {  0, 13,  0, 18 },
    {  0, 14,  0, 19 },
    {  0, 16,  0,  1 },
    {  0, 17,  0,  2 },
    {  1,  0,  1,  4 },
    {  1,  1,  1,  5 },
    {  1,  2,  1,  6 },
    {  1,  3,  1,  7 },
    {  1,  4,  1,  8 },
    {  1,  5,  1,  9 },
    {  1,  6,  1, 10 },
    {  1,  7,  1, 11 },
    {  1,  8,  1, 12 },
    {  1,  9,  1, 13 },
    {  1, 10,  1, 14 },
    {  1, 11,  1, 15 },
    {  1, 12,  1, 16 },
    {  1, 13,  1, 17 },
    {  1, 14,  1, 19 },
    {  1, 15,  1,  0 },
    {  1, 16,  1,  1 },
    {  1, 17,  1,  2 },
    {  2,  0,  2,  4 },
    {  2,  1,  2,  5 },
    {  2,  2,  2,  6 },
    {  2,  3,  2,  7 },
    {  2,  4,  2,  8 },
    {  2,  5,  2,  9 },
    {  2,  6,  2, 10 },
    {  2,  7,  2, 11 },
    {  2,  8,  2, 12 },
    {  2,  9,  2, 13 },
    {  2, 10,  2, 14 },
    {  2, 11,  2, 15 },
    {  2, 12,  2, 16 },
    {  2, 13,  2, 17 },
    {  2, 14,  2, 19 },
    {  2, 15,  2,  0 },
    {  2, 16,  2,  1 },
    {  2, 17,  2,  2 },
    {  3,  0,  3,  4 },
    {  3,  1,  3,  6 },
    {  3,  2,  3,  7 },
    {  3,  3,  3,  8 },
    {  3,  4,  3,  9 },
    {  3,  5,  3, 10 },
    {  3,  6,  3, 11 },
    {  3,  7,  3, 12 },
    {  3,  8,  3, 13 },
    {  3,  9,  3, 14 },
    {  3, 10,  3, 15 },
    {  3, 11,  3, 16 },
    {  3, 12,  4,  3 },
    {  3, 13,  3, 17 },
    {  3, 14,  3, 19 },
    {  3, 15,  3,  0 },
    {  3, 16,  3,  1 },
    {  3, 17,  3,  2 },
    {  4,  0,  4,  4 },
    {  4,  1,  4,  5 },
    {  4,  2,  4,  6 },
    {  4,  3,  4,  7 },
    {  4,  4,  4,  8 },
    {  4,  5,  4,  9 },
    {  4,  6,  4, 10 },
    {  4,  7,  4, 11 },
    {  4,  8,  4, 12 },
    {  4,  9,  4, 13 },
    {  4, 10,  4, 14 },
    {  4, 11,  4, 15 },
    {  4, 13,  4, 16 },
    {  4, 14,  4, 18 },
    {  4, 15,  4,  0 },
    {  4, 16,  4,  1 },
    {  4, 17,  4,  2 },
    {  5,  0,  5,  3 },
    {  5,  1,  5,  4 },
    {  5,  2,  5,  5 },
    {  5,  5,  1,  3 },
    {  5,  6,  5,  9 },
    {  5,  7,  2,  3 },
    {  5,  9,  5, 13 },
    {  5, 10,  5, 14 },
    {  5, 11,  5, 15 },
    {  5, 12,  5, 17 },
    {  5, 13,  5, 18 },
    {  5, 14,  5, 19 },
    {  5, 15,  5,  0 },
    {  5, 16,  0,  3 },
    {  5, 17,  5,  1 },
};

static const keychron_patch q12_iso_patch =
{
    KEYCHRON_Q12_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q12_iso_entries),
    q12_iso_entries
};

/*---------------------------------------------------------*\
| Geometry Group 22:                                        |
| 0x0211 Keychron K1 Pro ISO RGB                            |
| 0x0250 Keychron K5 Pro ANSI RGB                           |
| 0x0251 Keychron K5 Pro ISO RGB                            |
| 0x0280 Keychron K8 Pro RGB                                |
| 0x02A0 Keychron K10 Pro ANSI RGB                          |
| 0x02A1 Keychron K10 Pro ISO RGB                           |
| 0x0A50 Keychron K5 Max ANSI RGB                           |
| 0x0A51 Keychron K5 Max ISO RGB                            |
| 0x1680 Keychron K8 Ultra 8K ANSI                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry k1_pro_iso_rgb_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  3, 13,  3, 12 },
    {  4, 13,  4, 12 },
};

static const keychron_patch k1_pro_iso_rgb_patch =
{
    KEYCHRON_K1_PRO_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k1_pro_iso_rgb_entries),
    k1_pro_iso_rgb_entries
};

static const keychron_patch k5_pro_ansi_rgb_patch =
{
    KEYCHRON_K5_PRO_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k1_pro_iso_rgb_entries),
    k1_pro_iso_rgb_entries
};

static const keychron_patch k5_pro_iso_rgb_patch =
{
    KEYCHRON_K5_PRO_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k1_pro_iso_rgb_entries),
    k1_pro_iso_rgb_entries
};

static const keychron_patch k8_pro_rgb_patch =
{
    KEYCHRON_K8_PRO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k1_pro_iso_rgb_entries),
    k1_pro_iso_rgb_entries
};

static const keychron_patch k10_pro_ansi_rgb_patch =
{
    KEYCHRON_K10_PRO_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k1_pro_iso_rgb_entries),
    k1_pro_iso_rgb_entries
};

static const keychron_patch k10_pro_iso_rgb_patch =
{
    KEYCHRON_K10_PRO_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k1_pro_iso_rgb_entries),
    k1_pro_iso_rgb_entries
};

static const keychron_patch k5_max_ansi_rgb_patch =
{
    KEYCHRON_K5_MAX_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k1_pro_iso_rgb_entries),
    k1_pro_iso_rgb_entries
};

static const keychron_patch k5_max_iso_rgb_patch =
{
    KEYCHRON_K5_MAX_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k1_pro_iso_rgb_entries),
    k1_pro_iso_rgb_entries
};

static const keychron_patch k8_ultra_8k_ansi_patch =
{
    KEYCHRON_K8_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k1_pro_iso_rgb_entries),
    k1_pro_iso_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 23:                                        |
| 0x0220 Keychron K2 Pro RGB                                |
| 0x0A20 Keychron K2 Max ANSI RGB                           |
| 0x0A21 Keychron K2 Max ISO RGB                            |
| 0x0D40 Keychron K4 V3 ANSI RGB                            |
\*---------------------------------------------------------*/
static const keychron_patch_entry k2_pro_rgb_entries[] =
{
    {  3, 13,  3, 12 },
    {  4, 13,  4, 14 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
};

static const keychron_patch k2_pro_rgb_patch =
{
    KEYCHRON_K2_PRO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_pro_rgb_entries),
    k2_pro_rgb_entries
};

static const keychron_patch k2_max_ansi_rgb_patch =
{
    KEYCHRON_K2_MAX_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_pro_rgb_entries),
    k2_pro_rgb_entries
};

static const keychron_patch k2_max_iso_rgb_patch =
{
    KEYCHRON_K2_MAX_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_pro_rgb_entries),
    k2_pro_rgb_entries
};

static const keychron_patch k4_v3_ansi_rgb_patch =
{
    KEYCHRON_K4_V3_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_pro_rgb_entries),
    k2_pro_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 24:                                        |
| 0x0230 Keychron K3 Pro                                    |
| 0x0231 Keychron K3 Pro                                    |
| 0x0408 Keychron J4                                        |
| 0x0412 Keychron J9                                        |
| 0x0413 Keychron J9 ISO                                    |
| 0x041A Keychron Keycheron J7                              |
| 0x042B Keychron J9 Ultra 8K ANSI                          |
| 0x0437 Keychron C3 Pro 8K ANSI                            |
| 0x043A Keychron C3 Max ANSI                               |
| 0x0453 Keychron J5 ANSI                                   |
| 0x0457 Keychron J4 ANSI RGB                               |
| 0x0479 Keychron Z7 MAX ANSI RGB                           |
| 0x051D Keychron C1 Pro 8K ISO RGB                         |
| 0x0521 Keychron C1 Pro 8K ANSI RGB                        |
| 0x0530 Keychron C3 Pro 8K ANSI                            |
| 0x0531 Keychron C3 Pro 8K ISO RGB                         |
| 0x0830 Keychron Q3 Max ANSI                               |
| 0x0A10 Keychron K1 Max ANSI RGB                           |
| 0x0A11 Keychron K1 Max ISO RGB                            |
| 0x0A30 Keychron K3 Max ANSI RGB                           |
| 0x0A31 Keychron K3 Max ISO RGB                            |
| 0x0A3F Keychron K3 Max ANSI RGB                           |
| 0x0A80 Keychron K8 Max ANSI RGB                           |
| 0x0A81 Keychron K8 Max ISO RGB                            |
| 0x0AD0 Keychron K13 Max ANSI RGB                          |
| 0x0AD1 Keychron K13 Max ISO RGB                           |
| 0x0D10 Keychron K1 V6 ANSI RGB                            |
| 0x0D11 Keychron K1 V6 ISO RGB                             |
| 0x0D20 Keychron K2 V3 ANSI RGB                            |
| 0x0D21 Keychron K2 V3 ISO RGB                             |
| 0x0D30 Keychron K3 V3 ANSI RGB                            |
| 0x0D31 Keychron K3 V3 ISO RGB                             |
| 0x0D3B Keychron K3 Max SE RGB                             |
| 0x0D80 Keychron K8 V2 ANSI RGB                            |
| 0x0F30 Keychron V3 8K ANSI                                |
| 0x0F31 Keychron V3 8K ISO                                 |
| 0x1230 Keychron Q3 Ultra 8K ANSI                          |
| 0x1231 Keychron Q3 Ultra 8K ISO RGB                       |
| 0x1610 Keychron K1 Ultra 8K                               |
| 0x1611 Keychron K1 Ultra 8K ISO RGB                       |
| 0x1630 Keychron K3 Ultra 8K ANSI                          |
| 0x1631 Keychron K3 Ultra 8K ISO                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k3_pro_0230_entries[] =
{
    {  3, 13,  3, 12 },
    {  4, 13,  4, 12 },
};

static const keychron_patch k3_pro_0230_patch =
{
    KEYCHRON_K3_PRO_0230_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k3_pro_0231_patch =
{
    KEYCHRON_K3_PRO_0231_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch j4_patch =
{
    KEYCHRON_J4_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch j9_patch =
{
    KEYCHRON_J9_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch j9_iso_patch =
{
    KEYCHRON_J9_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch keycheron_j7_patch =
{
    KEYCHRON_KEYCHERON_J7_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch j9_ultra_8k_ansi_patch =
{
    KEYCHRON_J9_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch c3_pro_8k_ansi_0437_patch =
{
    KEYCHRON_C3_PRO_8K_ANSI_0437_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch c3_max_ansi_patch =
{
    KEYCHRON_C3_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch j5_ansi_patch =
{
    KEYCHRON_J5_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch j4_ansi_rgb_patch =
{
    KEYCHRON_J4_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch z7_max_ansi_rgb_patch =
{
    KEYCHRON_Z7_MAX_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch c1_pro_8k_iso_rgb_patch =
{
    KEYCHRON_C1_PRO_8K_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch c1_pro_8k_ansi_rgb_patch =
{
    KEYCHRON_C1_PRO_8K_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch c3_pro_8k_ansi_0530_patch =
{
    KEYCHRON_C3_PRO_8K_ANSI_0530_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch c3_pro_8k_iso_rgb_patch =
{
    KEYCHRON_C3_PRO_8K_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch q3_max_ansi_patch =
{
    KEYCHRON_Q3_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k1_max_ansi_rgb_patch =
{
    KEYCHRON_K1_MAX_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k1_max_iso_rgb_patch =
{
    KEYCHRON_K1_MAX_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k3_max_ansi_rgb_0a30_patch =
{
    KEYCHRON_K3_MAX_ANSI_RGB_0A30_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k3_max_iso_rgb_patch =
{
    KEYCHRON_K3_MAX_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k3_max_ansi_rgb_0a3f_patch =
{
    KEYCHRON_K3_MAX_ANSI_RGB_0A3F_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k8_max_ansi_rgb_patch =
{
    KEYCHRON_K8_MAX_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k8_max_iso_rgb_patch =
{
    KEYCHRON_K8_MAX_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k13_max_ansi_rgb_patch =
{
    KEYCHRON_K13_MAX_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k13_max_iso_rgb_patch =
{
    KEYCHRON_K13_MAX_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k1_v6_ansi_rgb_patch =
{
    KEYCHRON_K1_V6_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k1_v6_iso_rgb_patch =
{
    KEYCHRON_K1_V6_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k2_v3_ansi_rgb_patch =
{
    KEYCHRON_K2_V3_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k2_v3_iso_rgb_patch =
{
    KEYCHRON_K2_V3_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k3_v3_ansi_rgb_patch =
{
    KEYCHRON_K3_V3_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k3_v3_iso_rgb_patch =
{
    KEYCHRON_K3_V3_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k3_max_se_rgb_patch =
{
    KEYCHRON_K3_MAX_SE_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k8_v2_ansi_rgb_patch =
{
    KEYCHRON_K8_V2_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch v3_8k_ansi_patch =
{
    KEYCHRON_V3_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch v3_8k_iso_patch =
{
    KEYCHRON_V3_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch q3_ultra_8k_ansi_patch =
{
    KEYCHRON_Q3_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch q3_ultra_8k_iso_rgb_patch =
{
    KEYCHRON_Q3_ULTRA_8K_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k1_ultra_8k_patch =
{
    KEYCHRON_K1_ULTRA_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k1_ultra_8k_iso_rgb_patch =
{
    KEYCHRON_K1_ULTRA_8K_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k3_ultra_8k_ansi_patch =
{
    KEYCHRON_K3_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

static const keychron_patch k3_ultra_8k_iso_patch =
{
    KEYCHRON_K3_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_pro_0230_entries),
    k3_pro_0230_entries
};

/*---------------------------------------------------------*\
| Geometry Group 25:                                        |
| 0x0240 Keychron K4 Pro ANSI RGB                           |
| 0x0A40 Keychron K4 MAX ANSI RGB                           |
| 0x0A41 Keychron K4 MAX ISO RGB                            |
\*---------------------------------------------------------*/
static const keychron_patch_entry k4_pro_ansi_rgb_entries[] =
{
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  3, 13,  3, 12 },
    {  3, 14,  3, 15 },
    {  3, 15,  3, 16 },
    {  3, 16,  3, 17 },
    {  3, 17,  0, 18 },
    {  4, 13,  4, 14 },
    {  4, 14,  4, 15 },
    {  4, 15,  4, 16 },
    {  4, 16,  4, 17 },
    {  4, 17,  4, 18 },
};

static const keychron_patch k4_pro_ansi_rgb_patch =
{
    KEYCHRON_K4_PRO_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k4_pro_ansi_rgb_entries),
    k4_pro_ansi_rgb_entries
};

static const keychron_patch k4_max_ansi_rgb_patch =
{
    KEYCHRON_K4_MAX_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k4_pro_ansi_rgb_entries),
    k4_pro_ansi_rgb_entries
};

static const keychron_patch k4_max_iso_rgb_patch =
{
    KEYCHRON_K4_MAX_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k4_pro_ansi_rgb_entries),
    k4_pro_ansi_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 26:                                        |
| 0x0260 Keychron K6 Pro RGB                                |
| 0x0261 Keychron K6 Pro ISO RGB                            |
| 0x0A76 Keychron K7 Max V2 JIS RGB                         |
| 0x0B20 Keychron Q2 HE ANSI                                |
| 0x0E60 Keychron K6 HE ANSI                                |
| 0x0E61 Keychron K6 HE ISO RGB                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry k6_pro_rgb_entries[] =
{
    {  2, 13,  2, 12 },
};

static const keychron_patch k6_pro_rgb_patch =
{
    KEYCHRON_K6_PRO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k6_pro_rgb_entries),
    k6_pro_rgb_entries
};

static const keychron_patch k6_pro_iso_rgb_patch =
{
    KEYCHRON_K6_PRO_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k6_pro_rgb_entries),
    k6_pro_rgb_entries
};

static const keychron_patch k7_max_v2_jis_rgb_patch =
{
    KEYCHRON_K7_MAX_V2_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k6_pro_rgb_entries),
    k6_pro_rgb_entries
};

static const keychron_patch q2_he_ansi_patch =
{
    KEYCHRON_Q2_HE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k6_pro_rgb_entries),
    k6_pro_rgb_entries
};

static const keychron_patch k6_he_ansi_patch =
{
    KEYCHRON_K6_HE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k6_pro_rgb_entries),
    k6_pro_rgb_entries
};

static const keychron_patch k6_he_iso_rgb_patch =
{
    KEYCHRON_K6_HE_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k6_pro_rgb_entries),
    k6_pro_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 27:                                        |
| 0x0282 Keychron K8 Pro JIS RGB                            |
\*---------------------------------------------------------*/
static const keychron_patch_entry k8_pro_jis_rgb_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 11 },
    {  0, 10,  0, 12 },
    {  0, 11,  0, 13 },
    {  0, 12,  0, 14 },
    {  0, 13,  1, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  3, 13,  3, 12 },
    {  4, 15,  4, 16 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
};

static const keychron_patch k8_pro_jis_rgb_patch =
{
    KEYCHRON_K8_PRO_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k8_pro_jis_rgb_entries),
    k8_pro_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 28:                                        |
| 0x02A2 Keychron K10 Pro JIS RGB                           |
| 0x0A52 Keychron K5 Max JIS RGB                            |
| 0x0A58 Keychron K5 Max JIS RGB                            |
\*---------------------------------------------------------*/
static const keychron_patch_entry k10_pro_jis_rgb_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 11 },
    {  0, 10,  0, 12 },
    {  0, 11,  0, 13 },
    {  0, 12,  0, 14 },
    {  0, 13,  1, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  0, 18,  0, 19 },
    {  0, 19,  0, 20 },
    {  0, 20,  0, 21 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  1, 19,  1, 20 },
    {  1, 20,  1, 21 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  2, 19,  2, 20 },
    {  2, 20,  2, 21 },
    {  3, 13,  3, 12 },
    {  3, 17,  3, 18 },
    {  3, 18,  3, 19 },
    {  3, 19,  3, 20 },
    {  4, 15,  4, 16 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  4, 19,  4, 20 },
    {  4, 20,  4, 21 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 17,  5, 18 },
    {  5, 19,  5, 20 },
};

static const keychron_patch k10_pro_jis_rgb_patch =
{
    KEYCHRON_K10_PRO_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k10_pro_jis_rgb_entries),
    k10_pro_jis_rgb_entries
};

static const keychron_patch k5_max_jis_rgb_0a52_patch =
{
    KEYCHRON_K5_MAX_JIS_RGB_0A52_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k10_pro_jis_rgb_entries),
    k10_pro_jis_rgb_entries
};

static const keychron_patch k5_max_jis_rgb_0a58_patch =
{
    KEYCHRON_K5_MAX_JIS_RGB_0A58_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k10_pro_jis_rgb_entries),
    k10_pro_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 29:                                        |
| 0x0317 Keychron V1 BR                                     |
| 0x041D Keychron Z6-75 Ultra 8K RGB                        |
| 0x044D Keychron Z5-75 Ultra 8K ANSI                       |
| 0x0451 Keychron J10 Ultra 8K ANSI                         |
| 0x0452 Keychron J10 Ultra 8K ANSI                         |
| 0x0536 Keychron C3 Pro 8K ABTN2                           |
| 0x0A22 Keychron K2 Max JIS RGB                            |
| 0x1620 Keychron K2 Ultra 8K ANSI                          |
| 0x1621 Keychron K2 Ultra 8K ISO                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry v1_br_entries[] =
{
    {  3, 13,  3, 12 },
};

static const keychron_patch v1_br_patch =
{
    KEYCHRON_V1_BR_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v1_br_entries),
    v1_br_entries
};

static const keychron_patch z6_75_ultra_8k_rgb_patch =
{
    KEYCHRON_Z6_75_ULTRA_8K_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v1_br_entries),
    v1_br_entries
};

static const keychron_patch z5_75_ultra_8k_ansi_patch =
{
    KEYCHRON_Z5_75_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v1_br_entries),
    v1_br_entries
};

static const keychron_patch j10_ultra_8k_ansi_0451_patch =
{
    KEYCHRON_J10_ULTRA_8K_ANSI_0451_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v1_br_entries),
    v1_br_entries
};

static const keychron_patch j10_ultra_8k_ansi_0452_patch =
{
    KEYCHRON_J10_ULTRA_8K_ANSI_0452_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v1_br_entries),
    v1_br_entries
};

static const keychron_patch c3_pro_8k_abtn2_patch =
{
    KEYCHRON_C3_PRO_8K_ABTN2_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v1_br_entries),
    v1_br_entries
};

static const keychron_patch k2_max_jis_rgb_patch =
{
    KEYCHRON_K2_MAX_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v1_br_entries),
    v1_br_entries
};

static const keychron_patch k2_ultra_8k_ansi_patch =
{
    KEYCHRON_K2_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v1_br_entries),
    v1_br_entries
};

static const keychron_patch k2_ultra_8k_iso_patch =
{
    KEYCHRON_K2_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v1_br_entries),
    v1_br_entries
};

/*---------------------------------------------------------*\
| Geometry Group 30:                                        |
| 0x0327 Keychron V2 BR                                     |
\*---------------------------------------------------------*/
static const keychron_patch_entry v2_br_entries[] =
{
    {  1, 14,  1, 15 },
    {  2, 12,  4, 14 },
    {  2, 13,  2, 12 },
    {  2, 14,  2, 15 },
    {  4, 14,  4, 15 },
};

static const keychron_patch v2_br_patch =
{
    KEYCHRON_V2_BR_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v2_br_entries),
    v2_br_entries
};

/*---------------------------------------------------------*\
| Geometry Group 31:                                        |
| 0x0337 Keychron V3 BR                                     |
| 0x1032 Keychron Q3 HE 8K JIS                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry v3_br_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
};

static const keychron_patch v3_br_patch =
{
    KEYCHRON_V3_BR_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v3_br_entries),
    v3_br_entries
};

static const keychron_patch q3_he_8k_jis_patch =
{
    KEYCHRON_Q3_HE_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v3_br_entries),
    v3_br_entries
};

/*---------------------------------------------------------*\
| Geometry Group 32:                                        |
| 0x0350 Keychron V5 ANSI                                   |
| 0x0352 Keychron V5 ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry v5_ansi_0350_entries[] =
{
    {  2, 14,  2, 18 },
    {  3, 12,  0, 18 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 18 },
    {  4, 13,  4, 12 },
    {  5,  5,  4, 18 },
};

static const keychron_patch v5_ansi_0350_patch =
{
    KEYCHRON_V5_ANSI_0350_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_ansi_0350_entries),
    v5_ansi_0350_entries
};

static const keychron_patch v5_iso_0352_patch =
{
    KEYCHRON_V5_ISO_0352_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_ansi_0350_entries),
    v5_ansi_0350_entries
};

/*---------------------------------------------------------*\
| Geometry Group 33:                                        |
| 0x0351 Keychron V5 ANSI                                   |
| 0x0353 Keychron V5 ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry v5_ansi_0351_entries[] =
{
    {  2, 14,  2, 18 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 18 },
    {  4, 13,  4, 12 },
    {  5,  5,  4, 18 },
};

static const keychron_patch v5_ansi_0351_patch =
{
    KEYCHRON_V5_ANSI_0351_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_ansi_0351_entries),
    v5_ansi_0351_entries
};

static const keychron_patch v5_iso_0353_patch =
{
    KEYCHRON_V5_ISO_0353_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_ansi_0351_entries),
    v5_ansi_0351_entries
};

/*---------------------------------------------------------*\
| Geometry Group 34:                                        |
| 0x0363 Keychron V6 ISO                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry v6_iso_0363_entries[] =
{
    {  0, 17,  1, 19 },
    {  0, 18,  1, 20 },
    {  0, 19,  0, 17 },
    {  1, 19,  0, 18 },
    {  2, 19,  0, 19 },
    {  3, 13,  3, 12 },
    {  3, 14,  2, 19 },
    {  3, 15,  2, 20 },
    {  3, 16,  3, 19 },
    {  3, 19,  0, 20 },
    {  4, 13,  4, 12 },
    {  4, 14,  4, 19 },
    {  4, 16,  4, 20 },
    {  4, 19,  0, 13 },
    {  5, 18,  5, 19 },
};

static const keychron_patch v6_iso_0363_patch =
{
    KEYCHRON_V6_ISO_0363_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v6_iso_0363_entries),
    v6_iso_0363_entries
};

/*---------------------------------------------------------*\
| Geometry Group 35:                                        |
| 0x0367 Keychron V6 BR                                     |
\*---------------------------------------------------------*/
static const keychron_patch_entry v6_br_entries[] =
{
    {  3, 13,  0, 20 },
    {  3, 14,  1, 20 },
    {  3, 15,  2, 20 },
    {  3, 16,  4, 20 },
    {  5, 18,  5, 17 },
};

static const keychron_patch v6_br_patch =
{
    KEYCHRON_V6_BR_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v6_br_entries),
    v6_br_entries
};

/*---------------------------------------------------------*\
| Geometry Group 36:                                        |
| 0x03A3 Keychron V10 ISO                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry v10_iso_entries[] =
{
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  1, 15,  1, 16 },
    {  2, 15,  2, 16 },
    {  3, 15,  3, 16 },
    {  5,  3,  5,  4 },
    {  5,  4,  5,  5 },
    {  5,  9,  5, 11 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
};

static const keychron_patch v10_iso_patch =
{
    KEYCHRON_V10_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v10_iso_entries),
    v10_iso_entries
};

/*---------------------------------------------------------*\
| Geometry Group 37:                                        |
| 0x0402 Keychron P1 HE DAYZ                                |
| 0x0403 Keychron P1 HE DayZ ISO                            |
| 0x0B10 Keychron Q1 HE ANSI                                |
| 0x0B11 Keychron Q1 HE ISO                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry p1_he_dayz_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3, 13,  3, 15 },
    {  4, 12,  4, 11 },
    {  4, 13,  4, 12 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch p1_he_dayz_patch =
{
    KEYCHRON_P1_HE_DAYZ_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(p1_he_dayz_entries),
    p1_he_dayz_entries
};

static const keychron_patch p1_he_dayz_iso_patch =
{
    KEYCHRON_P1_HE_DAYZ_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(p1_he_dayz_entries),
    p1_he_dayz_entries
};

static const keychron_patch q1_he_ansi_patch =
{
    KEYCHRON_Q1_HE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(p1_he_dayz_entries),
    p1_he_dayz_entries
};

static const keychron_patch q1_he_iso_patch =
{
    KEYCHRON_Q1_HE_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(p1_he_dayz_entries),
    p1_he_dayz_entries
};

/*---------------------------------------------------------*\
| Geometry Group 38:                                        |
| 0x0405 Keychron J1                                        |
| 0x0406 Keychron J2                                        |
| 0x0456 Keychron J2 ANSI                                   |
| 0x0463 Keychron CM2-H1 ANSI                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry j1_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  1, 14,  1, 13 },
    {  2, 14,  2, 13 },
    {  5, 10,  5,  9 },
    {  5, 11,  5, 10 },
    {  5, 12,  5, 11 },
};

static const keychron_patch j1_patch =
{
    KEYCHRON_J1_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j1_entries),
    j1_entries
};

static const keychron_patch j2_patch =
{
    KEYCHRON_J2_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j1_entries),
    j1_entries
};

static const keychron_patch j2_ansi_patch =
{
    KEYCHRON_J2_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j1_entries),
    j1_entries
};

static const keychron_patch cm2_h1_ansi_patch =
{
    KEYCHRON_CM2_H1_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j1_entries),
    j1_entries
};

/*---------------------------------------------------------*\
| Geometry Group 39:                                        |
| 0x0409 Keychron J5                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry j5_entries[] =
{
    {  0, 18,  0, 19 },
    {  1, 18,  1, 19 },
    {  2, 18,  2, 19 },
    {  3, 13,  3, 18 },
    {  3, 14,  2, 18 },
    {  3, 15,  1, 18 },
    {  3, 16,  0, 18 },
    {  3, 18,  3, 19 },
    {  4, 13,  4, 18 },
    {  4, 14,  1, 20 },
    {  4, 16,  0, 20 },
    {  4, 18,  4, 19 },
    {  5,  8,  5, 20 },
    {  5,  9,  2, 20 },
    {  5, 18,  5, 19 },
};

static const keychron_patch j5_patch =
{
    KEYCHRON_J5_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j5_entries),
    j5_entries
};

/*---------------------------------------------------------*\
| Geometry Group 40:                                        |
| 0x040A Keychron Z6 Ultra                                  |
| 0x040D Keychron P6 Ultra 8K                               |
| 0x0420 Keychron P6 Ultra 8K ISO                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry z6_ultra_entries[] =
{
    {  5, 20,  4, 20 },
};

static const keychron_patch z6_ultra_patch =
{
    KEYCHRON_Z6_ULTRA_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(z6_ultra_entries),
    z6_ultra_entries
};

static const keychron_patch p6_ultra_8k_patch =
{
    KEYCHRON_P6_ULTRA_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(z6_ultra_entries),
    z6_ultra_entries
};

static const keychron_patch p6_ultra_8k_iso_patch =
{
    KEYCHRON_P6_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(z6_ultra_entries),
    z6_ultra_entries
};

/*---------------------------------------------------------*\
| Geometry Group 41:                                        |
| 0x040C Keychron J3 HE ANSI                                |
| 0x045E Keychron Keychroon J3 TMR                          |
| 0x045F Keychron Keychroon J3 TMR 静音轴                      |
\*---------------------------------------------------------*/
static const keychron_patch_entry j3_he_ansi_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  3, 13,  3, 12 },
    {  4, 13,  4, 12 },
    {  5, 16,  5, 15 },
};

static const keychron_patch j3_he_ansi_patch =
{
    KEYCHRON_J3_HE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j3_he_ansi_entries),
    j3_he_ansi_entries
};

static const keychron_patch keychroon_j3_tmr_045e_patch =
{
    KEYCHRON_KEYCHROON_J3_TMR_045E_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j3_he_ansi_entries),
    j3_he_ansi_entries
};

static const keychron_patch keychroon_j3_tmr_045f_patch =
{
    KEYCHRON_KEYCHROON_J3_TMR_045F_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j3_he_ansi_entries),
    j3_he_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 42:                                        |
| 0x0414 Keychron J9 JIS                                    |
| 0x1632 Keychron K3 Ultra 8K JIS                           |
| 0x1634 Keychron K3 Ultra 8K JIS                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry j9_jis_entries[] =
{
    {  3, 13,  3, 12 },
    {  4, 13,  4, 12 },
    {  4, 14,  4, 13 },
    {  4, 15,  4, 14 },
};

static const keychron_patch j9_jis_patch =
{
    KEYCHRON_J9_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j9_jis_entries),
    j9_jis_entries
};

static const keychron_patch k3_ultra_8k_jis_1632_patch =
{
    KEYCHRON_K3_ULTRA_8K_JIS_1632_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j9_jis_entries),
    j9_jis_entries
};

static const keychron_patch k3_ultra_8k_jis_1634_patch =
{
    KEYCHRON_K3_ULTRA_8K_JIS_1634_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j9_jis_entries),
    j9_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 43:                                        |
| 0x0416 Keychron J2 HE 8K ANSI                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry j2_he_8k_ansi_entries[] =
{
    {  0,  8,  0,  7 },
    {  0,  9,  0,  8 },
    {  1,  8,  1,  7 },
    {  1,  9,  1,  8 },
    {  1, 10,  1,  9 },
    {  1, 11,  1, 10 },
    {  1, 12,  1, 11 },
    {  1, 13,  1, 12 },
    {  1, 14,  1, 13 },
    {  2,  8,  2,  7 },
    {  2,  9,  2,  8 },
    {  2, 10,  2,  9 },
    {  2, 11,  2, 10 },
    {  2, 12,  2, 11 },
    {  2, 13,  2, 12 },
    {  2, 14,  2, 13 },
    {  3,  8,  3,  7 },
    {  3,  9,  3,  8 },
    {  3, 10,  3,  9 },
    {  3, 11,  3, 10 },
    {  3, 12,  3, 11 },
    {  3, 13,  3, 12 },
    {  4,  8,  4,  7 },
    {  4,  9,  4,  8 },
    {  4, 10,  4,  9 },
    {  4, 11,  4, 10 },
    {  4, 12,  4, 11 },
    {  4, 13,  4, 12 },
};

static const keychron_patch j2_he_8k_ansi_patch =
{
    KEYCHRON_J2_HE_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j2_he_8k_ansi_entries),
    j2_he_8k_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 44:                                        |
| 0x0417 Keychron J8 HE 8K ANSI                             |
| 0x045A Keychron eX75 HE 8K ANSI                           |
| 0x1010 Keychron Q1 HE 8K ANSI                             |
| 0x1016 Keychron Q1 HE 8K ANSI                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry j8_he_8k_ansi_entries[] =
{
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3, 14,  3, 15 },
    {  4, 13,  4, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch j8_he_8k_ansi_patch =
{
    KEYCHRON_J8_HE_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j8_he_8k_ansi_entries),
    j8_he_8k_ansi_entries
};

static const keychron_patch ex75_he_8k_ansi_patch =
{
    KEYCHRON_EX75_HE_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j8_he_8k_ansi_entries),
    j8_he_8k_ansi_entries
};

static const keychron_patch q1_he_8k_ansi_1010_patch =
{
    KEYCHRON_Q1_HE_8K_ANSI_1010_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j8_he_8k_ansi_entries),
    j8_he_8k_ansi_entries
};

static const keychron_patch q1_he_8k_ansi_1016_patch =
{
    KEYCHRON_Q1_HE_8K_ANSI_1016_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j8_he_8k_ansi_entries),
    j8_he_8k_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 45:                                        |
| 0x0418 Keychron J8 HE 8K ISO                              |
| 0x0B14 Keychron Q1 HE                                     |
| 0x0B15 Keychron Q1 HE V2 SILENT ANSI                      |
| 0x1011 Keychron Q1 HE 8K ISO RGB                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry j8_he_8k_iso_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3, 14,  3, 15 },
    {  4, 13,  4, 12 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch j8_he_8k_iso_patch =
{
    KEYCHRON_J8_HE_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j8_he_8k_iso_entries),
    j8_he_8k_iso_entries
};

static const keychron_patch q1_he_patch =
{
    KEYCHRON_Q1_HE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j8_he_8k_iso_entries),
    j8_he_8k_iso_entries
};

static const keychron_patch q1_he_v2_silent_ansi_patch =
{
    KEYCHRON_Q1_HE_V2_SILENT_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j8_he_8k_iso_entries),
    j8_he_8k_iso_entries
};

static const keychron_patch q1_he_8k_iso_rgb_patch =
{
    KEYCHRON_Q1_HE_8K_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j8_he_8k_iso_entries),
    j8_he_8k_iso_entries
};

/*---------------------------------------------------------*\
| Geometry Group 46:                                        |
| 0x0419 Keychron J8 HE 8K JIS                              |
| 0x0460 Keychron C75 TMR 8K JIS                            |
| 0x1012 Keychron Q1 HE 8K JIS                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry j8_he_8k_jis_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  2, 14,  2, 15 },
    {  3, 14,  3, 15 },
    {  5,  8,  5,  9 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch j8_he_8k_jis_patch =
{
    KEYCHRON_J8_HE_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j8_he_8k_jis_entries),
    j8_he_8k_jis_entries
};

static const keychron_patch c75_tmr_8k_jis_patch =
{
    KEYCHRON_C75_TMR_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j8_he_8k_jis_entries),
    j8_he_8k_jis_entries
};

static const keychron_patch q1_he_8k_jis_patch =
{
    KEYCHRON_Q1_HE_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j8_he_8k_jis_entries),
    j8_he_8k_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 47:                                        |
| 0x041B Keychron J14 HE                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry j14_he_entries[] =
{
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  3, 15,  3, 14 },
    {  3, 16,  3, 15 },
    {  3, 17,  3, 16 },
};

static const keychron_patch j14_he_patch =
{
    KEYCHRON_J14_HE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j14_he_entries),
    j14_he_entries
};

/*---------------------------------------------------------*\
| Geometry Group 48:                                        |
| 0x041C Keychron J12 HE                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry j12_he_entries[] =
{
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3, 13,  3, 15 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch j12_he_patch =
{
    KEYCHRON_J12_HE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j12_he_entries),
    j12_he_entries
};

/*---------------------------------------------------------*\
| Geometry Group 49:                                        |
| 0x041F Keychron J4 HE ANSI                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry j4_he_ansi_entries[] =
{
    {  3, 13,  0, 16 },
    {  3, 14,  1, 16 },
    {  3, 15,  2, 16 },
    {  4, 12,  4, 11 },
    {  4, 13,  4, 12 },
    {  5,  9,  5, 16 },
};

static const keychron_patch j4_he_ansi_patch =
{
    KEYCHRON_J4_HE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j4_he_ansi_entries),
    j4_he_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 50:                                        |
| 0x0429 Keychron J15 HE RGB                                |
| 0x1060 Keychron Q6 HE 8K                                  |
| 0x1061 Keychron Q6 HE 8K ISO                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry j15_he_rgb_entries[] =
{
    {  4, 13,  4, 12 },
    {  5, 18,  5, 19 },
};

static const keychron_patch j15_he_rgb_patch =
{
    KEYCHRON_J15_HE_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j15_he_rgb_entries),
    j15_he_rgb_entries
};

static const keychron_patch q6_he_8k_patch =
{
    KEYCHRON_Q6_HE_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j15_he_rgb_entries),
    j15_he_rgb_entries
};

static const keychron_patch q6_he_8k_iso_patch =
{
    KEYCHRON_Q6_HE_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j15_he_rgb_entries),
    j15_he_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 51:                                        |
| 0x042A Keychron X3 ANSI RGB                               |
| 0x0AAC Keychron K10 Max US V2 RGB                         |
| 0x0DA0 Keychron K10 V2 ANSI RGB                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry x3_ansi_rgb_entries[] =
{
    {  3, 13,  3, 12 },
    {  4, 13,  4, 12 },
    {  5, 18,  5, 19 },
};

static const keychron_patch x3_ansi_rgb_patch =
{
    KEYCHRON_X3_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(x3_ansi_rgb_entries),
    x3_ansi_rgb_entries
};

static const keychron_patch k10_max_us_v2_rgb_patch =
{
    KEYCHRON_K10_MAX_US_V2_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(x3_ansi_rgb_entries),
    x3_ansi_rgb_entries
};

static const keychron_patch k10_v2_ansi_rgb_patch =
{
    KEYCHRON_K10_V2_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(x3_ansi_rgb_entries),
    x3_ansi_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 52:                                        |
| 0x042D Keychron J3 SE                                     |
| 0x044E Keychron J3 Ultra 8K ANSI                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry j3_se_entries[] =
{
    {  3, 13,  3, 12 },
    {  3, 14,  2, 18 },
    {  4, 14,  4, 18 },
    {  5, 16,  5, 17 },
    {  5, 17,  1, 18 },
};

static const keychron_patch j3_se_patch =
{
    KEYCHRON_J3_SE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j3_se_entries),
    j3_se_entries
};

static const keychron_patch j3_ultra_8k_ansi_patch =
{
    KEYCHRON_J3_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j3_se_entries),
    j3_se_entries
};

/*---------------------------------------------------------*\
| Geometry Group 53:                                        |
| 0x042E Keychron Z5-98 Ultra 8K ANSI                       |
| 0x044C Keychron J11 Ultra 8K                              |
| 0x044F Keychron J11 Ultra 8K                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry z5_98_ultra_8k_ansi_entries[] =
{
    {  3, 13,  3, 12 },
    {  3, 18,  2, 18 },
    {  5, 18,  4, 18 },
};

static const keychron_patch z5_98_ultra_8k_ansi_patch =
{
    KEYCHRON_Z5_98_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(z5_98_ultra_8k_ansi_entries),
    z5_98_ultra_8k_ansi_entries
};

static const keychron_patch j11_ultra_8k_044c_patch =
{
    KEYCHRON_J11_ULTRA_8K_044C_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(z5_98_ultra_8k_ansi_entries),
    z5_98_ultra_8k_ansi_entries
};

static const keychron_patch j11_ultra_8k_044f_patch =
{
    KEYCHRON_J11_ULTRA_8K_044F_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(z5_98_ultra_8k_ansi_entries),
    z5_98_ultra_8k_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 54:                                        |
| 0x042F Keychron V1 HE ANSI                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry v1_he_ansi_entries[] =
{
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3, 13,  3, 15 },
    {  4, 12,  4, 11 },
    {  4, 13,  4, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch v1_he_ansi_patch =
{
    KEYCHRON_V1_HE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v1_he_ansi_entries),
    v1_he_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 55:                                        |
| 0x0442 Keychron Z11 Ultra L                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry z11_ultra_l_entries[] =
{
    {  0, 15,  0, 14 },
    {  1,  2,  1,  1 },
    {  2, 15,  2, 14 },
    {  3,  2,  3,  1 },
    {  4,  6,  4,  5 },
    {  4,  9,  4,  8 },
};

static const keychron_patch z11_ultra_l_patch =
{
    KEYCHRON_Z11_ULTRA_L_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(z11_ultra_l_entries),
    z11_ultra_l_entries
};

/*---------------------------------------------------------*\
| Geometry Group 56:                                        |
| 0x0443 Keychron Z11 Ultra R                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry z11_ultra_r_entries[] =
{
    {  0,  8,  0,  0 },
    {  0,  9,  0,  1 },
    {  0, 10,  0,  2 },
    {  0, 11,  0,  3 },
    {  0, 12,  0,  4 },
    {  0, 13,  0,  5 },
    {  0, 15,  0,  6 },
    {  1,  8,  1,  0 },
    {  1,  9,  1,  1 },
    {  1, 10,  1,  2 },
    {  1, 11,  1,  3 },
    {  1, 12,  1,  4 },
    {  1, 13,  1,  5 },
    {  1, 14,  1,  6 },
    {  1, 15,  1,  7 },
    {  2,  8,  2,  0 },
    {  2,  9,  2,  1 },
    {  2, 10,  2,  2 },
    {  2, 11,  2,  3 },
    {  2, 12,  2,  4 },
    {  2, 13,  2,  5 },
    {  2, 15,  2,  6 },
    {  3,  8,  3,  0 },
    {  3,  9,  3,  1 },
    {  3, 10,  3,  2 },
    {  3, 11,  3,  3 },
    {  3, 12,  3,  4 },
    {  3, 13,  3,  5 },
    {  3, 14,  3,  6 },
    {  3, 15,  3,  7 },
    {  4,  9,  4,  0 },
    {  4, 11,  4,  3 },
    {  4, 13,  4,  5 },
    {  4, 14,  4,  6 },
    {  4, 15,  4,  7 },
};

static const keychron_patch z11_ultra_r_patch =
{
    KEYCHRON_Z11_ULTRA_R_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(z11_ultra_r_entries),
    z11_ultra_r_entries
};

/*---------------------------------------------------------*\
| Geometry Group 57:                                        |
| 0x0446 Keychron J5 HE 8K ISO                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry j5_he_8k_iso_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  0, 18,  0, 19 },
    {  4, 13,  4, 12 },
    {  5, 18,  5, 19 },
};

static const keychron_patch j5_he_8k_iso_patch =
{
    KEYCHRON_J5_HE_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j5_he_8k_iso_entries),
    j5_he_8k_iso_entries
};

/*---------------------------------------------------------*\
| Geometry Group 58:                                        |
| 0x0447 Keychron J2 Ultra 8K ANSI                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry j2_ultra_8k_ansi_entries[] =
{
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  1, 14,  1, 13 },
    {  2, 14,  2, 13 },
};

static const keychron_patch j2_ultra_8k_ansi_patch =
{
    KEYCHRON_J2_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j2_ultra_8k_ansi_entries),
    j2_ultra_8k_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 59:                                        |
| 0x0448 Keychron J5 Ultra 8K                               |
| 0x0469 Keychron J5 Ultra 8K ISO                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry j5_ultra_8k_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  0, 18,  0, 19 },
    {  3, 13,  3, 12 },
    {  3, 15,  1, 20 },
    {  3, 16,  2, 20 },
    {  4, 13,  4, 12 },
    {  5, 18,  4, 20 },
};

static const keychron_patch j5_ultra_8k_patch =
{
    KEYCHRON_J5_ULTRA_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j5_ultra_8k_entries),
    j5_ultra_8k_entries
};

static const keychron_patch j5_ultra_8k_iso_patch =
{
    KEYCHRON_J5_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j5_ultra_8k_entries),
    j5_ultra_8k_entries
};

/*---------------------------------------------------------*\
| Geometry Group 60:                                        |
| 0x0458 Keychron J6 ANSI                                   |
| 0x0461 Keychron J6 Ultra 8K ANSI                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry j6_ansi_entries[] =
{
    {  3, 12,  0, 19 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 19 },
    {  3, 15,  2, 19 },
    {  3, 16,  3, 19 },
    {  4, 13,  4, 12 },
    {  4, 14,  4, 19 },
    {  4, 16,  4, 20 },
    {  5,  7,  0, 20 },
    {  5,  8,  1, 20 },
    {  5,  9,  2, 20 },
    {  5, 18,  5, 19 },
};

static const keychron_patch j6_ansi_patch =
{
    KEYCHRON_J6_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j6_ansi_entries),
    j6_ansi_entries
};

static const keychron_patch j6_ultra_8k_ansi_patch =
{
    KEYCHRON_J6_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j6_ansi_entries),
    j6_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 61:                                        |
| 0x045C Keychron eX99 HE 8K ANSI                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry ex99_he_8k_ansi_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 11 },
    {  0, 10,  0, 12 },
    {  0, 11,  0, 13 },
    {  0, 12,  0, 14 },
    {  0, 13,  0, 15 },
    {  0, 14,  0, 16 },
    {  0, 15,  0, 17 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  3, 13,  3, 15 },
    {  3, 14,  3, 16 },
    {  3, 15,  3, 17 },
    {  4, 13,  4, 14 },
    {  4, 14,  4, 15 },
    {  4, 15,  4, 16 },
    {  4, 16,  4, 17 },
    {  4, 17,  4, 18 },
};

static const keychron_patch ex99_he_8k_ansi_patch =
{
    KEYCHRON_EX99_HE_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(ex99_he_8k_ansi_entries),
    ex99_he_8k_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 62:                                        |
| 0x0467 Keychron J6 Full Size 8K                           |
| 0x0478 Keychron J6 Full Size 8K ISO                       |
| 0x0960 Keychron V6 Max ANSI                               |
| 0x0961 Keychron V6 Max ISO                                |
| 0x0F61 Keychron V6 8K ISO                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry j6_full_size_8k_entries[] =
{
    {  3, 20,  2, 20 },
    {  5, 20,  4, 20 },
};

static const keychron_patch j6_full_size_8k_patch =
{
    KEYCHRON_J6_FULL_SIZE_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j6_full_size_8k_entries),
    j6_full_size_8k_entries
};

static const keychron_patch j6_full_size_8k_iso_patch =
{
    KEYCHRON_J6_FULL_SIZE_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j6_full_size_8k_entries),
    j6_full_size_8k_entries
};

static const keychron_patch v6_max_ansi_patch =
{
    KEYCHRON_V6_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j6_full_size_8k_entries),
    j6_full_size_8k_entries
};

static const keychron_patch v6_max_iso_patch =
{
    KEYCHRON_V6_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j6_full_size_8k_entries),
    j6_full_size_8k_entries
};

static const keychron_patch v6_8k_iso_patch =
{
    KEYCHRON_V6_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j6_full_size_8k_entries),
    j6_full_size_8k_entries
};

/*---------------------------------------------------------*\
| Geometry Group 63:                                        |
| 0x0468 Keychron J2 TKL 8K                                 |
| 0x0F10 Keychron V1 8K ANSI                                |
| 0x0F11 Keychron V1 8K ISO                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry j2_tkl_8k_entries[] =
{
    {  2, 14,  2, 13 },
    {  3, 13,  3, 12 },
    {  4, 13,  4, 12 },
};

static const keychron_patch j2_tkl_8k_patch =
{
    KEYCHRON_J2_TKL_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j2_tkl_8k_entries),
    j2_tkl_8k_entries
};

static const keychron_patch v1_8k_ansi_patch =
{
    KEYCHRON_V1_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j2_tkl_8k_entries),
    j2_tkl_8k_entries
};

static const keychron_patch v1_8k_iso_patch =
{
    KEYCHRON_V1_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j2_tkl_8k_entries),
    j2_tkl_8k_entries
};

/*---------------------------------------------------------*\
| Geometry Group 64:                                        |
| 0x046B Keychron J8                                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry j8_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3, 13,  3, 12 },
    {  3, 14,  3, 15 },
    {  4, 13,  5, 15 },
};

static const keychron_patch j8_patch =
{
    KEYCHRON_J8_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(j8_entries),
    j8_entries
};

/*---------------------------------------------------------*\
| Geometry Group 65:                                        |
| 0x051E Keychron C1 Pro 8K JIS RGB                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry c1_pro_8k_jis_rgb_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  3, 12,  1, 14 },
    {  3, 13,  3, 12 },
    {  4, 15,  4, 16 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
};

static const keychron_patch c1_pro_8k_jis_rgb_patch =
{
    KEYCHRON_C1_PRO_8K_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(c1_pro_8k_jis_rgb_entries),
    c1_pro_8k_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 66:                                        |
| 0x0522 Keychron C2 Pro 8K ANSI RGB                        |
| 0x052D Keychron C2 Pro 8K ISO RGB                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry c2_pro_8k_ansi_rgb_entries[] =
{
    {  3, 13,  3, 12 },
    {  3, 14,  1, 19 },
    {  3, 15,  2, 19 },
    {  3, 16,  3, 19 },
    {  4, 12,  4, 19 },
    {  4, 13,  4, 12 },
    {  4, 14,  4, 20 },
    {  5,  7,  1, 20 },
    {  5,  8,  2, 20 },
    {  5, 18,  5, 19 },
};

static const keychron_patch c2_pro_8k_ansi_rgb_patch =
{
    KEYCHRON_C2_PRO_8K_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(c2_pro_8k_ansi_rgb_entries),
    c2_pro_8k_ansi_rgb_entries
};

static const keychron_patch c2_pro_8k_iso_rgb_patch =
{
    KEYCHRON_C2_PRO_8K_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(c2_pro_8k_ansi_rgb_entries),
    c2_pro_8k_ansi_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 67:                                        |
| 0x0532 Keychron C3 Pro 8K JIS                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry c3_pro_8k_jis_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  1, 13,  1, 14 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 13 },
    {  4, 15,  4, 16 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
};

static const keychron_patch c3_pro_8k_jis_patch =
{
    KEYCHRON_C3_PRO_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(c3_pro_8k_jis_entries),
    c3_pro_8k_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 68:                                        |
| 0x0542 Keychron C4 Pro JIS RGB                            |
\*---------------------------------------------------------*/
static const keychron_patch_entry c4_pro_jis_rgb_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  1, 19,  1, 20 },
    {  1, 20,  1, 21 },
    {  2, 13,  1, 14 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  2, 19,  2, 20 },
    {  2, 20,  2, 21 },
    {  3, 13,  2, 13 },
    {  3, 17,  3, 18 },
    {  3, 18,  3, 19 },
    {  3, 19,  3, 20 },
    {  4, 15,  4, 16 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  4, 19,  4, 20 },
    {  4, 20,  4, 21 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 17,  5, 18 },
    {  5, 18,  5, 20 },
};

static const keychron_patch c4_pro_jis_rgb_patch =
{
    KEYCHRON_C4_PRO_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(c4_pro_jis_rgb_entries),
    c4_pro_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 69:                                        |
| 0x0630 Keychron Q3 Pro                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry q3_pro_entries[] =
{
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  1,  0,  1,  1 },
    {  1,  1,  1,  2 },
    {  1,  2,  1,  3 },
    {  1,  3,  1,  4 },
    {  1,  4,  1,  5 },
    {  1,  5,  1,  6 },
    {  1,  6,  1,  7 },
    {  1,  7,  1,  8 },
    {  1,  8,  1,  9 },
    {  1,  9,  1, 10 },
    {  1, 10,  1, 11 },
    {  1, 11,  1, 12 },
    {  1, 12,  1, 13 },
    {  1, 13,  1, 14 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1,  0 },
    {  2,  0,  2,  1 },
    {  2,  1,  2,  2 },
    {  2,  2,  2,  3 },
    {  2,  3,  2,  4 },
    {  2,  4,  2,  5 },
    {  2,  5,  2,  6 },
    {  2,  6,  2,  7 },
    {  2,  7,  2,  8 },
    {  2,  8,  2,  9 },
    {  2,  9,  2, 10 },
    {  2, 10,  2, 11 },
    {  2, 11,  2, 12 },
    {  2, 12,  2, 13 },
    {  2, 13,  2, 14 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  3,  1,  3,  2 },
    {  3,  2,  3,  3 },
    {  3,  3,  3,  4 },
    {  3,  4,  3,  5 },
    {  3,  5,  3,  6 },
    {  3,  6,  3,  7 },
    {  3,  7,  3,  8 },
    {  3,  8,  3,  9 },
    {  3,  9,  3, 10 },
    {  3, 10,  3, 11 },
    {  3, 11,  3, 12 },
    {  3, 17,  2,  0 },
    {  4,  0,  4,  1 },
    {  4,  2,  4,  3 },
    {  4,  3,  4,  4 },
    {  4,  4,  4,  5 },
    {  4,  5,  4,  6 },
    {  4,  6,  4,  7 },
    {  4,  7,  4,  8 },
    {  4,  8,  4,  9 },
    {  4,  9,  4, 10 },
    {  4, 10,  4, 11 },
    {  4, 11,  4, 12 },
    {  4, 15,  4, 16 },
    {  4, 17,  4,  0 },
    {  5,  0,  5,  1 },
    {  5,  1,  5,  2 },
    {  5,  2,  5,  3 },
    {  5,  6,  5,  7 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 17,  5,  0 },
};

static const keychron_patch q3_pro_patch =
{
    KEYCHRON_Q3_PRO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_pro_entries),
    q3_pro_entries
};

/*---------------------------------------------------------*\
| Geometry Group 70:                                        |
| 0x0633 Keychron Q3 Pro ANSI                               |
| 0x0634 Keychron Q3 Pro ISO                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry q3_pro_ansi_entries[] =
{
    {  3, 12,  0, 16 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 16 },
    {  3, 15,  2, 16 },
    {  4, 13,  4, 12 },
    {  4, 14,  5, 16 },
};

static const keychron_patch q3_pro_ansi_patch =
{
    KEYCHRON_Q3_PRO_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_pro_ansi_entries),
    q3_pro_ansi_entries
};

static const keychron_patch q3_pro_iso_patch =
{
    KEYCHRON_Q3_PRO_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_pro_ansi_entries),
    q3_pro_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 71:                                        |
| 0x0650 Keychron Q5 Pro ANSI                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry q5_pro_ansi_entries[] =
{
    {  3, 13,  3, 12 },
    {  4, 13,  4, 12 },
    {  5, 17,  5, 16 },
};

static const keychron_patch q5_pro_ansi_patch =
{
    KEYCHRON_Q5_PRO_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_pro_ansi_entries),
    q5_pro_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 72:                                        |
| 0x0651 Keychron Q5 Pro ISO                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry q5_pro_iso_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  3, 13,  3, 12 },
    {  4, 13,  4, 12 },
    {  5, 17,  5, 16 },
};

static const keychron_patch q5_pro_iso_patch =
{
    KEYCHRON_Q5_PRO_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_pro_iso_entries),
    q5_pro_iso_entries
};

/*---------------------------------------------------------*\
| Geometry Group 73:                                        |
| 0x0660 Keychron Q6 Pro ANSI                               |
| 0x0661 Keychron Q6 Pro ISO                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry q6_pro_ansi_entries[] =
{
    {  3, 13,  3, 12 },
    {  4, 13,  4, 12 },
    {  5, 18,  5, 17 },
};

static const keychron_patch q6_pro_ansi_patch =
{
    KEYCHRON_Q6_PRO_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_pro_ansi_entries),
    q6_pro_ansi_entries
};

static const keychron_patch q6_pro_iso_patch =
{
    KEYCHRON_Q6_PRO_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_pro_ansi_entries),
    q6_pro_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 74:                                        |
| 0x06A0 Keychron Q10 Pro ANSI                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry q10_pro_ansi_entries[] =
{
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  1, 15,  1, 16 },
    {  2, 15,  2, 16 },
    {  3, 15,  3, 16 },
    {  4,  1,  4,  2 },
    {  5,  3,  5,  4 },
    {  5,  4,  5,  5 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 12 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
};

static const keychron_patch q10_pro_ansi_patch =
{
    KEYCHRON_Q10_PRO_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q10_pro_ansi_entries),
    q10_pro_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 75:                                        |
| 0x0810 Keychron Q1 Max ANSI                               |
| 0x0811 Keychron Q1 Max ISO                                |
| 0x1210 Keychron Q1 Ultra 8K ANSI                          |
| 0x1211 Keychron Q1 Ultra 8K ISO                           |
| 0x1213 Keychron Q1 Ultra 8K ANSI                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry q1_max_ansi_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3, 13,  3, 15 },
    {  4, 13,  4, 12 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch q1_max_ansi_patch =
{
    KEYCHRON_Q1_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1_max_ansi_entries),
    q1_max_ansi_entries
};

static const keychron_patch q1_max_iso_patch =
{
    KEYCHRON_Q1_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1_max_ansi_entries),
    q1_max_ansi_entries
};

static const keychron_patch q1_ultra_8k_ansi_1210_patch =
{
    KEYCHRON_Q1_ULTRA_8K_ANSI_1210_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1_max_ansi_entries),
    q1_max_ansi_entries
};

static const keychron_patch q1_ultra_8k_iso_patch =
{
    KEYCHRON_Q1_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1_max_ansi_entries),
    q1_max_ansi_entries
};

static const keychron_patch q1_ultra_8k_ansi_1213_patch =
{
    KEYCHRON_Q1_ULTRA_8K_ANSI_1213_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1_max_ansi_entries),
    q1_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 76:                                        |
| 0x0812 Keychron Q1 Max JIS                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry q1_max_jis_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  2, 14,  2, 15 },
    {  3, 13,  3, 15 },
    {  3, 14,  1, 15 },
    {  5,  8,  5,  9 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch q1_max_jis_patch =
{
    KEYCHRON_Q1_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1_max_jis_entries),
    q1_max_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 77:                                        |
| 0x0820 Keychron Q2 Max ANSI                               |
| 0x0821 Keychron Q2 Max ISO                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry q2_max_ansi_entries[] =
{
    {  1, 14,  1, 15 },
    {  2, 13,  2, 15 },
    {  3, 13,  3, 12 },
    {  4,  9,  4, 10 },
    {  4, 10,  4, 11 },
    {  4, 11,  4, 12 },
    {  4, 12,  4, 13 },
    {  4, 13,  4, 14 },
    {  4, 14,  4, 15 },
};

static const keychron_patch q2_max_ansi_patch =
{
    KEYCHRON_Q2_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q2_max_ansi_entries),
    q2_max_ansi_entries
};

static const keychron_patch q2_max_iso_patch =
{
    KEYCHRON_Q2_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q2_max_ansi_entries),
    q2_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 78:                                        |
| 0x0831 Keychron Q3 Max ISO                                |
| 0x1030 Keychron Q3 HE 8K                                  |
| 0x1031 Keychron Q3 HE 8K ISO                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry q3_max_iso_entries[] =
{
    {  4, 13,  4, 12 },
};

static const keychron_patch q3_max_iso_patch =
{
    KEYCHRON_Q3_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_max_iso_entries),
    q3_max_iso_entries
};

static const keychron_patch q3_he_8k_patch =
{
    KEYCHRON_Q3_HE_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_max_iso_entries),
    q3_max_iso_entries
};

static const keychron_patch q3_he_8k_iso_patch =
{
    KEYCHRON_Q3_HE_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_max_iso_entries),
    q3_max_iso_entries
};

/*---------------------------------------------------------*\
| Geometry Group 79:                                        |
| 0x0850 Keychron Q5 Max ANSI                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry q5_max_ansi_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  4, 13,  4, 14 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch q5_max_ansi_patch =
{
    KEYCHRON_Q5_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_max_ansi_entries),
    q5_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 80:                                        |
| 0x0851 Keychron Q5 Max ISO                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry q5_max_iso_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  3, 13,  3, 14 },
    {  5, 16,  5, 15 },
};

static const keychron_patch q5_max_iso_patch =
{
    KEYCHRON_Q5_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_max_iso_entries),
    q5_max_iso_entries
};

/*---------------------------------------------------------*\
| Geometry Group 81:                                        |
| 0x0852 Keychron Q5 Max JIS                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry q5_max_jis_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 11 },
    {  0, 10,  0, 12 },
    {  0, 11,  0, 13 },
    {  0, 12,  0, 14 },
    {  0, 13,  0, 15 },
    {  0, 14,  1, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  3, 13,  3, 15 },
    {  3, 15,  3, 16 },
    {  3, 16,  3, 17 },
    {  3, 17,  3, 18 },
    {  4, 14,  4, 15 },
    {  4, 15,  4, 16 },
    {  4, 16,  4, 17 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  5,  8,  5,  9 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 14 },
    {  5, 13,  5, 15 },
    {  5, 14,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 17,  5, 18 },
};

static const keychron_patch q5_max_jis_patch =
{
    KEYCHRON_Q5_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_max_jis_entries),
    q5_max_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 82:                                        |
| 0x0860 Keychron Q6 Max ANSI                               |
| 0x0861 Keychron Q6 Max ISO                                |
| 0x0AA0 Keychron K10 Max ANSI RGB                          |
| 0x0AA1 Keychron K10 Max ISO RGB                           |
| 0x13A9 Keychron K10 Pro SS ZMK RGB                        |
| 0x13AA Keychron K10 Ultra SE ANSI                         |
| 0x16A0 Keychron K10 Ultra 8K RGB                          |
| 0x16A1 Keychron K10 Ultra 8K ISO                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry q6_max_ansi_entries[] =
{
    {  3, 13,  0, 20 },
    {  3, 14,  1, 20 },
    {  3, 15,  2, 20 },
    {  3, 16,  4, 20 },
    {  4, 13,  4, 12 },
    {  5, 18,  5, 17 },
};

static const keychron_patch q6_max_ansi_patch =
{
    KEYCHRON_Q6_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_max_ansi_entries),
    q6_max_ansi_entries
};

static const keychron_patch q6_max_iso_patch =
{
    KEYCHRON_Q6_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_max_ansi_entries),
    q6_max_ansi_entries
};

static const keychron_patch k10_max_ansi_rgb_patch =
{
    KEYCHRON_K10_MAX_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_max_ansi_entries),
    q6_max_ansi_entries
};

static const keychron_patch k10_max_iso_rgb_patch =
{
    KEYCHRON_K10_MAX_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_max_ansi_entries),
    q6_max_ansi_entries
};

static const keychron_patch k10_pro_ss_zmk_rgb_patch =
{
    KEYCHRON_K10_PRO_SS_ZMK_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_max_ansi_entries),
    q6_max_ansi_entries
};

static const keychron_patch k10_ultra_se_ansi_patch =
{
    KEYCHRON_K10_ULTRA_SE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_max_ansi_entries),
    q6_max_ansi_entries
};

static const keychron_patch k10_ultra_8k_rgb_patch =
{
    KEYCHRON_K10_ULTRA_8K_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_max_ansi_entries),
    q6_max_ansi_entries
};

static const keychron_patch k10_ultra_8k_iso_patch =
{
    KEYCHRON_K10_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_max_ansi_entries),
    q6_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 83:                                        |
| 0x0880 Keychron Q8 Max ANSI                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry q8_max_ansi_entries[] =
{
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  1,  6,  1,  7 },
    {  1,  7,  1,  8 },
    {  1,  8,  1,  9 },
    {  1,  9,  1, 10 },
    {  1, 10,  1, 11 },
    {  1, 11,  1, 12 },
    {  1, 12,  1, 13 },
    {  1, 13,  1, 14 },
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3,  2,  3,  1 },
    {  3,  3,  3,  2 },
    {  3,  4,  3,  3 },
    {  3,  5,  3,  4 },
    {  3,  6,  3,  5 },
    {  3,  7,  3,  6 },
    {  3,  8,  3,  7 },
    {  3,  9,  3,  8 },
    {  3, 10,  3,  9 },
    {  3, 11,  3, 10 },
    {  3, 12,  3, 11 },
    {  3, 13,  3, 12 },
    {  4,  6,  4,  5 },
    {  4,  9,  4, 11 },
    {  4, 12,  4, 13 },
    {  4, 13,  4, 14 },
    {  4, 14,  4, 15 },
};

static const keychron_patch q8_max_ansi_patch =
{
    KEYCHRON_Q8_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q8_max_ansi_entries),
    q8_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 84:                                        |
| 0x08A0 Keychron Q10 Max ANSI                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry q10_max_ansi_entries[] =
{
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  1, 15,  1, 16 },
    {  2, 15,  2, 16 },
    {  3, 15,  3, 16 },
    {  4,  7,  5,  7 },
    {  5,  3,  5,  4 },
    {  5,  4,  5,  5 },
    {  5,  7,  4,  7 },
    {  5,  9,  5, 11 },
    {  5, 10,  5, 12 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
};

static const keychron_patch q10_max_ansi_patch =
{
    KEYCHRON_Q10_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q10_max_ansi_entries),
    q10_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 85:                                        |
| 0x08A1 Keychron Q10 Max ISO                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry q10_max_iso_entries[] =
{
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  1, 15,  1, 16 },
    {  2, 15,  2, 16 },
    {  3, 15,  3, 16 },
    {  5,  9,  5, 11 },
    {  5, 10,  5, 12 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
};

static const keychron_patch q10_max_iso_patch =
{
    KEYCHRON_Q10_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q10_max_iso_entries),
    q10_max_iso_entries
};

/*---------------------------------------------------------*\
| Geometry Group 86:                                        |
| 0x08B0 Keychron Q65 Max ANSI                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry q65_max_ansi_entries[] =
{
    {  0, 15,  0, 16 },
    {  1, 15,  1, 16 },
    {  2, 14,  2, 16 },
    {  4, 10,  4, 11 },
    {  4, 11,  4, 12 },
    {  4, 12,  4, 13 },
    {  4, 13,  4, 14 },
    {  4, 14,  4, 15 },
    {  4, 15,  4, 16 },
};

static const keychron_patch q65_max_ansi_patch =
{
    KEYCHRON_Q65_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q65_max_ansi_entries),
    q65_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 87:                                        |
| 0x08BA Keychron Q11 Ultra 8K R                            |
\*---------------------------------------------------------*/
static const keychron_patch_entry q11_ultra_8k_r_entries[] =
{
    {  0,  8,  0,  0 },
    {  0,  9,  0,  1 },
    {  0, 10,  0,  2 },
    {  0, 11,  0,  3 },
    {  0, 12,  0,  4 },
    {  0, 13,  0,  5 },
    {  0, 14,  0,  6 },
    {  0, 15,  0,  7 },
    {  1,  8,  1,  0 },
    {  1,  9,  1,  1 },
    {  1, 10,  1,  2 },
    {  1, 11,  1,  3 },
    {  1, 12,  1,  4 },
    {  1, 13,  1,  5 },
    {  1, 14,  1,  6 },
    {  1, 16,  1,  9 },
    {  2,  8,  2,  0 },
    {  2,  9,  2,  1 },
    {  2, 10,  2,  2 },
    {  2, 11,  2,  3 },
    {  2, 12,  2,  4 },
    {  2, 13,  2,  5 },
    {  2, 14,  2,  6 },
    {  2, 15,  2,  7 },
    {  2, 16,  2,  9 },
    {  3,  8,  3,  0 },
    {  3,  9,  3,  1 },
    {  3, 10,  3,  2 },
    {  3, 11,  3,  3 },
    {  3, 12,  3,  4 },
    {  3, 13,  3,  5 },
    {  3, 14,  3,  6 },
    {  3, 16,  3,  9 },
    {  4,  8,  4,  1 },
    {  4,  9,  4,  2 },
    {  4, 10,  4,  3 },
    {  4, 11,  4,  4 },
    {  4, 12,  4,  5 },
    {  4, 13,  4,  6 },
    {  4, 15,  4,  8 },
    {  5,  8,  5,  1 },
    {  5, 11,  5,  4 },
    {  5, 12,  5,  5 },
    {  5, 13,  5,  6 },
    {  5, 14,  5,  7 },
    {  5, 15,  5,  8 },
    {  5, 16,  5,  9 },
};

static const keychron_patch q11_ultra_8k_r_patch =
{
    KEYCHRON_Q11_ULTRA_8K_R_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q11_ultra_8k_r_entries),
    q11_ultra_8k_r_entries
};

/*---------------------------------------------------------*\
| Geometry Group 88:                                        |
| 0x08C0 Keychron Q60 Max                                   |
\*---------------------------------------------------------*/
static const keychron_patch_entry q60_max_entries[] =
{
    {  3, 13,  3, 14 },
};

static const keychron_patch q60_max_patch =
{
    KEYCHRON_Q60_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q60_max_entries),
    q60_max_entries
};

/*---------------------------------------------------------*\
| Geometry Group 89:                                        |
| 0x08C3 Keychron Q12 MAX ANSI                              |
| 0x08C4 Keychron Q12 Max ISO                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry q12_max_ansi_entries[] =
{
    {  3, 17,  3, 18 },
    {  5,  1,  5,  0 },
};

static const keychron_patch q12_max_ansi_patch =
{
    KEYCHRON_Q12_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q12_max_ansi_entries),
    q12_max_ansi_entries
};

static const keychron_patch q12_max_iso_patch =
{
    KEYCHRON_Q12_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q12_max_ansi_entries),
    q12_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 90:                                        |
| 0x08D0 Keychron Q13 Max ANSI                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry q13_max_ansi_entries[] =
{
    {  4,  3,  4,  2 },
    {  4,  4,  4,  3 },
    {  4,  5,  4,  4 },
    {  4,  6,  4,  5 },
    {  4,  7,  4,  6 },
    {  5,  4,  5,  3 },
    {  5,  6,  5,  4 },
    {  5,  7,  5,  6 },
    {  5,  9,  5,  8 },
};

static const keychron_patch q13_max_ansi_patch =
{
    KEYCHRON_Q13_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q13_max_ansi_entries),
    q13_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 91:                                        |
| 0x08D2 Keychron Q13 Max JIS                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry q13_max_jis_entries[] =
{
    {  5,  6,  5,  5 },
    {  5,  9,  5,  8 },
};

static const keychron_patch q13_max_jis_patch =
{
    KEYCHRON_Q13_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q13_max_jis_entries),
    q13_max_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 92:                                        |
| 0x08E0 Keychron Q14 Max ANSI                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry q14_max_ansi_entries[] =
{
    {  5,  9,  5,  8 },
    {  5, 12,  5, 11 },
};

static const keychron_patch q14_max_ansi_patch =
{
    KEYCHRON_Q14_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q14_max_ansi_entries),
    q14_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 93:                                        |
| 0x08F0 Keychron Q15 Max ANSI                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry q15_max_ansi_entries[] =
{
    {  4,  7,  4,  6 },
};

static const keychron_patch q15_max_ansi_patch =
{
    KEYCHRON_Q15_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q15_max_ansi_entries),
    q15_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 94:                                        |
| 0x0914 Keychron V2 Ultra ISO                              |
| 0x0915 Keychron V1 Max JIS                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry v2_ultra_iso_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  3, 13,  2, 13 },
};

static const keychron_patch v2_ultra_iso_patch =
{
    KEYCHRON_V2_ULTRA_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v2_ultra_iso_entries),
    v2_ultra_iso_entries
};

static const keychron_patch v1_max_jis_patch =
{
    KEYCHRON_V1_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v2_ultra_iso_entries),
    v2_ultra_iso_entries
};

/*---------------------------------------------------------*\
| Geometry Group 95:                                        |
| 0x0935 Keychron V3 Max JIS                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry v3_max_jis_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 14 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  4, 15,  4, 16 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
};

static const keychron_patch v3_max_jis_patch =
{
    KEYCHRON_V3_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v3_max_jis_entries),
    v3_max_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 96:                                        |
| 0x0950 Keychron V5 Max ANSI                               |
| 0x0951 Keychron V5 Max ISO                                |
| 0x0952 Keychron V5 Max JIS                                |
| 0x0C50 Keychron V5 Ultra 8K ANSI                          |
| 0x0C51 Keychron V5 Ultra ISO                              |
| 0x0C52 Keychron V5 Ultra JIS                              |
| 0x1250 Keychron Q5 Ultra 8K                               |
| 0x1251 Keychron Q5 Ultra 8K ISO                           |
| 0x1253 Keychron Q5 Ultra 8K ANSI                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry v5_max_ansi_entries[] =
{
    {  3, 18,  2, 18 },
};

static const keychron_patch v5_max_ansi_patch =
{
    KEYCHRON_V5_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_max_ansi_entries),
    v5_max_ansi_entries
};

static const keychron_patch v5_max_iso_patch =
{
    KEYCHRON_V5_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_max_ansi_entries),
    v5_max_ansi_entries
};

static const keychron_patch v5_max_jis_patch =
{
    KEYCHRON_V5_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_max_ansi_entries),
    v5_max_ansi_entries
};

static const keychron_patch v5_ultra_8k_ansi_patch =
{
    KEYCHRON_V5_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_max_ansi_entries),
    v5_max_ansi_entries
};

static const keychron_patch v5_ultra_iso_patch =
{
    KEYCHRON_V5_ULTRA_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_max_ansi_entries),
    v5_max_ansi_entries
};

static const keychron_patch v5_ultra_jis_patch =
{
    KEYCHRON_V5_ULTRA_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_max_ansi_entries),
    v5_max_ansi_entries
};

static const keychron_patch q5_ultra_8k_patch =
{
    KEYCHRON_Q5_ULTRA_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_max_ansi_entries),
    v5_max_ansi_entries
};

static const keychron_patch q5_ultra_8k_iso_patch =
{
    KEYCHRON_Q5_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_max_ansi_entries),
    v5_max_ansi_entries
};

static const keychron_patch q5_ultra_8k_ansi_patch =
{
    KEYCHRON_Q5_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_max_ansi_entries),
    v5_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 97:                                        |
| 0x0962 Keychron V6 Max JIS                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry v6_max_jis_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  0, 18,  0, 19 },
    {  0, 19,  0, 20 },
    {  0, 20,  0, 21 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  1, 19,  1, 20 },
    {  1, 20,  1, 21 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  2, 19,  2, 20 },
    {  3, 13,  1, 14 },
    {  3, 17,  3, 18 },
    {  3, 18,  3, 19 },
    {  3, 19,  3, 20 },
    {  3, 20,  2, 21 },
    {  4, 15,  4, 16 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  4, 19,  4, 20 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 17,  5, 18 },
    {  5, 19,  5, 20 },
    {  5, 20,  4, 21 },
};

static const keychron_patch v6_max_jis_patch =
{
    KEYCHRON_V6_MAX_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v6_max_jis_entries),
    v6_max_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 98:                                        |
| 0x0980 Keychron V8 Max ANSI                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry v8_max_ansi_entries[] =
{
    {  0, 14,  0, 13 },
    {  2, 14,  2, 13 },
    {  4,  5,  4,  4 },
    {  4,  9,  4,  8 },
};

static const keychron_patch v8_max_ansi_patch =
{
    KEYCHRON_V8_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v8_max_ansi_entries),
    v8_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 99:                                        |
| 0x09A0 Keychron V10 Max ANSI                              |
| 0x09A1 Keychron V10 Max ISO                               |
| 0x0CA0 Keychron V10 Ultra 8K ANSI                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry v10_max_ansi_entries[] =
{
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  5,  6,  5,  5 },
    {  5,  9,  5,  8 },
};

static const keychron_patch v10_max_ansi_patch =
{
    KEYCHRON_V10_MAX_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v10_max_ansi_entries),
    v10_max_ansi_entries
};

static const keychron_patch v10_max_iso_patch =
{
    KEYCHRON_V10_MAX_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v10_max_ansi_entries),
    v10_max_ansi_entries
};

static const keychron_patch v10_ultra_8k_ansi_patch =
{
    KEYCHRON_V10_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v10_max_ansi_entries),
    v10_max_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 100:                                       |
| 0x0A00 Keychron K17 Max ANSI RGB                          |
| 0x0A01 Keychron K17 Max ISO RGB                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k17_max_ansi_rgb_entries[] =
{
    {  1, 14,  1, 13 },
    {  2, 14,  2, 13 },
    {  3, 13,  3, 12 },
};

static const keychron_patch k17_max_ansi_rgb_patch =
{
    KEYCHRON_K17_MAX_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k17_max_ansi_rgb_entries),
    k17_max_ansi_rgb_entries
};

static const keychron_patch k17_max_iso_rgb_patch =
{
    KEYCHRON_K17_MAX_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k17_max_ansi_rgb_entries),
    k17_max_ansi_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 101:                                       |
| 0x0A06 Keychron K0 Max RGB                                |
| 0x0A09 Keychron K0 Max                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry k0_max_rgb_entries[] =
{
    {  5,  4,  4,  4 },
};

static const keychron_patch k0_max_rgb_patch =
{
    KEYCHRON_K0_MAX_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k0_max_rgb_entries),
    k0_max_rgb_entries
};

static const keychron_patch k0_max_patch =
{
    KEYCHRON_K0_MAX_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k0_max_rgb_entries),
    k0_max_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 102:                                       |
| 0x0A07 Keychron K17 Max JIS RGB                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k17_max_jis_rgb_entries[] =
{
    {  2, 14,  2, 13 },
    {  3, 13,  3, 12 },
    {  4, 14,  4, 13 },
    {  5,  8,  5,  9 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
};

static const keychron_patch k17_max_jis_rgb_patch =
{
    KEYCHRON_K17_MAX_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k17_max_jis_rgb_entries),
    k17_max_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 103:                                       |
| 0x0A12 Keychron K1 Max JIS RGB                            |
| 0x0A16 Keychron K1 Max JIS RGB                            |
\*---------------------------------------------------------*/
static const keychron_patch_entry k1_max_jis_rgb_0a12_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  3, 12,  1, 17 },
    {  3, 13,  3, 12 },
    {  4, 15,  4, 16 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
};

static const keychron_patch k1_max_jis_rgb_0a12_patch =
{
    KEYCHRON_K1_MAX_JIS_RGB_0A12_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k1_max_jis_rgb_0a12_entries),
    k1_max_jis_rgb_0a12_entries
};

static const keychron_patch k1_max_jis_rgb_0a16_patch =
{
    KEYCHRON_K1_MAX_JIS_RGB_0A16_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k1_max_jis_rgb_0a12_entries),
    k1_max_jis_rgb_0a12_entries
};

/*---------------------------------------------------------*\
| Geometry Group 104:                                       |
| 0x0A32 Keychron K3 Max JIS RGB                            |
| 0x0A39 Keychron K3 Max JIS RGB                            |
| 0x0A3B Keychron K3 Max JIS RGB                            |
| 0x0D22 Keychron K2 V3 JIS RGB                             |
| 0x0D32 Keychron K3 V3 JIS RGB                             |
| 0x0E42 Keychron K4 HE JIS                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry k3_max_jis_rgb_0a32_entries[] =
{
    {  3, 13,  3, 12 },
    {  4, 13,  4, 12 },
    {  4, 14,  4, 13 },
};

static const keychron_patch k3_max_jis_rgb_0a32_patch =
{
    KEYCHRON_K3_MAX_JIS_RGB_0A32_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_max_jis_rgb_0a32_entries),
    k3_max_jis_rgb_0a32_entries
};

static const keychron_patch k3_max_jis_rgb_0a39_patch =
{
    KEYCHRON_K3_MAX_JIS_RGB_0A39_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_max_jis_rgb_0a32_entries),
    k3_max_jis_rgb_0a32_entries
};

static const keychron_patch k3_max_jis_rgb_0a3b_patch =
{
    KEYCHRON_K3_MAX_JIS_RGB_0A3B_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_max_jis_rgb_0a32_entries),
    k3_max_jis_rgb_0a32_entries
};

static const keychron_patch k2_v3_jis_rgb_patch =
{
    KEYCHRON_K2_V3_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_max_jis_rgb_0a32_entries),
    k3_max_jis_rgb_0a32_entries
};

static const keychron_patch k3_v3_jis_rgb_patch =
{
    KEYCHRON_K3_V3_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_max_jis_rgb_0a32_entries),
    k3_max_jis_rgb_0a32_entries
};

static const keychron_patch k4_he_jis_patch =
{
    KEYCHRON_K4_HE_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_max_jis_rgb_0a32_entries),
    k3_max_jis_rgb_0a32_entries
};

/*---------------------------------------------------------*\
| Geometry Group 105:                                       |
| 0x0A42 Keychron K4 MAX JIS RGB                            |
\*---------------------------------------------------------*/
static const keychron_patch_entry k4_max_jis_rgb_entries[] =
{
    {  1, 13,  1, 14 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  3, 12,  4, 13 },
    {  3, 13,  3, 12 },
    {  3, 14,  3, 15 },
    {  3, 15,  3, 16 },
    {  3, 16,  3, 17 },
    {  3, 17,  0, 18 },
    {  4, 13,  4, 14 },
    {  4, 14,  4, 15 },
    {  4, 15,  4, 16 },
    {  4, 16,  4, 17 },
    {  4, 17,  4, 18 },
    {  5,  8,  1, 13 },
};

static const keychron_patch k4_max_jis_rgb_patch =
{
    KEYCHRON_K4_MAX_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k4_max_jis_rgb_entries),
    k4_max_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 106:                                       |
| 0x0AA2 Keychron K10 Max JIS RGB                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k10_max_jis_rgb_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  0, 18,  0, 19 },
    {  0, 19,  0, 21 },
    {  1, 13,  1, 14 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  1, 19,  1, 20 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  2, 19,  2, 20 },
    {  3, 13,  0, 20 },
    {  3, 14,  1, 21 },
    {  3, 15,  2, 21 },
    {  3, 16,  4, 21 },
    {  3, 17,  3, 18 },
    {  3, 18,  3, 19 },
    {  3, 19,  3, 20 },
    {  4, 15,  4, 16 },
    {  4, 16,  1, 13 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  4, 19,  4, 20 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 19,  5, 20 },
};

static const keychron_patch k10_max_jis_rgb_patch =
{
    KEYCHRON_K10_MAX_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k10_max_jis_rgb_entries),
    k10_max_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 107:                                       |
| 0x0AB3 Keychron K11 Max ANSI RGB                          |
| 0x0AB4 Keychron K11 Max ISO RGB                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k11_max_ansi_rgb_entries[] =
{
    {  2, 13,  2, 12 },
    {  3, 13,  3, 12 },
    {  3, 14,  3, 13 },
    {  3, 15,  3, 14 },
    {  4,  2,  4,  3 },
    {  4,  5,  4,  4 },
    {  4,  9,  4,  8 },
    {  4, 12,  4, 11 },
};

static const keychron_patch k11_max_ansi_rgb_patch =
{
    KEYCHRON_K11_MAX_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k11_max_ansi_rgb_entries),
    k11_max_ansi_rgb_entries
};

static const keychron_patch k11_max_iso_rgb_patch =
{
    KEYCHRON_K11_MAX_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k11_max_ansi_rgb_entries),
    k11_max_ansi_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 108:                                       |
| 0x0AB5 Keychron K11 Max JIS RGB                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k11_max_jis_rgb_entries[] =
{
    {  2,  6,  2,  7 },
    {  2,  7,  2,  8 },
    {  2,  8,  2,  9 },
    {  2,  9,  2, 10 },
    {  2, 10,  2, 11 },
    {  2, 11,  2, 12 },
    {  3,  2,  3,  1 },
    {  3,  3,  3,  2 },
    {  3,  4,  3,  3 },
    {  3,  5,  3,  4 },
    {  3,  6,  3,  5 },
    {  3,  7,  3,  6 },
    {  3,  8,  3,  7 },
    {  3,  9,  3,  8 },
    {  3, 10,  3,  9 },
    {  3, 11,  3, 10 },
    {  3, 12,  3, 11 },
    {  3, 13,  3, 12 },
    {  3, 14,  3, 13 },
    {  3, 15,  3, 14 },
    {  4,  6,  4,  5 },
    {  4,  7,  4,  6 },
    {  4,  9,  4,  7 },
    {  4, 11,  4, 10 },
};

static const keychron_patch k11_max_jis_rgb_patch =
{
    KEYCHRON_K11_MAX_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k11_max_jis_rgb_entries),
    k11_max_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 109:                                       |
| 0x0AD2 Keychron K13 Max JIS RGB                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k13_max_jis_rgb_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  3, 12,  1, 17 },
    {  3, 13,  3, 12 },
    {  3, 14,  3, 15 },
    {  3, 15,  3, 16 },
    {  3, 16,  3, 17 },
    {  4, 15,  4, 16 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
};

static const keychron_patch k13_max_jis_rgb_patch =
{
    KEYCHRON_K13_MAX_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k13_max_jis_rgb_entries),
    k13_max_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 110:                                       |
| 0x0AF0 Keychron K15 Max ANSI RGB                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry k15_max_ansi_rgb_entries[] =
{
    {  0, 15,  0, 16 },
    {  1, 15,  1, 16 },
    {  2, 15,  2, 16 },
    {  3,  7,  3,  6 },
    {  3, 15,  3, 16 },
    {  5,  3,  5,  4 },
    {  5,  9,  5,  8 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
};

static const keychron_patch k15_max_ansi_rgb_patch =
{
    KEYCHRON_K15_MAX_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k15_max_ansi_rgb_entries),
    k15_max_ansi_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 111:                                       |
| 0x0AF1 Keychron K15 Max ISO RGB                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k15_max_iso_rgb_entries[] =
{
    {  1, 15,  1, 16 },
    {  2, 15,  2, 16 },
    {  3,  7,  3,  6 },
    {  3, 15,  3, 16 },
    {  5,  3,  5,  4 },
    {  5,  9,  5,  8 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
};

static const keychron_patch k15_max_iso_rgb_patch =
{
    KEYCHRON_K15_MAX_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k15_max_iso_rgb_entries),
    k15_max_iso_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 112:                                       |
| 0x0AF2 Keychron K15 Max JIS RGB                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k15_max_jis_rgb_entries[] =
{
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  2, 15,  2, 17 },
    {  3,  7,  3,  6 },
    {  3, 15,  3, 17 },
    {  5,  8,  1, 17 },
    {  5,  9,  5,  8 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  4, 16 },
    {  5, 13,  5, 15 },
    {  5, 14,  5, 16 },
    {  5, 15,  5, 17 },
};

static const keychron_patch k15_max_jis_rgb_patch =
{
    KEYCHRON_K15_MAX_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k15_max_jis_rgb_entries),
    k15_max_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 113:                                       |
| 0x0B12 Keychron Q1 HE JIS                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry q1_he_jis_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3, 13,  3, 15 },
    {  3, 14,  1, 14 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch q1_he_jis_patch =
{
    KEYCHRON_Q1_HE_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1_he_jis_entries),
    q1_he_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 114:                                       |
| 0x0B30 Keychron Q3 HE ANSI                                |
| 0x0B31 Keychron Q3 HE ISO                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry q3_he_ansi_entries[] =
{
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 16 },
    {  3, 15,  2, 16 },
    {  4, 13,  4, 12 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
};

static const keychron_patch q3_he_ansi_patch =
{
    KEYCHRON_Q3_HE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_he_ansi_entries),
    q3_he_ansi_entries
};

static const keychron_patch q3_he_iso_patch =
{
    KEYCHRON_Q3_HE_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_he_ansi_entries),
    q3_he_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 115:                                       |
| 0x0B32 Keychron Q3 HE JIS                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry q3_he_jis_entries[] =
{
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 15 },
    {  0, 14,  0, 16 },
    {  0, 15,  0, 17 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  3, 12,  1, 14 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 17 },
    {  3, 15,  2, 17 },
    {  4, 15,  4, 16 },
    {  5,  8,  5,  9 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 15 },
    {  5, 14,  5, 16 },
    {  5, 15,  5, 17 },
};

static const keychron_patch q3_he_jis_patch =
{
    KEYCHRON_Q3_HE_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_he_jis_entries),
    q3_he_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 116:                                       |
| 0x0B40 Keychron Q4 HE ANSI                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry q4_he_ansi_entries[] =
{
    {  2, 13,  2, 12 },
    {  4,  9,  4, 10 },
    {  4, 10,  4, 11 },
    {  4, 11,  4, 12 },
};

static const keychron_patch q4_he_ansi_patch =
{
    KEYCHRON_Q4_HE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q4_he_ansi_entries),
    q4_he_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 117:                                       |
| 0x0B50 Keychron Q5 HE ANSI                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry q5_he_ansi_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  3, 15,  3, 16 },
    {  3, 16,  3, 17 },
    {  3, 17,  3, 18 },
    {  4, 12,  4, 11 },
    {  4, 13,  4, 12 },
    {  4, 15,  4, 16 },
    {  4, 16,  4, 17 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 17,  5, 18 },
};

static const keychron_patch q5_he_ansi_patch =
{
    KEYCHRON_Q5_HE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_he_ansi_entries),
    q5_he_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 118:                                       |
| 0x0B51 Keychron Q5 HE ISO                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry q5_he_iso_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  3, 13,  3, 15 },
    {  3, 15,  3, 16 },
    {  3, 16,  3, 17 },
    {  3, 17,  3, 18 },
    {  4, 12,  4, 11 },
    {  4, 13,  4, 12 },
    {  4, 15,  4, 16 },
    {  4, 16,  4, 17 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 17,  5, 18 },
};

static const keychron_patch q5_he_iso_patch =
{
    KEYCHRON_Q5_HE_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_he_iso_entries),
    q5_he_iso_entries
};

/*---------------------------------------------------------*\
| Geometry Group 119:                                       |
| 0x0B52 Keychron Q5 HE JIS                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry q5_he_jis_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  3, 13,  3, 15 },
    {  3, 14,  1, 14 },
    {  3, 15,  3, 16 },
    {  3, 16,  3, 17 },
    {  3, 17,  3, 18 },
    {  4, 14,  4, 15 },
    {  4, 15,  4, 16 },
    {  4, 16,  4, 17 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  5,  8,  5,  9 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 13 },
    {  5, 12,  5, 14 },
    {  5, 13,  5, 15 },
    {  5, 14,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 17,  5, 18 },
};

static const keychron_patch q5_he_jis_patch =
{
    KEYCHRON_Q5_HE_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_he_jis_entries),
    q5_he_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 120:                                       |
| 0x0B60 Keychron Q6 HE ANSI                                |
| 0x0B61 Keychron Q6 HE ISO                                 |
| 0x0B63 Keychron Q6 HE ANSI                                |
| 0x0B64 Keychron Q6 HE V2 Silent ANSI                      |
| 0x0EA4 Keychron K10 HE ISO                                |
| 0x0EA7 Keychron K10 HE V3 ANSI RGB                        |
\*---------------------------------------------------------*/
static const keychron_patch_entry q6_he_ansi_0b60_entries[] =
{
    {  4, 13,  4, 12 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 18,  5, 19 },
};

static const keychron_patch q6_he_ansi_0b60_patch =
{
    KEYCHRON_Q6_HE_ANSI_0B60_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_he_ansi_0b60_entries),
    q6_he_ansi_0b60_entries
};

static const keychron_patch q6_he_iso_patch =
{
    KEYCHRON_Q6_HE_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_he_ansi_0b60_entries),
    q6_he_ansi_0b60_entries
};

static const keychron_patch q6_he_ansi_0b63_patch =
{
    KEYCHRON_Q6_HE_ANSI_0B63_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_he_ansi_0b60_entries),
    q6_he_ansi_0b60_entries
};

static const keychron_patch q6_he_v2_silent_ansi_patch =
{
    KEYCHRON_Q6_HE_V2_SILENT_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_he_ansi_0b60_entries),
    q6_he_ansi_0b60_entries
};

static const keychron_patch k10_he_iso_0ea4_patch =
{
    KEYCHRON_K10_HE_ISO_0EA4_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_he_ansi_0b60_entries),
    q6_he_ansi_0b60_entries
};

static const keychron_patch k10_he_v3_ansi_rgb_patch =
{
    KEYCHRON_K10_HE_V3_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_he_ansi_0b60_entries),
    q6_he_ansi_0b60_entries
};

/*---------------------------------------------------------*\
| Geometry Group 121:                                       |
| 0x0B62 Keychron Q6 HE JIS                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry q6_he_jis_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  1, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  0, 18,  0, 19 },
    {  0, 19,  0, 20 },
    {  0, 20,  0, 21 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  1, 19,  1, 20 },
    {  1, 20,  1, 21 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  2, 19,  2, 20 },
    {  2, 20,  2, 21 },
    {  3, 17,  3, 18 },
    {  3, 18,  3, 19 },
    {  3, 19,  3, 20 },
    {  4, 15,  4, 16 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  4, 19,  4, 20 },
    {  4, 20,  4, 21 },
    {  5,  8,  5,  9 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 17,  5, 18 },
    {  5, 18,  5, 20 },
};

static const keychron_patch q6_he_jis_patch =
{
    KEYCHRON_Q6_HE_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_he_jis_entries),
    q6_he_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 122:                                       |
| 0x0BC0 Keychron Q12 HE ANSI                               |
| 0x0BC1 Keychron Q12 HE ISO                                |
| 0xD014 Keychron Q12 HE                                    |
\*---------------------------------------------------------*/
static const keychron_patch_entry q12_he_ansi_entries[] =
{
    {  3, 17,  3, 18 },
    {  4, 16,  4, 15 },
    {  4, 17,  4, 16 },
    {  4, 18,  4, 17 },
    {  5,  1,  5,  0 },
};

static const keychron_patch q12_he_ansi_patch =
{
    KEYCHRON_Q12_HE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q12_he_ansi_entries),
    q12_he_ansi_entries
};

static const keychron_patch q12_he_iso_patch =
{
    KEYCHRON_Q12_HE_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q12_he_ansi_entries),
    q12_he_ansi_entries
};

static const keychron_patch q12_he_patch =
{
    KEYCHRON_Q12_HE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q12_he_ansi_entries),
    q12_he_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 123:                                       |
| 0x0C00 Keychron V0 Ultra 8K ANSI                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry v0_ultra_8k_ansi_entries[] =
{
    {  5,  2,  5,  1 },
};

static const keychron_patch v0_ultra_8k_ansi_patch =
{
    KEYCHRON_V0_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v0_ultra_8k_ansi_entries),
    v0_ultra_8k_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 124:                                       |
| 0x0C12 Keychron V1 Ultra 8K JIS                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry v1_ultra_8k_jis_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
};

static const keychron_patch v1_ultra_8k_jis_patch =
{
    KEYCHRON_V1_ULTRA_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v1_ultra_8k_jis_entries),
    v1_ultra_8k_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 125:                                       |
| 0x0C32 Keychron V3 Ultra 8K JIS                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry v3_ultra_8k_jis_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  3, 13,  1, 14 },
    {  4, 15,  4, 16 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
};

static const keychron_patch v3_ultra_8k_jis_patch =
{
    KEYCHRON_V3_ULTRA_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v3_ultra_8k_jis_entries),
    v3_ultra_8k_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 126:                                       |
| 0x0C60 Keychron V6 Ultra 8K ANSI                          |
| 0x0C61 Keychron V6 Ultra ISO                              |
| 0x0C63 Keychron V6 Ultra SE ANSI                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry v6_ultra_8k_ansi_entries[] =
{
    {  3, 14,  2, 20 },
    {  3, 15,  1, 20 },
    {  3, 16,  0, 20 },
    {  5, 18,  4, 20 },
};

static const keychron_patch v6_ultra_8k_ansi_patch =
{
    KEYCHRON_V6_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v6_ultra_8k_ansi_entries),
    v6_ultra_8k_ansi_entries
};

static const keychron_patch v6_ultra_iso_patch =
{
    KEYCHRON_V6_ULTRA_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v6_ultra_8k_ansi_entries),
    v6_ultra_8k_ansi_entries
};

static const keychron_patch v6_ultra_se_ansi_patch =
{
    KEYCHRON_V6_ULTRA_SE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v6_ultra_8k_ansi_entries),
    v6_ultra_8k_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 127:                                       |
| 0x0C62 Keychron V6 Ultra 8K JIS                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry v6_ultra_8k_jis_entries[] =
{
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  1, 19,  1, 20 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  2, 19,  2, 20 },
    {  3, 13,  1, 14 },
    {  3, 14,  2, 21 },
    {  3, 15,  1, 21 },
    {  3, 16,  0, 20 },
    {  3, 17,  3, 18 },
    {  3, 18,  3, 19 },
    {  3, 19,  3, 20 },
    {  4, 15,  4, 16 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  4, 19,  4, 20 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 17,  5, 18 },
    {  5, 18,  4, 21 },
    {  5, 19,  5, 20 },
};

static const keychron_patch v6_ultra_8k_jis_patch =
{
    KEYCHRON_V6_ULTRA_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v6_ultra_8k_jis_entries),
    v6_ultra_8k_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 128:                                       |
| 0x0D12 Keychron K1 V6 JIS RGB                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry k1_v6_jis_rgb_entries[] =
{
    {  0,  1,  1, 14 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  3, 13,  3, 12 },
    {  4, 15,  4, 16 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
};

static const keychron_patch k1_v6_jis_rgb_patch =
{
    KEYCHRON_K1_V6_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k1_v6_jis_rgb_entries),
    k1_v6_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 129:                                       |
| 0x0D41 Keychron K4 V3 ISO RGB                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry k4_v3_iso_rgb_entries[] =
{
    {  3, 13,  3, 12 },
    {  4,  1,  4,  2 },
    {  4,  2,  4,  3 },
    {  4,  3,  4,  4 },
    {  4,  4,  4,  5 },
    {  4,  5,  4,  6 },
    {  4,  6,  4,  7 },
    {  4,  7,  4,  8 },
    {  4,  8,  4,  9 },
    {  4,  9,  4, 10 },
    {  4, 10,  4, 11 },
    {  4, 11,  4, 12 },
    {  4, 12,  4, 13 },
    {  4, 13,  4, 14 },
    {  5, 15,  5, 14 },
    {  5, 16,  5, 15 },
    {  5, 17,  5, 16 },
};

static const keychron_patch k4_v3_iso_rgb_patch =
{
    KEYCHRON_K4_V3_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k4_v3_iso_rgb_entries),
    k4_v3_iso_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 130:                                       |
| 0x0D50 Keychron K5 V2 ANSI RGB                            |
| 0x0D51 Keychron K5 V2 ISO RGB                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry k5_v2_ansi_rgb_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  3, 12,  0, 19 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 19 },
    {  3, 15,  2, 19 },
    {  3, 16,  3, 19 },
    {  4, 13,  4, 12 },
    {  4, 14,  4, 19 },
    {  4, 16,  4, 20 },
    {  5,  7,  0, 20 },
    {  5,  8,  1, 20 },
    {  5,  9,  2, 20 },
    {  5, 18,  5, 19 },
};

static const keychron_patch k5_v2_ansi_rgb_patch =
{
    KEYCHRON_K5_V2_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k5_v2_ansi_rgb_entries),
    k5_v2_ansi_rgb_entries
};

static const keychron_patch k5_v2_iso_rgb_patch =
{
    KEYCHRON_K5_V2_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k5_v2_ansi_rgb_entries),
    k5_v2_ansi_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 131:                                       |
| 0x0D52 Keychron K5 V2 JIS RGB                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry k5_v2_jis_rgb_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 11 },
    {  0, 10,  0, 12 },
    {  0, 11,  0, 13 },
    {  0, 12,  0, 14 },
    {  0, 13,  1, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  0, 18,  0, 19 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  3, 12,  0, 20 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 20 },
    {  3, 15,  2, 20 },
    {  3, 16,  3, 20 },
    {  3, 17,  3, 18 },
    {  3, 18,  3, 19 },
    {  4, 14,  4, 20 },
    {  4, 15,  4, 16 },
    {  4, 16,  4, 21 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  5,  5,  2, 21 },
    {  5,  7,  0, 21 },
    {  5,  8,  1, 21 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 17,  5, 18 },
    {  5, 18,  5, 20 },
};

static const keychron_patch k5_v2_jis_rgb_patch =
{
    KEYCHRON_K5_V2_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k5_v2_jis_rgb_entries),
    k5_v2_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 132:                                       |
| 0x0D60 Keychron K6X US RGB                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry k6x_us_rgb_entries[] =
{
    {  0, 14,  0, 13 },
    {  1, 14,  1, 13 },
    {  2, 13,  2, 12 },
};

static const keychron_patch k6x_us_rgb_patch =
{
    KEYCHRON_K6X_US_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k6x_us_rgb_entries),
    k6x_us_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 133:                                       |
| 0x0E20 Keychron K2 HE ANSI RGB                            |
| 0x0E21 Keychron K2 HE ISO RGB                             |
| 0x0E30 Keychron K3 HE                                     |
| 0x0E31 Keychron K3 HE ISO                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry k2_he_ansi_rgb_entries[] =
{
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3, 13,  3, 15 },
    {  4, 13,  4, 14 },
    {  4, 14,  4, 15 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch k2_he_ansi_rgb_patch =
{
    KEYCHRON_K2_HE_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_he_ansi_rgb_entries),
    k2_he_ansi_rgb_entries
};

static const keychron_patch k2_he_iso_rgb_patch =
{
    KEYCHRON_K2_HE_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_he_ansi_rgb_entries),
    k2_he_ansi_rgb_entries
};

static const keychron_patch k3_he_patch =
{
    KEYCHRON_K3_HE_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_he_ansi_rgb_entries),
    k2_he_ansi_rgb_entries
};

static const keychron_patch k3_he_iso_patch =
{
    KEYCHRON_K3_HE_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_he_ansi_rgb_entries),
    k2_he_ansi_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 134:                                       |
| 0x0E22 Keychron K2 HE JIS RGB                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry k2_he_jis_rgb_entries[] =
{
    {  2, 14,  2, 15 },
    {  3, 13,  3, 15 },
    {  4, 14,  4, 15 },
    {  5,  8,  5,  9 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch k2_he_jis_rgb_patch =
{
    KEYCHRON_K2_HE_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_he_jis_rgb_entries),
    k2_he_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 135:                                       |
| 0x0E23 Keychron K2 HE BR RGB                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry k2_he_br_rgb_entries[] =
{
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  4, 14,  4, 15 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch k2_he_br_rgb_patch =
{
    KEYCHRON_K2_HE_BR_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_he_br_rgb_entries),
    k2_he_br_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 136:                                       |
| 0x0E25 Keychron K2 HE V2 SILENT ANSI                      |
| 0x0E26 Keychron K2 HE V2 ISO                              |
| 0x1014 Keychron Q1 HE Marble 8K ANSI                      |
\*---------------------------------------------------------*/
static const keychron_patch_entry k2_he_v2_silent_ansi_entries[] =
{
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3, 14,  3, 15 },
    {  4, 13,  4, 14 },
    {  4, 14,  4, 15 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch k2_he_v2_silent_ansi_patch =
{
    KEYCHRON_K2_HE_V2_SILENT_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_he_v2_silent_ansi_entries),
    k2_he_v2_silent_ansi_entries
};

static const keychron_patch k2_he_v2_iso_patch =
{
    KEYCHRON_K2_HE_V2_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_he_v2_silent_ansi_entries),
    k2_he_v2_silent_ansi_entries
};

static const keychron_patch q1_he_marble_8k_ansi_patch =
{
    KEYCHRON_Q1_HE_MARBLE_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_he_v2_silent_ansi_entries),
    k2_he_v2_silent_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 137:                                       |
| 0x0E32 Keychron K3 HE JIS                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry k3_he_jis_entries[] =
{
    {  2, 14,  2, 15 },
    {  3, 13,  3, 15 },
    {  5,  8,  5,  9 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch k3_he_jis_patch =
{
    KEYCHRON_K3_HE_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k3_he_jis_entries),
    k3_he_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 138:                                       |
| 0x0E40 Keychron K4 HE ANSI                                |
| 0x0E41 Keychron K4 HE ISO                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry k4_he_ansi_entries[] =
{
    {  1, 14,  1, 13 },
    {  3, 13,  3, 12 },
    {  4, 13,  4, 12 },
};

static const keychron_patch k4_he_ansi_patch =
{
    KEYCHRON_K4_HE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k4_he_ansi_entries),
    k4_he_ansi_entries
};

static const keychron_patch k4_he_iso_patch =
{
    KEYCHRON_K4_HE_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k4_he_ansi_entries),
    k4_he_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 139:                                       |
| 0x0E80 Keychron K8 HE ANSI                                |
| 0x0E81 Keychron K8 HE ISO RGB                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry k8_he_ansi_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 16 },
    {  3, 15,  2, 16 },
    {  4, 13,  4, 12 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
};

static const keychron_patch k8_he_ansi_patch =
{
    KEYCHRON_K8_HE_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k8_he_ansi_entries),
    k8_he_ansi_entries
};

static const keychron_patch k8_he_iso_rgb_patch =
{
    KEYCHRON_K8_HE_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k8_he_ansi_entries),
    k8_he_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 140:                                       |
| 0x0E82 Keychron K8 HE JIS                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry k8_he_jis_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 11 },
    {  0, 10,  0, 12 },
    {  0, 11,  0, 13 },
    {  0, 12,  0, 14 },
    {  0, 13,  0, 15 },
    {  0, 14,  0, 16 },
    {  0, 15,  0, 17 },
    {  1, 13,  1, 14 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  3, 12,  1, 13 },
    {  3, 13,  3, 12 },
    {  3, 14,  1, 17 },
    {  3, 15,  2, 17 },
    {  4, 15,  4, 16 },
    {  5,  8,  5,  9 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 15 },
    {  5, 14,  5, 16 },
    {  5, 15,  5, 17 },
};

static const keychron_patch k8_he_jis_patch =
{
    KEYCHRON_K8_HE_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k8_he_jis_entries),
    k8_he_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 141:                                       |
| 0x0EA2 Keychron K10 HE JIS                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry k10_he_jis_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  1, 19,  1, 20 },
    {  1, 20,  1, 21 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  2, 19,  2, 20 },
    {  2, 20,  2, 21 },
    {  3, 13,  1, 14 },
    {  3, 17,  3, 18 },
    {  3, 18,  3, 19 },
    {  3, 19,  3, 20 },
    {  4, 15,  4, 16 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  4, 19,  4, 20 },
    {  4, 20,  4, 21 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 17,  5, 18 },
    {  5, 19,  5, 20 },
};

static const keychron_patch k10_he_jis_patch =
{
    KEYCHRON_K10_HE_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k10_he_jis_entries),
    k10_he_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 142:                                       |
| 0x0EA5 Keychron K10 HE ABNT2                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry k10_he_abnt2_entries[] =
{
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 18,  5, 19 },
};

static const keychron_patch k10_he_abnt2_patch =
{
    KEYCHRON_K10_HE_ABNT2_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k10_he_abnt2_entries),
    k10_he_abnt2_entries
};

/*---------------------------------------------------------*\
| Geometry Group 143:                                       |
| 0x0F50 Keychron V5 8K ANSI                                |
| 0x0F51 Keychron V5 8K ISO                                 |
| 0x0F53 Keychron V5 8K ANSI                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry v5_8k_ansi_0f50_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
};

static const keychron_patch v5_8k_ansi_0f50_patch =
{
    KEYCHRON_V5_8K_ANSI_0F50_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_8k_ansi_0f50_entries),
    v5_8k_ansi_0f50_entries
};

static const keychron_patch v5_8k_iso_patch =
{
    KEYCHRON_V5_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_8k_ansi_0f50_entries),
    v5_8k_ansi_0f50_entries
};

static const keychron_patch v5_8k_ansi_0f53_patch =
{
    KEYCHRON_V5_8K_ANSI_0F53_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v5_8k_ansi_0f50_entries),
    v5_8k_ansi_0f50_entries
};

/*---------------------------------------------------------*\
| Geometry Group 144:                                       |
| 0x0F60 Keychron V6 8K ANSI                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry v6_8k_ansi_entries[] =
{
    {  3, 20,  2, 20 },
    {  5, 19,  5, 18 },
    {  5, 20,  4, 20 },
};

static const keychron_patch v6_8k_ansi_patch =
{
    KEYCHRON_V6_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(v6_8k_ansi_entries),
    v6_8k_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 145:                                       |
| 0x1005 Keychron Q19 HE 8K                                 |
\*---------------------------------------------------------*/
static const keychron_patch_entry q19_he_8k_entries[] =
{
    {  3, 12,  3, 11 },
    {  3, 13,  3, 12 },
};

static const keychron_patch q19_he_8k_patch =
{
    KEYCHRON_Q19_HE_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q19_he_8k_entries),
    q19_he_8k_entries
};

/*---------------------------------------------------------*\
| Geometry Group 146:                                       |
| 0x1013 Keychron Q1 HE 8K ABNT2                            |
\*---------------------------------------------------------*/
static const keychron_patch_entry q1_he_8k_abnt2_entries[] =
{
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3, 14,  3, 15 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch q1_he_8k_abnt2_patch =
{
    KEYCHRON_Q1_HE_8K_ABNT2_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1_he_8k_abnt2_entries),
    q1_he_8k_abnt2_entries
};

/*---------------------------------------------------------*\
| Geometry Group 147:                                       |
| 0x1020 Keychron Q2 HE 8K                                  |
\*---------------------------------------------------------*/
static const keychron_patch_entry q2_he_8k_entries[] =
{
    {  3, 13,  3, 12 },
    {  3, 14,  3, 13 },
};

static const keychron_patch q2_he_8k_patch =
{
    KEYCHRON_Q2_HE_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q2_he_8k_entries),
    q2_he_8k_entries
};

/*---------------------------------------------------------*\
| Geometry Group 148:                                       |
| 0x1050 Keychron Q5 HE 8K                                  |
\*---------------------------------------------------------*/
static const keychron_patch_entry q5_he_8k_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  3, 14,  3, 15 },
    {  3, 15,  3, 16 },
    {  3, 16,  3, 17 },
    {  3, 17,  3, 18 },
    {  4, 13,  4, 12 },
    {  4, 15,  4, 16 },
    {  4, 16,  4, 17 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 17,  5, 18 },
};

static const keychron_patch q5_he_8k_patch =
{
    KEYCHRON_Q5_HE_8K_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_he_8k_entries),
    q5_he_8k_entries
};

/*---------------------------------------------------------*\
| Geometry Group 149:                                       |
| 0x1062 Keychron Q6 HE 8K JIS                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry q6_he_8k_jis_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  0, 18,  0, 19 },
    {  0, 19,  0, 20 },
    {  0, 20,  0, 21 },
    {  5, 19,  5, 20 },
};

static const keychron_patch q6_he_8k_jis_patch =
{
    KEYCHRON_Q6_HE_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_he_8k_jis_entries),
    q6_he_8k_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 150:                                       |
| 0x10F4 Keychron Q16 HE 8K ISO                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry q16_he_8k_iso_entries[] =
{
    {  0, 14,  0, 15 },
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3, 13,  3, 14 },
    {  3, 14,  3, 15 },
    {  4,  9,  4, 10 },
    {  4, 10,  4, 11 },
    {  4, 11,  4, 12 },
    {  4, 12,  4, 13 },
    {  4, 13,  4, 14 },
    {  4, 14,  4, 15 },
};

static const keychron_patch q16_he_8k_iso_patch =
{
    KEYCHRON_Q16_HE_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q16_he_8k_iso_entries),
    q16_he_8k_iso_entries
};

/*---------------------------------------------------------*\
| Geometry Group 151:                                       |
| 0x1212 Keychron Q1 Ultra 8K JIS                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry q1_ultra_8k_jis_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  1, 14,  1, 15 },
    {  2, 14,  2, 15 },
    {  3, 13,  3, 15 },
    {  3, 14,  1, 14 },
    {  5,  8,  5,  9 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
};

static const keychron_patch q1_ultra_8k_jis_patch =
{
    KEYCHRON_Q1_ULTRA_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q1_ultra_8k_jis_entries),
    q1_ultra_8k_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 152:                                       |
| 0x1232 Keychron Q3 Ultra 8K JIS RGB                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry q3_ultra_8k_jis_rgb_entries[] =
{
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  3, 12,  1, 14 },
    {  3, 13,  3, 12 },
    {  4, 15,  4, 16 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
};

static const keychron_patch q3_ultra_8k_jis_rgb_patch =
{
    KEYCHRON_Q3_ULTRA_8K_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q3_ultra_8k_jis_rgb_entries),
    q3_ultra_8k_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 153:                                       |
| 0x1252 Keychron Q5 Ultra 8K JIS                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry q5_ultra_8k_jis_entries[] =
{
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  0, 19,  0, 14 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  1, 19,  1, 14 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 19,  2, 13 },
    {  3, 14,  3, 15 },
    {  3, 15,  3, 16 },
    {  3, 16,  3, 17 },
    {  3, 17,  3, 18 },
    {  3, 18,  2, 19 },
    {  4, 14,  4, 15 },
    {  4, 15,  4, 16 },
    {  4, 16,  4, 17 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 17,  5, 18 },
    {  5, 19,  5, 14 },
};

static const keychron_patch q5_ultra_8k_jis_patch =
{
    KEYCHRON_Q5_ULTRA_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q5_ultra_8k_jis_entries),
    q5_ultra_8k_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 154:                                       |
| 0x1260 Keychron Q6 Ultra 8K ANSI                          |
| 0x1261 Keychron Q6 Ultra 8K ISO RGB                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry q6_ultra_8k_ansi_entries[] =
{
    {  3, 14,  2, 20 },
    {  3, 15,  1, 20 },
    {  3, 16,  0, 20 },
    {  5, 18,  5, 19 },
    {  5, 19,  4, 20 },
};

static const keychron_patch q6_ultra_8k_ansi_patch =
{
    KEYCHRON_Q6_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_ultra_8k_ansi_entries),
    q6_ultra_8k_ansi_entries
};

static const keychron_patch q6_ultra_8k_iso_rgb_patch =
{
    KEYCHRON_Q6_ULTRA_8K_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_ultra_8k_ansi_entries),
    q6_ultra_8k_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 155:                                       |
| 0x1262 Keychron Q6 Ultra 8K JIS RGB                       |
\*---------------------------------------------------------*/
static const keychron_patch_entry q6_ultra_8k_jis_rgb_entries[] =
{
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  0, 18,  0, 19 },
    {  0, 19,  0, 20 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  1, 19,  1, 20 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  2, 19,  2, 20 },
    {  3, 13,  1, 14 },
    {  3, 14,  2, 21 },
    {  3, 15,  1, 21 },
    {  3, 16,  0, 21 },
    {  3, 17,  3, 18 },
    {  3, 18,  3, 19 },
    {  3, 19,  3, 20 },
    {  4, 15,  4, 16 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  4, 19,  4, 20 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 17,  5, 18 },
    {  5, 18,  5, 20 },
    {  5, 19,  4, 21 },
};

static const keychron_patch q6_ultra_8k_jis_rgb_patch =
{
    KEYCHRON_Q6_ULTRA_8K_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q6_ultra_8k_jis_rgb_entries),
    q6_ultra_8k_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 156:                                       |
| 0x12D0 Keychron Q13 Ultra 8K RGB                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry q13_ultra_8k_rgb_entries[] =
{
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  4,  3,  4,  2 },
    {  4,  4,  4,  3 },
    {  4,  5,  4,  4 },
    {  4,  6,  4,  5 },
    {  4,  7,  4,  6 },
    {  5,  4,  5,  3 },
    {  5,  6,  5,  4 },
    {  5,  7,  5,  6 },
    {  5,  9,  5,  8 },
};

static const keychron_patch q13_ultra_8k_rgb_patch =
{
    KEYCHRON_Q13_ULTRA_8K_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(q13_ultra_8k_rgb_entries),
    q13_ultra_8k_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 157:                                       |
| 0x1362 Keychron K6 Ultra ZMK JIS                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry k6_ultra_zmk_jis_entries[] =
{
    {  1, 14,  1, 15 },
    {  2, 13,  0, 15 },
    {  2, 14,  2, 15 },
    {  3, 14,  3, 15 },
    {  4,  8,  4,  9 },
    {  4,  9,  4, 10 },
    {  4, 10,  4, 11 },
    {  4, 11,  4, 12 },
    {  4, 12,  4, 13 },
    {  4, 13,  4, 14 },
    {  4, 14,  4, 15 },
};

static const keychron_patch k6_ultra_zmk_jis_patch =
{
    KEYCHRON_K6_ULTRA_ZMK_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k6_ultra_zmk_jis_entries),
    k6_ultra_zmk_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 158:                                       |
| 0x1432 Keychron R3 JIS RGB                                |
\*---------------------------------------------------------*/
static const keychron_patch_entry r3_jis_rgb_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  3, 13,  1, 14 },
    {  4, 15,  4, 16 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
};

static const keychron_patch r3_jis_rgb_patch =
{
    KEYCHRON_R3_JIS_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(r3_jis_rgb_entries),
    r3_jis_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 159:                                       |
| 0x1450 Keychron R5 ANSI RGB                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry r5_ansi_rgb_entries[] =
{
    {  3, 17,  2, 17 },
    {  5, 17,  4, 17 },
};

static const keychron_patch r5_ansi_rgb_patch =
{
    KEYCHRON_R5_ANSI_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(r5_ansi_rgb_entries),
    r5_ansi_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 160:                                       |
| 0x1460 Keychron R6 ANSI RGB                               |
| 0x1463 Keychron R6 ANSI RGB                               |
\*---------------------------------------------------------*/
static const keychron_patch_entry r6_ansi_rgb_1460_entries[] =
{
    {  3, 13,  3, 12 },
    {  4, 13,  4, 12 },
    {  5, 20,  4, 20 },
};

static const keychron_patch r6_ansi_rgb_1460_patch =
{
    KEYCHRON_R6_ANSI_RGB_1460_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(r6_ansi_rgb_1460_entries),
    r6_ansi_rgb_1460_entries
};

static const keychron_patch r6_ansi_rgb_1463_patch =
{
    KEYCHRON_R6_ANSI_RGB_1463_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(r6_ansi_rgb_1460_entries),
    r6_ansi_rgb_1460_entries
};

/*---------------------------------------------------------*\
| Geometry Group 161:                                       |
| 0x1532 Keychron C3 HE 8K JIS                              |
\*---------------------------------------------------------*/
static const keychron_patch_entry c3_he_8k_jis_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 11 },
    {  0, 10,  0, 12 },
    {  0, 11,  0, 13 },
    {  0, 12,  0, 14 },
};

static const keychron_patch c3_he_8k_jis_patch =
{
    KEYCHRON_C3_HE_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(c3_he_8k_jis_entries),
    c3_he_8k_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 162:                                       |
| 0x1540 Keychron C4 HE 8K ANSI                             |
| 0x1541 Keychron C4 HE 8K ANSI                             |
\*---------------------------------------------------------*/
static const keychron_patch_entry c4_he_8k_ansi_1540_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  4, 13,  4, 12 },
    {  5, 18,  5, 19 },
};

static const keychron_patch c4_he_8k_ansi_1540_patch =
{
    KEYCHRON_C4_HE_8K_ANSI_1540_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(c4_he_8k_ansi_1540_entries),
    c4_he_8k_ansi_1540_entries
};

static const keychron_patch c4_he_8k_ansi_1541_patch =
{
    KEYCHRON_C4_HE_8K_ANSI_1541_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(c4_he_8k_ansi_1540_entries),
    c4_he_8k_ansi_1540_entries
};

/*---------------------------------------------------------*\
| Geometry Group 163:                                       |
| 0x1622 Keychron K2 Ultra 8K JIS                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k2_ultra_8k_jis_entries[] =
{
    {  3, 13,  3, 12 },
    {  4, 14,  4, 13 },
    {  4, 15,  4, 14 },
};

static const keychron_patch k2_ultra_8k_jis_patch =
{
    KEYCHRON_K2_ULTRA_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k2_ultra_8k_jis_entries),
    k2_ultra_8k_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 164:                                       |
| 0x1640 Keychron K4 Ultra 8K RGB                           |
| 0x1641 Keychron K4 Ultra 8K ISO                           |
| 0x1643 Keychron K4 Ultra 8K ISO                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k4_ultra_8k_rgb_entries[] =
{
    {  1, 14,  1, 13 },
    {  3, 18,  2, 18 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 18,  4, 18 },
};

static const keychron_patch k4_ultra_8k_rgb_patch =
{
    KEYCHRON_K4_ULTRA_8K_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k4_ultra_8k_rgb_entries),
    k4_ultra_8k_rgb_entries
};

static const keychron_patch k4_ultra_8k_iso_1641_patch =
{
    KEYCHRON_K4_ULTRA_8K_ISO_1641_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k4_ultra_8k_rgb_entries),
    k4_ultra_8k_rgb_entries
};

static const keychron_patch k4_ultra_8k_iso_1643_patch =
{
    KEYCHRON_K4_ULTRA_8K_ISO_1643_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k4_ultra_8k_rgb_entries),
    k4_ultra_8k_rgb_entries
};

/*---------------------------------------------------------*\
| Geometry Group 165:                                       |
| 0x1642 Keychron K4 Ultra 8K JIS                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k4_ultra_8k_jis_entries[] =
{
    {  3, 18,  2, 18 },
    {  5,  8,  5,  9 },
    {  5,  9,  5, 10 },
    {  5, 10,  5, 11 },
    {  5, 11,  5, 12 },
    {  5, 12,  5, 13 },
    {  5, 13,  5, 14 },
    {  5, 14,  5, 15 },
    {  5, 18,  4, 18 },
};

static const keychron_patch k4_ultra_8k_jis_patch =
{
    KEYCHRON_K4_ULTRA_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k4_ultra_8k_jis_entries),
    k4_ultra_8k_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 166:                                       |
| 0x1650 Keychron K5 Ultra 8K ANSI                          |
| 0x1651 Keychron K5 Ultra 8K ISO                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k5_ultra_8k_ansi_entries[] =
{
    {  3, 13,  3, 12 },
    {  3, 14,  0, 20 },
    {  3, 15,  1, 20 },
    {  3, 16,  2, 20 },
    {  4, 13,  4, 12 },
    {  5, 18,  4, 20 },
};

static const keychron_patch k5_ultra_8k_ansi_patch =
{
    KEYCHRON_K5_ULTRA_8K_ANSI_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k5_ultra_8k_ansi_entries),
    k5_ultra_8k_ansi_entries
};

static const keychron_patch k5_ultra_8k_iso_patch =
{
    KEYCHRON_K5_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k5_ultra_8k_ansi_entries),
    k5_ultra_8k_ansi_entries
};

/*---------------------------------------------------------*\
| Geometry Group 167:                                       |
| 0x1653 Keychron K5 Ultra 8K ABNT2                         |
\*---------------------------------------------------------*/
static const keychron_patch_entry k5_ultra_8k_abnt2_entries[] =
{
    {  3, 13,  3, 12 },
    {  3, 14,  0, 20 },
    {  3, 15,  1, 20 },
    {  3, 16,  2, 20 },
    {  5, 18,  4, 20 },
};

static const keychron_patch k5_ultra_8k_abnt2_patch =
{
    KEYCHRON_K5_ULTRA_8K_ABNT2_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k5_ultra_8k_abnt2_entries),
    k5_ultra_8k_abnt2_entries
};

/*---------------------------------------------------------*\
| Geometry Group 168:                                       |
| 0x1681 Keychron K8 Ultra 8K ISO                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k8_ultra_8k_iso_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 10 },
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  4, 13,  4, 12 },
};

static const keychron_patch k8_ultra_8k_iso_patch =
{
    KEYCHRON_K8_ULTRA_8K_ISO_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k8_ultra_8k_iso_entries),
    k8_ultra_8k_iso_entries
};

/*---------------------------------------------------------*\
| Geometry Group 169:                                       |
| 0x1682 Keychron K8 Ultra 8K JIS                           |
\*---------------------------------------------------------*/
static const keychron_patch_entry k8_ultra_8k_jis_entries[] =
{
    {  0,  1,  0,  2 },
    {  0,  2,  0,  3 },
    {  0,  3,  0,  4 },
    {  0,  4,  0,  5 },
    {  0,  5,  0,  6 },
    {  0,  6,  0,  7 },
    {  0,  7,  0,  8 },
    {  0,  8,  0,  9 },
    {  0,  9,  0, 11 },
    {  0, 10,  0, 12 },
    {  0, 11,  0, 13 },
    {  0, 12,  0, 14 },
    {  0, 13,  0, 15 },
    {  0, 14,  0, 16 },
    {  0, 15,  0, 17 },
    {  0, 16,  1, 17 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  3, 13,  3, 12 },
    {  4, 15,  4, 16 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
};

static const keychron_patch k8_ultra_8k_jis_patch =
{
    KEYCHRON_K8_ULTRA_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k8_ultra_8k_jis_entries),
    k8_ultra_8k_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 170:                                       |
| 0x16A2 Keychron K10 Ultra 8K JIS                          |
\*---------------------------------------------------------*/
static const keychron_patch_entry k10_ultra_8k_jis_entries[] =
{
    {  0, 10,  0, 11 },
    {  0, 11,  0, 12 },
    {  0, 12,  0, 13 },
    {  0, 13,  0, 14 },
    {  0, 14,  0, 15 },
    {  0, 15,  0, 16 },
    {  0, 16,  0, 17 },
    {  0, 17,  0, 18 },
    {  0, 18,  0, 19 },
    {  0, 19,  0, 20 },
    {  1, 14,  1, 15 },
    {  1, 15,  1, 16 },
    {  1, 16,  1, 17 },
    {  1, 17,  1, 18 },
    {  1, 18,  1, 19 },
    {  1, 19,  1, 20 },
    {  2, 14,  2, 15 },
    {  2, 15,  2, 16 },
    {  2, 16,  2, 17 },
    {  2, 17,  2, 18 },
    {  2, 18,  2, 19 },
    {  2, 19,  2, 20 },
    {  3, 13,  0, 21 },
    {  3, 14,  1, 21 },
    {  3, 15,  2, 21 },
    {  3, 16,  4, 21 },
    {  3, 17,  3, 18 },
    {  3, 18,  3, 19 },
    {  3, 19,  3, 20 },
    {  4, 14,  1, 14 },
    {  4, 15,  4, 16 },
    {  4, 17,  4, 18 },
    {  4, 18,  4, 19 },
    {  4, 19,  4, 20 },
    {  5,  3,  5,  4 },
    {  5,  6,  5,  5 },
    {  5, 14,  5, 15 },
    {  5, 15,  5, 16 },
    {  5, 16,  5, 17 },
    {  5, 19,  5, 20 },
};

static const keychron_patch k10_ultra_8k_jis_patch =
{
    KEYCHRON_K10_ULTRA_8K_JIS_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k10_ultra_8k_jis_entries),
    k10_ultra_8k_jis_entries
};

/*---------------------------------------------------------*\
| Geometry Group 171:                                       |
| 0x16A3 Keychron K10 Ultra 8K ISO RGB                      |
\*---------------------------------------------------------*/
static const keychron_patch_entry k10_ultra_8k_iso_rgb_entries[] =
{
    {  3, 14,  1, 20 },
    {  3, 15,  2, 20 },
    {  3, 16,  4, 20 },
    {  4, 13,  4, 12 },
    {  5, 18,  5, 17 },
};

static const keychron_patch k10_ultra_8k_iso_rgb_patch =
{
    KEYCHRON_K10_ULTRA_8K_ISO_RGB_PID,
    KEYCHRON_PATCH_ENTRIES_COUNT(k10_ultra_8k_iso_rgb_entries),
    k10_ultra_8k_iso_rgb_entries
};

/*---------------------------------------------------------*\
| List of all Keychron matrix patches                       |
\*---------------------------------------------------------*/
const keychron_patch* const keychron_patches[] =
{
    &q1v2_ansi_0106_patch,
    &q1v2_ansi_0107_patch,
    &q1v2_iso_0108_patch,
    &q1v2_iso_0109_patch,
    &q1v2_jis_010a_patch,
    &q1v2_jis_010b_patch,
    &q2_ansi_0110_patch,
    &q2_ansi_0111_patch,
    &q2_iso_0112_patch,
    &q2_iso_0113_patch,
    &q3_ansi_0120_patch,
    &q3_ansi_0121_patch,
    &q3_iso_0122_patch,
    &q3_iso_0123_patch,
    &q4_ansi_patch,
    &q4_iso_patch,
    &q5_ansi_0150_patch,
    &q5_ansi_0151_patch,
    &q5_iso_0152_patch,
    &q5_iso_0153_patch,
    &q6_ansi_0160_patch,
    &q6_ansi_0161_patch,
    &q6_iso_patch,
    &q6_ansi_0163_patch,
    &q7_ansi_patch,
    &q7_iso_patch,
    &q8_ansi_0180_patch,
    &q8_ansi_0181_patch,
    &q8_iso_0182_patch,
    &q8_iso_0183_patch,
    &q10_ansi_patch,
    &q10_iso_patch,
    &q65_ansi_patch,
    &q60_ansi_patch,
    &q12_ansi_patch,
    &q12_iso_patch,
    &k1_pro_iso_rgb_patch,
    &k2_pro_rgb_patch,
    &k3_pro_0230_patch,
    &k3_pro_0231_patch,
    &k4_pro_ansi_rgb_patch,
    &k5_pro_ansi_rgb_patch,
    &k5_pro_iso_rgb_patch,
    &k6_pro_rgb_patch,
    &k6_pro_iso_rgb_patch,
    &k7_pro_rgb_patch,
    &k7_pro_iso_rgb_patch,
    &k8_pro_rgb_patch,
    &k8_pro_jis_rgb_patch,
    &k10_pro_ansi_rgb_patch,
    &k10_pro_iso_rgb_patch,
    &k10_pro_jis_rgb_patch,
    &v1_ansi_0310_patch,
    &v1_ansi_0311_patch,
    &v1_iso_0312_patch,
    &v1_iso_0313_patch,
    &v1_br_patch,
    &v2_ansi_0320_patch,
    &v2_ansi_0321_patch,
    &v2_iso_patch,
    &v2_br_patch,
    &v3_ansi_0330_patch,
    &v3_ansi_0331_patch,
    &v3_iso_0332_patch,
    &v3_iso_0333_patch,
    &v3_br_patch,
    &v4_ansi_patch,
    &v4_iso_patch,
    &v5_ansi_0350_patch,
    &v5_ansi_0351_patch,
    &v5_iso_0352_patch,
    &v5_iso_0353_patch,
    &v6_ansi_0360_patch,
    &v6_ansi_0361_patch,
    &v6_iso_0362_patch,
    &v6_iso_0363_patch,
    &v6_br_patch,
    &v7_ansi_patch,
    &v7_iso_patch,
    &v8_ansi_patch,
    &v10_ansi_patch,
    &v10_iso_patch,
    &p1_he_dayz_patch,
    &p1_he_dayz_iso_patch,
    &j1_patch,
    &j2_patch,
    &j4_patch,
    &j5_patch,
    &z6_ultra_patch,
    &j3_he_ansi_patch,
    &p6_ultra_8k_patch,
    &j9_patch,
    &j9_iso_patch,
    &j9_jis_patch,
    &j2_he_8k_ansi_patch,
    &j8_he_8k_ansi_patch,
    &j8_he_8k_iso_patch,
    &j8_he_8k_jis_patch,
    &keycheron_j7_patch,
    &j14_he_patch,
    &j12_he_patch,
    &z6_75_ultra_8k_rgb_patch,
    &j4_he_ansi_patch,
    &p6_ultra_8k_iso_patch,
    &j15_he_rgb_patch,
    &x3_ansi_rgb_patch,
    &j9_ultra_8k_ansi_patch,
    &j3_se_patch,
    &z5_98_ultra_8k_ansi_patch,
    &v1_he_ansi_patch,
    &c3_pro_8k_ansi_0437_patch,
    &c3_max_ansi_patch,
    &z11_ultra_l_patch,
    &z11_ultra_r_patch,
    &j5_he_8k_iso_patch,
    &j2_ultra_8k_ansi_patch,
    &j5_ultra_8k_patch,
    &j11_ultra_8k_044c_patch,
    &z5_75_ultra_8k_ansi_patch,
    &j3_ultra_8k_ansi_patch,
    &j11_ultra_8k_044f_patch,
    &j10_ultra_8k_ansi_0451_patch,
    &j10_ultra_8k_ansi_0452_patch,
    &j5_ansi_patch,
    &j2_ansi_patch,
    &j4_ansi_rgb_patch,
    &j6_ansi_patch,
    &ex75_he_8k_ansi_patch,
    &ex99_he_8k_ansi_patch,
    &keychroon_j3_tmr_045e_patch,
    &keychroon_j3_tmr_045f_patch,
    &c75_tmr_8k_jis_patch,
    &j6_ultra_8k_ansi_patch,
    &cm2_h1_ansi_patch,
    &j6_full_size_8k_patch,
    &j2_tkl_8k_patch,
    &j5_ultra_8k_iso_patch,
    &j8_patch,
    &j6_full_size_8k_iso_patch,
    &z7_max_ansi_rgb_patch,
    &c1_pro_8k_iso_rgb_patch,
    &c1_pro_8k_jis_rgb_patch,
    &c1_pro_8k_ansi_rgb_patch,
    &c2_pro_8k_ansi_rgb_patch,
    &c2_pro_8k_iso_rgb_patch,
    &c3_pro_8k_ansi_0530_patch,
    &c3_pro_8k_iso_rgb_patch,
    &c3_pro_8k_jis_patch,
    &c3_pro_8k_abtn2_patch,
    &c4_pro_jis_rgb_patch,
    &q1_pro_ansi_patch,
    &q1_pro_iso_patch,
    &q2_pro_ansi_patch,
    &q2_pro_iso_patch,
    &q3_pro_patch,
    &q3_pro_ansi_patch,
    &q3_pro_iso_patch,
    &q4_pro_ansi_rgb_patch,
    &q5_pro_ansi_patch,
    &q5_pro_iso_patch,
    &q6_pro_ansi_patch,
    &q6_pro_iso_patch,
    &q8_pro_ansi_patch,
    &q10_pro_ansi_patch,
    &q1_max_ansi_patch,
    &q1_max_iso_patch,
    &q1_max_jis_patch,
    &q2_max_ansi_patch,
    &q2_max_iso_patch,
    &q3_max_ansi_patch,
    &q3_max_iso_patch,
    &q5_max_ansi_patch,
    &q5_max_iso_patch,
    &q5_max_jis_patch,
    &q6_max_ansi_patch,
    &q6_max_iso_patch,
    &q8_max_ansi_patch,
    &q10_max_ansi_patch,
    &q10_max_iso_patch,
    &q65_max_ansi_patch,
    &q11_ultra_8k_r_patch,
    &q60_max_patch,
    &q12_max_ansi_patch,
    &q12_max_iso_patch,
    &q13_max_ansi_patch,
    &q13_max_jis_patch,
    &q14_max_ansi_patch,
    &q15_max_ansi_patch,
    &v1_max_ansi_patch,
    &v2_ultra_iso_patch,
    &v1_max_jis_patch,
    &v3_max_jis_patch,
    &v4_max_ansi_patch,
    &v5_max_ansi_patch,
    &v5_max_iso_patch,
    &v5_max_jis_patch,
    &v6_max_ansi_patch,
    &v6_max_iso_patch,
    &v6_max_jis_patch,
    &v8_max_ansi_patch,
    &v10_max_ansi_patch,
    &v10_max_iso_patch,
    &k17_max_ansi_rgb_patch,
    &k17_max_iso_rgb_patch,
    &k0_max_rgb_patch,
    &k17_max_jis_rgb_patch,
    &k0_max_patch,
    &k1_max_ansi_rgb_patch,
    &k1_max_iso_rgb_patch,
    &k1_max_jis_rgb_0a12_patch,
    &k1_max_jis_rgb_0a16_patch,
    &k2_max_ansi_rgb_patch,
    &k2_max_iso_rgb_patch,
    &k2_max_jis_rgb_patch,
    &k3_max_ansi_rgb_0a30_patch,
    &k3_max_iso_rgb_patch,
    &k3_max_jis_rgb_0a32_patch,
    &k3_max_jis_rgb_0a39_patch,
    &k3_max_jis_rgb_0a3b_patch,
    &k3_max_ansi_rgb_0a3f_patch,
    &k4_max_ansi_rgb_patch,
    &k4_max_iso_rgb_patch,
    &k4_max_jis_rgb_patch,
    &k5_max_ansi_rgb_patch,
    &k5_max_iso_rgb_patch,
    &k5_max_jis_rgb_0a52_patch,
    &k5_max_jis_rgb_0a58_patch,
    &k7_max_ansi_rgb_patch,
    &k7_max_iso_rgb_patch,
    &k7_max_v2_jis_rgb_patch,
    &k8_max_ansi_rgb_patch,
    &k8_max_iso_rgb_patch,
    &k9_max_ansi_rgb_patch,
    &k10_max_ansi_rgb_patch,
    &k10_max_iso_rgb_patch,
    &k10_max_jis_rgb_patch,
    &k10_max_us_v2_rgb_patch,
    &k11_max_ansi_rgb_patch,
    &k11_max_iso_rgb_patch,
    &k11_max_jis_rgb_patch,
    &k13_max_ansi_rgb_patch,
    &k13_max_iso_rgb_patch,
    &k13_max_jis_rgb_patch,
    &k15_max_ansi_rgb_patch,
    &k15_max_iso_rgb_patch,
    &k15_max_jis_rgb_patch,
    &q1_he_ansi_patch,
    &q1_he_iso_patch,
    &q1_he_jis_patch,
    &q1_he_patch,
    &q1_he_v2_silent_ansi_patch,
    &q2_he_ansi_patch,
    &q3_he_ansi_patch,
    &q3_he_iso_patch,
    &q3_he_jis_patch,
    &q4_he_ansi_patch,
    &q5_he_ansi_patch,
    &q5_he_iso_patch,
    &q5_he_jis_patch,
    &q6_he_ansi_0b60_patch,
    &q6_he_iso_patch,
    &q6_he_jis_patch,
    &q6_he_ansi_0b63_patch,
    &q6_he_v2_silent_ansi_patch,
    &q12_he_ansi_patch,
    &q12_he_iso_patch,
    &v0_ultra_8k_ansi_patch,
    &v1_ultra_8k_ansi_patch,
    &v1_ultra_8k_iso_patch,
    &v1_ultra_8k_jis_patch,
    &v1_ultra_se_ansi_patch,
    &v1_ultra_8k_patch,
    &v3_ultra_8k_jis_patch,
    &v5_ultra_8k_ansi_patch,
    &v5_ultra_iso_patch,
    &v5_ultra_jis_patch,
    &v6_ultra_8k_ansi_patch,
    &v6_ultra_iso_patch,
    &v6_ultra_8k_jis_patch,
    &v6_ultra_se_ansi_patch,
    &v10_ultra_8k_ansi_patch,
    &k1_v6_ansi_rgb_patch,
    &k1_v6_iso_rgb_patch,
    &k1_v6_jis_rgb_patch,
    &k2_v3_ansi_rgb_patch,
    &k2_v3_iso_rgb_patch,
    &k2_v3_jis_rgb_patch,
    &k3_v3_ansi_rgb_patch,
    &k3_v3_iso_rgb_patch,
    &k3_v3_jis_rgb_patch,
    &k3_max_se_rgb_patch,
    &k4_v3_ansi_rgb_patch,
    &k4_v3_iso_rgb_patch,
    &k5_v2_ansi_rgb_patch,
    &k5_v2_iso_rgb_patch,
    &k5_v2_jis_rgb_patch,
    &k6x_us_rgb_patch,
    &k8_v2_ansi_rgb_patch,
    &k10_v2_ansi_rgb_patch,
    &k2_he_ansi_rgb_patch,
    &k2_he_iso_rgb_patch,
    &k2_he_jis_rgb_patch,
    &k2_he_br_rgb_patch,
    &k2_he_v2_silent_ansi_patch,
    &k2_he_v2_iso_patch,
    &k3_he_patch,
    &k3_he_iso_patch,
    &k3_he_jis_patch,
    &k4_he_ansi_patch,
    &k4_he_iso_patch,
    &k4_he_jis_patch,
    &k6_he_ansi_patch,
    &k6_he_iso_rgb_patch,
    &k8_he_ansi_patch,
    &k8_he_iso_rgb_patch,
    &k8_he_jis_patch,
    &k10_he_jis_patch,
    &k10_he_iso_0ea4_patch,
    &k10_he_abnt2_patch,
    &k10_he_v3_ansi_rgb_patch,
    &v1_8k_ansi_patch,
    &v1_8k_iso_patch,
    &v3_8k_ansi_patch,
    &v3_8k_iso_patch,
    &v5_8k_ansi_0f50_patch,
    &v5_8k_iso_patch,
    &v5_8k_ansi_0f53_patch,
    &v6_8k_ansi_patch,
    &v6_8k_iso_patch,
    &q19_he_8k_patch,
    &q1_he_8k_ansi_1010_patch,
    &q1_he_8k_iso_rgb_patch,
    &q1_he_8k_jis_patch,
    &q1_he_8k_abnt2_patch,
    &q1_he_marble_8k_ansi_patch,
    &q1_he_8k_ansi_1016_patch,
    &q2_he_8k_patch,
    &q3_he_8k_patch,
    &q3_he_8k_iso_patch,
    &q3_he_8k_jis_patch,
    &q5_he_8k_patch,
    &q6_he_8k_patch,
    &q6_he_8k_iso_patch,
    &q6_he_8k_jis_patch,
    &q16_he_8k_iso_patch,
    &q1_ultra_8k_ansi_1210_patch,
    &q1_ultra_8k_iso_patch,
    &q1_ultra_8k_jis_patch,
    &q1_ultra_8k_ansi_1213_patch,
    &q3_ultra_8k_ansi_patch,
    &q3_ultra_8k_iso_rgb_patch,
    &q3_ultra_8k_jis_rgb_patch,
    &q5_ultra_8k_patch,
    &q5_ultra_8k_iso_patch,
    &q5_ultra_8k_jis_patch,
    &q5_ultra_8k_ansi_patch,
    &q6_ultra_8k_ansi_patch,
    &q6_ultra_8k_iso_rgb_patch,
    &q6_ultra_8k_jis_rgb_patch,
    &q13_ultra_8k_rgb_patch,
    &k6_ultra_zmk_jis_patch,
    &k10_pro_ss_zmk_rgb_patch,
    &k10_ultra_se_ansi_patch,
    &r3_jis_rgb_patch,
    &r5_ansi_rgb_patch,
    &r6_ansi_rgb_1460_patch,
    &r6_ansi_rgb_1463_patch,
    &c3_he_8k_jis_patch,
    &c4_he_8k_ansi_1540_patch,
    &c4_he_8k_ansi_1541_patch,
    &k1_ultra_8k_patch,
    &k1_ultra_8k_iso_rgb_patch,
    &k2_ultra_8k_ansi_patch,
    &k2_ultra_8k_iso_patch,
    &k2_ultra_8k_jis_patch,
    &k3_ultra_8k_ansi_patch,
    &k3_ultra_8k_iso_patch,
    &k3_ultra_8k_jis_1632_patch,
    &k3_ultra_8k_jis_1634_patch,
    &k4_ultra_8k_rgb_patch,
    &k4_ultra_8k_iso_1641_patch,
    &k4_ultra_8k_jis_patch,
    &k4_ultra_8k_iso_1643_patch,
    &k5_ultra_8k_ansi_patch,
    &k5_ultra_8k_iso_patch,
    &k5_ultra_8k_abnt2_patch,
    &k8_ultra_8k_ansi_patch,
    &k8_ultra_8k_iso_patch,
    &k8_ultra_8k_jis_patch,
    &k10_ultra_8k_rgb_patch,
    &k10_ultra_8k_iso_patch,
    &k10_ultra_8k_jis_patch,
    &k10_ultra_8k_iso_rgb_patch,
    &q12_he_patch,
};

const unsigned int KEYCHRON_PATCH_COUNT = sizeof(keychron_patches) / sizeof(keychron_patches[0]);
