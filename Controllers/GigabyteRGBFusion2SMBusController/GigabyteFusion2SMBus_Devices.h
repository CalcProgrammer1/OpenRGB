/*---------------------------------------------------------*\
| GigabyteFusion2SMBus_Devices.h                            |
|                                                           |
|   Gigabyte RGB Fusion 2 SMBus (ITE IT8295) board layouts  |
|   and mapping of zones to MCU divisions                   |
|                                                           |
|   vic.gombert                                 08 Nov 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>

/*---------------------------------------------------------*\
| The IT8295 carries 10 "division" slots of 16 bytes each.  |
| A zone maps to one division. Which divisions actually     |
| drive LEDs depends on the board, so a layout lists only   |
| the populated ones.                                       |
\*---------------------------------------------------------*/
#define GB_FUSION2_SMBUS_DIVISIONS_MAX      10

typedef struct
{
    unsigned char   division;       /* IT8295 slot index 0-9        */
    const char*     name;           /* Zone / LED name              */
} gb_fusion2_smbus_zone;

typedef struct
{
    const char*                     board_name;     /* DMI baseboard product    */
    const gb_fusion2_smbus_zone*    zones;
    unsigned int                    zone_count;
} gb_fusion2_smbus_layout;

#define GB_FUSION2_SMBUS_LAYOUT(board, table) { board, table, sizeof(table) / sizeof(table[0]) }

/*---------------------------------------------------------*\
| Defined in GigabyteFusion2SMBus_Devices.cpp               |
\*---------------------------------------------------------*/
extern const gb_fusion2_smbus_layout    gb_fusion2_smbus_generic_layout;
extern const gb_fusion2_smbus_layout*   gb_fusion2_smbus_layout_list;
extern const unsigned int               GB_FUSION2_SMBUS_LAYOUT_COUNT;

/*---------------------------------------------------------*\
| Returns the layout for a DMI board name, or the generic   |
| layout when the board is not listed                       |
\*---------------------------------------------------------*/
const gb_fusion2_smbus_layout* GetGigabyteFusion2SMBusLayout(const std::string& board_name);
