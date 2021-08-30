/*-----------------------------------------*\
|  SteelSeriesRival3Controller.h            |
|                                           |
|  Definitions and types for SteelSeries    |
|  Rival 3 lighting controller              |
|                                           |
|  B Horn (bahorn) 29/8/2021                |
\*-----------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>

#include "SteelSeriesGeneric.h"

#pragma once

#define STEELSERIES_RIVAL_3_BRIGHTNESS_MAX          0x64

/*-----------------------------------------------------------*\
| Theses are the specific values that get sent to set a mode  |
\*-----------------------------------------------------------*/
enum
{
    STEELSERIES_RIVAL_3_EFFECT_SPECTRUM_CYCLE       = 0x00,
    STEELSERIES_RIVAL_3_EFFECT_BREATHING_MAX        = 0x01,
    STEELSERIES_RIVAL_3_EFFECT_BREATHING_MID        = 0x02,
    STEELSERIES_RIVAL_3_EFFECT_BREATHING_MIN        = 0x03,
    STEELSERIES_RIVAL_3_EFFECT_DIRECT               = 0x04,
    STEELSERIES_RIVAL_3_EFFECT_RAINBOW_BREATHING    = 0x05,
    STEELSERIES_RIVAL_3_EFFECT_DISCO                = 0x06
};

class SteelSeriesRival3Controller
{
public:
    SteelSeriesRival3Controller
        (
        hid_device*         dev_handle,
        steelseries_type    proto_type,
        const char*         path
        );

    ~SteelSeriesRival3Controller();

    std::string         GetDeviceLocation();
    char*               GetDeviceName();
    std::string         GetSerialString();
    std::string         GetFirmwareVersion();

    steelseries_type    GetMouseType();

    void                Save();

    void                SetLightEffectAll(uint8_t effect);

    void                SetColor
                            (
                            unsigned char   zone_id,
                            unsigned char   red,
                            unsigned char   green,
                            unsigned char   blue,
                            unsigned char   brightness
                            );

private:
    char                    device_name[32];
    hid_device*             dev;
    std::string             location;
    steelseries_type        proto;
};
