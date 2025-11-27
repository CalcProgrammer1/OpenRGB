/*---------------------------------------------------------*\
| Gigabyte_Fusion2_USB_Devices.h                            |
|                                                           |
|   Gigabyte Fusion 2 USB Device layouts and                |
|     and mapping to the device IDs stored on chip          |
|                                                           |
|   megadjc                                     31 Jul 2025 |
|   chrism                                      29 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <map>
#include "RGBController.h"

#define GB_FUSION2_ZONES_MAX                8

/*--------------------------------------------------------*\
| Base LED mappings found on all controllers.              |
\*--------------------------------------------------------*/
enum GB_FUSION2_LED_IDX
{
    LED1                = 0,
    LED2                = 1,
    LED3                = 2,
    LED4                = 3,
    LED5                = 4,
    LED6                = 5,
    LED7                = 6,
    LED8                = 7,

/*--------------------------------------------------------*\
| IT8297/IT5701/IT5702 ARGB Headers                        |
\*--------------------------------------------------------*/
    HDR_D_LED1          = 5,
    HDR_D_LED2          = 6,
    HDR_D_LED1_ARGB     = 0x58,
    HDR_D_LED2_ARGB     = 0x59,

/*--------------------------------------------------------*\
| Additional LED mappings found on IT5711 controllers.     |
\*--------------------------------------------------------*/
    LED9                = 8,
    LED10               = 9,
    LED11               = 10,

/*--------------------------------------------------------*\
| IT5711 additional ARGB Headers.                          |
\*--------------------------------------------------------*/
    HDR_D_LED3          = 7,
    HDR_D_LED4          = 8,
    HDR_D_LED3_ARGB     = 0x62,
    HDR_D_LED4_ARGB     = 0x63,
};

/*-------------------------------------------------*\
| LED mapping                                       |
\*-------------------------------------------------*/
using FwdLedHeaders     = std::map<std::string, GB_FUSION2_LED_IDX>;
using RvrseLedHeaders   = std::map<GB_FUSION2_LED_IDX, std::string>;
const FwdLedHeaders LedLookup =
{
    {"LED1",                LED1            },
    {"LED2",                LED2            },
    {"LED3",                LED3            },
    {"LED4",                LED4            },
    {"LED5",                LED5            },
    {"LED6",                LED6            },
    {"LED7",                LED7            },
    {"LED8",                LED8            },
    {"LED9",                LED9            },
    {"LED10",               LED10           },
    {"LED11",               LED11           },
    {"HDR_D_LED1",          HDR_D_LED1      },
    {"HDR_D_LED2",          HDR_D_LED2      },
    {"HDR_D_LED3",          HDR_D_LED3      },
    {"HDR_D_LED4",          HDR_D_LED4      },
    /*-------------------------------------------------*\
    | The DLED ARGB index is not required for parsing   |
    \*-------------------------------------------------*/
    /*-------------------------------------------------*\
    {"HDR_D_LED1_RGB",      HDR_D_LED1_ARGB },
    {"HDR_D_LED2_RGB",      HDR_D_LED2_ARGB },
    {"HDR_D_LED3_RGB",      HDR_D_LED3_ARGB },
    {"HDR_D_LED4_RGB",      HDR_D_LED4_ARGB },
    \*-------------------------------------------------*/
};

typedef struct
{
    GB_FUSION2_LED_IDX  idx;
    uint16_t            leds_min;
    uint16_t            leds_max;
    std::string         name;
}   gb_fusion2_zone;

typedef const gb_fusion2_zone* gb_fusion2_layout[GB_FUSION2_ZONES_MAX];

typedef struct
{
    gb_fusion2_layout*  zones;
    uint32_t            layout_id;
    uint8_t             device_num;
    std::string         name;
}   gb_fusion2_device;

/*---------------------------------------------------------------------*\
| These constant values are defined in GigabyteFusion2USB_Devices.cpp   |
\*---------------------------------------------------------------------*/
extern const unsigned int GB_FUSION2_DEVICE_COUNT;
extern const gb_fusion2_device** gb_fusion2_device_list;

