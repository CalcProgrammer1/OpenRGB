/*---------------------------------------------------------*\
| CorsairDRAMDevices.cpp                                    |
|                                                           |
|   Device list for Corsair DRAM RGB controllers            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                07 Apr 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "CorsairDRAMDevices.h"

static const corsair_dram_device corsair_vengeance_pro_ddr4_device =
{
    "Corsair Vengeance RGB Pro DDR4",
    {
        CORSAIR_VENGEANCE_PRO_DDR4_PID_1,
        CORSAIR_VENGEANCE_PRO_DDR4_PID_2,
        0,
        0,
        0,
        0,
    },
    10,
    false
};

static const corsair_dram_device corsair_dominator_platinum_ddr4_device =
{
    "Corsair Dominator Platinum RGB DDR4",
    {
        CORSAIR_DOMINATOR_PLATINUM_DDR4_PID_1,
        CORSAIR_DOMINATOR_PLATINUM_DDR4_PID_2,
        0,
        0,
        0,
        0,
    },
    12,
    true
};

static const corsair_dram_device corsair_vengeance_pro_sl_ddr4_device =
{
    "Corsair Vengeance RGB Pro SL DDR4",
    {
        CORSAIR_VENGEANCE_PRO_SL_DDR4_PID_1,
        CORSAIR_VENGEANCE_PRO_SL_DDR4_PID_2,
        0,
        0,
        0,
        0,
    },
    10,
    false
};

static const corsair_dram_device corsair_vengeance_rs_ddr4_device =
{
    "Corsair Vengeance RGB RS DDR4",
    {
        CORSAIR_VENGEANCE_RS_DDR4_PID_1,
        CORSAIR_VENGEANCE_RS_DDR4_PID_2,
        0,
        0,
        0,
        0,
    },
    6,
    false
};

static const corsair_dram_device corsair_dominator_platinum_ddr5_device =
{
    "Corsair Dominator Platinum RGB DDR5",
    {
        CORSAIR_DOMINATOR_PLATINUM_DDR5_PID_1,
        CORSAIR_DOMINATOR_PLATINUM_DDR5_PID_2,
        0,
        0,
        0,
        0,
    },
    12,
    true
};

static const corsair_dram_device corsair_dominator_titanium_ddr5_device =
{
    "Corsair Dominator Titanium RGB DDR5",
    {
        CORSAIR_DOMINATOR_TITANIUM_DDR5_PID_1,
        CORSAIR_DOMINATOR_TITANIUM_DDR5_PID_2,
        CORSAIR_DOMINATOR_TITANIUM_DDR5_PID_3,
        CORSAIR_DOMINATOR_TITANIUM_DDR5_PID_4,
        0,
        0,
    },
    12,
    true
};

static const corsair_dram_device corsair_vengeance_ddr5_device =
{
    "Corsair Vengeance RGB DDR5",
    {
        CORSAIR_VENGEANCE_DDR5_PID_1,
        CORSAIR_VENGEANCE_DDR5_PID_2,
        CORSAIR_VENGEANCE_DDR5_PID_3,
        CORSAIR_VENGEANCE_DDR5_PID_4,
        CORSAIR_VENGEANCE_DDR5_PID_5,
        CORSAIR_VENGEANCE_DDR5_PID_6,
    },
    10,
    false
};

static const corsair_dram_device corsair_vengeance_shugo_series_ddr5_device =
{
    "Corsair Vengeance Shugo Series DDR5",
    {
        CORSAIR_VENGEANCE_SHUGO_SERIES_DDR5_PID_1,
        CORSAIR_VENGEANCE_SHUGO_SERIES_DDR5_PID_2,
        CORSAIR_VENGEANCE_SHUGO_SERIES_DDR5_PID_3,
        CORSAIR_VENGEANCE_SHUGO_SERIES_DDR5_PID_4,
        0,
        0,
    },
    10,
    false
};

static const corsair_dram_device corsair_vengeance_rs_ddr5_device =
{
    "Corsair Vengeance RGB RS DDR5",
    {
        CORSAIR_VENGEANCE_RS_DDR5_PID_1,
        CORSAIR_VENGEANCE_RS_DDR5_PID_2,
        0,
        0,
        0,
        0,
    },
    6,
    false
};

static const corsair_dram_device* device_list[] =
{
    &corsair_vengeance_pro_ddr4_device,
    &corsair_dominator_platinum_ddr4_device,
    &corsair_vengeance_pro_sl_ddr4_device,
    &corsair_vengeance_rs_ddr4_device,
    &corsair_dominator_platinum_ddr5_device,
    &corsair_dominator_titanium_ddr5_device,
    &corsair_vengeance_ddr5_device,
    &corsair_vengeance_shugo_series_ddr5_device,
    &corsair_vengeance_rs_ddr5_device,
};

const unsigned int CORSAIR_DRAM_NUM_DEVICES = (sizeof(device_list) / sizeof(device_list[ 0 ]));
const corsair_dram_device** corsair_dram_device_list = device_list;
