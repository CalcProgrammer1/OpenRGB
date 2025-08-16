/*---------------------------------------------------------*\
| SteelSeriesRivalController.h                              |
|                                                           |
|   Driver for SteelSeries Rival                            |
|                                                           |
|   B Horn (bahorn)                             13 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "SteelSeriesGeneric.h"

/* Mode, we then use these to set actual effect based on speed. */
enum
{
    STEELSERIES_RIVAL_DIRECT                = 0x00,
    STEELSERIES_RIVAL_PULSATE               = 0x01
};

/* Effects */
enum
{
    STEELSERIES_RIVAL_EFFECT_DIRECT         = 0x01,
    STEELSERIES_RIVAL_EFFECT_PULSATE_MIN    = 0x02,
    STEELSERIES_RIVAL_EFFECT_PULSATE_MID    = 0x03,
    STEELSERIES_RIVAL_EFFECT_PULSATE_MAX    = 0x04
};

class SteelSeriesRivalController
{
public:
    SteelSeriesRivalController
        (
        hid_device*         dev_handle,
        steelseries_type    proto_type,
        const char*         path,
        std::string         dev_name
        );

    ~SteelSeriesRivalController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();
    std::string GetSerialString();
    std::string GetFirmwareVersion();

    steelseries_type GetMouseType();

    void SaveMode();

    void SetLightEffect
            (
            unsigned char   zone_id,
            unsigned char   effect
            );

    void SetLightEffectAll
            (
            unsigned char   effect
            );

    void SetColor
            (
            unsigned char   zone_id,
            unsigned char   red,
            unsigned char   green,
            unsigned char   blue
            );
    void SetColorAll
            (
                unsigned char red,
                unsigned char green,
                unsigned char blue
            );

private:
    hid_device*             dev;
    std::string             location;
    std::string             name;
    steelseries_type        proto;

    void SetRival650Color
            (
            unsigned char   zone_id,
            unsigned char   red,
            unsigned char   green,
            unsigned char   blue
            );

    void SetRival600Color
            (
            unsigned char   zone_id,
            unsigned char   red,
            unsigned char   green,
            unsigned char   blue
            );

    void SetRival700Color
            (
            unsigned char   zone_id,
            unsigned char   red,
            unsigned char   green,
            unsigned char   blue
            );
};
