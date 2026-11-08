/*---------------------------------------------------------*\
| GigabyteFusion2SMBus_Devices.cpp                          |
|                                                           |
|   Gigabyte RGB Fusion 2 SMBus (ITE IT8295) board layouts  |
|   and mapping of zones to MCU divisions                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "GigabyteFusion2SMBus_Devices.h"

/*---------------------------------------------------------*\
| Generic layout: expose every slot (legacy behaviour,      |
| used when the board is not listed below)                  |
\*---------------------------------------------------------*/
static const gb_fusion2_smbus_zone generic_zones[] =
{
    { 0, "CPU"              },
    { 1, "???"              },
    { 2, "Motherboard Logo" },
    { 3, "Case Rear"        },
    { 4, "Case"             },
    { 5, "???"              },
    { 6, "???"              },
    { 7, "ARGB Header 1"    },
    { 8, "ARGB Header 2"    },
    { 9, "???"              }
};

/*---------------------------------------------------------*\
| Z370 AORUS Ultra Gaming (LED ID 0x0950005F)               |
|                                                           |
| RGB Fusion 3.26 draws this board with canvas "Area95";    |
| Uid = division.                                           |
|   0     CPU socket area                                   |
|   1     DRAM slots                                        |
|   2     Accent light guide right of the DRAM slots        |
|   3     PCIe slot area                                    |
|   4     12V RGB header, labeled both LED C1 and LED C2    |
|         (one channel drives both headers)                 |
|   5, 6  hidden, not populated                             |
|   7     Digital LED C1 (ARGB header)                      |
|   8     Digital LED C2 (ARGB header)                      |
| The firmware drives the ARGB strip itself; the MCU has    |
| no LED-count register.  Divisions 0-3 confirmed on        |
| hardware by lighting one at a time.                       |
\*---------------------------------------------------------*/
static const gb_fusion2_smbus_zone z370_aorus_ultra_gaming_zones[] =
{
    { 0, "CPU"              },
    { 1, "DRAM"             },
    { 2, "Accent"           },
    { 3, "PCIe"             },
    { 4, "LED C1/C2 (12V)"  },
    { 7, "Digital LED C1"   },
    { 8, "Digital LED C2"   }
};

/*---------------------------------------------------------*\
| Layout list.  board_name must match the DMI baseboard     |
| product string exactly, the same string used in the       |
| detector's SupportedDevices list.                         |
\*---------------------------------------------------------*/
const gb_fusion2_smbus_layout gb_fusion2_smbus_generic_layout =
    GB_FUSION2_SMBUS_LAYOUT("", generic_zones);

static const gb_fusion2_smbus_layout gb_fusion2_smbus_layout_list_data[] =
{
    GB_FUSION2_SMBUS_LAYOUT("Z370 AORUS Ultra Gaming-CF", z370_aorus_ultra_gaming_zones),
};

const unsigned int              GB_FUSION2_SMBUS_LAYOUT_COUNT   = (sizeof(gb_fusion2_smbus_layout_list_data) / sizeof(gb_fusion2_smbus_layout_list_data[0]));
const gb_fusion2_smbus_layout*  gb_fusion2_smbus_layout_list    = gb_fusion2_smbus_layout_list_data;

const gb_fusion2_smbus_layout* GetGigabyteFusion2SMBusLayout(const std::string& board_name)
{
    for(unsigned int layout_idx = 0; layout_idx < GB_FUSION2_SMBUS_LAYOUT_COUNT; layout_idx++)
    {
        if(board_name == gb_fusion2_smbus_layout_list[layout_idx].board_name)
        {
            return(&gb_fusion2_smbus_layout_list[layout_idx]);
        }
    }

    return(&gb_fusion2_smbus_generic_layout);
}
