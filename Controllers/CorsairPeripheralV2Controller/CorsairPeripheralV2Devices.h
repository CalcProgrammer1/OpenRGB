/*---------------------------------------------------------*\
| CorsairPeripheralV2Devices.h                              |
|                                                           |
|   Device list for Corsair V2 peripherals                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "RGBController.h"
#include "RGBControllerKeyNames.h"
#include "KeyboardLayoutManager.h"

#define CORSAIR_ZONES_MAX               6

enum corsair_v2_device_mode
{
    CORSAIR_V2_MODE_HW                  = 0x01,     /* Hardware RGB mode                */
    CORSAIR_V2_MODE_SW                  = 0x02,     /* Software RGB mode                */
};

enum corsair_v2_supports
{
    CORSAIR_V2_TYPE_SW_COLOUR_BLOCK     = 1,
    CORSAIR_V2_TYPE_HW_COLOUR_BLOCK     = 2,
    CORSAIR_V2_TYPE_SW_TRIPLETS         = 3,
    CORSAIR_V2_TYPE_HW_TRIPLETS         = 4,
};

enum corsair_v2_kb_layout
{
    CORSAIR_V2_KB_LAYOUT_ANSI           = 0x01,     /* US ANSI Layout                   */
    CORSAIR_V2_KB_LAYOUT_ISO            = 0x02,     /* EURO ISO Layout                  */
    CORSAIR_V2_KB_LAYOUT_ABNT           = 0x03,     /* Brazilian Layout                 */
    CORSAIR_V2_KB_LAYOUT_JIS            = 0x04,     /* Japanese Layout                  */
};

typedef struct
{
    std::string                         name;
    zone_type                           type;
    uint8_t                             rows;
    uint8_t                             cols;
}   corsair_v2_zone;

typedef struct
{
    uint8_t                             zone;
    uint8_t                             row;
    uint8_t                             col;
    uint8_t                             index;
    const char*                         name;
}   corsair_v2_led;

typedef struct
{
    uint16_t                            pid;
    device_type                         type;
    uint8_t                             rows;
    uint8_t                             cols;
    const corsair_v2_zone*              zones[CORSAIR_ZONES_MAX];
    keyboard_keymap_overlay_values*     layout_new;
}   corsair_v2_device;

/*-----------------------------------------------------*\
| Corsair V2 Protocol Keyboards                         |
\*-----------------------------------------------------*/
#define CORSAIR_K55_RGB_PRO_PID                     0x1BA4
#define CORSAIR_K60_RGB_PRO_PID                     0x1BA0
#define CORSAIR_K60_RGB_PRO_LP_PID                  0x1BAD
#define CORSAIR_K60_RGB_PRO_TKL_B_PID               0x1BC7
#define CORSAIR_K60_RGB_PRO_TKL_W_PID               0x1BED
#define CORSAIR_K70_CORE_RGB_PID                    0x1BFD
#define CORSAIR_K70_RGB_PRO_PID                     0x1BC4
#define CORSAIR_K70_RGB_PRO_V2_PID                  0x1BB3
#define CORSAIR_K70_RGB_TKL_PID                     0x1B73
#define CORSAIR_K70_RGB_TKL_CS_PID                  0x1BB9
#define CORSAIR_K95_PLATINUM_XT_PID                 0x1B89
#define CORSAIR_K100_OPTICAL_V1_PID                 0x1B7C
#define CORSAIR_K100_OPTICAL_V2_PID                 0x1BC5
#define CORSAIR_K100_MXRED_PID                      0x1B7D

/*-----------------------------------------------------*\
| Corsair V2 Protocol Mice                              |
\*-----------------------------------------------------*/
#define CORSAIR_DARK_CORE_RGB_PID                   0x1B4B
#define CORSAIR_DARK_CORE_RGB_PRO_PID               0x1B7E
#define CORSAIR_HARPOON_WIRELESS_PID                0x1B5E
#define CORSAIR_IRONCLAW_WIRELESS_PID               0x1B4C
#define CORSAIR_KATAR_PRO_PID                       0x1B93
#define CORSAIR_KATAR_PRO_V2_PID                    0x1BBA
#define CORSAIR_KATAR_PRO_XT_PID                    0x1BAC
#define CORSAIR_M55_RGB_PRO_PID                     0x1B70
#define CORSAIR_M65_RGB_ULTRA_WIRED_PID             0x1B9E
#define CORSAIR_M65_RGB_ULTRA_WIRELESS_PID          0x1BB5
#define CORSAIR_SLIPSTREAM_WIRELESS_PID1            0x1BA6
#define CORSAIR_SLIPSTREAM_WIRELESS_PID2            0x1B65

/*-----------------------------------------------------*\
| Corsair V2 Protocol Mousemats                         |
\*-----------------------------------------------------*/
#define CORSAIR_MM700_PID                           0x1B9B
#define CORSAIR_MM700_3XL_PID                       0x1BC9

/*-----------------------------------------------------*\
| These constant values are defined in                  |
| CorsairPeripheralV2Devices.cpp                        |
\*-----------------------------------------------------*/
extern const unsigned int CORSAIR_V2_DEVICE_COUNT;
extern const corsair_v2_device** corsair_v2_device_list;
