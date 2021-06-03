/*-----------------------------------------*\
|  AsusAuraHeadsetStandController.h         |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Mola19 06/04/2021                        |
\*-----------------------------------------*/

#include "RGBController.h"

#include <hidapi/hidapi.h>

#pragma once

enum
{
    AURA_HEADSET_STAND_ZONE_UNDERGLOW   = 0,
    AURA_HEADSET_STAND_ZONE_LOGO        = 1
};

enum
{
    AURA_HEADSET_STAND_MODE_DIRECT      = 0,
    AURA_HEADSET_STAND_MODE_STATIC      = 1,
    AURA_HEADSET_STAND_MODE_BREATHING   = 2,
    AURA_HEADSET_STAND_MODE_STROBING    = 3,
    AURA_HEADSET_STAND_MODE_COLOR_CYCLE = 4,
    AURA_HEADSET_STAND_MODE_RAINBOW     = 5
};

class AuraHeadsetStandController
{
public:
    AuraHeadsetStandController(hid_device* dev_handle, const char* path);
    virtual ~AuraHeadsetStandController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void UpdateLeds
        (
        std::vector<RGBColor>    colors
        );

    void UpdateDevice
        (
        unsigned char   mode,
        unsigned char   red,
        unsigned char   grn,
        unsigned char   blu,
        unsigned char   speed,
        bool            save
        );

private:
    hid_device*                 dev;
    std::string                 location;
};
