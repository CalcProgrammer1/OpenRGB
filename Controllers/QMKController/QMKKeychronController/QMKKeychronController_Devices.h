/*---------------------------------------------------------*\
| QMKKechronController_Devices.h                            |
|                                                           |
|   Keychron device IDs and matrix correction data          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

/*---------------------------------------------------------*\
| Keychron product IDs                                      |
\*---------------------------------------------------------*/
#define KEYCHRON_C1_PRO_8K_PID                      0x0521
#define KEYCHRON_C2_PRO_8K_PID                      0x0522
#define KEYCHRON_C3_PRO_8K_JIS_PID                  0x0532
#define KEYCHRON_K1_MAX_PID                         0x0A10
#define KEYCHRON_K1_MAX_ISO_PID                     0x0A11
#define KEYCHRON_K1_MAX_JIS_PID                     0x0A12
#define KEYCHRON_K1_V6_JIS_PID                      0x0D12
#define KEYCHRON_K10_HE_PID                         0x0EA0
#define KEYCHRON_K10_HE_ISO_PID                     0x0EA1
#define KEYCHRON_K10_MAX_PID                        0x0AA0
#define KEYCHRON_K10_MAX_ISO_PID                    0x0AA1
#define KEYCHRON_K10_MAX_JIS_PID                    0x0AA2
#define KEYCHRON_K10_ULTRA_8K_ISO_16A3_PID          0x16A3
#define KEYCHRON_K10_V2_ISO_PID                     0x0DA1
#define KEYCHRON_K10_V2_JIS_PID                     0x0DA2
#define KEYCHRON_K10X_PID                           0x0DA0
#define KEYCHRON_K11_MAX_ISO_KNOB_PID               0x0AB4
#define KEYCHRON_K11_MAX_JIS_KNOB_PID               0x0AB5
#define KEYCHRON_K11_MAX_KNOB_PID                   0x0AB3
#define KEYCHRON_K13_MAX_PID                        0x0AD0
#define KEYCHRON_K13_MAX_ISO_PID                    0x0AD1
#define KEYCHRON_K13_MAX_JIS_PID                    0x0AD2
#define KEYCHRON_K15_MAX_ISO_KNOB_PID               0x0AF1
#define KEYCHRON_K15_MAX_JIS_KNOB_PID               0x0AF2
#define KEYCHRON_K15_MAX_KNOB_PID                   0x0AF0
#define KEYCHRON_K17_MAX_ISO_KNOB_PID               0x0A01
#define KEYCHRON_K17_MAX_JIS_KNOB_PID               0x0A02
#define KEYCHRON_K17_MAX_KNOB_PID                   0x0A00
#define KEYCHRON_K1X_ISO_PID                        0x0D11
#define KEYCHRON_K1X_US_PID                         0x0D10
#define KEYCHRON_K2_HE_ANSI_PID                     0x0E20
#define KEYCHRON_K2_HE_ISO_PID                      0x0E21
#define KEYCHRON_K2_HE_JIS_PID                      0x0E22
#define KEYCHRON_K2_MAX_PID                         0x0A20
#define KEYCHRON_K2_MAX_ISO_PID                     0x0A21
#define KEYCHRON_K2_MAX_JIS_PID                     0x0A22
#define KEYCHRON_K2_V3_ISO_PID                      0x0D21
#define KEYCHRON_K2X_PID                            0x0D20
#define KEYCHRON_K3_MAX_PID                         0x0A30
#define KEYCHRON_K3_MAX_ISO_PID                     0x0A31
#define KEYCHRON_K3_MAX_JIS_PID                     0x0A32
#define KEYCHRON_K3_V3_ISO_PID                      0x0D31
#define KEYCHRON_K3_V3_JIS_PID                      0x0D32
#define KEYCHRON_K3X_PID                            0x0D30
#define KEYCHRON_K4_HE_PID                          0x0E40
#define KEYCHRON_K4_HE_ISO_PID                      0x0E41
#define KEYCHRON_K4_HE_JIS_PID                      0x0E42
#define KEYCHRON_K4_MAX_ANSI_PID                    0x0A40
#define KEYCHRON_K4X_JIS_PID                        0x0D42
#define KEYCHRON_K5_MAX_PID                         0x0A50
#define KEYCHRON_K5_MAX_ISO_PID                     0x0A51
#define KEYCHRON_K5_MAX_JIS_0A52_PID                0x0A52
#define KEYCHRON_K5X_ISO_PID                        0x0D51
#define KEYCHRON_K6_HE_PID                          0x0E60
#define KEYCHRON_K7_MAX_PID                         0x0A70
#define KEYCHRON_K7_MAX_ISO_PID                     0x0A71
#define KEYCHRON_K7_MAX_JIS_PID                     0x0A72
#define KEYCHRON_K7_MAX_JIS_V2_PID                  0x0A76
#define KEYCHRON_K8_HE_PID                          0x0E80
#define KEYCHRON_K8_HE_ISO_PID                      0x0E81
#define KEYCHRON_K8_HE_JIS_RBG_PID                  0x0E82
#define KEYCHRON_K8_MAX_PID                         0x0A80
#define KEYCHRON_K8_MAX_ISO_PID                     0x0A81
#define KEYCHRON_K8_MAX_JIS_PID                     0x0A82
#define KEYCHRON_K8_V2_PID                          0x0D80
#define KEYCHRON_K8_V2_JIS_PID                      0x0D82
#define KEYCHRON_K9_MAX_PID                         0x0A90
#define KEYCHRON_C0_HE_8K_PID                       0x1500
#define KEYCHRON_C1_PRO_8K_ISO_PID                  0x051D
#define KEYCHRON_C1_PRO_8K_JIS_PID                  0x051E
#define KEYCHRON_C2_PRO_ISO_8K_PID                  0x052D
#define KEYCHRON_C3_PRO_8K_PID                      0x0530
#define KEYCHRON_C3_PRO_ISO_8K_PID                  0x0531
#define KEYCHRON_J1_PID                             0x0405
#define KEYCHRON_J2_PID                             0x0406
#define KEYCHRON_J2_ANSI_KNOB_PID                   0x0456
#define KEYCHRON_J2_HE_8K_ANSI_PID                  0x0416
#define KEYCHRON_J3_SE_KNOB_PID                     0x042D
#define KEYCHRON_J4_PID                             0x0408
#define KEYCHRON_J4_ANSI_PID                        0x0457
#define KEYCHRON_J5_PID                             0x0409
#define KEYCHRON_J5_ANSI_KNOB_PID                   0x0453
#define KEYCHRON_K0_MAX_PID                         0x0A06
#define KEYCHRON_K1_ULTRA_8K_KNOB_PID               0x1610
#define KEYCHRON_K10_ULTRA_8K_PID                   0x16A0
#define KEYCHRON_K10_ULTRA_8K_ISO_16A1_PID          0x16A1
#define KEYCHRON_K2_ULTRA_8K_ANSI_PID               0x1620
#define KEYCHRON_K2_ULTRA_8K_ISO_PID                0x1621
#define KEYCHRON_K2_V3_JIS_PID                      0x0D22
#define KEYCHRON_K3_HE_PID                          0x0E30
#define KEYCHRON_K3_HE_ISO_PID                      0x0E31
#define KEYCHRON_K3_MAX_SE_PID                      0x0D3B
#define KEYCHRON_K3_ULTRA_8K_ANSI_PID               0x1630
#define KEYCHRON_K3_ULTRA_8K_ISO_PID                0x1631
#define KEYCHRON_K4_MAX_ISO_PID                     0x0A41
#define KEYCHRON_K4_MAX_JIS_PID                     0x0A42
#define KEYCHRON_K4_ULTRA_8K_PID                    0x1640
#define KEYCHRON_K4_V3_PID                          0x0D40
#define KEYCHRON_K4_V3_ISO_PID                      0x0D41
#define KEYCHRON_K5_MAX_JIS_0A58_PID                0x0A58
#define KEYCHRON_K5_ULTRA_8K_KNOB_ANSI_PID          0x1650
#define KEYCHRON_K5_ULTRA_8K_KNOB_ISO_PID           0x1651
#define KEYCHRON_K5_V2_PID                          0x0D50
#define KEYCHRON_K5_V2_JIS_PID                      0x0D52
#define KEYCHRON_K6_HE_ISO_PID                      0x0E61
#define KEYCHRON_K8_ULTRA_8K_ANSI_PID               0x1680
#define KEYCHRON_K8_ULTRA_8K_ISO_PID                0x1681
#define KEYCHRON_K8_V2_ISO_PID                      0x0D81
#define KEYCHRON_Q0_HE_V2_KNOB_PID                  0x0B01
#define KEYCHRON_Q1_HE_PID                          0x0B14
#define KEYCHRON_Q1_HE_8K_ANSI_PID                  0x1010
#define KEYCHRON_Q1_HE_8K_ISO_KNOB_PID              0x1011
#define KEYCHRON_Q1_HE_ANSI_V2_SILENT_PID           0x0B15
#define KEYCHRON_Q1_HE_DAYZ_PID                     0x0B13
#define KEYCHRON_Q1_ULTRA_8K_ANSI_PID               0x1210
#define KEYCHRON_Q11_ULTRA_8K_KNOB_PID              0x12B0
#define KEYCHRON_Q11_ULTRA_8K_KNOB_ISO_PID          0x12B2
#define KEYCHRON_Q12_HE_ISO_KNOB_PID                0x0BC1
#define KEYCHRON_Q12_HE_KNOB_PID                    0x0BC0
#define KEYCHRON_Q13_MAX_JIS_KNOB_PID               0x08D2
#define KEYCHRON_Q13_ULTRA_8K_KNOB_PID              0x12D0
#define KEYCHRON_Q16_HE_8K_10F3_PID                 0x10F3
#define KEYCHRON_Q16_HE_8K_10F6_PID                 0x10F6
#define KEYCHRON_Q16_HE_8K_ISO_PID                  0x10F4
#define KEYCHRON_Q2_HE_8K_PID                       0x1020
#define KEYCHRON_Q3_HE_8K_PID                       0x1030
#define KEYCHRON_Q3_HE_8K_ANSI_PID                  0x1033
#define KEYCHRON_Q3_HE_8K_ISO_PID                   0x1031
#define KEYCHRON_Q3_HE_8K_JIS_PID                   0x1032
#define KEYCHRON_Q3_HE_JIS_KNOB_PID                 0x0B32
#define KEYCHRON_Q3_ULTRA_8K_KNOB_ISO_PID           0x1231
#define KEYCHRON_Q5_HE_8K_1050_PID                  0x1050
#define KEYCHRON_Q5_HE_8K_1053_PID                  0x1053
#define KEYCHRON_Q5_ULTRA_8K_KNOB_PID               0x1250
#define KEYCHRON_Q5_ULTRA_8K_KNOB_ANSI_PID          0x1253
#define KEYCHRON_Q6_HE_8K_1060_PID                  0x1060
#define KEYCHRON_Q6_HE_8K_1063_PID                  0x1063
#define KEYCHRON_Q6_HE_8K_ISO_PID                   0x1061
#define KEYCHRON_Q6_HE_8K_JIS_PID                   0x1062
#define KEYCHRON_Q6_HE_ANSI_KNOB_0B60_PID           0x0B60
#define KEYCHRON_Q6_HE_ANSI_KNOB_0B63_PID           0x0B63
#define KEYCHRON_Q6_HE_ISO_KNOB_PID                 0x0B61
#define KEYCHRON_Q6_HE_JIS_KNOB_PID                 0x0B62
#define KEYCHRON_Q6_HE_V2_ANSI_KNOB_SILENT_PID      0x0B64
#define KEYCHRON_Q6_ULTRA_8K_ANSI_PID               0x1260
#define KEYCHRON_Q6_ULTRA_8K_KNOB_ISO_PID           0x1261
#define KEYCHRON_V1_8K_ANSI_KNOB_PID                0x0F10
#define KEYCHRON_V1_ULTRA_8K_0C10_PID               0x0C10
#define KEYCHRON_V1_ULTRA_8K_0C14_PID               0x0C14
#define KEYCHRON_V1_ULTRA_8K_ISO_PID                0x0C11
#define KEYCHRON_V1_ULTRA_8K_JIS_PID                0x0C12
#define KEYCHRON_V1_ULTRA_SE_KNOB_ANSI_PID          0x0C13
#define KEYCHRON_V10_ULTRA_8K_KNOB_PID              0x0CA0
#define KEYCHRON_V2_MAX_JIS_KNOB_PID                0x0922
#define KEYCHRON_V2_ULTRA_8K_PID                    0x0C20
#define KEYCHRON_V3_ULTRA_8K_PID                    0x0C30
#define KEYCHRON_V3_ULTRA_8K_KNOB_ISO_PID           0x0C31
#define KEYCHRON_V3_ULTRA_8K_KNOB_JIS_PID           0x0C32
#define KEYCHRON_V5_8K_ANSI_KNOB_0F50_PID           0x0F50
#define KEYCHRON_V5_8K_ANSI_KNOB_0F53_PID           0x0F53
#define KEYCHRON_V5_ULTRA_ANSI_PID                  0x0C50
#define KEYCHRON_V6_8K_ANSI_KNOB_PID                0x0F60
#define KEYCHRON_V6_8K_ISO_KNOB_PID                 0x0F61
#define KEYCHRON_V6_ULTRA_ANSI_PID                  0x0C60
#define KEYCHRON_Q0_HE_KNOB_PID                     0x0B00
#define KEYCHRON_Q0_MAX_KNOB_PID                    0x0800
#define KEYCHRON_Q1_HE_8K_JIS_PID                   0x1012
#define KEYCHRON_Q1_HE_ISO_PID                      0x0B11
#define KEYCHRON_Q1_HE_JIS_PID                      0x0B12
#define KEYCHRON_Q1_HE_KNOB_PID                     0x0B10
#define KEYCHRON_Q1_MAX_ISO_KNOB_PID                0x0811
#define KEYCHRON_Q1_MAX_JIS_KNOB_PID                0x0812
#define KEYCHRON_Q1_MAX_KNOB_PID                    0x0810
#define KEYCHRON_Q10_MAX_ISO_KNOB_PID               0x08A1
#define KEYCHRON_Q10_MAX_JIS_PID                    0x08A2
#define KEYCHRON_Q10_MAX_KNOB_PID                   0x08A0
#define KEYCHRON_Q12_MAX_ISO_KNOB_PID               0x08C4
#define KEYCHRON_Q12_MAX_JIS_KNOB_PID               0x08C5
#define KEYCHRON_Q12_MAX_KNOB_PID                   0x08C3
#define KEYCHRON_Q13_MAX_KNOB_PID                   0x08D0
#define KEYCHRON_Q14_MAX_KNOB_PID                   0x08E0
#define KEYCHRON_Q15_MAX_KNOB_PID                   0x08F0
#define KEYCHRON_Q2_HE_KNOB_PID                     0x0B20
#define KEYCHRON_Q2_MAX_ISO_KNOB_PID                0x0821
#define KEYCHRON_Q2_MAX_JIS_KNOB_PID                0x0822
#define KEYCHRON_Q2_MAX_KNOB_PID                    0x0820
#define KEYCHRON_Q3_HE_PID                          0x0B30
#define KEYCHRON_Q3_HE_ISO_KNOB_PID                 0x0B31
#define KEYCHRON_Q3_MAX_ISO_KNOB_PID                0x0831
#define KEYCHRON_Q3_MAX_JIS_KNOB_PID                0x0832
#define KEYCHRON_Q3_MAX_KNOB_PID                    0x0830
#define KEYCHRON_Q3_ULTRA_PID                       0x1230
#define KEYCHRON_Q4_HE_PID                          0x0B40
#define KEYCHRON_Q5_HE_ISO_KNOB_PID                 0x0B51
#define KEYCHRON_Q5_HE_JIS_KNOB_PID                 0x0B52
#define KEYCHRON_Q5_HE_KNOB_PID                     0x0B50
#define KEYCHRON_Q5_MAX_ISO_KNOB_PID                0x0851
#define KEYCHRON_Q5_MAX_JIS_KNOB_PID                0x0852
#define KEYCHRON_Q5_MAX_KNOB_PID                    0x0850
#define KEYCHRON_Q6_HE_VERSION2_ISO_PID             0x0B65
#define KEYCHRON_Q6_MAX_ISO_KNOB_PID                0x0861
#define KEYCHRON_Q6_MAX_JIS_KNOB_PID                0x0862
#define KEYCHRON_Q6_MAX_KNOB_PID                    0x0860
#define KEYCHRON_Q60_MAX_PID                        0x08C0
#define KEYCHRON_Q65_MAX_KNOB_PID                   0x08B0
#define KEYCHRON_Q8_MAX_KNOB_PID                    0x0880
#define KEYCHRON_V0_ULTRA_ANSI_PID                  0x0C00
#define KEYCHRON_V1_MAX_ISO_KNOB_PID                0x0914
#define KEYCHRON_V1_MAX_JIS_KNOB_PID                0x0915
#define KEYCHRON_V1_MAX_KNOB_PID                    0x0913
#define KEYCHRON_V10_MAX_ISO_KNOB_PID               0x09A1
#define KEYCHRON_V10_MAX_US_KNOB_PID                0x09A0
#define KEYCHRON_V2_MAX_ISO_KNOB_PID                0x0921
#define KEYCHRON_V2_MAX_KNOB_PID                    0x0920
#define KEYCHRON_V3_8K_KNOB_PID                     0x0F30
#define KEYCHRON_V3_MAX_ISO_KNOB_PID                0x0934
#define KEYCHRON_V3_MAX_JIS_KNOB_PID                0x0935
#define KEYCHRON_V3_MAX_KNOB_PID                    0x0933
#define KEYCHRON_V4_MAX_PID                         0x0940
#define KEYCHRON_V4_MAX_ISO_PID                     0x0941
#define KEYCHRON_V5_MAX_ISO_KNOB_PID                0x0951
#define KEYCHRON_V5_MAX_JIS_KNOB_PID                0x0952
#define KEYCHRON_V5_MAX_KNOB_PID                    0x0950
#define KEYCHRON_V6_MAX_ISO_KNOB_PID                0x0961
#define KEYCHRON_V6_MAX_JIS_KNOB_PID                0x0962
#define KEYCHRON_V6_MAX_KNOB_PID                    0x0960
#define KEYCHRON_V8_MAX_ISO_KNOB_PID                0x0981
#define KEYCHRON_V8_MAX_KNOB_PID                    0x0980

/*---------------------------------------------------------*\
| Keychron Patch Types                                      |
|                                                           |
|   LED index is assumed to match Launcher layout_index.    |
|   row/col are the corrected visual matrix position.       |
\*---------------------------------------------------------*/
typedef struct
{
    unsigned int                led_idx;
    unsigned int                row;
    unsigned int                col;
} keychron_patch_entry;

typedef struct
{
    unsigned short              pid;
    unsigned int                num_entries;
    const keychron_patch_entry* patch;
} keychron_patch;

/*---------------------------------------------------------*\
| List of all Keychron matrix patches                       |
\*---------------------------------------------------------*/
extern const keychron_patch* const keychron_patches[];
extern const unsigned int          KEYCHRON_PATCH_COUNT;
