/*---------------------------------------------------------*\
| SteelSeriesGeneric.h                                      |
|                                                           |
|   Generic file for SteelSeries devices                    |
|                                                           |
|   B Horn (bahorn)                             17 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi/hidapi.h>

/* Allows us to handle variation in the protocol.
 * Defined in a single enum so we can keep the device_list struct the same
 * for every possible device. */
typedef enum
{
    RIVAL_100   = 0x00,
    RIVAL_300   = 0x01,
    RIVAL_650   = 0x02,
    SIBERIA_350 = 0x03,
    APEX        = 0x04,
    APEX_M      = 0x05,
    APEX_OLD    = 0x06,
    SENSEI      = 0x07,
    RIVAL_600   = 0x08,
    RIVAL_3     = 0x09,
    APEX_TZONE  = 0x0A,
    RIVAL_700   = 0x0B,
    AEROX_3     = 0x0C,
    APEX_8ZONE  = 0x0D,
} steelseries_type;

