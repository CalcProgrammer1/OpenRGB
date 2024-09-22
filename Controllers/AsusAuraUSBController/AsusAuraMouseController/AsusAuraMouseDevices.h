/*---------------------------------------------------------*\
| AsusAuraMouseDevices.h                                    |
|                                                           |
|   Device list for ASUS Aura mouse                         |
|                                                           |
|   Chris M (Dr_No)                             11 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include <map>

#define AURA_ROG_GLADIUS_II_CORE_PID                    0x18DD
#define AURA_ROG_GLADIUS_II_PID                         0x1845
#define AURA_ROG_GLADIUS_II_ORIGIN_PID                  0x1877
#define AURA_ROG_GLADIUS_II_ORIGIN_PNK_LTD_PID          0x18CD
#define AURA_ROG_GLADIUS_II_ORIGIN_COD_PID              0x18B1
#define AURA_ROG_GLADIUS_II_WIRELESS_1_PID              0x189E
#define AURA_ROG_GLADIUS_II_WIRELESS_2_PID              0x18A0
#define AURA_ROG_GLADIUS_III_PID                        0x197B
#define AURA_ROG_GLADIUS_III_WIRELESS_USB_PID           0x197D
#define AURA_ROG_GLADIUS_III_WIRELESS_2_4_PID           0x197F
#define AURA_ROG_GLADIUS_III_WIRELESS_BT_PID            0x1981
#define AURA_ROG_GLADIUS_III_WIRELESS_AIMPOINT_USB_PID  0x1A70
#define AURA_ROG_GLADIUS_III_WIRELESS_AIMPOINT_2_4_PID  0x1A72
#define AURA_ROG_GLADIUS_III_WIRELESS_AIMPOINT_BT_PID   0x1A74
#define AURA_ROG_CHAKRAM_WIRELESS_PID                   0x18E5
#define AURA_ROG_CHAKRAM_WIRED_1_PID                    0x18E3
#define AURA_ROG_CHAKRAM_CORE_PID                       0x1958
#define AURA_ROG_CHAKRAM_X_USB_PID                      0x1A18
#define AURA_ROG_CHAKRAM_X_2_4_PID                      0x1A1A
#define AURA_ROG_SPATHA_X_USB_PID                       0x1977
#define AURA_ROG_SPATHA_X_2_4_PID                       0x1979
#define AURA_ROG_SPATHA_X_DOCK_PID                      0x1979
#define AURA_ROG_PUGIO_PID                              0x1846
#define AURA_ROG_PUGIO_II_WIRED_PID                     0x1906
#define AURA_ROG_PUGIO_II_WIRELESS_PID                  0x1908
#define AURA_ROG_STRIX_IMPACT_PID                       0x1847
#define AURA_ROG_STRIX_IMPACT_II_PID                    0x18E1
#define AURA_ROG_STRIX_IMPACT_II_GUNDAM_PID             0x189E
#define AURA_ROG_STRIX_IMPACT_II_PUNK_PID               0x1956
#define AURA_ROG_STRIX_IMPACT_II_WHITE_PID              0x19D2
#define AURA_ROG_STRIX_IMPACT_II_WIRELESS_USB_PID       0x1947
#define AURA_ROG_STRIX_IMPACT_II_WIRELESS_2_4_PID       0x1949
#define AURA_ROG_STRIX_IMPACT_III_PID                   0x1A88
#define AURA_ROG_KERIS                                  0x195C
#define AURA_ROG_KERIS_WIRELESS_USB_PID                 0x195E
#define AURA_ROG_KERIS_WIRELESS_2_4_PID                 0x1960
#define AURA_ROG_KERIS_WIRELESS_BT_PID                  0x1962
#define AURA_ROG_KERIS_WIRELESS_AIMPOINT_USB_PID        0x1A66
#define AURA_ROG_KERIS_WIRELESS_AIMPOINT_2_4_PID        0x1A68
#define AURA_ROG_KERIS_WIRELESS_AIMPOINT_BT_PID         0x1A6A
#define AURA_TUF_M3_PID                                 0x1910
#define AURA_TUF_M3_GEN_II_PID                          0x1A9B
#define AURA_TUF_M5_PID                                 0x1898

#define AURA_ROG_SPATHA_X_DOCK_FAKE_PID                 0xFFFF

enum
{
    AURA_MOUSE_ZONE_LOGO        = 0,
    AURA_MOUSE_ZONE_SCROLL      = 1,
    AURA_MOUSE_ZONE_UNDERGLOW   = 2,
    AURA_MOUSE_ZONE_ALL         = 3,
    AURA_MOUSE_ZONE_DOCK        = 4,
};

enum
{
    AURA_MOUSE_MODE_STATIC      = 0,
    AURA_MOUSE_MODE_BREATHING   = 1,
    AURA_MOUSE_MODE_SPECTRUM    = 2,
    AURA_MOUSE_MODE_WAVE        = 3,
    AURA_MOUSE_MODE_REACTIVE    = 4,
    AURA_MOUSE_MODE_COMET       = 5,
    AURA_MOUSE_MODE_BATTERY     = 6,
    AURA_MOUSE_MODE_DIRECT      = 254,
    AURA_MOUSE_MODE_NONE        = 255,
};

typedef struct
{
    uint8_t                 speed_min;
    uint8_t                 speed_max;
    uint8_t                 brightness_min;
    uint8_t                 brightness_max;
    bool                    wireless;
    int                     version_protocol;
    bool                    direct;
    std::vector<uint8_t>    mouse_zones;
    std::vector<uint8_t>    mouse_modes;
} mouse_type;

/*-----------------------------------------------------------------*\
|  DEVICE MAP                                                       |
|                                                                   |
|    This structure maps the OpenRGB modes to the mode values       |
|    sent to each mouse. As not all modes are present on each       |
|    mouse the "mode index" is different. Eg. "Reactive" is mode    |
|    4 on the Gladius II and mode 3 on the Gladius II wireless      |
\*-----------------------------------------------------------------*/
static std::map<int,mouse_type> aura_mouse_devices =
{
    {
        AURA_ROG_GLADIUS_II_CORE_PID, // ROG Gladius II Core
        {
            0,      // Speed Min - The Asus Mouse protocol defines larger numbers as slow
            0,      // Speed Max
            0,      // Brightness Min
            4,      // Brightness Max
            false,  // is wireless? (important for fetching the version)
            1,      // version protocol
            false,  // direct - defines whether the mouse has a native direct mode or just uses static as direct (only present on newer devices)
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_ROG_GLADIUS_II_PID, // ROG Gladius II
        {
            255,
            1,
            0,
            4,
            false,
            0,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET }
        }
    },
    {
        AURA_ROG_GLADIUS_II_ORIGIN_PID, // ROG Gladius II Origin
        {
            255,
            1,
            0,
            4,
            false,
            0,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET }
        }
    },
    {
        AURA_ROG_GLADIUS_II_ORIGIN_COD_PID, // ROG Gladius II COD
        {
            255,
            1,
            0,
            4,
            false,
            0,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET }
        }
    },
    {
        AURA_ROG_GLADIUS_II_ORIGIN_PNK_LTD_PID, // ROG Gladius II PNK LTD
        {
            255,
            1,
            0,
            4,
            false,
            0,
            false,
            { AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET }
        }
    },
    {
        AURA_ROG_GLADIUS_II_WIRELESS_1_PID, // ROG Gladius II Wireless
        {
            0,
            0,
            0,
            4,
            true,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_GLADIUS_II_WIRELESS_2_PID, // ROG Gladius II Wireless
        {
            0,
            0,
            0,
            4,
            true,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_GLADIUS_III_PID, // ROG Gladius III
        {
            255,
            1,
            0,
            100,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_GLADIUS_III_WIRELESS_USB_PID, // ROG Gladius III Wireless USB
        {
            255,
            1,
            0,
            100,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_GLADIUS_III_WIRELESS_2_4_PID, // ROG Gladius III Wireless 2.4 GHz Dongle
        {
            255,
            1,
            0,
            100,
            true,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_GLADIUS_III_WIRELESS_BT_PID, // ROG Gladius III Wireless Bluetooth
        {
            255,
            1,
            0,
            100,
            true,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_GLADIUS_III_WIRELESS_AIMPOINT_USB_PID, // ROG Gladius III Wireless AimPoint USB
        {
            255,
            1,
            0,
            100,
            false,
            2,
            true,
            { AURA_MOUSE_ZONE_LOGO },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_GLADIUS_III_WIRELESS_AIMPOINT_2_4_PID, // ROG Gladius III Wireless AimPoint 2.4 GHz Dongle
        {
            255,
            1,
            0,
            100,
            true,
            2,
            true,
            { AURA_MOUSE_ZONE_LOGO },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_CHAKRAM_WIRELESS_PID, // ROG Chakram Wireless
        {
            15,
            1,
            0,
            4,
            true,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_CHAKRAM_WIRED_1_PID,  // ROG Chakram Wired 1
        {
            15,
            1,
            0,
            4,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_CHAKRAM_CORE_PID, // ROG Chakram Core
        {
            0,
            0,
            0,
            4,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_ROG_CHAKRAM_X_USB_PID, // ROG Chakram X USB
        {
            15, // technically until 255, but unusably slow after 15
            1,
            0,
            100,
            false,
            2,
            true,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_CHAKRAM_X_2_4_PID, // ROG Chakram X 2.4GHz Dongle
        {
            15, // technically until 255, but unusably slow after 15
            1,
            0,
            100,
            true,
            2,
            true,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_SPATHA_X_USB_PID, // ROG Spatha X USB
        {
            15, // technically until 255, but unusably slow after 15
            1,
            0,
            100,
            false,
            1,
            true,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_SPATHA_X_2_4_PID, // ROG Spatha X 2.4GHz Dock
        {
            15, // technically until 255, but unusably slow after 15
            1,
            0,
            100,
            true,
            1,
            true,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_PUGIO_PID, // ROG Pugio
        {
            255,
            1,
            0,
            4,
            false,
            0,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET }
        }
    },
    {
        AURA_ROG_PUGIO_II_WIRED_PID, // ROG Pugio II Wired
        {
            15,
            1,
            0,
            4,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_PUGIO_II_WIRELESS_PID, // ROG Pugio II Wireless
        {
            15,
            1,
            0,
            4,
            true,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_STRIX_IMPACT_PID, // ROG Strix Impact
        {
            0,
            0,
            0,
            4,
            false,
            4,
            false,
            { AURA_MOUSE_ZONE_LOGO },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_ROG_STRIX_IMPACT_II_PID, // ROG Strix Impact II
        {
            0,
            0,
            0,
            4,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_ROG_STRIX_IMPACT_II_GUNDAM_PID, // ROG Strix Impact II Gundam
        {
            0,
            0,
            0,
            4,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_ROG_STRIX_IMPACT_II_PUNK_PID, // ROG Strix Impact II Electro Punk
        {
            0,
            0,
            0,
            4,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_ROG_STRIX_IMPACT_II_WHITE_PID, // ROG Strix Impact II Moonlight White
        {
            0,
            0,
            0,
            4,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_ROG_STRIX_IMPACT_II_WIRELESS_USB_PID, // ROG Strix Impact II Wireless USB
        {
            0,
            0,
            0,
            4,
            false,
            1, // not tested, but likely same as ROG Strix Impact II non wireless
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_STRIX_IMPACT_II_WIRELESS_2_4_PID, // ROG Strix Impact II Wireless 2.4 GHz Dongle
        {
            0,
            0,
            0,
            4,
            true,
            1, // not tested, but likely same as ROG Strix Impact II non wireless
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_STRIX_IMPACT_III_PID, // ROG Strix Impact III
        {
            0,
            0,
            0,
            100,
            false,
            1,
            true,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_ROG_KERIS, // ROG Keris
        {
            0,
            0,
            0,
            4,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_ROG_KERIS_WIRELESS_USB_PID, // ROG Keris
        {
            0,
            0,
            0,
            4,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_KERIS_WIRELESS_2_4_PID, // ROG Keris
        {
            0,
            0,
            0,
            4,
            true,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_KERIS_WIRELESS_BT_PID, // ROG Keris
        {
            0,
            0,
            0,
            4,
            true,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_KERIS_WIRELESS_AIMPOINT_USB_PID, // ROG Keris Wireless AimPoint
        {
            0,
            0,
            0,
            100,
            false,
            1,
            true,
            { AURA_MOUSE_ZONE_LOGO },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_KERIS_WIRELESS_AIMPOINT_2_4_PID, // ROG Keris Wireless AimPoint
        {
            0,
            0,
            0,
            100,
            true,
            2,
            true,
            { AURA_MOUSE_ZONE_LOGO },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_TUF_M3_PID, // TUF M3
        {
            0,
            0,
            0,
            4,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_TUF_M3_GEN_II_PID, // TUF M3 Gen II
        {
            0,
            0,
            0,
            100,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_LOGO },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_TUF_M5_PID, // TUF M5
        {
            0,
            0,
            0,
            4,
            false,
            3,
            false,
            { AURA_MOUSE_ZONE_LOGO },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_ROG_SPATHA_X_DOCK_FAKE_PID, // Asus ROG Spatha X Dock (only in wireless mode)
        {
            0,
            0,
            0,
            100,
            false,
            1,
            false,
            { AURA_MOUSE_ZONE_DOCK },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
};

typedef struct
{
    unsigned char                                           led_amount;
    std::map< unsigned char, std::vector<unsigned char>>    map;
} led_map;

/*-----------------------------------------------------------------*\
|  LED MAP                                                          |
|                                                                   |
|    This maps the LEDs to the correct zone.                        |
|    This is necessary, because openrgb doesn't support per-zone    |
|    lighting. To allow for per-zone color of modes like breathing, |
|    the leds defined need to be the zones, which means the actual  |
|    LEDs can't be implemented.                                     |
\*-----------------------------------------------------------------*/
static std::map<unsigned short, led_map > aura_mouse_led_maps =
{
    {
        AURA_ROG_GLADIUS_III_WIRELESS_AIMPOINT_USB_PID,
        {
            1,
            {
                { AURA_MOUSE_ZONE_LOGO, { 0 } },
            }
        }
    },
    {
        AURA_ROG_GLADIUS_III_WIRELESS_AIMPOINT_2_4_PID,
        {
            1,
            {
                { AURA_MOUSE_ZONE_LOGO, { 0 } },
            }
        }
    },
    {
        AURA_ROG_KERIS_WIRELESS_AIMPOINT_USB_PID,
        {
            1,
            {
                { AURA_MOUSE_ZONE_LOGO, { 0 } },
            }
        }
    },
    {
        AURA_ROG_KERIS_WIRELESS_AIMPOINT_2_4_PID,
        {
            1,
            {
                { AURA_MOUSE_ZONE_LOGO, { 0 } },
            }
        }
    },
    {
        AURA_ROG_SPATHA_X_2_4_PID,
        {
            5,
            {
                { AURA_MOUSE_ZONE_LOGO, { 0 } },
                { AURA_MOUSE_ZONE_SCROLL, { 1 } },
                { AURA_MOUSE_ZONE_UNDERGLOW, { 2, 3, 4 } },
            }
        }
    },
    {
        AURA_ROG_SPATHA_X_USB_PID,
        {
            5,
            {
                { AURA_MOUSE_ZONE_LOGO, { 0 } },
                { AURA_MOUSE_ZONE_SCROLL, { 1 } },
                { AURA_MOUSE_ZONE_UNDERGLOW, { 2, 3, 4 } },
            }
        }
    },
    {
        AURA_ROG_CHAKRAM_X_USB_PID,
        {
            9,
            {
                { AURA_MOUSE_ZONE_LOGO, { 7 } },
                { AURA_MOUSE_ZONE_SCROLL, { 8 } },
                { AURA_MOUSE_ZONE_UNDERGLOW, { 0, 1, 2, 3, 4, 5, 6 } },
            }
        }
    },
    {
        AURA_ROG_CHAKRAM_X_2_4_PID,
        {
            9,
            {
                { AURA_MOUSE_ZONE_LOGO, { 7 } },
                { AURA_MOUSE_ZONE_SCROLL, { 8 } },
                { AURA_MOUSE_ZONE_UNDERGLOW, { 0, 1, 2, 3, 4, 5, 6 } },
            }
        }
    },
    {
        AURA_ROG_STRIX_IMPACT_III_PID,
        {
            2,
            {
                { AURA_MOUSE_ZONE_LOGO, { 0 } },
                { AURA_MOUSE_ZONE_SCROLL, { 1 } },
            }
        }
    }
};
