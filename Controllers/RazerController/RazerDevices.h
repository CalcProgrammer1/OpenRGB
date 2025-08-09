/*---------------------------------------------------------*\
| RazerDevices.h                                            |
|                                                           |
|   Device list for Razer devices                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                04 Sep 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "RGBController.h"
#include "RGBControllerKeyNames.h"
#include "RazerController.h"
#include "KeyboardLayoutManager.h"

/*-----------------------------------------------------*\
| Razer vendor ID                                       |
\*-----------------------------------------------------*/
#define RAZER_VID                                       0x1532

/*-----------------------------------------------------*\
| Razer maximum zones                                   |
|   If a new device has more than RAZER_MAX_ZONES,      |
|   increment RAZER_MAX_ZONES and update all device     |
|   tables accordingly.                                 |
\*-----------------------------------------------------*/
#define RAZER_MAX_ZONES                                 6

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
|   List taken from OpenRazer                           |
|   Non-RGB keyboards were omitted from this list       |
\*-----------------------------------------------------*/
#define RAZER_BLACKWIDOW_2019_PID                       0x0241
#define RAZER_BLACKWIDOW_CHROMA_PID                     0x0203
#define RAZER_BLACKWIDOW_CHROMA_TE_PID                  0x0209
#define RAZER_BLACKWIDOW_CHROMA_V2_PID                  0x0221
#define RAZER_BLACKWIDOW_ELITE_PID                      0x0228
#define RAZER_BLACKWIDOW_ESSENTIAL_PID                  0x0237
#define RAZER_BLACKWIDOW_LITE_PID                       0x0235
#define RAZER_BLACKWIDOW_OVERWATCH_PID                  0x0211
#define RAZER_BLACKWIDOW_V3_PID                         0x024E
#define RAZER_BLACKWIDOW_V3_MINI_WIRED_PID              0x0258
#define RAZER_BLACKWIDOW_V3_MINI_WIRELESS_PID           0x0271
#define RAZER_BLACKWIDOW_V3_PRO_WIRED_PID               0x025A
#define RAZER_BLACKWIDOW_V3_PRO_BLUETOOTH_PID           0x025B
#define RAZER_BLACKWIDOW_V3_PRO_WIRELESS_PID            0x025C
#define RAZER_BLACKWIDOW_V3_TKL_PID                     0x0A24
#define RAZER_BLACKWIDOW_V4_PID                         0x0287
#define RAZER_BLACKWIDOW_V4_PRO_PID                     0x028D
#define RAZER_BLACKWIDOW_V4_X_PID                       0x0293
#define RAZER_BLACKWIDOW_V4_PRO_75_WIRED_PID            0x02B3
#define RAZER_BLACKWIDOW_X_CHROMA_PID                   0x0216
#define RAZER_BLACKWIDOW_X_CHROMA_TE_PID                0x021A
#define RAZER_BLADE_2016_PID                            0x020F
#define RAZER_BLADE_LATE_2016_PID                       0x0224
#define RAZER_BLADE_2018_ADVANCED_PID                   0x0233
#define RAZER_BLADE_2018_BASE_PID                       0x023B
#define RAZER_BLADE_2018_MERCURY_PID                    0x0240
#define RAZER_BLADE_2019_ADVANCED_PID                   0x023A
#define RAZER_BLADE_2019_BASE_PID                       0x0246
#define RAZER_BLADE_2019_MERCURY_PID                    0x0245
#define RAZER_BLADE_2019_STUDIO_PID                     0x024D
#define RAZER_BLADE_2020_ADVANCED_PID                   0x0253
#define RAZER_BLADE_2020_BASE_PID                       0x0255
#define RAZER_BLADE_LATE_2020_PID                       0x0268
#define RAZER_BLADE_2021_ADVANCED_PID                   0x026D
#define RAZER_BLADE_2021_BASE_PID                       0x026F
#define RAZER_BLADE_2021_BASE_V2_PID                    0x027A
#define RAZER_BLADE_LATE_2021_ADVANCED_PID              0x0276

#define RAZER_BLADE_14_2021_PID                         0x0270
#define RAZER_BLADE_14_2022_PID                         0x028C
#define RAZER_BLADE_14_2023_PID                         0x029D
#define RAZER_BLADE_15_2022_PID                         0x028A

#define RAZER_BLADE_PRO_2016_PID                        0x0210
#define RAZER_BLADE_PRO_2017_PID                        0x0225
#define RAZER_BLADE_PRO_2017_FULLHD_PID                 0x022F
#define RAZER_BLADE_PRO_2019_PID                        0x0234
#define RAZER_BLADE_PRO_LATE_2019_PID                   0x024C
#define RAZER_BLADE_PRO_17_2020_PID                     0x0256
#define RAZER_BLADE_PRO_17_2021_PID                     0x0279

#define RAZER_BLADE_STEALTH_2016_PID                    0x0205
#define RAZER_BLADE_STEALTH_LATE_2016_PID               0x0220
#define RAZER_BLADE_STEALTH_2017_PID                    0x022D
#define RAZER_BLADE_STEALTH_LATE_2017_PID               0x0232
#define RAZER_BLADE_STEALTH_2019_PID                    0x0239
#define RAZER_BLADE_STEALTH_LATE_2019_PID               0x024A
#define RAZER_BLADE_STEALTH_2020_PID                    0x0252
#define RAZER_BLADE_STEALTH_LATE_2020_PID               0x0259

#define RAZER_BOOK_13_2020_PID                          0x026A

#define RAZER_CYNOSA_CHROMA_PID                         0x022A
#define RAZER_CYNOSA_LITE_PID                           0x023F
#define RAZER_CYNOSA_V2_PID                             0x025E
#define RAZER_DEATHSTALKER_CHROMA_PID                   0x0204
#define RAZER_DEATHSTALKER_V2_PID                       0x0295
#define RAZER_DEATHSTALKER_V2_PRO_TKL_WIRELESS_PID      0x0296
#define RAZER_DEATHSTALKER_V2_PRO_TKL_WIRED_PID         0x0298
#define RAZER_DEATHSTALKER_V2_PRO_TKL_BT_PID            0x0297
#define RAZER_DEATHSTALKER_V2_PRO_WIRELESS_PID          0x0290
#define RAZER_DEATHSTALKER_V2_PRO_WIRED_PID             0x0292
#define RAZER_HUNTSMAN_ELITE_PID                        0x0226
#define RAZER_HUNTSMAN_PID                              0x0227
#define RAZER_HUNTSMAN_MINI_PID                         0x0257
#define RAZER_HUNTSMAN_MINI_ANALOG_PID                  0x0282
#define RAZER_HUNTSMAN_TE_PID                           0x0243
#define RAZER_HUNTSMAN_V2_ANALOG_PID                    0x0266
#define RAZER_HUNTSMAN_V2_TKL_PID                       0x026B
#define RAZER_HUNTSMAN_V2_PID                           0x026C
#define RAZER_HUNTSMAN_V3_PRO_PID                       0x02A6
#define RAZER_HUNTSMAN_V3_PRO_TKL_WHITE_PID             0x02A7
#define RAZER_ORBWEAVER_CHROMA_PID                      0x0207
#define RAZER_ORNATA_CHROMA_PID                         0x021E
#define RAZER_ORNATA_CHROMA_V2_PID                      0x025D
#define RAZER_ORNATA_V3_PID                             0x028F
#define RAZER_ORNATA_V3_REV2_PID                        0x02A1
#define RAZER_ORNATA_V3_TKL_PID                         0x02A3
#define RAZER_ORNATA_V3_X_PID                           0x0294
#define RAZER_ORNATA_V3_X_REV2_PID                      0x02A2
#define RAZER_TARTARUS_CHROMA_PID                       0x0208
#define RAZER_TARTARUS_PRO_PID                          0x0244
#define RAZER_TARTARUS_V2_PID                           0x022B

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
|   List taken from OpenRazer                           |
\*-----------------------------------------------------*/
#define RAZER_ABYSSUS_1800_PID                          0x0020
#define RAZER_ABYSSUS_2000_PID                          0x005E
#define RAZER_ABYSSUS_ELITE_DVA_EDITION_PID             0x006A
#define RAZER_ABYSSUS_ESSENTIAL_PID                     0x006B
#define RAZER_ABYSSUS_PID                               0x0042
#define RAZER_ABYSSUS_V2_PID                            0x005B
#define RAZER_ATHERIS_RECEIVER_PID                      0x0062
#define RAZER_BASILISK_PID                              0x0064
#define RAZER_BASILISK_ESSENTIAL_PID                    0x0065
#define RAZER_BASILISK_ULTIMATE_WIRED_PID               0x0086
#define RAZER_BASILISK_ULTIMATE_WIRELESS_PID            0x0088
#define RAZER_BASILISK_X_HYPERSPEED_PID                 0x0083
#define RAZER_BASILISK_V2_PID                           0x0085
#define RAZER_BASILISK_V3_PID                           0x0099
#define RAZER_BASILISK_V3_35K_PID                       0x00CB
#define RAZER_BASILISK_V3_PRO_WIRED_PID                 0x00AA
#define RAZER_BASILISK_V3_PRO_WIRELESS_PID              0x00AB
#define RAZER_BASILISK_V3_PRO_35K_WIRED_PID             0x00CC
#define RAZER_BASILISK_V3_PRO_35K_WIRELESS_PID          0x00CD
#define RAZER_BASILISK_V3_PRO_BLUETOOTH_PID             0x00AC
#define RAZER_BASILISK_V3_X_HYPERSPEED_PID              0x00B9
#define RAZER_COBRA_PID                                 0x00A3
#define RAZER_COBRA_PRO_WIRED_PID                       0x00AF
#define RAZER_COBRA_PRO_WIRELESS_PID                    0x00B0
#define RAZER_DEATHADDER_1800_PID                       0x0038
#define RAZER_DEATHADDER_2000_PID                       0x004F
#define RAZER_DEATHADDER_2013_PID                       0x0037
#define RAZER_DEATHADDER_3_5G_PID                       0x0016
#define RAZER_DEATHADDER_3500_PID                       0x0054
#define RAZER_DEATHADDER_CHROMA_PID                     0x0043
#define RAZER_DEATHADDER_ELITE_PID                      0x005C
#define RAZER_DEATHADDER_ESSENTIAL_PID                  0x006E
#define RAZER_DEATHADDER_ESSENTIAL_V2_PID               0x0098
#define RAZER_DEATHADDER_ESSENTIAL_WHITE_EDITION_PID    0x0071
#define RAZER_DEATHADDER_V2_MINI_PID                    0x008C
#define RAZER_DEATHADDER_V2_PID                         0x0084
#define RAZER_DEATHADDER_V2_PRO_WIRED_PID               0x007C
#define RAZER_DEATHADDER_V2_PRO_WIRELESS_PID            0x007D
#define RAZER_DIAMONDBACK_CHROMA_PID                    0x004C
#define RAZER_IMPERATOR_PID                             0x002F
#define RAZER_LANCEHEAD_TE_WIRED_PID                    0x0060
#define RAZER_LANCEHEAD_2017_WIRED_PID                  0x0059
#define RAZER_LANCEHEAD_2017_WIRELESS_PID               0x005A
#define RAZER_LANCEHEAD_2019_WIRED_PID                  0x0070
#define RAZER_LANCEHEAD_2019_WIRELESS_PID               0x006F
#define RAZER_MAMBA_2012_WIRED_PID                      0x0024
#define RAZER_MAMBA_2012_WIRELESS_PID                   0x0025
#define RAZER_MAMBA_2015_WIRED_PID                      0x0044
#define RAZER_MAMBA_2015_WIRELESS_PID                   0x0045
#define RAZER_MAMBA_2018_WIRED_PID                      0x0073
#define RAZER_MAMBA_2018_WIRELESS_PID                   0x0072
#define RAZER_MAMBA_ELITE_PID                           0x006C
#define RAZER_MAMBA_HYPERFLUX_PID                       0x0069
#define RAZER_MAMBA_TE_PID                              0x0046
#define RAZER_NAGA_2012_PID                             0x002E
#define RAZER_NAGA_2014_PID                             0x0040
#define RAZER_NAGA_CHROMA_PID                           0x0053
#define RAZER_NAGA_CLASSIC_PID                          0x0093
#define RAZER_NAGA_EPIC_CHROMA_DOCK_PID                 0x003F
#define RAZER_NAGA_EPIC_CHROMA_PID                      0x003E
#define RAZER_NAGA_HEX_PID                              0x0041
#define RAZER_NAGA_HEX_RED_PID                          0x0036
#define RAZER_NAGA_HEX_V2_PID                           0x0050
#define RAZER_NAGA_LEFT_HANDED_PID                      0x008D
#define RAZER_NAGA_TRINITY_PID                          0x0067
#define RAZER_NAGA_PRO_WIRED_PID                        0x008F
#define RAZER_NAGA_PRO_WIRELESS_PID                     0x0090
#define RAZER_NAGA_PRO_V2_WIRED_PID                     0x00A7
#define RAZER_NAGA_PRO_V2_WIRELESS_PID                  0x00A8
#define RAZER_OROCHI_2011_PID                           0x0013
#define RAZER_OROCHI_2013_PID                           0x0039
#define RAZER_OROCHI_CHROMA_PID                         0x0048
#define RAZER_OUROBOROS_PID                             0x0032
#define RAZER_TAIPAN_PID                                0x0034
#define RAZER_VIPER_8KHZ_PID                            0x0091
#define RAZER_VIPER_MINI_PID                            0x008A
#define RAZER_VIPER_PID                                 0x0078
#define RAZER_VIPER_ULTIMATE_WIRED_PID                  0x007A
#define RAZER_VIPER_ULTIMATE_WIRELESS_PID               0x007B

/*-----------------------------------------------------*\
| Headset product IDs                                   |
\*-----------------------------------------------------*/
#define RAZER_KRAKEN_CLASSIC_ALT_PID                    0x0506
#define RAZER_KRAKEN_CLASSIC_PID                        0x0501
#define RAZER_KRAKEN_KITTY_EDITION_PID                  0x0F19
#define RAZER_KRAKEN_KITTY_BLACK_EDITION_PID            0x0F21
#define RAZER_KRAKEN_PID                                0x0504
#define RAZER_KRAKEN_ULTIMATE_PID                       0x0527
#define RAZER_KRAKEN_V2_PID                             0x0510
#define RAZER_KRAKEN_KITTY_BLACK_EDITION_V2_PID         0x0560
#define RAZER_TIAMAT_71_V2_PID                          0x0F03

/*-----------------------------------------------------*\
| Accessory product IDs                                 |
|   List taken from OpenRazer                           |
\*-----------------------------------------------------*/
#define RAZER_BASE_STATION_CHROMA_PID                   0x0F08
#define RAZER_BASE_STATION_V2_CHROMA_PID                0x0F20
#define RAZER_CHARGING_PAD_CHROMA_PID                   0x0F26
#define RAZER_CHROMA_ADDRESSABLE_RGB_CONTROLLER_PID     0x0F1F
#define RAZER_CHROMA_HDK_PID                            0x0F09
#define RAZER_CHROMA_MUG_PID                            0x0F07
#define RAZER_CHROMA_PC_CASE_LIGHTING_KIT_PID           0x0F0E
#define RAZER_CORE_PID                                  0x0215
#define RAZER_CORE_X_PID                                0x0F1A
#define RAZER_FIREFLY_HYPERFLUX_PID                     0x0068
#define RAZER_FIREFLY_PID                               0x0C00
#define RAZER_FIREFLY_V2_PID                            0x0C04
#define RAZER_FIREFLY_V2_PRO_PID                        0x0C08
#define RAZER_GOLIATHUS_CHROMA_EXTENDED_PID             0x0C02
#define RAZER_GOLIATHUS_CHROMA_PID                      0x0C01
#define RAZER_GOLIATHUS_CHROMA_3XL_PID                  0x0C06
#define RAZER_LAPTOP_STAND_CHROMA_PID                   0x0F0D
#define RAZER_LAPTOP_STAND_CHROMA_V2_PID                0x0F2B
#define RAZER_LEVIATHAN_V2_PID                          0x0532
#define RAZER_LEVIATHAN_V2X_PID                         0x054A
#define RAZER_MOUSE_BUNGEE_V3_CHROMA_PID                0x0F1D
#define RAZER_MOUSE_DOCK_CHROMA_PID                     0x007E
#define RAZER_MOUSE_DOCK_PRO_PID                        0x00A4
#define RAZER_NOMMO_CHROMA_PID                          0x0517
#define RAZER_NOMMO_PRO_PID                             0x0518
#define RAZER_O11_DYNAMIC_PID                           0x0F13
#define RAZER_SEIREN_EMOTE_PID                          0x0F1B
#define RAZER_STRIDER_CHROMA_PID                        0x0C05
#define RAZER_THUNDERBOLT_4_DOCK_CHROMA_PID             0x0F21
#define RAZER_HANBO_CHROMA_PID                          0x0F35

typedef struct
{
    std::string                         name;
    unsigned int                        type;
    unsigned int                        rows;
    unsigned int                        cols;
} razer_zone;

typedef struct
{
    std::string                         name;
    unsigned short                      pid;
    device_type                         type;
    unsigned char                       matrix_type;
    unsigned char                       transaction_id;
    unsigned int                        rows;
    unsigned int                        cols;
    const razer_zone*                   zones[RAZER_MAX_ZONES];
    keyboard_keymap_overlay_values*     layout;
} razer_device;

/*-----------------------------------------------------*\
| These constant values are defined in RazerDevices.cpp |
\*-----------------------------------------------------*/
extern const unsigned int RAZER_NUM_DEVICES;
extern const razer_device** device_list;
