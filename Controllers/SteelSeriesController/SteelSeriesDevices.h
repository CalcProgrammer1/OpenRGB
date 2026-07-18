/*---------------------------------------------------------*\
| SteelSeriesDevices.h                                      |
|                                                           |
|   Device list for SteelSeries devices                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

/*---------------------------------------------------------*\
| Vendor ID                                                 |
\*---------------------------------------------------------*/
#define STEELSERIES_VID                                 0x1038

/*---------------------------------------------------------*\
| Mouse product IDs                                         |
\*---------------------------------------------------------*/
#define STEELSERIES_AEROX_3_PID                         0x1836
#define STEELSERIES_AEROX_3_WIRELESS_PID                0x1838
#define STEELSERIES_AEROX_3_WIRELESS_WIRED_PID          0x183A
#define STEELSERIES_AEROX_3_CS2_WIRELESS_PID            0x1878
#define STEELSERIES_AEROX_3_CS2_WIRELESS_WIRED_PID      0x187A
#define STEELSERIES_AEROX_5_WIRELESS_PID                0x1852
#define STEELSERIES_AEROX_5_WIRELESS_WIRED_PID          0x1854
#define STEELSERIES_AEROX_5_DESTINY_WIRELESS_PID        0x185C
#define STEELSERIES_AEROX_5_DESTINY_WIRELESS_WIRED_PID  0x185E
#define STEELSERIES_AEROX_5_DIABLO_WIRELESS_PID         0x1860
#define STEELSERIES_AEROX_5_DIABLO_WIRELESS_WIRED_PID   0x1862
#define STEELSERIES_AEROX_9_WIRELESS_PID                0x1858
#define STEELSERIES_AEROX_9_WIRELESS_WIRED_PID          0x185A
#define STEELSERIES_AEROX_5_PID                         0x1850
#define STEELSERIES_AEROX_9_PID                         0x185A
#define STEELSERIES_RIVAL_100_PID                       0x1702
#define STEELSERIES_RIVAL_100_DOTA_PID                  0x170C
#define STEELSERIES_RIVAL_105_PID                       0x1814
#define STEELSERIES_RIVAL_106_PID                       0x1816
#define STEELSERIES_RIVAL_110_PID                       0x1729
#define STEELSERIES_RIVAL_300_PID                       0x1710
#define ACER_PREDATOR_RIVAL_300_PID                     0x1714
#define STEELSERIES_RIVAL_300_CSGO_PID                  0x1394
#define STEELSERIES_RIVAL_300_CSGO_STM32_PID            0x1716
#define STEELSERIES_RIVAL_300_CSGO_HYPERBEAST_PID       0x171A
#define STEELSERIES_RIVAL_300_DOTA_PID                  0x1392
#define STEELSERIES_RIVAL_300_HP_PID                    0x1718
#define STEELSERIES_RIVAL_300_BLACKOPS_PID              0x1710
#define STEELSERIES_RIVAL_310_PID                       0x1720
#define STEELSERIES_RIVAL_310_CSGO_HOWL_PID             0x171E
#define STEELSERIES_RIVAL_310_PUBG_PID                  0x1736
#define STEELSERIES_RIVAL_600_PID                       0x1724
#define STEELSERIES_RIVAL_600_DOTA_2_PID                0x172E
#define STEELSERIES_RIVAL_650_PID                       0x172B
#define STEELSERIES_RIVAL_650_WIRELESS_PID              0x1726
#define STEELSERIES_RIVAL_700_PID                       0x1700
#define STEELSERIES_RIVAL_710_PID                       0x1730
#define STEELSERIES_RIVAL_3_OLD_PID                     0x1824
#define STEELSERIES_RIVAL_3_PID                         0x184C
#define STEELSERIES_SENSEI_TEN_PID                      0x1832
#define STEELSERIES_SENSEI_TEN_CSGO_NEON_RIDER_PID      0x1834
#define STEELSERIES_SENSEI_310_PID                      0x1722

/*---------------------------------------------------------*\
| Headset product IDs                                       |
\*---------------------------------------------------------*/
#define STEELSERIES_SIBERIA_350_PID                     0x1229
#define STEELSERIES_ARCTIS_NOVA_3_PID                   0x12EC
#define STEELSERIES_ARCTIS_5_PID                        0x1250
#define STEELSERIES_ARCTIS_5_V2_PID                     0x12AA

/*--------------------------------------------------------------------*\
| Mousemat product IDs                                                 |
\*--------------------------------------------------------------------*/
#define STEELSERIES_QCK_PRISM_CLOTH_MED_PID                       0x150A
#define STEELSERIES_QCK_PRISM_CLOTH_XL_PID                        0x150D
#define STEELSERIES_QCK_PRISM_CLOTH_XL_DESTINY_PID                0x151E
#define STEELSERIES_QCK_PRISM_CLOTH_XL_CSGO_NEON_RIDER_PID        0x1514
#define STEELSERIES_QCK_PRISM_CLOTH_XL_CSGO_NEO_NOIR_PID          0x151C
#define STEELSERIES_QCK_PRISM_CLOTH_3XL_PID                       0x1516
#define STEELSERIES_QCK_PRISM_CLOTH_4XL_PID                       0x1518
#define STEELSERIES_QCK_PRISM_CLOTH_5XL_PID                       0x151A
#define STEELSERIES_QCK_PRISM_CLOTH_XL_DESTINY_2_LIGHTFALL_ED_PID 0x1520

/*---------------------------------------------------------*\
| Keyboard product IDs                                      |
\*---------------------------------------------------------*/
#define STEELSERIES_APEX_3_PID                      0x161A
#define STEELSERIES_APEX_3_TKL_PID                  0x1622
#define STEELSERIES_APEX_5_PID                      0x161C
#define STEELSERIES_APEX_7_PID                      0x1612
#define STEELSERIES_APEX_7_TKL_PID                  0x1618
#define STEELSERIES_APEX_9_TKL_PID                  0x1634
#define STEELSERIES_APEX_9_MINI_PID                 0x1620
#define STEELSERIES_APEX_PRO_PID                    0x1610
#define STEELSERIES_APEX_PRO_TKL_PID                0x1614
#define STEELSERIES_APEX_PRO_TKL_2023_PID           0x1628
#define STEELSERIES_APEX_PRO_TKL_2023_WL_PID_1      0x1630
#define STEELSERIES_APEX_PRO_TKL_2023_WL_PID_2      0x1632
#define STEELSERIES_APEX_PRO_TKL_GEN3_PID           0x1642
#define STEELSERIES_APEX_PRO_TKL_GEN3_WL_PID_1      0x1644
#define STEELSERIES_APEX_PRO_TKL_GEN3_WL_PID_2      0x1646
#define STEELSERIES_APEX_M750_PID                   0x0616
#define STEELSERIES_APEX_OG_PID                     0x1202
#define STEELSERIES_APEX_350_PID                    0x1206
#define STEELSERIES_APEX_PRO3_PID                   0x1640

/*--------------------------------------------------------*\
| Allows us to handle variation in the protocol.           |
| Defined in a single enum so we can keep the device_list  |
| struct the same for every possible device.               |
\*--------------------------------------------------------*/
typedef enum
{
    RIVAL_100                      = 0x00,
    RIVAL_300                      = 0x01,
    RIVAL_650                      = 0x02,
    SIBERIA_350                    = 0x03,
    APEX                           = 0x04,
    APEX_M                         = 0x05,
    APEX_OLD                       = 0x06,
    SENSEI                         = 0x07,
    RIVAL_600                      = 0x08,
    RIVAL_3                        = 0x09,
    APEX_TZONE                     = 0x0A,
    RIVAL_700                      = 0x0B,
    AEROX_3                        = 0x0C,
    APEX_8ZONE                     = 0x0D,
    AEROX_3_WIRELESS               = 0x0E,
    AEROX_3_WIRELESS_WIRED         = 0x0F,
    AEROX_5_WIRELESS               = 0x10,
    AEROX_5_WIRELESS_WIRED         = 0x11,
    AEROX_5_DESTINY_WIRELESS       = 0x12,
    AEROX_5_DESTINY_WIRELESS_WIRED = 0x13,
    AEROX_5_DIABLO_WIRELESS        = 0x14,
    AEROX_5_DIABLO_WIRELESS_WIRED  = 0x15,
    AEROX_9_WIRELESS               = 0x16,
    AEROX_9_WIRELESS_WIRED         = 0x17,
} steelseries_type;
