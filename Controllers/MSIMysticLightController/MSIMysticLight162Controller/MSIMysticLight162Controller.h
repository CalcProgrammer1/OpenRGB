/*---------------------------------------------------------*\
| MSIMysticLight162Controller.h                             |
|                                                           |
|   Driver for MSI Mystic Light 162-byte motherboard        |
|                                                           |
|   T-bond                                      03 Apr 2020 |
|   Adam Honse                                  06 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstring>
#include <limits>
#include <hidapi.h>
#include "MSIMysticLightCommon.h"
#include "RGBController.h"

class MSIMysticLight162Controller
{
public:
    MSIMysticLight162Controller
                        (
                        hid_device*     handle,
                        const char*     path,
                        unsigned short  pid,
                        std::string     dev_name
                        );

    ~MSIMysticLight162Controller();

    void            SetMode
                        (
                        MSI_ZONE        zone,
                        MSI_MODE        mode,
                        MSI_SPEED       speed,
                        MSI_BRIGHTNESS  brightness,
                        bool            rainbow_color
                        );

    MSI_MODE        GetMode();

    void            GetMode
                        (
                        MSI_ZONE        zone,
                        MSI_MODE        &mode,
                        MSI_SPEED       &speed,
                        MSI_BRIGHTNESS  &brightness,
                        bool            &rainbow_color,
                        unsigned int    &color
                        );

    void            SetZoneColor
                        (
                        MSI_ZONE        zone,
                        unsigned char   red1,
                        unsigned char   grn1,
                        unsigned char   blu1,
                        unsigned char   red2,
                        unsigned char   grn2,
                        unsigned char   blu2
                        );

    bool            Update
                        (
                        bool save
                        );

    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    std::string     GetFWVersion();
    std::string     GetSerial();

    size_t          GetMaxOnboardLeds();
    const std::vector<MSI_ZONE>*
                    GetSupportedZones() { return supported_zones; }

private:
    hid_device*                     dev;
    std::string                     name;
    std::string                     location;
    std::string                     version_APROM;
    std::string                     version_LDROM;

    FeaturePacket_162               data;
    size_t                          numof_onboard_leds;
    const std::vector<MSI_ZONE>*    supported_zones;

    bool            ReadSettings();
    bool            ReadFwVersion();
    ZoneData*       GetZoneData
                        (
                        FeaturePacket_162&  dataPacket,
                        MSI_ZONE            zone
                        );
};
