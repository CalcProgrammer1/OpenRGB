/*-----------------------------------------*\
|  SteelSeriesRivalController.h             |
|                                           |
|  Definitions and types for SteelSeries    |
|  Rival lighting controller                |
|                                           |
|  B Horn (bahorn) 13/5/2020                |
\*-----------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>

#include "SteelSeriesGeneric.h"

#pragma once

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
        const char*         path
        );

    ~SteelSeriesRivalController();

    std::string GetDeviceLocation();
    char*       GetDeviceName();
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
    char                    device_name[32];
    hid_device*             dev;
    std::string             location;
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
