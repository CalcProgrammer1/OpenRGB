/*---------------------------------------------------------*\
| SteelSeriesGeneric.h                                      |
|                                                           |
|   Generic file for SteelSeries devices                    |
|                                                           |
|   B Horn (bahorn)                             17 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>

/* Allows us to handle variation in the protocol.
 * Defined in a single enum so we can keep the device_list struct the same
 * for every possible device. */
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
    APEX_9_TKL                     = 0x18,
    APEX_9_MINI                    = 0x19,
} steelseries_type;
