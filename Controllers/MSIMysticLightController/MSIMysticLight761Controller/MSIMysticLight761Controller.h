/*---------------------------------------------------------*\
| MSIMysticLight185Controller.cpp                           |
|                                                           |
|   Driver for MSI Mystic Light 761-byte motherboard        |
|                                                           |
|   Direct mode functionality has been implemented based on |
|   the SignalRGB project                                   |
|   (https://signalrgb.com/)                                |
|                                                           |
|   rom4ster                                    11 Jun 2025 |
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

class MSIMysticLight761Controller
{
public:
    MSIMysticLight761Controller
        (
            hid_device*     handle,
            const char*     path,
            unsigned short  pid,
            std::string     dev_name
            );

    ~MSIMysticLight761Controller();

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

    void            SetLedColor
        (
            MSI_ZONE        zone,
            int             index,
            unsigned char   red,
            unsigned char   grn,
            unsigned char   blu
            );

    void            SetCycleCount
        (
            MSI_ZONE        zone,
            unsigned char   cycle_num
            );

    bool            Update
        (
            bool save
            );

    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    std::string     GetFWVersion();
    std::string     GetSerial();

    void            SetDirectMode
        (
            bool mode
            );
    bool            IsDirectModeActive() { return true; }
    size_t          GetMaxDirectLeds
        (
            MSI_ZONE zone
            );
    const std::vector<MSI_ZONE>*
    GetSupportedZones() { return supported_zones; }

    enum DIRECT_MODE
    {
        DIRECT_MODE_DISABLED,
        DIRECT_MODE_PER_LED,
        DIRECT_MODE_ZONE_BASED
    };

    DIRECT_MODE     GetSupportedDirectMode() { return DIRECT_MODE_PER_LED; }

    struct ZoneConfig
    {
        MSI_ZONE msi_zone;
        ZoneData * zone_data;
    };

private:
    std::string                                             name;
    std::vector<MSIMysticLight761Controller::ZoneConfig>    zone_configs;
    std::vector<MSI_ZONE>*                                  supported_zones;
    hid_device*                                             dev;
    std::string                                             location;
    std::string                                             version_APROM;
    std::string                                             version_LDROM;
    FeaturePacket_761*                                      data;

    bool            ReadSettings();
    bool            ReadFwVersion();

    ZoneData*       GetZoneData
        (
            FeaturePacket_761&  data_packet,
            MSI_ZONE            zone
            );
    RainbowZoneData* GetRainbowZoneData(MSI_ZONE zone);
    Color*          GetPerLedZoneData
        (
            MSI_ZONE zone
            );
    void            SelectPerLedProtocol();
};
