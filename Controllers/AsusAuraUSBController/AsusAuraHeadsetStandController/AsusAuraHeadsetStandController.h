/*---------------------------------------------------------*\
| AsusAuraHeadsetStandController.h                          |
|                                                           |
|   Driver for ASUS Aura headset stand                      |
|                                                           |
|   Mola19                                      06 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "RGBController.h"

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
    AuraHeadsetStandController(hid_device* dev_handle, const char* path, std::string dev_name);
    virtual ~AuraHeadsetStandController();

    std::string GetDeviceLocation();
    std::string GetName();
    std::string GetSerialString();
    std::string GetVersion();

    void        UpdateLeds
                    (
                    std::vector<RGBColor>    colors
                    );

    void        UpdateDevice
                    (
                    unsigned char   mode,
                    unsigned char   red,
                    unsigned char   grn,
                    unsigned char   blu,
                    unsigned char   speed,
                    unsigned char   brightness
                    );

    void        SaveMode();

private:
    hid_device* dev;
    std::string location;
    std::string name;
};
