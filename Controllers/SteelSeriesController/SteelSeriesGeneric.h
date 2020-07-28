/*-----------------------------------------*\
|  SteelSeriesGeneric.h                     |
|                                           |
|  Definitions and types for the SteelSeries|
|  Devices.                                 |
|                                           |
|  B Horn (bahorn) 17/5/2020                |
\*-----------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>

#pragma once

/* Allows us to handle variation in the protocol.
 * Defined in a single enum so we can keep the device_list struct the same
 * for every possible device. */
typedef enum
{
    RIVAL_100   = 0x00,
    RIVAL_300   = 0x01,
    SIBERIA_350 = 0x03,
    APEX_7		= 0x04,
    APEX_7_TKL	= 0x05,
} steelseries_type;

