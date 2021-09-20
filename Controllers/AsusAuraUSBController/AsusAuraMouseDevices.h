/*-------------------------------------------------------------------*\
|  AsusAuraMouseDevices.h                                             |
|                                                                     |
|  Asus Aura Mouse definitions and mapping                            |
|                                                                     |
|  Chris M (Dr_No)         11th July 2021                             |
\*-------------------------------------------------------------------*/

#include <vector>
#include <map>

#pragma once

#define AURA_ROG_GLADIUS_II_CORE_PID            0x18DD
#define AURA_ROG_GLADIUS_II_PID                 0x1845
#define AURA_ROG_GLADIUS_II_ORIGIN_PID          0x1877
#define AURA_ROG_GLADIUS_II_ORIGIN_PNK_LTD_PID  0x18CD
#define AURA_ROG_GLADIUS_II_ORIGIN_COD_PID      0x18B1
#define AURA_ROG_GLADIUS_II_WIRELESS_1_PID      0x189E
#define AURA_ROG_GLADIUS_II_WIRELESS_2_PID      0x18A0
#define AURA_ROG_CHAKRAM_WIRELESS_PID           0x18E5
#define AURA_ROG_CHAKRAM_WIRED_1_PID            0x18E3
#define AURA_ROG_CHAKRAM_WIRED_2_PID            0x1958
#define AURA_ROG_PUGIO_PID                      0x1846
#define AURA_ROG_PUGIO_II_WIRED_PID             0x1906
#define AURA_ROG_PUGIO_II_WIRELESS_PID          0x1908
#define AURA_ROG_STRIX_IMPACT_II_PID            0x18E1
#define AURA_TUF_M3_PID                         0x1910
#define AURA_TUF_M5_PID                         0x1910

enum
{
    AURA_MOUSE_ZONE_LOGO        = 0,
    AURA_MOUSE_ZONE_SCROLL      = 1,
    AURA_MOUSE_ZONE_UNDERGLOW   = 2,
    AURA_MOUSE_ZONE_ALL         = 3,
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
    AURA_MOUSE_MODE_NONE        = 255,
};

typedef struct
{
    uint8_t                 speed_min;
    uint8_t                 speed_max;
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
            0,    //Speed Min - The Asus Mouse protocol defines larger numbers as slow
            0,    //Speed Max
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_ROG_GLADIUS_II_PID, // ROG Gladius II
        {
            255,
            1,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET }
        }
    },
    {
        AURA_ROG_GLADIUS_II_ORIGIN_PID, // ROG Gladius II Origin
        {
            255,
            1,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET }
        }
    },
    {
        AURA_ROG_GLADIUS_II_ORIGIN_COD_PID, // ROG Gladius II COD
        {
            255,
            1,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET }
        }
    },
    {
        AURA_ROG_GLADIUS_II_ORIGIN_PNK_LTD_PID, // ROG Gladius II PNK LTD
        {
            255,
            1,
            { AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET }
        }
    },
    {
        AURA_ROG_GLADIUS_II_WIRELESS_1_PID, // ROG Gladius II Wireless
        {
            0,
            0,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_GLADIUS_II_WIRELESS_2_PID, // ROG Gladius II Wireless
        {
            0,
            0,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_NONE, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_CHAKRAM_WIRELESS_PID, // ROG Chakram Wireless
        {
            15,
            1,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_CHAKRAM_WIRED_1_PID,  // ROG Chakram Wired 1
        {
            15,
            1,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_CHAKRAM_WIRED_2_PID, // ROG Chakram Wired 2
        {
            15,
            1,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_PUGIO_PID, // ROG Pugio
        {
            255,
            1,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET }
        }
    },
    {
        AURA_ROG_PUGIO_II_WIRED_PID, // ROG Pugio II Wired
        {
            15,
            1,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_PUGIO_II_WIRELESS_PID, // ROG Pugio II Wireless
        {
            15,
            1,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_WAVE, AURA_MOUSE_MODE_REACTIVE, AURA_MOUSE_MODE_COMET, AURA_MOUSE_MODE_BATTERY }
        }
    },
    {
        AURA_ROG_STRIX_IMPACT_II_PID, // ROG Strix Impact II
        {
            0,
            0,
            { AURA_MOUSE_ZONE_LOGO, AURA_MOUSE_ZONE_SCROLL, AURA_MOUSE_ZONE_UNDERGLOW },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_TUF_M3_PID, // TUF M3
        {
            0,
            0,
            { AURA_MOUSE_ZONE_LOGO },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
    {
        AURA_TUF_M5_PID, // TUF M5
        {
            0,
            0,
            { AURA_MOUSE_ZONE_LOGO },
            { AURA_MOUSE_MODE_STATIC, AURA_MOUSE_MODE_BREATHING, AURA_MOUSE_MODE_SPECTRUM, AURA_MOUSE_MODE_REACTIVE }
        }
    },
};
