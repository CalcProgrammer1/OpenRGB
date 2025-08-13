/*---------------------------------------------------------*\
| MSIMysticLight185Controller.h                             |
|                                                           |
|   Driver for MSI Mystic Light 185-byte motherboard        |
|                                                           |
|   Direct mode functionality has been implemented based on |
|   the mystic-why project provided by Aleksandr            |
|   Garashchenko                                            |
|   (https://github.com/garashchenko/mystic-why)            |
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

class MSIMysticLight185Controller
{
public:
    MSIMysticLight185Controller
        (
        hid_device*     handle,
        const char*     path,
        unsigned short  pid,
        std::string     dev_name
        );

    ~MSIMysticLight185Controller();

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
    bool            IsDirectModeActive() { return direct_mode; }
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

    DIRECT_MODE     GetSupportedDirectMode() { return per_led_mode; }

private:
    hid_device*                     dev;
    std::string                     name;
    std::string                     location;
    std::string                     version_APROM;
    std::string                     version_LDROM;

    FeaturePacket_185               data;
    FeaturePacket_PerLED_185        per_led_data_onboard_and_sync;
    FeaturePacket_PerLED_185        per_led_data_jrainbow1;
    FeaturePacket_PerLED_185        per_led_data_jrainbow2;
    FeaturePacket_PerLED_185        per_led_data_jcorsair;
    FeaturePacket_185               zone_based_per_led_data;
    bool                            direct_mode;
    bool                            sync_direct_mode;
    bool                            no_onboards;
    bool                            no_jrainbow1;
    bool                            no_jrainbow2;
    bool                            no_jcorsair;
    bool                            mixed_mode_support;
    int                             numof_onboard_leds;
    int                             numof_pipe1_leds;
    int                             numof_pipe2_leds;
    int                             numof_JRGBs;
    const std::vector<MSI_ZONE>*    supported_zones;
    DIRECT_MODE                     per_led_mode;

    bool            ReadSettings();
    bool            ReadFwVersion();
    ZoneData*       GetZoneData
                        (
                        FeaturePacket_185&  data_packet,
                        MSI_ZONE            zone
                        );
    RainbowZoneData* GetRainbowZoneData(MSI_ZONE zone);
    Color*          GetPerLedZoneData
                        (
                        MSI_ZONE zone
                        );
    void            SelectPerLedProtocol();
};
