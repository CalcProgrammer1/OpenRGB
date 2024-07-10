/*-----------------------------------------*\
|  SteelSeriesSenseiController.h            |
|                                           |
|  Definitions and types for SteelSeries    |
|  Sensei lighting controller               |
|  Based on Rival controller by             |
|  B Horn (bahorn) 13/5/2020                |
\*-----------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>

#include "SteelSeriesGeneric.h"

#pragma once

/*-------------------------------------------------------------*\
| Mode, we then use these to set actual effect based on speed.  |
\*-------------------------------------------------------------*/
enum
{
    STEELSERIES_SENSEI_MODE_DIRECT              = 0x00,
    STEELSERIES_SENSEI_MODE_BREATHING           = 0x01,
    STEELSERIES_SENSEI_MODE_RAINBOW             = 0x02
};

/*-------------------------------------------------------------*\
| Effects                                                       |
\*-------------------------------------------------------------*/
enum
{
    STEELSERIES_SENSEI_EFFECT_DIRECT            = 0x01,
    STEELSERIES_SENSEI_EFFECT_BREATHING_MIN     = 0x02,
    STEELSERIES_SENSEI_EFFECT_BREATHING_MID     = 0x03,
    STEELSERIES_SENSEI_EFFECT_BREATHING_MAX     = 0x04,
    STEELSERIES_SENSEI_EFFECT_RAINBOW_MIN       = 0x05,
    STEELSERIES_SENSEI_EFFECT_RAINBOW_MID       = 0x06,
    STEELSERIES_SENSEI_EFFECT_RAINBOW_MAX       = 0x07
};

class SteelSeriesSenseiController
{
public:
    SteelSeriesSenseiController
        (
        hid_device*         dev_handle,
        steelseries_type    proto_type,
        const char*         path
        );

    ~SteelSeriesSenseiController();

    std::string GetDeviceLocation();
    char*       GetDeviceName();
    std::string GetSerialString();

    steelseries_type GetMouseType();

    void Save();

    void SetLightEffect
            (
            unsigned char   zone_id,
            unsigned char   effect,
            unsigned char   speed,
            unsigned char   red,
            unsigned char   green,
            unsigned char   blue
            );

    void SetLightEffectAll
            (
            unsigned char   effect,
            unsigned char   speed,
            unsigned char   red,
            unsigned char   green,
            unsigned char   blue
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
};
